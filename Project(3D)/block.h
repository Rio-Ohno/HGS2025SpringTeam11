//============================================================
//
//�V���[�e�B���O�Q�[[BLOCK.h]
//Author:Rio Ohno
//
//============================================================

#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"

//�u���b�N�̎��
typedef enum
{
	BLOCK_NULL=0,
	BLOCK_THORN,					//��
	BLOCK_LIFT,
	BLOCK_MAX
}BLOCKTYPE;

//�v���C���[�̒e�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;				//�ʒu
	D3DXVECTOR3 move;				//�ړ���
	D3DXVECTOR3 rot;				//����
	BLOCKTYPE type;					//���
	int nCnt;
	float fWidthBlock;				//�|���S���̕�
	float fHeightBlock;				//�|���S���̍���
	float fTex_x;					//�e�N�X�`����x���W
	float fTex_y;					//�e�N�X�`���̂����W
	bool bUse;						//�g�p���Ă��邩�ǂ�
}Block;

//�e�N�X�`��
static const char* BLOCK_TEXTURE[BLOCK_MAX] =
{
	"data\\TEXTURE\\block000.jpg",
	"data\\TEXTURE\\toge000.png",
	"data\\TEXTURE\\block000.jpg",
};

//�v���g�^�C�v�錾
void InitBlock();
void UninitBlock();
void UpdateBlock();
void DrawBlock();
void SetBlock(D3DXVECTOR3 pos, BLOCKTYPE type, float fWidthBlock, float fHeightBlock);
bool CollisionBlock_X(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, bool* pDash, bool* pStick, float fWidth, float fHeight);
bool CollisionBlock_Y(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, bool* pDash, float fWidth, float fHeight, Block** pBlock_X);

//============================================================================================================================================
//�f�o�b�N�p
//============================================================================================================================================
int GetNumBlock();
#endif // !_BLOCK_H