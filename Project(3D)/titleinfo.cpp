//==============================
//
//  �I������[titleinfo.h]
//  Author:kaiti
//
//==============================
#include "titleinfo.h"
#include "game.h"
#include "input.h"
#include "ranking.h"
#include "fade.h"
//#include "tutorial.h"
#include "sound.h"
//#include "player.h"
#include "mouse.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTexturetitleinfo[TITLE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufftitleinfo = NULL;
D3DXVECTOR3 g_Selectpos;
//���S
LPDIRECT3DTEXTURE9 g_pTexturetitleinfoLogo = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufftitleinfoLogo = NULL;
D3DXVECTOR3 g_Logopos;

int g_titleinfoMenu;
//============
//����������
//============
void InitTitleInfo(void)
{
	g_titleinfoMenu = TITLE_START;

	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	g_Selectpos = D3DXVECTOR3(640.0f, 360.0f, 0.0f);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\gamestart.png", &g_pTexturetitleinfo[0]); //START
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\titleranking.png", &g_pTexturetitleinfo[1]); //�����L���O
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\exit.png", &g_pTexturetitleinfo[2]); //�����L���O

	//���_�o�b�t�@�̐����E���_���̐ݒ�
	VERTEX_2D* pVtx;
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TITLE_MAX,
		                        D3DUSAGE_WRITEONLY,
		                        FVF_VERTEX_2D,
		                        D3DPOOL_MANAGED,
		                        &g_pVtxBufftitleinfo,
		                        NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBufftitleinfo->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCnt = 0; nCnt < TITLE_MAX; nCnt++)
	{//���j���[
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_Selectpos.x - TITLESELECT_WIDTH / 2, g_Selectpos.y - TITLESELECT_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Selectpos.x + TITLESELECT_WIDTH / 2, g_Selectpos.y - TITLESELECT_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Selectpos.x - TITLESELECT_WIDTH / 2, g_Selectpos.y + TITLESELECT_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Selectpos.x + TITLESELECT_WIDTH / 2, g_Selectpos.y + TITLESELECT_HEIGHT / 2, 0.0f);
		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		g_Selectpos.y += 120.0f;
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBufftitleinfo->Unlock();


	//���S
	g_Logopos = D3DXVECTOR3(640.0f, 0.0f, 0.0f);

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\title2.png", &g_pTexturetitleinfoLogo); //START
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBufftitleinfoLogo,
		NULL);
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBufftitleinfoLogo->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_Logopos.x - TITLELOGO_WIDTH / 2, g_Logopos.y - TITLELOGO_HEIGHT / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Logopos.x + TITLELOGO_WIDTH / 2, g_Logopos.y - TITLELOGO_HEIGHT / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Logopos.x - TITLELOGO_WIDTH / 2, g_Logopos.y + TITLELOGO_HEIGHT / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Logopos.x + TITLELOGO_WIDTH / 2, g_Logopos.y + TITLELOGO_HEIGHT / 2, 0.0f);
	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBufftitleinfoLogo->Unlock();
}
//==========
//�I������
//==========
void UninitTitleInfo(void)
{
	// �e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < TITLE_MAX; nCnt++)
	{
		UninitTexture(g_pTexturetitleinfo[nCnt]);
	}
	UninitTexture(g_pTexturetitleinfoLogo);

	// �o�b�t�@�̔j��
	UninitBuffer(g_pVtxBufftitleinfo);
	UninitBuffer(g_pVtxBufftitleinfoLogo);
}
//==========
//�X�V����
//==========
void UpdateTitleInfo(int zDelta)
{
	FADE g_fade;
	g_fade = GetFade();

	//Player* pPlayer = GetPlayer();

	UpdateSelect(zDelta);

	UpdateLighting();

	if ((KeyboardTrigger(DIK_RETURN) == true|| GetJoypadTrigger(JOYKEY_A) == true || OnMouseTriggerDown(0) == true) && g_fade == FADE_NONE)
	{//ENTER�������ꂽ
		if (g_Logopos.y < LOGO_END_Y)
		{//���S������Ă��Ă��鎞
			g_Logopos.y += LOGO_END_Y;
		}
		else
		{//���S������؂����Ƃ�
			StopSound();
			//PlaySound(SOUND_LABEL_DESICION);
			//���j���[�ɍ��킹�ă��[�h�̐؂�ւ�
			if (g_titleinfoMenu == TITLE_START)
			{	//START�ɂ���ꍇ
				SetFade(MODE_STAGEONE);
				//pPlayer->bfirst = true;

				//PlaySound(SOUND_LABEL_GAME);
			}
			else if (g_titleinfoMenu == TITLE_RANK)
			{	//RANK�ɂ���ꍇ
				SetFade(MODE_RANK);
				SetRankMode(RANKMODE_SELECT);

				//PlaySound(SOUND_LABEL_RANKING);
			}
			else if (g_titleinfoMenu == TITLE_FIN)
			{	//FIN�ɂ���ꍇ
				SetFade(MODE_END);
			}
		}
	}

	VERTEX_2D* pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBufftitleinfoLogo->Lock(0, 0, (void**)&pVtx, 0);

	//���S��������
	if (g_Logopos.y < LOGO_END_Y)
	{
		g_Logopos.y++;
	}
	//���S��Y����
	if (g_Logopos.y >= LOGO_END_Y)
	{
		g_Logopos.y = LOGO_END_Y;
	}

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_Logopos.x - TITLELOGO_WIDTH / 2, g_Logopos.y - TITLELOGO_HEIGHT / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Logopos.x + TITLELOGO_WIDTH / 2, g_Logopos.y - TITLELOGO_HEIGHT / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Logopos.x - TITLELOGO_WIDTH / 2, g_Logopos.y + TITLELOGO_HEIGHT / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Logopos.x + TITLELOGO_WIDTH / 2, g_Logopos.y + TITLELOGO_HEIGHT / 2, 0.0f);


	//���_�o�b�t�@���A�����b�N
	g_pVtxBufftitleinfoLogo->Unlock();
}
//===========
//�`�揈��
//===========
void DrawTitleInfo(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBufftitleinfo, 0, sizeof(VERTEX_2D));

	int nCnt;
	for (nCnt = 0; nCnt < TITLE_MAX; nCnt++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturetitleinfo[nCnt]);
		//�v���C���[�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBufftitleinfoLogo, 0, sizeof(VERTEX_2D));
	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTexturetitleinfoLogo);
	//�v���C���[�̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}

