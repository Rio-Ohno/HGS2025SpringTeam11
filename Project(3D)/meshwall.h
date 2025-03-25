//===================================
//
// メッシュ壁表示処理[meshWall.h]
// Author:kaiti
//
//===================================

#ifndef _MESHWALL_H
#define _MESHWALL_H_

#include "main.h"

//マクロ定義
#define MAX_MESHWALL (4)
#define MAX_TEX_WALL (64)

//影の構造体
typedef struct
{
	D3DXVECTOR3 pos;								//位置
	D3DXVECTOR3 rot;								//向き
	D3DXMATRIX mtxWorld;							//ワールドマトリックス
	LPDIRECT3DINDEXBUFFER9 IdxBuffMeshWall;			//インデックスバッファへのポインタ
	LPDIRECT3DVERTEXBUFFER9 VtxBuffMeshWall;		//頂点情報へのポインタ
	int textype;									//テクスチャのタイプ
	int nDiviX;										//分割数x
	int nDiviY;										//分割数y
	int nDiviZ;										//分割数z
	int nWidth;										//幅
	int nHeight;									//高さ
	int nMaxVtx;									//頂点数
	int nPolyNum;									//ポリゴン数
	bool bUse;										//使用しているかどうか
}MeshWall;

//プロトタイプ宣言
void InitMeshWall(void);
void UninitMeshWall(void);
void UpdateMeshWall(void);
void DrawMeshWall(void);
void SetMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int textype, int nDiviX, int nDiviZ, int nDiviY, int nWidth, int nHeight);
void SetwallTexture(int indx);			//テクスチャ設定

#endif