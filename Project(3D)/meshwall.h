//===================================
//
// ���b�V���Ǖ\������[meshWall.h]
// Author:kaiti
//
//===================================

#ifndef _MESHWALL_H
#define _MESHWALL_H_

#include "main.h"

//�}�N����`
#define MAX_MESHWALL (4)
#define MAX_TEX_WALL (64)

//�e�̍\����
typedef struct
{
	D3DXVECTOR3 pos;								//�ʒu
	D3DXVECTOR3 rot;								//����
	D3DXMATRIX mtxWorld;							//���[���h�}�g���b�N�X
	LPDIRECT3DINDEXBUFFER9 IdxBuffMeshWall;			//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 VtxBuffMeshWall;		//���_���ւ̃|�C���^
	int textype;									//�e�N�X�`���̃^�C�v
	int nDiviX;										//������x
	int nDiviY;										//������y
	int nDiviZ;										//������z
	int nWidth;										//��
	int nHeight;									//����
	int nMaxVtx;									//���_��
	int nPolyNum;									//�|���S����
	bool bUse;										//�g�p���Ă��邩�ǂ���
}MeshWall;

//�v���g�^�C�v�錾
void InitMeshWall(void);
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(void);
void SetMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int textype, int nDiviX, int nDiviZ, int nDiviY, int nWidth, int nHeight);
void SetwallTexture(int indx);			//�e�N�X�`���ݒ�

#endif