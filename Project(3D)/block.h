//============================================================
//
//シューティングゲー[BLOCK.h]
//Author:Rio Ohno
//
//============================================================

#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"

//ブロックの種類
typedef enum
{
	BLOCK_NULL=0,
	BLOCK_THORN,					//棘
	BLOCK_LIFT,
	BLOCK_MAX
}BLOCKTYPE;

//プレイヤーの弾構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;				//位置
	D3DXVECTOR3 move;				//移動量
	D3DXVECTOR3 rot;				//向き
	BLOCKTYPE type;					//種類
	int nCnt;
	float fWidthBlock;				//ポリゴンの幅
	float fHeightBlock;				//ポリゴンの高さ
	float fTex_x;					//テクスチャのx座標
	float fTex_y;					//テクスチャのｙ座標
	bool bUse;						//使用しているかどう
}Block;

//テクスチャ
static const char* BLOCK_TEXTURE[BLOCK_MAX] =
{
	"data\\TEXTURE\\block000.jpg",
	"data\\TEXTURE\\toge000.png",
	"data\\TEXTURE\\block000.jpg",
};

//プロトタイプ宣言
void InitBlock();
void UninitBlock();
void UpdateBlock();
void DrawBlock();
void SetBlock(D3DXVECTOR3 pos, BLOCKTYPE type, float fWidthBlock, float fHeightBlock);
bool CollisionBlock_X(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, bool* pDash, bool* pStick, float fWidth, float fHeight);
bool CollisionBlock_Y(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, bool* pDash, float fWidth, float fHeight, Block** pBlock_X);

//============================================================================================================================================
//デバック用
//============================================================================================================================================
int GetNumBlock();
#endif // !_BLOCK_H