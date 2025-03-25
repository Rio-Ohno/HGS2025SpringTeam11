//==============================
//
//  �|�[�Y����[pause.h]
//  Author:kaiti
//
//==============================
#include "pause.h"
#include "game.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
//#include "player.h"
#include "mouse.h"

#define MAX_TEXTURE (5)

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTexturePause[MAX_TEXTURE] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;
PAUSE_MENU g_pauseMenu = PAUSE_MENU_CONTNUE;
D3DXVECTOR3 g_Pausepos;
//============
//����������
//============
void InitPause(void)
{
	g_pauseMenu = PAUSE_MENU_CONTNUE;

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\settei01.png", &g_pTexturePause[1]); //�ݒ�
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\quit01.png", &g_pTexturePause[2]); //�߂�
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\retry2.png", &g_pTexturePause[3]); //��蒼��
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\exit2.png", &g_pTexturePause[4]); //�^�C�g���ɖ߂�

	g_Pausepos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - PAUSESELECT_HEIGHT * 1.5f, 0.0f);

	//���_�o�b�t�@�̐����E���_���̐ݒ�
	VERTEX_2D* pVtx;
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TEXTURE,
		                        D3DUSAGE_WRITEONLY,
		                        FVF_VERTEX_2D,
		                        D3DPOOL_MANAGED,
		                        &g_pVtxBuffPause,
		                        NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_TEXTURE ; nCnt++)
	{//�|�[�Y���j���[
		if (nCnt == 0)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);
			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DCOLOR_RGBA(1, 1, 1, 190);
			pVtx[1].col = D3DCOLOR_RGBA(1, 1, 1, 190);
			pVtx[2].col = D3DCOLOR_RGBA(1, 1, 1, 190);
			pVtx[3].col = D3DCOLOR_RGBA(1, 1, 1, 190);

		}
		else
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_Pausepos.x - (PAUSESELECT_WIDTH / 2), g_Pausepos.y - (PAUSESELECT_HEIGHT / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Pausepos.x + (PAUSESELECT_WIDTH / 2), g_Pausepos.y - (PAUSESELECT_HEIGHT / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Pausepos.x - (PAUSESELECT_WIDTH / 2), g_Pausepos.y + (PAUSESELECT_HEIGHT / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Pausepos.x + (PAUSESELECT_WIDTH / 2), g_Pausepos.y + (PAUSESELECT_HEIGHT / 2), 0.0f);
			g_Pausepos.y += PAUSESELECT_HEIGHT * 1.25;

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffPause->Unlock();
}
//==========
//�I������
//==========
void UninitPause(void)
{
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		//�e�N�X�`���̔j��
		if (g_pTexturePause[nCnt] != NULL)
		{
			g_pTexturePause[nCnt]->Release();
			g_pTexturePause[nCnt] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}
//==========
//�X�V����
//==========
void UpdatePause(int zDelta)
{
	FADE g_fade;
	g_fade = GetFade();

	//Player* pPlayer = GetPlayer();

	VERTEX_2D* pVtx;
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);
		
	//���j���[�̑I��(�㉺)
	//Contnue�ɂ���ꍇ
	if (KeyboardTrigger(DIK_W) == true || GetJoypadTrigger(JOYKEY_UP) == true || zDelta > 0)
	{
		//PlaySound(SOUND_LABEL_SELECT);
		switch (g_pauseMenu)
		{
		case PAUSE_MENU_OPTION:
			g_pauseMenu = PAUSE_MENU_QUIT;
			break;
		case PAUSE_MENU_CONTNUE:
			g_pauseMenu = PAUSE_MENU_OPTION;
			break;
		case PAUSE_MENU_RETRY:
			g_pauseMenu = PAUSE_MENU_CONTNUE;
			break;
		case PAUSE_MENU_QUIT:
			g_pauseMenu = PAUSE_MENU_RETRY;
			break;
		default:
			break;
		}
	}
	else if (KeyboardTrigger(DIK_S) == true || GetJoypadTrigger(JOYKEY_DOWN) == true || zDelta < 0)
	{
		//PlaySound(SOUND_LABEL_SELECT);
		switch (g_pauseMenu)
		{
		case PAUSE_MENU_OPTION:
			g_pauseMenu = PAUSE_MENU_CONTNUE;
			break;
		case PAUSE_MENU_CONTNUE:
			g_pauseMenu = PAUSE_MENU_RETRY;
			break;
		case PAUSE_MENU_RETRY:
			g_pauseMenu = PAUSE_MENU_QUIT;
			break;
		case PAUSE_MENU_QUIT:
			g_pauseMenu = PAUSE_MENU_OPTION;
			break;
		default:
			break;
		}
	}

	pVtx += 4;
	for (int nCnt = 0; nCnt < PAUSE_MENU_MAX; nCnt++)
	{
		if (nCnt == g_pauseMenu)
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 1, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 1, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 1, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 1, 255);
		}
		else
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffPause->Unlock();

	if ((KeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A) == true || OnMouseTriggerDown(0) == true) && g_fade == FADE_NONE)
	{
		//PlaySound(SOUND_LABEL_DESICION);
		//���j���[�ɍ��킹�ă��[�h�̐؂�ւ�
		if (g_pauseMenu == PAUSE_MENU_CONTNUE)
		{	//Contnue�ɂ���ꍇ
			SetEnablePause(false);
			SetGameState(GAMESTATE_NORMAL);
		}
		else if (g_pauseMenu == PAUSE_MENU_RETRY)
		{	//RETRY�ɂ���ꍇ
			SetFade(MODE_STAGEONE);
			//pPlayer->bfirst = true;

			StopSound();
			//PlaySound(SOUND_LABEL_GAME);
		}
		else if (g_pauseMenu == PAUSE_MENU_QUIT)
		{	//QUIT�ɂ���ꍇ
			SetFade(MODE_TITLE);

			StopSound();
			//PlaySound(SOUND_LABEL_TITLE);
		}
		else if (g_pauseMenu == PAUSE_MENU_OPTION)
		{	//QUIT�ɂ���ꍇ
			SetEnableOption(true);
			SetEnablePause(false);
			SetGameState(GAMESTATE_OPTION);
		}
	}
}
//===========
//�`�揈��
//===========
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePause[nCnt]);
		//�v���C���[�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}
}