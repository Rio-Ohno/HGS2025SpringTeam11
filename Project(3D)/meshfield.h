//=====================================
//
// ���b�V�����\������[meshfield.h]
// Author:Rio Ohno
//
//=====================================
#ifndef _MESHFIELD_H
#define _MESHFIELD_H_

#include "main.h"

//�}�N����`
#define MESH_NUM_MAX (32)//���b�V���t�B�[���h�̐�
#define MAX_TEX_FIELD (64)//�e�N�X�`���̍ő吔

//�|���S��(��)�̍\����
typedef struct
{
	D3DXVECTOR3 pos;											//�ʒu
	D3DXVECTOR3 rot;											//����
	D3DXMATRIX mtxWorld;										//���[���h�}�g���b�N�X
	LPDIRECT3DINDEXBUFFER9 IdxBuffMeshField;					//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffMeshfield;					//���_���ւ̃|�C���^
	int textype;												//�e�N�X�`���̃^�C�v
	int nDiviX;													//������x
	int nDiviY;													//������y
	int nDiviZ;													//������z
	int nWidth;													//��
	int nHeight;												//����
	int nMaxVtx;												//���_��
	int nPolyNum;												//�|���S����
	bool bUse;													//�g�p���Ă��邩�ǂ���
}MeshField;

typedef enum
{
	TEX_SKY=0,
	TEX_MOUNTAIN,
	TEX_GROUND,
	TEX_STREET,
	TEX_STREETCROSS,
	TEX_WALL,
	TEX_TREE,
	TEX_DEADTREE01,
	TEX_DEADTREE02,
	TEX_DEADTREE03,
	TEX_GRASS,
	TEX_MAX
}TEX_TYPE;

//�v���g�^�C�v�錾
void InitMeshfield(void);
void UninitMeshfield(void);
void UpdateMeshfield(void);
void DrawMeshfield(void);
void SetMeshfield(D3DXVECTOR3 pos,D3DXVECTOR3 rot,int textype,int nDiviX,int nDiviY,int nDiviZ,int nWidth,int nHeight);//�ʒu�A�����A�e�N�X�`���A������x�A������y�A������z�A���A����
void SetfieldTexture(char *pFileName,int TexIndx);				//�e�N�X�`���ݒ�
//LPDIRECT3DTEXTURE9* GetfieldTexture();						//�e�N�X�`���̎擾����
LPDIRECT3DTEXTURE9 GetTexture2(int TexType);				//�e�N�X�`���̎擾����
#endif