//*********************
// ���̃��[�h��I�ׂ邩
//*********************
bool isSelectUpCondition(int zDelta)
{
	FADE g_fade;
	g_fade = GetFade();
	return KeyboardTrigger(DIK_S) == true || KeyboardTrigger(DIK_DOWN) == true || GetJoypadTrigger(JOYKEY_DOWN) == true || zDelta < 0;
}

//*********************
// ��̃��[�h��I�ׂ邩
//*********************
bool isSelectDownCondition(int zDelta)
{
	FADE g_fade;
	g_fade = GetFade();
	return KeyboardTrigger(DIK_W) == true || KeyboardTrigger(DIK_UP) == true || GetJoypadTrigger(JOYKEY_UP) == true || zDelta > 0;
}

//*********************************
// ���������݂̃��[�h�Ɠ�����������
//*********************************
bool isTitleMode(TITLEINFO TitleType)
{
	return g_titleinfoMenu == TitleType;
}

//*******************************
// ���������݂̃��[�h�ȉ���������
//*******************************
bool isLessMode(TITLEINFO TitleType)
{
	return g_titleinfoMenu < TitleType;
}

//*******************************
// ���������݂̃��[�h�ȏゾ������
//*******************************
bool isGreaterMode(TITLEINFO TitleType)
{
	return g_titleinfoMenu > TitleType;
}

//*******************
// �I��ł鍀�ڂ��X�V
//*******************
void UpdateSelect(int zDelta)
{
	//���j���[�̑I��(�㉺)
	if (isSelectDownCondition(zDelta) == true)
	{
		//PlaySound(SOUND_LABEL_SELECT);

		g_titleinfoMenu--;
		if (isLessMode(TITLE_START) == true)
		{
			g_titleinfoMenu = TITLE_FIN;
		}
	}
	else if (isSelectUpCondition(zDelta) == true)
	{
		//PlaySound(SOUND_LABEL_SELECT);

		g_titleinfoMenu++;
		if (isGreaterMode(TITLE_FIN) == true)
		{
			g_titleinfoMenu = TITLE_START;
		}
	}
}

//*****************************
// �I��ł鍀�ڂ�ڗ������鏈��
//*****************************
void UpdateLighting()
{
	VERTEX_2D* pVtx = NULL;
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBufftitleinfo->Lock(0, 0, (void**)&pVtx, 0);

	g_Selectpos = D3DXVECTOR3(640.0f, 360.0f, 0.0f);

	for (int TypeCount = 0; TypeCount < TITLE_MAX; TypeCount++)
	{
		if (isTitleMode((TITLEINFO)TypeCount) == true)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_Selectpos.x - TITLESELECT_WIDTH / 2 * ZOOM_SELECT, g_Selectpos.y - TITLESELECT_HEIGHT / 2 * ZOOM_SELECT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Selectpos.x + TITLESELECT_WIDTH / 2 * ZOOM_SELECT, g_Selectpos.y - TITLESELECT_HEIGHT / 2 * ZOOM_SELECT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Selectpos.x - TITLESELECT_WIDTH / 2 * ZOOM_SELECT, g_Selectpos.y + TITLESELECT_HEIGHT / 2 * ZOOM_SELECT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Selectpos.x + TITLESELECT_WIDTH / 2 * ZOOM_SELECT, g_Selectpos.y + TITLESELECT_HEIGHT / 2 * ZOOM_SELECT, 0.0f);

			pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

		}
		else
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_Selectpos.x - TITLESELECT_WIDTH / 2, g_Selectpos.y - TITLESELECT_HEIGHT / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Selectpos.x + TITLESELECT_WIDTH / 2, g_Selectpos.y - TITLESELECT_HEIGHT / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Selectpos.x - TITLESELECT_WIDTH / 2, g_Selectpos.y + TITLESELECT_HEIGHT / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Selectpos.x + TITLESELECT_WIDTH / 2, g_Selectpos.y + TITLESELECT_HEIGHT / 2, 0.0f);

			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		g_Selectpos.y += 120.0f;
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBufftitleinfo->Unlock();
}