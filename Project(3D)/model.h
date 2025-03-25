//****************************
//
// ���f���\������[model.h]
// Author:kaiti
//
//****************************
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

#define MAX_TEX (32)			// �e�N�X�`���̍ő吔
#define MAX_PARTS (15)			// �p�[�c�̍ő吔
#define MAX_STAGEMODEL (128)	// �X�e�[�W�ɏo�����f���̍ő吔

// ���f���̎�ނ̗񋓌^
typedef enum
{
	MODELTYPE_ZERO = 0,
	MODELTYPE_ONE,
	MODELTYPE_TWO,
	MODELTYPE_THREE,
	MODELTYPE_FOUR,
	MODELTYPE_FIVE,
	MODELTYPE_SIX,
	MODELTYPE_SEVEN,
	MODELTYPE_EIGHT,
	MODELTYPE_NINE,
	MODELTYPE_TEN,
	MODELTYPE_ELEVEN,
	MODELTYPE_TWELVE,
	MODELTYPE_THIRTEEN,
	MODELTYPE_MAX
}MODELTYPE;

// OBB�̍\����
typedef struct
{
	D3DXVECTOR3 CenterPos;
	D3DXVECTOR3 RotVec[3];
	FLOAT fLength[3];
	D3DXMATRIX CenterMtx;
}OBB;

//���f���̍\����
typedef struct
{
	LPD3DXMESH pMesh;		//���b�V���ւ̃|�C���^
	LPD3DXBUFFER pBuffMat;	//�}�e���A���ւ̃|�C���^
	DWORD dwNumMat;			//�}�e���A���̐�
	int nIdxModelParent;	//�e���f���̃C���f�b�N�X
	int nIndx, Parent;
	D3DXVECTOR3 pos;		//�ʒu(�I�t�Z�b�g)
	D3DXVECTOR3 OffSetPos;
	D3DXVECTOR3 rot;		//����
	D3DXVECTOR3 OffSetRot;
	D3DXVECTOR3 size;		//�T�C�Y
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	LPDIRECT3DTEXTURE9 pTexture[MAX_TEX];
}MODELINFO;

// OBB�̂��ƂƂȂ镨�̍\����
typedef struct
{
	LPD3DXMESH pMesh;		//���b�V���ւ̃|�C���^
	LPD3DXBUFFER pBuffMat;	//�}�e���A���ւ̃|�C���^
	DWORD dwNumMat;			//�}�e���A���̐�
	LPDIRECT3DTEXTURE9 pTexture[MAX_TEX];
}MODELORIGIN;

// �X�e�[�W�ɏo�����f���̍\����
typedef struct
{
	MODELTYPE nType;
	D3DXVECTOR3 pos;		//�ʒu(�I�t�Z�b�g)
	D3DXVECTOR3 rot;		//����
	D3DXVECTOR3 Max;
	D3DXVECTOR3 Min;
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	OBB ObbModel;
	bool bUse;
	bool btest;
	bool bHitRayCamera;
}STAGEMODEL;

void InitStageModel();
void UninitStageModel();
void UpdateStageModel();
void DrawStageModel();
STAGEMODEL* GetModel();
void SetStageModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, MODELTYPE nType);
void SetObbInfo(int Indx);
void SetStageModelInfo(char *ModelPath[],int nType);
void LenOBBToPoint(OBB& obb, D3DXVECTOR3& p);
void CollOBBs(OBB& obb, D3DXVECTOR3& p,int Indx);
FLOAT LenSegOnSeparateAxis(D3DXVECTOR3* Sep, D3DXVECTOR3* e1, D3DXVECTOR3* e2, D3DXVECTOR3* e3 = 0);
void DotOBBToPoint(OBB& obb, D3DXVECTOR3& p);
#endif