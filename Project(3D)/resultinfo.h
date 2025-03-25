//==============================
//
//  リザルト表示処理[resultinfo.h]
//  Author:kaiti
//
//==============================
#ifndef _RESULTINFO_H_
#define _RESULTINFO_H_

#include"main.h"
#include "result.h"

#define RESULT_X (400)
#define RESULT_Y (200)

//プロトタイプ宣言
void InitResultInfo(void);
void UninitResultInfo(void);
void UpdateResultInfo(void);
void DrawResultInfo(void);
void SetResult(RESULT result);

#endif
