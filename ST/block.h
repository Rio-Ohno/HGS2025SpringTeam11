//==================================================================
//
//ブロック処理<block.h>
//Author:Saeki Takuto
//
//==================================================================

#ifndef _BLOCK_H_	//このマクロ定義がされてなかったら
#define _BLOCK_H_	//2重インクルード防止のマクロ定義

#include "main.h"

//マクロ定義
#define WIDTHBLOCK (40)
#define HEIGHTBLOCK (40)
#define MAX_BLOCK (128)//弾の最大数
#define MAX_SPEED (5)	//アニメーションスピード
#define TYPE_BLOCK (8)

//弾の種類
typedef enum
{
	BLOCKTYPE_NORMAL=0,//ノーマルブロック
	BLOCKTYPE_CONVEYOR_L,//コンベア
	BLOCKTYPE_CONVEYOR_R,
	BLOCKTYPE_TRAMPOLINE,
	BLOCKTYPE_CONVEYOR_L_HIGH,
	BLOCKTYPE_CONVEYOR_R_HIGH,
	BLOCKTYPE_CLEARBUTTON,
	BLOCKTYPE_SOUL,
	BLOCKTYPE_MAX
}BLOCKTYPE;

//弾構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;
	float fWidth;
	float fHeight;
	int ntype;
	bool bUse;			//使用しているかどうか
}Block;

//プロトタイプ宣言
void InitBlock(void);
void UninitBlock(void);
void UpdateBlock(void);
void DrawBlock(void);
void SetBlock(D3DXVECTOR3 pos, D3DXVECTOR3 move,float fWidth, float fHeight,int ntype);
void CollisionBlockX(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fHeight, float fWidth, Block** pBlock);
bool CollisionBlockY(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fHeight, float fWidth, Block** pBlock);
int GetNumSoul();





#endif
