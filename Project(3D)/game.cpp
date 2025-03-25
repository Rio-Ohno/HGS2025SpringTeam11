//================================
//
//  ゲーム画面表示処理[game.cpp]
//  Author:kaiti
//
//================================

#include "game.h"
#include "result.h"
#include "camera.h"
#include "light.h"
#include "input.h"
//#include "player.h"
#include "shadow.h"
#include "meshwall.h"
#include "meshfield.h"
#include "fade.h"
#include "pause.h"
#include "sound.h"
#include <time.h>
#include "mouse.h"
#include "meshsphere.h"
#include "score.h"
#include <cassert>
#include "resultinfo.h"

bool isState(GAMESTATE State);
bool isUpdateGameCondition();

//void UpdateEditer();

//グローバル変数
GAMESTATE g_gamestate = GAMESTATE_NONE;
int g_nCounterGameState = 0;
bool g_bPause = false;  //ポーズ中かどうか
bool bAbo = false;//全滅フラグ
bool g_bTutorial = true;
bool g_bOption = false;

//=============
// 初期化処理
//=============
void InitGame(void)
{
	MODE Mode = GetMode();
	//メッシュフィールドの初期化
	InitMeshfield();
	SetMeshfield(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 10, 0, 10, 100, 100);

	// 球体の初期化
	InitSphere();

	//影の初期化
	InitShadow();

	//メッシュ壁の初期化
	InitMeshWall();

	//カメラの初期化
	InitCamera();

	//ライトの初期化
	InitLight();

	//ポーズの初期化
	InitPause();

	// 球体の設定処理
	SetSphere(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 16, 5, 2000.0f, false, true);

	////プレイヤーの情報取得
	//Player* pPlayer = GetPlayer();

	//各初期化
	g_gamestate = GAMESTATE_NORMAL;		//ゲームステート
	g_nCounterGameState = 0;			//ステートカウンター
	srand((int)time(0));				//シード値(アイテムrand)
	g_bPause = false;					//ポーズしていない状態へ
}

//===========
// 終了処理
//===========
void UninitGame(void)
{
	//ポーズの終了処理
	UninitPause();

	//影の終了処理
	UninitShadow();

	//メッシュ壁の終了処理
	UninitMeshWall();

	//カメラの終了処理
	UninitCamera();

	//ライトの終了処理
	UninitLight();

	//メッシュフィールドの終了処理
	UninitMeshfield();

	UninitSphere();
}

//===========
// 更新処理
//===========
void UpdateGame(void)
{
	MODE Mode = GetMode();
	//Option* pOption = GetOption();

	//if (SetMasterVolume(pOption->Sound) == false)
	//{
	//	HWND hWnd = GetActiveWindow();
	//	MessageBox(hWnd, "マスターボリュームの設定に失敗", "警告！", MB_ICONWARNING);
	//}

	if ((KeyboardTrigger(DIK_P) == true || GetJoypadTrigger(JOYKEY_START) == true) && g_bOption == false)
	{//ポーズキーが押された
		//PlaySound(SOUND_LABEL_DESICION);
		g_bPause = g_bPause ? false : true;
	}

	if (isUpdateGameCondition() == true)
	{
		if (g_bPause == true)
		{//ポーズ中
			//ポーズの更新処理
			g_gamestate = GAMESTATE_PAUSE;
			UpdatePause(0);
			g_bTutorial = false;
		}

		else if (g_bPause == false && g_bTutorial == false && g_bOption == false)
		{
			//メッシュフィールドの更新処理
			UpdateMeshfield();

			UpdateSphere();

			//影の更新処理
			UpdateShadow();


#ifdef _DEBUG

			//リザルトに飛ぶ
			if (KeyboardTrigger(DIK_1) == true)
			{//Clear
				SetFade(MODE_RESULT);
				SetResult(RESULT_CLEAR);
			}
			if (KeyboardTrigger(DIK_2) == true)
			{//over
				SetFade(MODE_RESULT);
				SetResult(RESULT_GAMEOVER);
			}

#endif // DEBUG

			//メッシュ壁の更新処理
			UpdateMeshWall();

			//カメラの更新処理
			UpdateCamera();

			UpdateGameCamera();

			//ライトの更新処理
			UpdateLight();

			//敵の数を取得する
			MODE mode = GetMode();

			////全てのwaveが終わったなら
			//if (GetFinish() == true)
			//{
			//	g_gamestate = GAMESTATE_CLEAR;
			//}

			//ゲームステート管理
			switch (g_gamestate)
			{
			case GAMESTATE_NORMAL:

				break;

			case GAMESTATE_CLEAR:

				g_nCounterGameState++;
				if (g_nCounterGameState >= 30)
				{
					g_gamestate = GAMESTATE_NONE;
					//モードをリザルトにする
					SetFade(MODE_RESULT);
					SetResult(RESULT_CLEAR);

					//if (pPlayer->Status.fHP == PLAYER_HP)
					//{
					//	AddScore(10000);
					//}
					//else if (pPlayer->Status.fHP >= PLAYER_HP / 2)
					//{
					//	AddScore(5000);
					//}

					StopSound();
					//PlaySound(SOUND_LABEL_GAMECLEAR);
				}
				break;

			case GAMESTATE_GAMEOVER:

				g_nCounterGameState++;
				if (g_nCounterGameState >= 40)
				{
					g_gamestate = GAMESTATE_NONE;
					//モードをリザルトにする
					SetFade(MODE_RESULT);
					SetResult(RESULT_GAMEOVER);

					StopSound();
					//PlaySound(SOUND_LABEL_GAMEOVER);
				}
				break;

			case GAMESTATE_EFFECTEDITER:

				break;
			}
		}
	}
}

//===========
// 描画処理
//===========
void DrawGame(void)
{
	//カメラの設定処理
	SetCamera();

	//メッシュフィールドの描画処理
	DrawMeshfield();

	DrawSphere();

	DrawSphere();

	//影の描画処理
	DrawShadow();

	//メッシュ壁の描画処理
	DrawMeshWall();

	//ポーズしているなら
	if (g_bPause == true)
	{//ポーズ中
		//ポーズの描画処理
		DrawPause();
	}
}

//===============
// ゲームの状態
//===============
void SetGameState(GAMESTATE state)
{
	g_gamestate = state;
}

//=====================
// ゲームの状態の取得
//=====================
GAMESTATE GetGameSatate(void)
{
	return g_gamestate;
}

//=======================
// ポーズの有効無効設定
//=======================
void SetEnablePause(bool bPause)
{
	g_bPause = bPause;
}

//
//
//
void SetEnableTutorial(bool bTutorial)
{
	g_bTutorial = bTutorial;
}

bool isState(GAMESTATE State)
{
	return g_gamestate == State;
}

bool isUpdateGameCondition()
{
	return isState(GAMESTATE_EFFECTEDITER) == false && isState(GAMESTATE_CAMERAEDITER) == false;
}

//void UpdateEditer()
//{
//	if (isState(GAMESTATE_EFFECTEDITER) == true)
//	{
//		UpdateEffect();
//	}
//	else if (isState(GAMESTATE_CAMERAEDITER) == true)
//	{
//		UpdateCameraEditer();
//		CameraMove();
//	}
//}
//
//
//
void SetEnableOption(bool bOption)
{
	g_bOption = bOption;
}