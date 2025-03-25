//=====================================
//
// メッシュ床表示処理[meshfield.h]
// Author:Rio Ohno
//
//=====================================
#ifndef _MESHFIELD_H
#define _MESHFIELD_H_

#include "main.h"

//マクロ定義
#define MESH_NUM_MAX (32)//メッシュフィールドの数
#define MAX_TEX_FIELD (64)//テクスチャの最大数

//ポリゴン(横)の構造体
typedef struct
{
	D3DXVECTOR3 pos;											//位置
	D3DXVECTOR3 rot;											//向き
	D3DXMATRIX mtxWorld;										//ワールドマトリックス
	LPDIRECT3DINDEXBUFFER9 IdxBuffMeshField;					//インデックスバッファへのポインタ
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffMeshfield;					//頂点情報へのポインタ
	int textype;												//テクスチャのタイプ
	int nDiviX;													//分割数x
	int nDiviY;													//分割数y
	int nDiviZ;													//分割数z
	int nWidth;													//幅
	int nHeight;												//高さ
	int nMaxVtx;												//頂点数
	int nPolyNum;												//ポリゴン数
	bool bUse;													//使用しているかどうか
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

//プロトタイプ宣言
void InitMeshfield(void);
void UninitMeshfield(void);
void UpdateMeshfield(void);
void DrawMeshfield(void);
void SetMeshfield(D3DXVECTOR3 pos,D3DXVECTOR3 rot,int textype,int nDiviX,int nDiviY,int nDiviZ,int nWidth,int nHeight);//位置、向き、テクスチャ、分割数x、分割数y、分割数z、幅、高さ
void SetfieldTexture(char *pFileName,int TexIndx);				//テクスチャ設定
//LPDIRECT3DTEXTURE9* GetfieldTexture();						//テクスチャの取得処理
LPDIRECT3DTEXTURE9 GetTexture2(int TexType);				//テクスチャの取得処理
#endif