//============================================================
//
//�e[bullet.cpp]
//Author:Rio Ohno
//
//============================================================

#include"bullet.h"
//#include"exploosion.h"
#include"player.h"
#include"score.h"

//�}�N����`
#define MAX_BULLET (256)                                   //�e�̍ő吔
#define BULLET_WIDTH (10)                                  //�|���S���̕�
#define BULLET_HEIGHT (10)                                 //�|���S���̍���
#define BULLET_SPEED ((float)1.0f)						   //�e�̃X�s�[�h

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;				   //�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;           //���_�o�b�t�@�ւ̃|�C���^
Bullet g_aBullet[MAX_BULLET];                              //�e�̏��
int nNumBullet = 0;											

//=============================================================================================================
//����������
//=============================================================================================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet = NULL;

	//�f�o�C�X�Ɏ擾
	pDevice = GetDevice();

	//�e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bullet000.png",
		&g_pTextureBullet);


	//�e�̏��̏�����
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);									//�ʒu�̏�����
		g_aBullet[nCntBullet].move = D3DXVECTOR3(-BULLET_SPEED*0.5f, BULLET_SPEED, 0.0f);									//�ړ��ʂ̏�����
		g_aBullet[nCntBullet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);									//�p�x�̏�����

		//�Ίp���̒������Z�o����
		g_aBullet[nCntBullet].fLengthBullet = sqrtf(BULLET_WIDTH * BULLET_WIDTH + BULLET_HEIGHT * BULLET_HEIGHT) / 2.0f;

		//�Ίp���̊p�x���Z�o����
		g_aBullet[nCntBullet].fAngleBullet = atan2f(BULLET_WIDTH, BULLET_HEIGHT);

		g_aBullet[nCntBullet].fLife = 0;															//�e�̎����̏�����
		g_aBullet[nCntBullet].bUse = false;															//�g�p���Ă��Ȃ���Ԃɂ���
	}

	nNumBullet = 0;																					//�e���̏�����

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_2D * pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//���_���W�̐ݒ�
 		pVtx[0].pos.x = 0.0f;
		pVtx[0].pos.y = 0.0f;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = 0.0f;
		pVtx[1].pos.y = 0.0f;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = 0.0f;
		pVtx[2].pos.y = 0.0f;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = 0.0f;
		pVtx[3].pos.y = 0.0f;
		pVtx[3].pos.z = 0.0f;

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

		pVtx += 4;//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���_�o�b�t�@�̃A�����b�N
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================================================
//�e�̏I������
//=============================================================================================================
void UninitBullet()
{
	//�e�N�X�`���̔j��
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//=============================================================================================================
//�e�̍X�V����
//=============================================================================================================
void UpdateBullet()
{
	int nCntBullet = NULL;
	int nCnt = 0;

	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//�e���g�p����Ă���Ƃ�

			Player* pPlayer;																//�v���C���[�ւ̃|�C���^

			//�v���C���[�̎擾
			pPlayer = GetPlayer();

			if (pPlayer->bUse == true)
			{
				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - NUM_WIDTH
					&& g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + NUM_WIDTH
					&& g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - NUM_HEIGHT
					&& g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + NUM_HEIGHT
					)
				{
					ReflectBullet(nCntBullet);
				}
			}
		}

		//g_aBullet[nCntBullet].move.y += g_aBullet[nCntBullet].move.y * sinf(g_aBullet[nCntBullet].rot.z);
		//g_aBullet[nCntBullet].move.x += g_aBullet[nCntBullet].move.x * cosf(g_aBullet[nCntBullet].rot.z);

		//�O�̈ʒu�̕ۑ�
		g_aBullet[nCntBullet].oldpos = g_aBullet[nCntBullet].pos;

		//�e�̈ʒu�X�V
		g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
		g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;


		//���_���W�̐ݒ�
		pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z - (D3DX_PI - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
		pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z - (D3DX_PI - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (D3DX_PI - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
		pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (D3DX_PI - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (0.0f - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
		pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (0.0f - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z - (0.0f - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
		pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z - (0.0f - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
		pVtx[3].pos.z = 0.0f;

		//if (g_aBullet[nCntBullet].pos.x<=0.0f|| g_aBullet[nCntBullet].pos.x >= SCREEN_WIDTH
		//	                                  || g_aBullet[nCntBullet].pos.y <= 0.0f|| g_aBullet[nCntBullet].pos.y >= SCREEN_HEIGHT)//�e����ʊO�ɂł�
		//{
		//}

		//g_aBullet[nCntBullet].fLife --;//�����f�N�������g

		if (g_aBullet[nCntBullet].fLife <= 0)//�������s����
		{
			g_aBullet[nCntBullet].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���
			nNumBullet--;
		}
		pVtx += 4;
	}
	//�A�����b�N
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================================================
//�e�̕`�揈��
//=============================================================================================================
void DrawBullet()
{
	int nCntBullet;
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//�e���g�p����Ă���

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBullet);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,nCntBullet*4,2);
		}
	}
}

//=============================================================================================================
//�e�̐ݒ菈��
//=============================================================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fLength,float fLife)
{
	VERTEX_2D* pVtx;
	int nCntBullet;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{
			//�e���g�p����Ă��Ȃ��Ƃ�
			g_aBullet[nCntBullet].pos = pos;
			g_aBullet[nCntBullet].rot = rot;
			g_aBullet[nCntBullet].fLife = fLife;

			//�ڕW�̈ړ������i�p�x�j�̕␳
			if (g_aBullet[nCntBullet].rot.z > D3DX_PI)
			{
				g_aBullet[nCntBullet].rot.z -= D3DX_PI * 2.0f;
			}
			else if (g_aBullet[nCntBullet].rot.z < -D3DX_PI)
			{
				g_aBullet[nCntBullet].rot.z += D3DX_PI * 2.0f;
			}

			//���_���W�̐ݒ�
			pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z - (D3DX_PI - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
			pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z - (D3DX_PI - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (D3DX_PI - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
			pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (D3DX_PI - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (0.0f - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
			pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (0.0f - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z - (0.0f - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
			pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z - (0.0f - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
			pVtx[3].pos.z = 0.0f;

			g_aBullet[nCntBullet].fLengthBullet = fLength;
			g_aBullet[nCntBullet].bUse = true;//�g�p���Ă��Ԃɂ���
			nNumBullet++;//�e�̐��J�E���g
			break;
		}
		pVtx += 4;
	}
	//�A�����b�N
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================================================
//�e�̐��̎擾
//=============================================================================================================
int GetNumBullet()
{
	return nNumBullet;
}

//=============================================================================================================
//�e�̔��ˏ���
//=============================================================================================================
void ReflectBullet(int indx)
{//�v���C���[�Ƃ̓����蔻��
	//�v���C���[�̎擾
	Player *pPlayer = GetPlayer();

		D3DXVECTOR3 vecplayer = D3DXVECTOR3(pPlayer->pos.x - NUM_WIDTH - pPlayer->pos.x + NUM_WIDTH, pPlayer->pos.y - NUM_HEIGHT - pPlayer->pos.y + NUM_HEIGHT, 0.0f);
		D3DXVECTOR3 vecBullet = g_aBullet[indx].pos - g_aBullet[indx].oldpos;
		D3DXVECTOR3 nor;
		D3DXVECTOR3 c, e, d, f, g = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		float Dot;

		D3DXVec3Cross(&nor, &vecplayer, &vecBullet);

		//�t�x�N�g���ɂ���
		vecBullet *= -1.0f;

		//����
		Dot = D3DXVec3Dot(&vecBullet, &nor);
		c = vecBullet + Dot * nor;
		e = vecBullet + 2 * Dot * nor;

		//�����蔻��(����)
	if (g_aBullet[indx].pos.x >= pPlayer->pos.x - NUM_WIDTH
		&& g_aBullet[indx].pos.x <= pPlayer->pos.x + NUM_WIDTH)
	{
		if (g_aBullet[indx].oldpos.y <=  pPlayer->pos.y - NUM_HEIGHT && g_aBullet[indx].pos.y > pPlayer->pos.y - NUM_HEIGHT)
		{
			g_aBullet[indx].move.y = e.y;
		}
		else if (g_aBullet[indx].oldpos.y >= pPlayer->pos.y + NUM_HEIGHT && g_aBullet[indx].pos.y < pPlayer->pos.y + NUM_HEIGHT)
		{
			g_aBullet[indx].move.y = e.y;
		}
	}
	//�����蔻��(����)
	if (g_aBullet[indx].pos.y >= pPlayer->pos.y - NUM_HEIGHT
		&& g_aBullet[indx].pos.y <= pPlayer->pos.y + NUM_HEIGHT)
	{
		if (g_aBullet[indx].oldpos.x <= pPlayer->pos.x - NUM_WIDTH && g_aBullet[indx].pos.x > pPlayer->pos.x - NUM_WIDTH)
		{
			g_aBullet[indx].move.x = e.x;
			g_aBullet[indx].move.y = e.y;
		}
		else if (g_aBullet[indx].oldpos.x >= pPlayer->pos.x + NUM_WIDTH && g_aBullet[indx].pos.x < pPlayer->pos.x + NUM_WIDTH)
		{
			g_aBullet[indx].move.x = e.x;
			g_aBullet[indx].move.y = e.y;
		}
	}
}