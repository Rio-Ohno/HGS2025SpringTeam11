//==============================
//
//  ゲーム画面表示処理[game.h]
//  Author:kaiti
//
//==============================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

#define STAGE_SIZE (float)(945.0f)

//ゲームの状態
typedef enum
{
	GAMESTATE_NONE = 0,	//何もしていない状態
	GAMESTATE_NORMAL,	//通常状態（ゲームプレイ中）
	GAMESTATE_PAUSE,	//
	GAMESTATE_OPTION,	//
	GAMESTATE_CLEAR,	//クリア
	GAMESTATE_GAMEOVER,	//ゲームオーバー
	GAMESTATE_EFFECTEDITER,	//ゲームオーバー
	GAMESTATE_CAMERAEDITER,
	GAMESTATE_MAX
}GAMESTATE;

//ステージ
typedef enum
{
	STAGE_NO_0 = 0,
	STAGE_NO_1,
	STAGE_NO_2,
	STAGE_NO_3,
	STAGE_NO_MAX,
}STAGE_NO;

//プロトタイプ宣言
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetGameState(GAMESTATE state);
GAMESTATE GetGameSatate(void);
void SetEnablePause(bool bPause);
void SetEnableTutorial(bool bTutorial);
void SetEnableOption(bool bOption);

#endif