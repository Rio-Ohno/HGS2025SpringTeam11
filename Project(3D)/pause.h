//==============================
//
//  ポーズ処理[pause.h]
//  Author:kaiti
//
//==============================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"
#define PAUSESELECT_HEIGHT (100)
#define PAUSESELECT_WIDTH (400)

//ポーズメニュー
typedef enum
{
	PAUSE_MENU_OPTION=0,//戻る
	PAUSE_MENU_CONTNUE,//戻る
	PAUSE_MENU_RETRY,    //やり直す
	PAUSE_MENU_QUIT,     //タイトルに戻る
	PAUSE_MENU_MAX,
}PAUSE_MENU;

//プロトタイプ宣言
void InitPause(void);
void UninitPause(void);
void UpdatePause(int zDelta);
void DrawPause(void);
#endif