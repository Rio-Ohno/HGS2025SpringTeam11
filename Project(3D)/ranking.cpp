//==============================
//
//  �����L���O����[ranking.cpp]
//  Author:kaiti
//
//==============================
#include "ranking.h"
#include "score.h"
#include "input.h"
#include "fade.h"
#include "game.h"
#include "sound.h"
#include "mouse.h"
//�����L���O�X�R�A�\����
typedef struct
{
	D3DXVECTOR3 pos; //�ʒu
	int nScore;      //�X�R�A
}RankScore;
//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureBack = NULL;
LPDIRECT3DTEXTURE9 g_pTextureRank[MAX_RANKING] = {};
LPDIRECT3DTEXTURE9 g_pTextureRankScore[4] = {};

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRank = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankScore = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankBack = NULL;

D3DXVECTOR3 g_Rankpos;
D3DXVECTOR3 g_Rankscorepos;

RankScore g_aRankScore[MAX_RANKING]; //�����L���O�X�R�A���
int g_nRankUpdata = -1;              //�X�V�����NNo.
int g_nTimerRanking = 0;             //�����L���O��ʕ\���^�C�}�[
RANK_MODE g_RankMode = RANKMODE_RESULT;
int g_RankColor;
int g_RankState;
float g_ScorePosAdd;
float g_RankPosAdd;
int g_Rank;
//=============
// ����������
//=============
void InitRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`��
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\ranking.jpg", &g_pTextureBack);        //�w�i

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\ranking1.png", &g_pTextureRank[0]);     //����
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\ranking2.png", &g_pTextureRank[1]);     //����
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\ranking3.png", &g_pTextureRank[2]);     //����
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\ranking4.png", &g_pTextureRank[3]);     //����
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\ranking5.png", &g_pTextureRank[4]);     //����

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number200.png", &g_pTextureRankScore[0]);	//�X�R�A
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number002.png", &g_pTextureRankScore[1]);	//�X�R�A
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\number300.png", &g_pTextureRankScore[2]);	//�X�R�A
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\UInumber.png", &g_pTextureRankScore[3]);		//�X�R�A

	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		g_aRankScore[nCnt].nScore = 0;
		g_aRankScore[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	g_RankColor = 255;
	g_RankState = 0;
	g_ScorePosAdd = 0;
	g_RankPosAdd = 0;
	g_Rankpos = D3DXVECTOR3(-600.0f, 180.0f, 0.0f);
	g_Rankscorepos = D3DXVECTOR3(-500.0f, 180.0f, 0.0f);
	g_Rank = 0;
	//���_�o�b�t�@�̐����E���_���̐ݒ�
	VERTEX_2D* pVtx;

	//�w�i
	{
		//���_�o�b�t�@�̐���
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffRankBack,
			NULL);
		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffRankBack->Lock(0, 0, (void**)&pVtx, 0);

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);
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
		g_pVtxBuffRankBack->Unlock();
	}

	//����
	{
		//���_�o�b�t�@�̐���
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RANKING,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffRank,
			NULL);
		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);
		for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_Rankpos.x - RANK_SIZE, g_Rankpos.y - RANK_SIZE, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Rankpos.x + RANK_SIZE, g_Rankpos.y - RANK_SIZE, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Rankpos.x - RANK_SIZE, g_Rankpos.y + RANK_SIZE, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Rankpos.x + RANK_SIZE, g_Rankpos.y + RANK_SIZE, 0.0f);
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
			g_Rankpos.y += 120.0f;
			pVtx += 4;
		}
		//���_�o�b�t�@���A�����b�N
		g_pVtxBuffRank->Unlock();
	}


	//�X�R�A
	{
		//���_�o�b�t�@�̐���
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_RANKING * MAX_SCORE,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_2D,
			D3DPOOL_MANAGED,
			&g_pVtxBuffRankScore,
			NULL);
		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);
		for (int nCnt = 0; nCnt < MAX_RANKING * MAX_SCORE; nCnt++)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_Rankscorepos.x - RANK_SIZE, g_Rankscorepos.y - RANK_SIZE, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Rankscorepos.x + RANK_SIZE, g_Rankscorepos.y - RANK_SIZE, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Rankscorepos.x - RANK_SIZE, g_Rankscorepos.y + RANK_SIZE, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Rankscorepos.x + RANK_SIZE, g_Rankscorepos.y + RANK_SIZE, 0.0f);

			g_Rankscorepos.x += RANK_SIZE * 2;

			if (nCnt == MAX_SCORE - 1)
			{
				g_Rankscorepos.x = -500.0f;
				g_Rankscorepos.y += 120.0f;
			}
			else if (nCnt == MAX_SCORE * 2 - 1)
			{
				g_Rankscorepos.x = -500.0f;
				g_Rankscorepos.y += 120.0f;
			}
			else if (nCnt == MAX_SCORE * 3 - 1)
			{
				g_Rankscorepos.x = -500.0f;
				g_Rankscorepos.y += 120.0f;
			}
			else if (nCnt == MAX_SCORE * 4 - 1)
			{
				g_Rankscorepos.x = -500.0f;
				g_Rankscorepos.y += 120.0f;
			}
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

			pVtx += 4;
		}
		//���_�o�b�t�@���A�����b�N
		g_pVtxBuffRankScore->Unlock();
	}
	ResetRanking();
}
//==========
// �I������
//==========
void UninitRanking(void)
{
	if (g_pTextureBack != NULL)
	{
		g_pTextureBack->Release();
		g_pTextureBack = NULL;
	}
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		if (g_pTextureRank[nCnt] != NULL)
		{
			g_pTextureRank[nCnt]->Release();
			g_pTextureRank[nCnt] = NULL;
		}
	}
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		if (g_pTextureRankScore[nCnt] != NULL)
		{
			g_pTextureRankScore[nCnt]->Release();
			g_pTextureRankScore[nCnt] = NULL;
		}
	}

	if (g_pVtxBuffRank != NULL)
	{
		g_pVtxBuffRank->Release();
		g_pVtxBuffRank = NULL;
	}
	if (g_pVtxBuffRankBack != NULL)
	{
		g_pVtxBuffRankBack->Release();
		g_pVtxBuffRankBack = NULL;
	}
	if (g_pVtxBuffRankScore != NULL)
	{
		g_pVtxBuffRankScore->Release();
		g_pVtxBuffRankScore = NULL;
	}
}
//============
// �X�V����
//============
void UpdateRanking(void)
{
	FADE g_fade;
	g_fade = GetFade();

	VERTEX_2D* pVtx;
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);

	MoveRanking();

	if (g_nRankUpdata != -1)
		{
			g_RankState++;
			if (g_RankColor == 255 && g_RankState == RANKSCORE_TIME)
			{
				g_RankColor = 0;
				g_RankState = 0;
			}
			if (g_RankColor == 0 && g_RankState == RANKSCORE_TIME)
			{
				g_RankColor = 255;
				g_RankState = 0;
			}

			for (int nCnt = 0; nCnt < MAX_RANKING * MAX_SCORE; nCnt++)
			{
				//�X�R�A
				if (nCnt >= 0 && nCnt < MAX_SCORE)
				{
					if (g_nRankUpdata == 0)
					{
						//���_�J���[�̐ݒ�
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_RankColor);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_RankColor);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_RankColor);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_RankColor);
					}
				}
				else if (nCnt >= MAX_SCORE && nCnt < MAX_SCORE * 2)
				{
					if (g_nRankUpdata == 1)
					{
						//���_�J���[�̐ݒ�
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_RankColor);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_RankColor);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_RankColor);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_RankColor);
					}
				}
				else if (nCnt >= MAX_SCORE * 2 && nCnt < MAX_SCORE * 3)
				{
					if (g_nRankUpdata == 2)
					{
						//���_�J���[�̐ݒ�
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_RankColor);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_RankColor);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_RankColor);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_RankColor);
					}
				}
				else if (nCnt >= MAX_SCORE * 3 && nCnt < MAX_SCORE * 4)
				{
					if (g_nRankUpdata == 3)
					{
						//���_�J���[�̐ݒ�
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_RankColor);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_RankColor);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_RankColor);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_RankColor);
					}
				}
				else if (nCnt >= MAX_SCORE * 4 && nCnt < MAX_SCORE * 5)
				{
					if (g_nRankUpdata == 4)
					{
						//���_�J���[�̐ݒ�
						pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_RankColor);
						pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_RankColor);
						pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_RankColor);
						pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_RankColor);
					}
				}
				pVtx += 4;
			}
		}
	if ((KeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A) == true || OnMouseTriggerDown(0) == true) && g_fade == FADE_NONE)
	{
		if (g_RankMode == RANKMODE_RESULT || g_RankMode == RANKMODE_SELECT)
		{//���U���g�E�I�����炫��
			StopSound();
			SetFade(MODE_TITLE);
		}
		if (g_RankMode == RANKMODE_TITLE)
		{//�^�C�g�����炫��
			SetFade(MODE_TITLE);
			g_nTimerRanking = 0;
		}
	}
	if (g_RankMode == RANKMODE_TITLE)
	{//�^�C�g�����炫��
		g_nTimerRanking++;
		if (g_nTimerRanking >= RANKING_TIME)
		{
			//�^�C�g���ɖ߂�
			SetFade(MODE_TITLE);
			g_nTimerRanking = 0;
		}
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffRankScore->Unlock();
}
//===========
// �`�揈��
//===========
void DrawRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�w�i
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRankBack, 0, sizeof(VERTEX_2D));
	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBack);
	//�v���C���[�̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//����
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRank, 0, sizeof(VERTEX_2D));
	for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureRank[nCnt]);
		//�v���C���[�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}

	//�X�R�A
	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRankScore, 0, sizeof(VERTEX_2D));
	for (int nCnt = 0; nCnt < MAX_RANKING * MAX_SCORE; nCnt++)
	{
		//�e�N�X�`���̐ݒ�
		if (nCnt < MAX_SCORE)
		{
			pDevice->SetTexture(0, g_pTextureRankScore[0]);
		}
		else if (nCnt < MAX_SCORE * 2)
		{
			pDevice->SetTexture(0, g_pTextureRankScore[1]);
		}
		else if (nCnt < MAX_SCORE * 3)
		{
			pDevice->SetTexture(0, g_pTextureRankScore[2]);
		}
		else
		{
			pDevice->SetTexture(0, g_pTextureRankScore[3]);
		}

		//�v���C���[�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}
}
//======================
// �����L���O�̃��Z�b�g
//======================
void ResetRanking(void)
{
	//�����L���O�X�R�A���̏����ݒ�
	FILE* pFile;
	pFile = fopen("data\\rankscore00.txt", "r");

	if (pFile != NULL)
	{
		for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
		{
			fscanf(pFile, "%d\n", &g_aRankScore[nCnt].nScore);
		}
		fclose(pFile);
	}
	else
	{
	}

	int nCntRank = 0;
	int aPosTexU[MAX_SCORE]; //������

	VERTEX_2D* pVtx;
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx, 0);
	for (nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		int nData = SCOREDATA * 10;
		int nData2 = SCOREDATA;

		for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
		{
			aPosTexU[nCnt] = (g_aRankScore[nCntRank].nScore % nData) / nData2;
			nData /= 10;
			nData2 /= 10;

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(aPosTexU[nCnt] * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(aPosTexU[nCnt] * 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(aPosTexU[nCnt] * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(aPosTexU[nCnt] * 0.1f + 0.1f, 1.0f);

			pVtx += 4;
		}
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffRankScore->Unlock();
}
//==================
// �����L���O�̐ݒ�
//==================
void SetRanking(int nScore)
{
	int nCntRank = 0;

	//�����L���O�X�R�A�̕��בւ�
	//�w��̃X�R�A�������N�C��������g_nRankUpdata���X�V
	if (nScore >= g_aRankScore[4].nScore)
	{
		g_aRankScore[4].nScore = g_aRankScore[3].nScore;

		if (nScore >= g_aRankScore[3].nScore)
		{
			g_aRankScore[3].nScore = g_aRankScore[2].nScore;

			if (nScore >= g_aRankScore[2].nScore)
			{
				g_aRankScore[2].nScore = g_aRankScore[1].nScore;

				if (nScore >= g_aRankScore[1].nScore)
				{
					g_aRankScore[1].nScore = g_aRankScore[0].nScore;

					if (nScore >= g_aRankScore[0].nScore)
					{
						g_aRankScore[0].nScore = nScore;
						g_nRankUpdata = 0;
					}
					else
					{
						g_aRankScore[1].nScore = nScore;
						g_nRankUpdata = 1;
					}
				}
				else
				{
					g_aRankScore[2].nScore = nScore;
					g_nRankUpdata = 2;
				}
			}
			else
			{
				g_aRankScore[3].nScore = nScore;
				g_nRankUpdata = 3;
			}
		}
		else
		{
			g_aRankScore[4].nScore = nScore;
			g_nRankUpdata = 4;
		}
	}
	else
	{
		g_nRankUpdata = -1;
	}

	FILE* pFile;
	pFile = fopen("data\\rankscore00.txt", "w");
	if (pFile != NULL)
	{
		for (nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
		{
			fprintf(pFile, "%d\n", g_aRankScore[nCntRank].nScore);
		}
		fclose(pFile);
	}
	else
	{
	}
}
//==================================
// �����L���O���ǂ�����\�����ꂽ��
//==================================
void SetRankMode(RANK_MODE mode)
{
	g_RankMode = mode;
	g_nRankUpdata = -1;
}

//
//
//
void MoveRanking(void)
{
	g_Rankpos.y = 180.0f;
	if (g_RankPosAdd >= 950.0f && g_Rank < 4)
	{
		g_Rank++;
		g_RankPosAdd = 0;
		g_ScorePosAdd = 0;
	}

	if (g_RankPosAdd <= 950.0f)
	{
		g_RankPosAdd += 40;
		g_ScorePosAdd += 40;
	}
	Ranking();
}
void Ranking(void)
{
	//���_�o�b�t�@�̐����E���_���̐ݒ�
	VERTEX_2D* pVtx;
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);

	VERTEX_2D* pVtx1;
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRankScore->Lock(0, 0, (void**)&pVtx1, 0);

	g_Rankscorepos = D3DXVECTOR3(-500.0f, 180.0f, 0.0f);

	g_Rankpos.y += 120.0f * g_Rank;
	g_Rankscorepos.y += 120.0f * g_Rank;
	g_Rankpos.x = -600.0f;

	pVtx += 4 * g_Rank;
	pVtx1 += 4 * (g_Rank * (MAX_SCORE));
	
	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_Rankpos.x - RANK_SIZE + g_RankPosAdd, g_Rankpos.y - RANK_SIZE, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Rankpos.x + RANK_SIZE + g_RankPosAdd, g_Rankpos.y - RANK_SIZE, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Rankpos.x - RANK_SIZE + g_RankPosAdd, g_Rankpos.y + RANK_SIZE, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Rankpos.x + RANK_SIZE + g_RankPosAdd, g_Rankpos.y + RANK_SIZE, 0.0f);

	for (int nCnt = 0; nCnt < MAX_SCORE; nCnt++)
	{
		//���_���W�̐ݒ�
		pVtx1[0].pos = D3DXVECTOR3(g_Rankscorepos.x - RANK_SIZE + g_ScorePosAdd, g_Rankscorepos.y - RANK_SIZE, 0.0f);
		pVtx1[1].pos = D3DXVECTOR3(g_Rankscorepos.x + RANK_SIZE + g_ScorePosAdd, g_Rankscorepos.y - RANK_SIZE, 0.0f);
		pVtx1[2].pos = D3DXVECTOR3(g_Rankscorepos.x - RANK_SIZE + g_ScorePosAdd, g_Rankscorepos.y + RANK_SIZE, 0.0f);
		pVtx1[3].pos = D3DXVECTOR3(g_Rankscorepos.x + RANK_SIZE + g_ScorePosAdd, g_Rankscorepos.y + RANK_SIZE, 0.0f);

		g_Rankscorepos.x += RANK_SIZE * 2;

		pVtx1 += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffRank->Unlock();

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffRankScore->Unlock();
}