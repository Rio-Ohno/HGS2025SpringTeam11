//==================================================================
//
//�u���b�N����<block.h>
//Author:Saeki Takuto
//
//==================================================================

#ifndef _BLOCK_H_	//���̃}�N����`������ĂȂ�������
#define _BLOCK_H_	//2�d�C���N���[�h�h�~�̃}�N����`

#include "main.h"

//�}�N����`
#define WIDTHBLOCK (40)
#define HEIGHTBLOCK (40)
#define MAX_BLOCK (128)//�e�̍ő吔
#define MAX_SPEED (5)	//�A�j���[�V�����X�s�[�h
#define TYPE_BLOCK (8)

//�e�̎��
typedef enum
{
	BLOCKTYPE_NORMAL=0,//�m�[�}���u���b�N
	BLOCKTYPE_CONVEYOR_L,//�R���x�A
	BLOCKTYPE_CONVEYOR_R,
	BLOCKTYPE_TRAMPOLINE,
	BLOCKTYPE_CONVEYOR_L_HIGH,
	BLOCKTYPE_CONVEYOR_R_HIGH,
	BLOCKTYPE_CLEARBUTTON,
	BLOCKTYPE_SOUL,
	BLOCKTYPE_MAX
}BLOCKTYPE;

//�e�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;
	float fWidth;
	float fHeight;
	int ntype;
	bool bUse;			//�g�p���Ă��邩�ǂ���
}Block;

//�v���g�^�C�v�錾
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void SetBlock(D3DXVECTOR3 pos, D3DXVECTOR3 move,float fWidth, float fHeight,int ntype);
void CollisionBlockX(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fHeight, float fWidth, Block** pBlock);
bool CollisionBlockY(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fHeight, float fWidth, Block** pBlock);
int GetNumSoul();





#endif
