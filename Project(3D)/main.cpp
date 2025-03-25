//================================
//
//　ウインドウ表示処理[main.cpp]
//　Author:kaiti
//
//================================
#include "main.h"
#include "input.h"
#include "fade.h"
#include "title.h"
#include "result.h"
#include "game.h"
#include "ranking.h"
#include "sound.h"
#include "camera.h"
//#include "player.h"
#include "mouse.h"
#include "titleinfo.h"
#include "pause.h"
#include <crtdbg.h>
#include <stdio.h>

//グローバル変数宣言
LPDIRECT3D9 g_pD3D = NULL;
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;
MODE g_mode = MODE_TITLE;
LPD3DXFONT g_pFont;
RECT g_windowRect; // ウィンドウ切り替え変数
bool bDispFont = true;
bool g_isFullscreen = false; // 初期状態をフルスクリーンにしない

// プロトタイプ宣言
void ToggleFullScreen(HWND hWnd); // フルスクリーン変更処理

//=============
// メイン関数
//=============
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_ LPSTR ipCmdLine, _In_ int nCmdShow)
{
	// メモリリーク検知用のフラグ
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WindowProc,
		0,
		0,
		hInstance,
		LoadIcon(NULL,IDI_APPLICATION),
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		LoadIcon(NULL,IDI_APPLICATION)
	};

	HWND hWnd;
	MSG msg;

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	//ウインドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウインドウを生成
	hWnd = CreateWindowEx(0,
		                  CLASS_NAME,
		                  WINDOW_NAME,
		                  WS_OVERLAPPEDWINDOW,
		                  CW_USEDEFAULT,
		                  CW_USEDEFAULT,
		                  (rect.right - rect.left),
		                  (rect.bottom - rect.top),
		                  NULL,
		                  NULL,
		                  hInstance,
		                  NULL);

	DWORD dwCurrentTime;
	DWORD dwExecLastTime;

	//初期化処理
	//初期化処理
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{// 初期化処理が失敗したとき
		return -1;
	}

	
	//分解能を設定
	timeBeginPeriod(1);
	dwCurrentTime = 0;
	dwExecLastTime = timeGetTime();	

	//ウインドウの表示 (フルスクリーン)
	ShowWindow(hWnd, SW_SHOWMAXIMIZED);
	UpdateWindow(hWnd);

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{//DirectXの処理
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;

				if (g_mode == MODE_END)
				{
					break;
				}

				//更新処理
				Update();
				//描画処理
				Draw();
			}
		}
	}
	//終了処理
	Uninit();

	//
	timeEndPeriod(1);

	//ウインドウクラスの解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;
}

//========================
// ウインドウプロシージャ
//========================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	switch (g_mode)
	{
	case MODE_END:
		DestroyWindow(hWnd);
		break;

	default:
		break;
	}

	switch (uMsg)
	{
	case WM_DESTROY:
		//WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN: //キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE://[ESC]キーが押された場合
			nID = MessageBox(hWnd, "終了しますか?", "終了メッセージ", MB_YESNO);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd);
			}
			else
			{
				return 0;
			}
			break;

		case VK_F11:
			// F11キーが押された
			ToggleFullScreen(hWnd);
			break;

		default:
			break;
		}
		break;

	case WM_MOUSEWHEEL:
	{
		GAMESTATE state = GetGameSatate();
		int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		//SetMouseWheel(zDelta);
		//使用する魔法の種類を変更
		if (g_mode == MODE_TITLE)
		{
			UpdateTitleInfo(zDelta);
		}
		if(state == GAMESTATE_PAUSE)
		{
			UpdatePause(zDelta);
		}
	}
		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//==================
