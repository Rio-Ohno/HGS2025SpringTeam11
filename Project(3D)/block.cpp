//==================================================================
//
//�u���b�N����<block.cpp>
//Author:Saeki Takuto
//
//==================================================================

#include "block.h"
#include "main.h"
#include "Block.h"
#include "input.h"
#include "player.h"
#include "stage.h"

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureBlock[TYPE_BLOCK] = {};		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;//���_�o�b�t�@�ւ̃|�C���^
Block g_aBlock[MAX_BLOCK];					//�e�̏��
int nNowStage;

//�e�̏���������
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBlock;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/block000.jpg",//�ʏ�u���b�N
		&g_pTextureBlock[0]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/block001.png",//���x���R���u���b�N
		&g_pTextureBlock[1]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/block002.png",//�E�x���R���u���b�N
		&g_pTextureBlock[2]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/block003.png",//�g�����|�����u���b�N
		&g_pTextureBlock[3]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/block004.png",//���x���R�������u���b�N
		&g_pTextureBlock[4]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/block005.png",//�E�x���R�������u���b�N
		&g_pTextureBlock[5]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/block006.png",//�N���A�{�^���u���b�N
		&g_pTextureBlock[6]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/block007.png",//�N���A�{�^���u���b�N
		&g_pTextureBlock[7]);

	//�e�̏��̏�����
	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		g_aBlock[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].fWidth = 0.0f;
		g_aBlock[nCntBlock].fHeight = 0.0f;
		g_aBlock[nCntBlock].bUse = false;//�g�p���Ă��Ȃ���Ԃɂ���

	}
	nNowStage = 0;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlock,
		NULL);

	VERTEX_2D* pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].fWidth / 2), g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].fHeight / 2), g_aBlock[nCntBlock].pos.z);
		pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth / 2), g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].fHeight / 2), g_aBlock[nCntBlock].pos.z);
		pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].fWidth / 2), g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHeight / 2), g_aBlock[nCntBlock].pos.z);
		pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth / 2), g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHeight / 2), g_aBlock[nCntBlock].pos.z);

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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBlock->Unlock();
}

//�e�̏I������
void UninitBlock(void)
{
	for (int nCnt = 0; nCnt < TYPE_BLOCK; nCnt++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureBlock[nCnt] != NULL)
		{
			g_pTextureBlock[nCnt]->Release();
			g_pTextureBlock[nCnt] = NULL;
		}
	}
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}
}

//�e�̍X�V����
void UpdateBlock(void)
{
	int nCntBlock;
	Player* pPlayer = GetPlayer();

	VERTEX_2D* pVtx;							//���_���ւ̃|�C���^

	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			g_aBlock[nCntBlock].pos += g_aBlock[nCntBlock].move;
		}

		//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].fWidth / 2), g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].fHeight / 2), g_aBlock[nCntBlock].pos.z);
		pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth / 2), g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].fHeight / 2), g_aBlock[nCntBlock].pos.z);
		pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].fWidth / 2), g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHeight / 2), g_aBlock[nCntBlock].pos.z);
		pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth / 2), g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHeight / 2), g_aBlock[nCntBlock].pos.z);

		pVtx += 4;
	}


	if (pPlayer->bGoal == true)
	{
		nNowStage++;
		if (nNowStage > MAX_STAGE)
		{
			SetFade(MODE_RESULT);
		}
		else
		{
			for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
			{
				g_aBlock[nCntBlock].bUse = false;
			}

			LoadStage(nNowStage);

			pPlayer->bGoal = false;
		}
	}
	else if (pPlayer->bFall == true)
	{
			for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
			{
				g_aBlock[nCntBlock].bUse = false;
			}

			LoadStage(nNowStage);

			pPlayer->bFall = false;
	}


	g_pVtxBuffBlock->Unlock();
}

//�e�̕`�揈��
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBlock;

	//�f�o�C�X�̏���
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBlock, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{//�e���g�p����Ă���

			if (g_aBlock[nCntBlock].ntype == BLOCKTYPE_NORMAL)
			{
				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pTextureBlock[0]);
			}
			else if (g_aBlock[nCntBlock].ntype == BLOCKTYPE_CONVEYOR_L)
			{
				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pTextureBlock[1]);
			}
			else if (g_aBlock[nCntBlock].ntype == BLOCKTYPE_CONVEYOR_R)
			{
				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pTextureBlock[2]);
			}
			else if (g_aBlock[nCntBlock].ntype == BLOCKTYPE_TRAMPOLINE)
			{
				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pTextureBlock[3]);
			}
			else if (g_aBlock[nCntBlock].ntype == BLOCKTYPE_CONVEYOR_L_HIGH)
			{
				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pTextureBlock[4]);
			}
			else if (g_aBlock[nCntBlock].ntype == BLOCKTYPE_CONVEYOR_R_HIGH)
			{
				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pTextureBlock[5]);
			}
			else if (g_aBlock[nCntBlock].ntype == BLOCKTYPE_CLEARBUTTON)
			{
				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pTextureBlock[6]);
			}
			else if (g_aBlock[nCntBlock].ntype == BLOCKTYPE_SOUL)
			{
				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pTextureBlock[7]);
			}


			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				4 * nCntBlock,//���_�C���f�b�N�X
				2);
		}
	}
}

