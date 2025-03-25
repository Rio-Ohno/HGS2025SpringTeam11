//****************************
//
// モデル表示処理[model.h]
// Author:kaiti
//
//****************************
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"

#define MAX_TEX (32)			// テクスチャの最大数
#define MAX_PARTS (15)			// パーツの最大数
#define MAX_STAGEMODEL (128)	// ステージに出すモデルの最大数

// モデルの種類の列挙型
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

// OBBの構造体
typedef struct
{
	D3DXVECTOR3 CenterPos;
	D3DXVECTOR3 RotVec[3];
	FLOAT fLength[3];
	D3DXMATRIX CenterMtx;
}OBB;

//モデルの構造体
typedef struct
{
	LPD3DXMESH pMesh;		//メッシュへのポインタ
	LPD3DXBUFFER pBuffMat;	//マテリアルへのポインタ
	DWORD dwNumMat;			//マテリアルの数
	int nIdxModelParent;	//親モデルのインデックス
	int nIndx, Parent;
	D3DXVECTOR3 pos;		//位置(オフセット)
	D3DXVECTOR3 OffSetPos;
	D3DXVECTOR3 rot;		//向き
	D3DXVECTOR3 OffSetRot;
	D3DXVECTOR3 size;		//サイズ
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	LPDIRECT3DTEXTURE9 pTexture[MAX_TEX];
}MODELINFO;

// OBBのもととなる物の構造体
typedef struct
{
	LPD3DXMESH pMesh;		//メッシュへのポインタ
	LPD3DXBUFFER pBuffMat;	//マテリアルへのポインタ
	DWORD dwNumMat;			//マテリアルの数
	LPDIRECT3DTEXTURE9 pTexture[MAX_TEX];
}MODELORIGIN;

// ステージに出すモデルの構造体
typedef struct
{
	MODELTYPE nType;
	D3DXVECTOR3 pos;		//位置(オフセット)
	D3DXVECTOR3 rot;		//向き
	D3DXVECTOR3 Max;
	D3DXVECTOR3 Min;
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
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