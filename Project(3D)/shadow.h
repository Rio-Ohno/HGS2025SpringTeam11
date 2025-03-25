//================================
//
// 影表示処理[shadow.h]
// Author:kaiti
//
//================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"

#define MAX_SHADOW (128)		//影の最大数
#define LIMI_SIZE (0.25f)		//サイズの下限(何倍か)

//影の構造体
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXVECTOR3 rot;			//向き
	D3DXMATRIX mtxWorld;		//ワールドマトリックス
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffShadow;//頂点情報
	float fRadius;				//半径
	bool bUse;					//使用しているかどうか
}Shadow;

//プロトタイプ宣言
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
void DrwaShadowPlayer(int Indx,D3DXMATRIX mtxWorld);
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot,float fRadius);					//影の初期位置、向き、半径の設定
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos,bool bUse);				//影の位置更新
void SetSizeShadow(D3DXVECTOR3 pos,int nIndx);						//影のサイズ更新
Shadow* GetShadow(void);
#endif