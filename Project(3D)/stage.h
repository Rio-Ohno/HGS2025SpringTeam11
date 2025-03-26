//==================================================================
//
//ステージ処理<stage.h>
//Author:Saeki Takuto
//
//==================================================================
#ifndef _STAGE_H_
#define _STAGE_H_

#include "main.h"

#define MAX_STAGE (6)
#define MAX_WORD (4096)

typedef struct
{
	D3DXVECTOR3 pos;//位置;
	D3DXVECTOR3 move;//位置;
	float fWidth;//横幅
	float fHeight;//縦幅
	int nType;
}Stage;

//プロトタイプ宣言
void InitStage(void);
void UninitStage(void);
void UpdateStage(void);
void LoadStage(int nStage);

#endif
