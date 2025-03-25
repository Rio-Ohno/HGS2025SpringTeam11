//==============================
//
//  �X�R�A����[score.cpp]
//  Author:kaiti
//
//==============================
#include "score.h"
#include <cassert>

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;
D3DXVECTOR3 g_posScore;
int g_nScore;
int g_nDestScore;
//=============
//����������
//=============
void InitScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number200.png", &g_pTextureScore); //1
	
	g_posScore = D3DXVECTOR3(750.0f, 0.0f, 0.0f);
	g_nScore = 0;
	g_nDestScore = 0;

	//���_�o�b�t�@�̐����E���_���̐ݒ�
	VERTEX_2D* pVtx;
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SCORE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);
	int nCnt;
	for (nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_posScore.x - SCORE_SIZE, g_posScore.y - 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_posScore.x + SCORE_SIZE, g_posScore.y - 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_posScore.x - SCORE_SIZE, g_posScore.y + 50.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_posScore.x + SCORE_SIZE, g_posScore.y + 50.0f, 0.0f);
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
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		g_posScore.x += SCORE_SIZE * 2;

		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffScore->Unlock();
}
//==========
//�I������
//==========
void UninitScore(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}

}
//==========
//�X�V����
//==========
void UpdateScore(void)
{
	if (g_nScore <= g_nDestScore - 1)
	{
		g_nScore += g_nDestScore / SCORE_SPEED;
	}
	int aPosTexU[MAX_SCORE];
	int nData = SCOREDATA * 10;
	int nData2 = SCOREDATA;
	int nCnt;
	for (nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		aPosTexU[nCnt] = g_nScore % nData / nData2;
		nData /= 10;
		nData2 /= 10;
	}

	VERTEX_2D* pVtx;
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);
	int nData3 = 0;
	for (nCnt = 0; nCnt < 4 * MAX_SCORE; nCnt++)
	{
		//�e�N�X�`�����W�̐ݒ�
		pVtx[nCnt].tex = D3DXVECTOR2((aPosTexU[nData3] / 10.0f), 0.0f);
		pVtx[nCnt + 1].tex = D3DXVECTOR2((aPosTexU[nData3] / 10.0f) + 0.1f, 0.0f);
		pVtx[nCnt + 2].tex = D3DXVECTOR2((aPosTexU[nData3] / 10.0f), 1.0f);
		pVtx[nCnt + 3].tex = D3DXVECTOR2((aPosTexU[nData3] / 10.0f) + 0.1f, 1.0f);

		nData3++;
		nCnt += 3;
	}
	assert(nCnt <= 4 * MAX_SCORE);
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffScore->Unlock();
}
//===========
//�`�揈��
//===========
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));
	
	int nCnt;
	for (nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureScore);
		//�v���C���[�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt*4, 2);
	}
}
//=================
//�X�R�A�̐ݒ�
//=================
void SetScore(int nScore)
{
	int aPosTexU[MAX_SCORE]; //������	
	g_nScore = nScore;
	if (g_nScore <= 0)
	{
		g_nScore = 0;
	}

	int nData = SCOREDATA * 10;
	int nData2 = SCOREDATA;
	int nCnt;
	for (nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		aPosTexU[nCnt] = g_nScore % nData / nData2;
		nData /= 10;
		nData2 /= 10;
	}

	VERTEX_2D* pVtx;
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);
	for (nCnt = 0; nCnt <  MAX_SCORE; nCnt++)
	{
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2((aPosTexU[nCnt] * 0.1f), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((aPosTexU[nCnt] * 0.1f) + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((aPosTexU[nCnt] * 0.1f), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((aPosTexU[nCnt] * 0.1f) + 0.1f, 1.0f);

		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffScore->Unlock();
}
//==================
//�X�R�A�̉��Z����
//==================
void AddScore(int nValue)
{
	g_nDestScore += nValue;
	if (g_nDestScore <= 0)
	{
		g_nDestScore = 0;
	}
}
//==============
//�X�R�A�̎擾
//==============
int GetScore(void)
{
	return g_nScore;
}