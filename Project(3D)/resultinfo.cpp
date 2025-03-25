//================================
//
//  ���U���g�\������[ResultInfoinfo.cpp]
//  Author:kaiti
//
//================================
#include "resultinfo.h"
#include "input.h"
#include "fade.h"
#include "game.h"
#include "ranking.h"
#include "score.h"
#include "sound.h"
#include "mouse.h"

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureResultInfo[RESULT_MAX] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultInfo = NULL;
RESULT g_Resultstate = RESULT_NONE;

D3DXVECTOR3 g_posResult;
//===========
//����������
//===========
void InitResultInfo(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	g_posResult = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\GAMECLEAR.png", &g_pTextureResultInfo[0]); //�N���A
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\GAMEOVER.png", &g_pTextureResultInfo[1]); //�Q�[���I�[�o�[
	
	//���_�o�b�t�@�̐����E���_���̐ݒ�
	VERTEX_2D* pVtx;
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		                        D3DUSAGE_WRITEONLY,
		                        FVF_VERTEX_2D,
		                        D3DPOOL_MANAGED,
		                        &g_pVtxBuffResultInfo,
		                        NULL);
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResultInfo->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_posResult.x - RESULT_X, g_posResult.y - RESULT_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posResult.x + RESULT_X, g_posResult.y - RESULT_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posResult.x - RESULT_X, g_posResult.y + RESULT_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posResult.x + RESULT_X, g_posResult.y + RESULT_Y, 0.0f);
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
	g_pVtxBuffResultInfo->Unlock();
}
//==========
//�I������
//==========
void UninitResultInfo(void)
{
	for (int nCnt = 0; nCnt < RESULT_MAX; nCnt++)
	{
		if (g_pTextureResultInfo[nCnt] != NULL)
		{
			//�e�N�X�`���̔j��
			g_pTextureResultInfo[nCnt]->Release();
			g_pTextureResultInfo[nCnt] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffResultInfo != NULL)
	{
		g_pVtxBuffResultInfo->Release();
		g_pVtxBuffResultInfo = NULL;
	}
}
//==========
//�X�V����
//==========
void UpdateResultInfo(void)
{
	FADE g_fade;
	g_fade = GetFade();

	if ((KeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A) == true || OnMouseTriggerDown(0) == true) && g_fade == FADE_NONE)
	{//����L�[�������ꂽ
		//�����L���O�̐ݒ�
		SetFade(MODE_RANK);

		SetRankMode(RANKMODE_RESULT);
		SetRanking(GetScore());

		StopSound();
		//PlaySound(SOUND_LABEL_RANKING);
	}
}
//==========
//�`�揈��
//==========
void DrawResultInfo(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResultInfo, 0, sizeof(VERTEX_2D));

	if (g_Resultstate == RESULT_CLEAR)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureResultInfo[0]);
	}
	else if (g_Resultstate == RESULT_GAMEOVER)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureResultInfo[1]);
	}


	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
//===============
//���U���g�ݒ�
//===============
void SetResult(RESULT result)
{
	g_Resultstate = result;
}