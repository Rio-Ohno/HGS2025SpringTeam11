//==================================================================
//
//ステージエディット処理<edit.h>
//Author:Saeki Takuto
//
//==================================================================
#ifndef _EDIT_H_
#define _EDIT_H_

#include "main.h"
#include "block.h"

#define MAX_STAGE (3)
#define MAX_WORD (4096)

//プロトタイプ宣言
void InitEdit(void);
void UninitEdit(void);
void UpdateEdit(void);
void DrawEdit(void);
void LoadEdit(void);
void SaveEdit(void);


#endif