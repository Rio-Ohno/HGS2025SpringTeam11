//========================================================
// 
// タイマー処理[Time.h]
// Author:Rio Ohno
// 
//========================================================

#ifndef _TIME_H_
#define _TIME_H_

#include"main.h"

typedef enum
{
	TIME_CNTUP=0,//ストップウォッチ
	TIME_CNTDOWN,//タイマー
	TIME_MAX
}TIMETYPE;

//プロトタイプ宣言
void InitTime(TIMETYPE type);
void UninitTime();
void UpdateTime();
void SetTime();
void DrawTime();
bool GetTimeOver();

#endif // !_TIME_H_