// デバイスの取得
//==================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//=============
// 初期化処理
//=============
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// マウスカーソルの非表示
	ShowCursor(FALSE);

	D3DDISPLAYMODE d3ddm;
	D3DPRESENT_PARAMETERS d3dpp;
	//Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}
	//現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.Windowed = bWindow;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//Direct3Dデバイスの生成
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		                            D3DDEVTYPE_HAL,
		                            hWnd,
		                            D3DCREATE_HARDWARE_VERTEXPROCESSING,
		                            &d3dpp,
		                            &g_pD3DDevice)))
	{//Direct3Dデバイスの生成
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			                            D3DDEVTYPE_HAL,
			                            hWnd,
			                            D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			                            &d3dpp,
			                            &g_pD3DDevice)))
		{//Direct3Dデバイスの生成
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				                            D3DDEVTYPE_REF,
				                            hWnd,
				                            D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				                            &d3dpp,
				                            &g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

	//サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	
	//テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2,D3DTA_CURRENT);

	//------------------------------
	//各種オブジェクトの初期化処理
	//------------------------------
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	if (FAILED(InitMouse(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	if (FAILED(InitJoypad()))
	{
		return E_FAIL;
	}

	// デバック表示用フォントの生成
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0,
		FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		"Terminal", &g_pFont);

	//サウンドの初期化処理
	InitSound(hWnd);
	//
	InitRanking();
	//フェード
	InitFade(g_mode);

	return S_OK;
}

//==========
// 終了処理
//==========
void Uninit(void)
{
	//----------------------------
	//各種オブジェクトの終了処理
	//----------------------------
	//サウンドの終了処理
	UninitSound();

	UninitKeyboard();

	UninitMouse();

	UninitJoypad();

	//
	UninitFade();

	//D3Dデバイスの破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}
	//D3Dオブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

}

//==========
// 更新処理
//==========
void Update(void)
{
	//----------------------------
	//各種オブジェクトの更新処理
	//----------------------------
	UpdateMouse();
	UpdateKeyboard();
	UpdateJoypad();

	switch (g_mode)
	{
	case MODE_TITLE:
		UpdateTitle();
		break;
	case MODE_STAGEONE:
		UpdateGame();
		break;
	case MODE_STAGETWO:
		UpdateGame();
		break;
	case MODE_STAGETHREE:
		UpdateGame();
		break;
	case MODE_BOSSMOVIE:
		break;
	case MODE_STAGEFOUR:
		UpdateGame();
		break;
	case MODE_RESULT:
		UpdateResult();
		break;
	case MODE_RANK:
		UpdateRanking();
		break;
	default:
		break;
	}
	//
	UpdateFade();

#ifdef _DEBUG
	//ワイヤー
	if (GetKeyboardPress(DIK_F1) == true)
	{
		onWireFrame();
	}
	else if (GetKeyboardPress(DIK_F2) == true)
	{
		offWireFrame();
	}

	//デバックフォントの表示非表示
	if (KeyboardTrigger(DIK_F4) == true)
	{
		bDispFont = bDispFont ? false : true;
	}

#endif 
}

//==========
// 描画処理
//==========
void Draw(void)
{
	//画面クリア
	g_pD3DDevice->Clear(0, NULL,
		                (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		                D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f ,0);
	//描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//描画開始成功時
		//-----------------------------
		//各種オブジェクトの描画処理
		//-----------------------------
		switch (g_mode)
		{
		case MODE_TITLE:
			DrawTitle();
			break;
		case MODE_STAGEONE:
			DrawGame();
			break;
		case MODE_STAGETWO:
			DrawGame();
			break;
		case MODE_STAGETHREE:
			DrawGame();
			break;
		case MODE_STAGEFOUR:
			DrawGame();
			break;
		case MODE_BOSSMOVIE:
			break;
		case MODE_RESULT:
			DrawResult();
			break;
		case MODE_RANK:
			DrawRanking();
			break;
		default:
			break;
		}

#ifdef _DEBUG

		if (bDispFont == true)
		{
			DrawPlayerCollision();
			//DrawEffectEditer();
			DrawCameraInfo();
			//DrawPlayerInfo();
			//DrawTestInfo();
			//DrawBossInfo();
		}

#endif // DEBUG

		DrawFade();

		//描画終了
		g_pD3DDevice->EndScene();
	}
	//バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//==============
// モードの設定
//==============
void SetMode(MODE mode)
{
	//現在の画面の終了処理
	switch (g_mode)
	{
	case MODE_TITLE:
		UninitTitle();
		break;
	case MODE_STAGEONE:
		UninitGame();
		break;
	case MODE_STAGETWO:
		UninitGame();
		break;
	case MODE_STAGETHREE:
		UninitGame();
		break;
	case MODE_STAGEFOUR:
		UninitGame();
		break;
	case MODE_BOSSMOVIE:
		break;
	case MODE_RESULT:
		UninitResult();
		break;
	case MODE_RANK:
		UninitRanking();
		break;
	default:
		break;
	}
	MODE OldMode = g_mode;
	g_mode = mode;

	//新しい画面の初期化処理
	switch (mode)
	{
	case MODE_TITLE:
		InitTitle();
		break;
	case MODE_STAGEONE:
		InitGame();
		break;
	case MODE_STAGETWO:
		InitGame();
		break;
	case MODE_STAGETHREE:
		InitGame();
		break;
	case MODE_STAGEFOUR:
		InitGame();
		break;
	case MODE_BOSSMOVIE:
		break;
	case MODE_RESULT:
		InitResult(OldMode);
		break;
	case MODE_RANK:
		InitRanking();
		break;
	case MODE_END:
		break;
	default:
		break;
	}
}

//=============
// モード取得
//=============
MODE GetMode(void)
{
	return g_mode;
}

//===========================
//プレイヤーの当たり判定表示
//===========================
void DrawPlayerCollision()
{
	//PARTICLEEDITER* pEditer = GetParticleInfo();
	//RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	//char aStr[256];
	//Player* pPlayer = GetPlayer();

	//// 文字列に代入
	//sprintf(&aStr[0], "当たっているか:%d\nプレイヤーのポス:%3.2f,%3.2f,%3.2f",pPlayer->btest,pPlayer->pos.x, pPlayer->pos.y, pPlayer->pos.z);

	//// テキスト表示
	//g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(200, 255, 0, 255));
}

////============================
////エフェクトエディターの表示
////============================
//void DrawEffectEditer()
//{
//	PARTICLEEDITER* pEditer = GetParticleInfo();
//	RECT rect = { 0,40,SCREEN_WIDTH,SCREEN_HEIGHT };
//	char aStr[256];
//
//	// 文字列に代入
//	sprintf(&aStr[0], "*****************PARTICLEEDITER*****************\nカラー変更:R,G,B\n放射角変更:X,Y,Z\n粒子の数変更:L\n寿命変更:H\nそれぞれSHIFT押しながらやると減少");
//
//	// テキスト表示
//	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(200, 255, 0, 255));
//
//	rect = { 0,160,SCREEN_WIDTH,SCREEN_HEIGHT };
//
//	// 文字列に代入
//	sprintf(&aStr[0], "R,G,B:%3.2f,%3.2f,%3.2f\nX,Y,Z:%3.2f,%3.2f,%3.2f\nエフェクトの最大数:%d\nエフェクトの寿命:%d\n粒子の大きさX,Y,Z:%3.2f,%3.2f,%3.2f", pEditer->ParticleInfo.col.r,
//		pEditer->ParticleInfo.col.g, 
//		pEditer->ParticleInfo.col.b,
//		pEditer->ParticleInfo.dir.x,
//		pEditer->ParticleInfo.dir.y, 
//		pEditer->ParticleInfo.dir.z,
//		pEditer->ParticleInfo.forlimit,
//		pEditer->ParticleInfo.nLife,
//		pEditer->ParticleInfo.Scale.x,
//		pEditer->ParticleInfo.Scale.y, 
//		pEditer->ParticleInfo.Scale.z);
//
//	// テキスト表示
//	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(200, 255, 0, 255));
//}

//==================
//カメラの情報表示
//==================
void DrawCameraInfo()
{
	Camera* pCamera = GetCamera();

	RECT rect = { 0,260,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	// 文字列に代入
	sprintf(&aStr[0], "*****************カメラ情報*****************");

	// テキスト表示
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(200, 255, 0, 255));

	rect = { 0,280,SCREEN_WIDTH,SCREEN_HEIGHT };

	// 文字列に代入
	sprintf(&aStr[0], "カメラの視点の向き:%3.2f,%3.2f,%3.2f", pCamera->rot.x,
		pCamera->rot.y,
		pCamera->rot.z);

	// テキスト表示
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(200, 255, 0, 255));



}

////======================
////プレイヤーの情報表示
////======================
//void DrawPlayerInfo()
//{
//	Player* pPlayer = GetPlayer();
//
//	RECT rect = { 0,300,SCREEN_WIDTH,SCREEN_HEIGHT };
//	char aStr[256];
//
//	// 文字列に代入
//	sprintf(&aStr[0], "*****************プレイヤー情報*****************");
//
//	// テキスト表示
//	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(200, 255, 0, 255));
//
//	rect = { 0,320,SCREEN_WIDTH,SCREEN_HEIGHT };
//
//	// 文字列に代入
//	sprintf(&aStr[0], "プレイヤーの向き:%3.2f,%3.2f,%3.2f", pPlayer->rot.x,
//		pPlayer->rot.y,
//		pPlayer->rot.z);
//
//	// テキスト表示
//	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(200, 255, 0, 255));
//
//	rect = { 0,340,SCREEN_WIDTH,SCREEN_HEIGHT };
//
//	// 文字列に代入
//	sprintf(&aStr[0], "プレイヤーの目標の向き:%3.2f,%3.2f,%3.2f", pPlayer->rotDest.x,
//		pPlayer->rotDest.y,
//		pPlayer->rotDest.z);
//
//	// テキスト表示
//	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(200, 255, 0, 255));
//
//	rect = { 0,360,SCREEN_WIDTH,SCREEN_HEIGHT };
//
//	// 文字列に代入
//	sprintf(&aStr[0], "プレイヤーの位置:%3.2f,%3.2f,%3.2f", pPlayer->pos.x,
//		pPlayer->pos.y,
//		pPlayer->pos.z);
//
//	// テキスト表示
//	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(200, 255, 0, 255));
//
//	rect = { 0,380,SCREEN_WIDTH,SCREEN_HEIGHT };
//
//	// 文字列に代入
//	sprintf(&aStr[0], "プレイヤーの昔の位置:%3.2f,%3.2f,%3.2f", pPlayer->posOld.x,
//		pPlayer->posOld.y,
//		pPlayer->posOld.z);
//
//	// テキスト表示
//	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(200, 255, 0, 255));
//
//	rect = { 0,400,SCREEN_WIDTH,SCREEN_HEIGHT };
//
//	//// 文字列に代入
//	//sprintf(&aStr[0], "プレイヤーの次のキー:%d", pPlayer->PlayerMotion.NextKey);
//
//		// 文字列に代入
//	sprintf(&aStr[0], "プレイヤーのステータス:%d", pPlayer->state);
//
//	// テキスト表示
//	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(200, 255, 0, 255));
//
//	rect = { 0,420,SCREEN_WIDTH,SCREEN_HEIGHT };
//
//	// 文字列に代入
//	sprintf(&aStr[0], "プレイヤーの今のモーション:%d", pPlayer->PlayerMotion.motionType);
//
//	// テキスト表示
//	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(200, 255, 0, 255));
//}
//void DrawTestInfo()
//{
//	D3DXVECTOR2 test = Gettest();
//	RECT rect = { 0,440,SCREEN_WIDTH,SCREEN_HEIGHT };
//	char aStr[256];
//	int *Num = GetNumEnemy();
//	// 文字列に代入
//	sprintf(&aStr[0], "敵の総数:%d", *Num);
//
//	// テキスト表示
//	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(200, 255, 0, 255));
//}
//void DrawBossInfo()
//{
//	BOSS* pBoss = GetBoss();
//	D3DXVECTOR2 test = Gettest();
//	RECT rect = { 0,460,SCREEN_WIDTH,SCREEN_HEIGHT };
//	char aStr[256];
//
//	// 文字列に代入
//	sprintf(&aStr[0], "ボスの優先度:%3.2f,%3.2f,%3.2f", pBoss->BossAi.primary[0], 
//		pBoss->BossAi.primary[1],
//		pBoss->BossAi.primary[2]);
//
//	// テキスト表示
//	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(200, 255, 0, 255));
//
//	rect = { 0,480,SCREEN_WIDTH,SCREEN_HEIGHT };
//
//	// 文字列に代入
//	sprintf(&aStr[0], "ボスの行動パターン:%d", pBoss->ActionType);
//
//	// テキスト表示
//	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(200, 255, 0, 255));
//}
//=============
// ワイヤー
//=============
void onWireFrame()
{
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
}
void offWireFrame()
{
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

//*******************************
// ラジアン値からPIに変換する処理
//*******************************
D3DXVECTOR3* PitoRadian(D3DXVECTOR3* rot)
{
	// ラジアン値からPIに変換
	rot->x = rot->x * TOPI;
	rot->y = rot->y * TOPI;
	rot->z = rot->z * TOPI;

	// 角度の正規化(X)
	if (rot->x >= D3DX_PI)
	{
		rot->x -= D3DX_PI * 2.0f;
	}
	else if (rot->x <= -D3DX_PI)
	{
		rot->x += D3DX_PI * 2.0f;
	}
	// 角度の正規化(Y)
	if (rot->y >= D3DX_PI)
	{
		rot->y -= D3DX_PI * 2.0f;
	}
	else if (rot->y <= -D3DX_PI)
	{
		rot->y += D3DX_PI * 2.0f;
	}
	// 角度の正規化(Z)
	if (rot->z >= D3DX_PI)
	{
		rot->z -= D3DX_PI * 2.0f;
	}
	else if (rot->z <= -D3DX_PI)
	{
		rot->z += D3DX_PI * 2.0f;
	}

	return rot;
}
//****************************************
// ウィンドウをフルスクリーンにする処理
//****************************************
void ToggleFullScreen(HWND hWnd)
{
	// 現在のウィンドウスタイルを取得
	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);
	if (g_isFullscreen)
	{
		// ウィンドウモードに切り替え
		SetWindowLong(hWnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, g_windowRect.left, g_windowRect.top,
			g_windowRect.right - g_windowRect.left, g_windowRect.bottom - g_windowRect.top,
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_NORMAL);
	}
	else
	{
		// フルスクリーンモードに切り替え
		GetWindowRect(hWnd, &g_windowRect);
		SetWindowLong(hWnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_MAXIMIZE);
	}
	g_isFullscreen = !g_isFullscreen;
}

//*****************
// テクスチャの破棄
//*****************
void UninitTexture(LPDIRECT3DTEXTURE9 &Texture)
{
	if (Texture != NULL)
	{
		Texture->Release();
		Texture = NULL;
	}
}

//*********************
// 頂点バッファーの破棄
//*********************

void UninitBuffer(LPDIRECT3DVERTEXBUFFER9 &Buffer)
{
	if (Buffer != NULL)
	{
		Buffer->Release();
		Buffer = NULL;
	}
}
//***************
// メッシュの破棄
//***************

void UninitMesh(LPD3DXMESH &Mesh)
{
	//メッシュの破棄
	if (Mesh != NULL)
	{
		Mesh->Release();
		Mesh = NULL;
	}
}
void UninitBuffMat(LPD3DXBUFFER &Buffer)
{
	//マテリアルの破棄
	if (Buffer != NULL)
	{
		Buffer->Release();
		Buffer = NULL;
	}
}
//***************************
// インデックスバッファの破棄
//***************************
void UninitIndxBuffer(LPDIRECT3DINDEXBUFFER9 &IndxBuffer)
{
	if (IndxBuffer != NULL)
	{
		IndxBuffer->Release();
		IndxBuffer = NULL;
	}
}

//********************
// 1次元ベクトルを引く
//********************
float Vector(float StartPoint, float EndPoint)
{
	return EndPoint - StartPoint;
}

//*************************
// 2Dベクトルの角度を求める
//*************************
float Angle(D3DXVECTOR2 Vec)
{
	return atan2f(Vec.x, Vec.y);
}

//*********************
// 角度の近道を見つける
//*********************
void NearRot(float Angle, float& OutAngle)
{
	// 角度の近道
	if (Angle >= D3DX_PI) SetAngle(OutAngle += D3DX_PI * 2.0f,OutAngle);
	// 角度の近道
	else if (Angle <= -D3DX_PI) SetAngle(OutAngle -= D3DX_PI * 2.0f,OutAngle);
}

//*****************************
// 現在の向きを設定
//*****************************
void SetAngle(float fAngle, float& OutAngle)
{
	OutAngle = fAngle;
}