//�e�̐ݒ菈��
void SetBlock(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight, int ntype)
{
	int nCntBlock;
	VERTEX_2D* pVtx;							//���_���ւ̃|�C���^

	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);


	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == false)
		{//�e���g�p����Ă��Ȃ�
			g_aBlock[nCntBlock].pos = pos;
			g_aBlock[nCntBlock].move = move;
			g_aBlock[nCntBlock].fWidth = fWidth;
			g_aBlock[nCntBlock].fHeight = fHeight;
			g_aBlock[nCntBlock].ntype = ntype;

			g_aBlock[nCntBlock].bUse = true;//�g�p���Ă����Ԃɂ���

			//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
			pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].fWidth / 2), g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].fHeight / 2), g_aBlock[nCntBlock].pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth / 2), g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].fHeight / 2), g_aBlock[nCntBlock].pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].fWidth / 2), g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHeight / 2), g_aBlock[nCntBlock].pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth / 2), g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHeight / 2), g_aBlock[nCntBlock].pos.z);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(fWidth / WIDTHBLOCK, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, fHeight / HEIGHTBLOCK);
			pVtx[3].tex = D3DXVECTOR2(fWidth / WIDTHBLOCK, fHeight / HEIGHTBLOCK);

			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffBlock->Unlock();

}

//�u���b�N�����蔻��(X)
void CollisionBlockX(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fHeight, float fWidth,Block **pBlock)
{
	bool bLanding = false;//���n���Ă��邩�ǂ���

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			//�v���C���[��y�͈̔͂��u���b�N�ɏd�Ȃ��Ă��鍶�E�̓����蔻��
			if (pPosOld->y > g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHeight / 2 && pPosOld->y - PLAYER_HEIGHT < g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight / 2)
			{
				//������E�Ƀu���b�N���߂荞�񂾏ꍇ
				if (pPosOld->x + PLAYER_WIDTH / 2 + g_aBlock[nCntBlock].move.x <= g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].fWidth / 2) + pMove->x && pPos->x + PLAYER_WIDTH / 2 > g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].fWidth / 2))
				{
					if (g_aBlock[nCntBlock].ntype == BLOCKTYPE_SOUL)
					{
						g_aBlock[nCntBlock].bUse = false;
					}
					else
					{
						pPos->x = g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fWidth / 2 - PLAYER_WIDTH / 2;
						pMove->x = 0.0f;
						*pBlock = &g_aBlock[nCntBlock];
					}
				}
				//�E���獶�Ƀu���b�N���߂荞�񂾏ꍇ
				else if (pPosOld->x - PLAYER_WIDTH / 2 + g_aBlock[nCntBlock].move.x >= g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth / 2) + pMove->x && pPos->x - PLAYER_WIDTH / 2 < g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth / 2))
				{
					if (g_aBlock[nCntBlock].ntype == BLOCKTYPE_SOUL)
					{
						g_aBlock[nCntBlock].bUse = false;
					}
					else
					{
						pPos->x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth / 2 + PLAYER_WIDTH / 2;
						pMove->x = 0.0f;
						*pBlock = &g_aBlock[nCntBlock];
					}
				}
			}

		}
	}
}

//�u���b�N�̓����蔻��(Y)
bool CollisionBlockY(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fHeight, float fWidth,Block **pBlock)
{
	bool bLanding = false;//���n���Ă��邩�ǂ���

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			//�v���C���[��x�͈̔͂��u���b�N�ɏd�Ȃ��Ă���㉺�̓����蔻��
			if (pPos->x + PLAYER_WIDTH / 2 > g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fWidth / 2 && pPos->x - PLAYER_WIDTH / 2 < g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth / 2)
			{
				//�ォ�牺�Ƀu���b�N���߂荞�񂾏ꍇ
				if (pPosOld->y <= g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHeight / 2 && pPos->y > g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHeight / 2)
				{
					if (g_aBlock[nCntBlock].ntype == BLOCKTYPE_SOUL)
					{
						g_aBlock[nCntBlock].bUse = false;
					}
					else
					{

						bLanding = true;
						pPos->y = g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHeight / 2;
						pMove->y = 0.0f;
						*pBlock = &g_aBlock[nCntBlock];
					}
				}
				//�������Ƀu���b�N���߂荞�񂾏ꍇ
				else if (pPosOld->y - PLAYER_HEIGHT >= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight / 2 && pPos->y - PLAYER_HEIGHT < g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight / 2)
				{
					if (g_aBlock[nCntBlock].ntype == BLOCKTYPE_SOUL)
					{
						g_aBlock[nCntBlock].bUse = false;
					}
					else
					{
						pPos->y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight / 2 + PLAYER_HEIGHT;
						pMove->y = 0.0f;
					}
				}

			}

		}
	}
	return bLanding;
}

int GetNumSoul()
{
	int nCntSoul = 0;

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true&&g_aBlock[nCntBlock].ntype==BLOCKTYPE_SOUL)
		{
			nCntSoul++;
		}
	}

	return nCntSoul;
}
