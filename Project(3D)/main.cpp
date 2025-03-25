//====================================
//
// マップエディター試作版 [main.cpp]
// Author: Asuma Nishio
//
//====================================

//****************************
// インクルードファイル宣言
//****************************
#include "main.h"
#include <time.h>
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>
#include "camera.h"
#include "light.h"
#include "input.h"
#include "meshfield.h"
#include "mouse.h"

//*****************************
// グローバル変数宣言
//*****************************
LPDIRECT3D9 g_pD3D = NULL;				// Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;	// Direct3Dデバイスへのポインタ
MODE g_mode = MODE_EDIT;				// 現在のモード
bool g_isFullscreen = false;			// ウィンドウを切り替えるためのフラグ
RECT g_windowRect;						// ウィンドウを切り替えるための変数
LPD3DXFONT g_pFont = NULL;				// フォントへのポインタ
int g_nCountFPS = 0;					// FPSカウンタ
int nGetNumber = 0;						// 番号
int Filenamepass;						// ファイル名
int nBlockSet = 0;						// 配置するブロック数

//******************************
// プロトタイプ宣言
//******************************
void ToggleFullscreen(HWND hWnd);	// ウィンドウをフルスクリーンにする方法
void DrawModeChange();				// モード切り替え
void DebugEditModelInfo();			// 配置モデル情報
void DrawCameraPos(void);			// カメラの座標表示
void onWireFrame();					// ワイヤーフレーム起動
void offWireFrame();				// ワイヤーフレーム終了

//===============================
// メイン関数
//===============================
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hInstancePrev, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// メモリリーク検知用のフラグ
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);		

	DWORD dwCurrentTime;			// 現在時刻
	DWORD dwExecLastTime;			// 終了時刻

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				 // WNDCLASSEXのメモリサイズ
		CS_CLASSDC,						 // ウインドウのスタイル
		WindowProc,						 // ウインドウプロシージャ
		0,								 // 0にする
		0,								 // 0にする
		hInstance,						 // インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),  // タスクバーアイコン
		LoadCursor(NULL,IDC_ARROW),      // マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),      // クライアント領域の背景色
		NULL,                            // メニューバー
		CLASS_NAME,                      // ウインドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION),  // ファイルアイコン
	};


	HWND hWnd = {};  // ウインドウハンドル(識別子)
	MSG msg;    //メッセージを格納する変数

	// 画面サイズの構造
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	// フルスクリーンモード
	BOOL bWindow = FALSE;

	// ウインドウクラスの登録
	RegisterClassEx(&wcex);

	// クライアント領域
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// ウインドウを生成
	hWnd = CreateWindowEx(0,		// 拡張ウインドウスタイル
		CLASS_NAME,					// ウインドウクラスの名前
		WINDOW_NAME,				// ウインドウの名前
		WS_OVERLAPPEDWINDOW,		// ウインドウスタイル
		CW_USEDEFAULT,				// ウインドウの左上のX座標
		CW_USEDEFAULT,				// ウインドウの左上のY座標
		(rect.right - rect.left),   // ウインドウ幅
		(rect.bottom - rect.top),   // ウインドウの高さ
		NULL,						// NULL
		NULL,						// NULL
		hInstance,					// インスタンスハンドル
		NULL);						// ウインドウ作成データ


	// 初期化処理
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{
		// 初期化処理が失敗したとき
		return -1;
	}

	// 分解能をせってい
	timeBeginPeriod(1);
	dwCurrentTime = 0;					// 初期化
	dwExecLastTime = timeGetTime();		// 現在時刻を保存

	// ウインドウの表示
	ShowWindow(hWnd, nCmdShow); // ウインドウの表示状態の設定
	UpdateWindow(hWnd);				    // クライアント領域の更新

	// 初期化
	DWORD dwFrameCount;					// フレームカウント
	DWORD dwFPSLastTime;				// 最後にFPSを計測した時刻

	dwFrameCount = 0;					// フレームカウントを初期化
	dwFPSLastTime = timeGetTime();		// 現在の時刻を取得

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			// Windowsの処理
			if (msg.message == WM_QUIT)
			{
				// WM_QUITメッセージを受け取ったらループ抜ける
				break;
			}
			else
			{
				// メッセージ設定
				TranslateMessage(&msg);	// 仮想キーメッセージを文字メッセージへ変換

				DispatchMessage(&msg);	// ウインドウプロシージャへメッセージを送出
			}
		}

		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, 0, 0)) 
			{
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
			
		}
		else
		{// DirectXの処理
			dwCurrentTime = timeGetTime();		// 現在時刻の取得
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{
				// 0.5秒経過
				// FPSを計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;	// 保存

				dwFrameCount = 0;				// 0に戻す

			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				// 60/1経過
				dwExecLastTime = dwCurrentTime;	// 処理開始の時刻

				dwFrameCount++;					// フレームカウントを加算

				// 更新処理
				Update();

				// 描画処理
				Draw();

			}
		}
	}

	// 終了処理
	Uninit();

	// 分解能を戻す
	timeEndPeriod(1);

	// ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// メッセージを返す
	return(int)msg.wParam;
}

//=======================================
// ウインドウプロシージャ
//=======================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	const RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT }; // ウインドウの領域
	int nID;

	switch (uMsg)
	{
	case WM_ACTIVATE:	// アクティブ時：1　非アクティブ時：0
		return 0L;

	case WM_DESTROY: // ウインドウ破棄メッセージ
		// WM_QUITメッセージを送る
		PostQuitMessage(0); // ウインドウの破棄
		break;

	case WM_KEYDOWN: // キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE:	// [ESC]キーが押された

			nID = MessageBox(hWnd, "終了しますか?", "終了メッセージ", MB_YESNO);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd); // ウインドウ破棄メッセージ
			}
			else
			{
				return 0;			 // 返す
			}

			break;
		}
		break;

	case WM_MOUSEWHEEL: // マウスのホイール判定

		// ローカル変数
		int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);

		// ホイール情報
		MouseWheel(zDelta);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam); // 既定の処理を繰り返す
}
//=========================
// 初期化処理
//=========================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			// ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	// プレゼンテーションパラメータ

	// Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}
	// 現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// パラメーターの0クリア
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth = SCREEN_WIDTH;						// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;						// バックバッファの形式
	d3dpp.BackBufferCount = 1;									// バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// ダブルバッファの切り替え(映像信号に同期)
	d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// デプスバッファとして16bitを使用
	d3dpp.Windowed = bWindow;									// ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュシレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

	// Direct3Dデバイスの生成(描画処理と頂点処理をハードウェアで行う)
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		// Direct3Dデバイスの生成(描画処理はハードウェア,頂点処理をCPUで行う)
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			// Direct3Dデバイスの生成(描画処理,頂点処理をCPUで行う)
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

	// レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// テクスチャステージチャートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//==============================
	// 初期化処理
	//==============================
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{// キーボード
		return E_FAIL;
	}
	if (FAILED(InitJoypad()))
	{// パッド
		return E_FAIL;
	}

	// デバッグ表示用のフォントを設定
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"Terminal", &g_pFont);

	// マウス初期化
	InitMouse(hInstance,hWnd);

	// ライト
	InitLight();

	// カメラ
	InitCamera();

	// メッシュの描画
	InitMeshfield();
	SetMeshfield(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 10, 0, 10, 100, 100);

	return S_OK; // 結果を返す

}
//==================
// 終了処理
//==================
void Uninit(void)
{
	//==================
	// 各種終了処理
	//==================

	// キーボード
	UninitKeyboard();

	// マウス
	UninitMouse();

	// パッド
	UninitJoypad();

	// ライト
	UninitLight();

	// メッシュの終了
	UninitMeshfield();

	// デバッグ表示用のフォントの破棄
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	// Direct3Dデバイスの破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

}
//===================
// 更新処理
//===================
void Update(void)
{
	//===========================
	// 各種オブジェクト更新処理
	//===========================
	// キーボードの更新処理
	UpdateKeyboard();

	// マウスの更新処理
	UpdateMouse();

	// ゲームパッドの更新処理
	UpdateJoypad();

	// カメラの更新
	UpdateCamera();

	if (KeyboardTrigger(DIK_F1))
	{// F3キー
		// ワイヤーフレーム起動
		onWireFrame();
	}
	else if (KeyboardTrigger(DIK_F2))
	{// F4キー
		// ワイヤーフレーム消去
		offWireFrame();
	}

	// モード切り替え
	//if (KeyboardTrigger(DIK_F2))
	//{
	//	g_mode = MODE_EDIT;
	//}

}
//===================
// 描画処理
//===================
void Draw(void)
{
	// カメラ取得
	Camera* pCamera = GetCamera();

	// 画面クリア(バックバッファ&Zバッファのクリア)
	g_pD3DDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// 描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		// 描画成功時
		//================
		// 描画処理
		//================
			
		// カメラをセット
		SetCamera();

		// メッシュの描画
		DrawMeshfield();


		if (g_mode == MODE_EDIT)
		{
			DrawCameraPos();

			DebugEditModelInfo();
		}
		
		// デバッグフォント関係
		DrawFPS();

		// モード切替
		DrawModeChange();


		// 描画終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);

}
//======================
// FPSの表示
//======================
void DrawFPS(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aString[256];

	//文字列に代入
	wsprintf(&aString[0], "//************************\n// ショートカットキー情報\n//************************\n");

	//テキストの描画
	g_pFont->DrawText(NULL, &aString[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}
//========================
// カメラ表示
//========================
void DrawCameraPos(void)
{
	// ローカル変数
	RECT rect = { 0,140,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect1 = { 0,160,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect2 = { 0,60,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect3 = { 0,120,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect4 = { 0,180,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect5 = { 0,200,SCREEN_WIDTH,SCREEN_HEIGHT };

	// 文字列
	char aString[256];
	char aString1[256];
	char aString2[256];
	char aString3[256];
	char aString4[64];
	char aString5[64];

	// カメラの取得
	Camera* pCamera = GetCamera();

	// 文字列に代入
	wsprintf(&aString2[0], "//************************\n// カメラ情報\n//************************\n");
	sprintf(&aString[0],   "[ カメラのrot.y ]: < %.2f >\n", pCamera->rot.y);
	sprintf(&aString1[0],  "[ カメラのrot.x ]: < %.2f >\n", pCamera->rot.x);
	wsprintf(&aString3[0], "[ 視点 / 注視点 ] : < Shift + マウス操作 >\n");
	wsprintf(&aString4[0], "[ ズーム ] :   < マウスホイール >\n");
	wsprintf(&aString5[0], "[ リセット ] : < マウスホイール押し込み > / TABキー\n");

	// テキスト描画
	g_pFont->DrawText(NULL, &aString[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aString1[0], -1, &rect1, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aString2[0], -1, &rect2, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aString3[0], -1, &rect3, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aString4[0], -1, &rect4, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aString5[0], -1, &rect5, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

}
//============================
// エディターモデル関係の表示
//============================
void DebugEditModelInfo()
{
#if 0
	// 取得関係
	int nModel = ReturnEdit(); // 配置カウント
	MAPMODELINFO* pEdit = MapInfo(); // 配置時の情報
	EDITMODEL* pModelEdit = GetBlockInfo(pEdit[nModel].mapedit.nType); // モデル情報

	// ローカル変数
	RECT rect = { 0,220,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect1 = { 0,320,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect2 = { 0,340,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect3 = { 0,360,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect4 = { 0,400,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect5 = { 0,420,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect6 = { 0,440,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect7 = { 0,480,SCREEN_WIDTH,SCREEN_HEIGHT };

	// 文字列
	char aString[256];
	char aString1[256];
	char aString2[256];
	char aString3[256];
	char aString4[256];
	char aString5[256];
	char aString6[256];
	char aString7[256];

	// 文字列に代入
	wsprintf(&aString[0], "//************************\n// 配置するモデル情報\n//************************\n");
	sprintf(&aString1[0], "[ モデルのX座標 ] : < %.2f >\n", pEdit[nModel].mapedit.pos.x);
	sprintf(&aString2[0], "[ モデルのY座標 ] : < %.2f >\n", pEdit[nModel].mapedit.pos.y);
	sprintf(&aString3[0], "[ モデルのZ座標 ] : < %.2f >\n", pEdit[nModel].mapedit.pos.z);
	sprintf(&aString4[0], "[ モデルのXの拡大率 ] : < %.2f > \n", pEdit[nModel].mapedit.Scal.x);
	sprintf(&aString5[0], "[ モデルのYの拡大率 ] : < %.2f >\n", pEdit[nModel].mapedit.Scal.y);
	sprintf(&aString6[0], "[ モデルのZの拡大率 ] : < %.2f >\n", pEdit[nModel].mapedit.Scal.z);
	sprintf(&aString7[0], "[ 選択中のモデルの種類 ] : < %s > \n", pModelEdit->FileName);

	// テキスト描画
	g_pFont->DrawText(NULL, &aString[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aString1[0], -1, &rect1, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aString2[0], -1, &rect2, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aString3[0], -1, &rect3, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aString4[0], -1, &rect4, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aString5[0], -1, &rect5, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aString6[0], -1, &rect6, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aString7[0], -1, &rect7, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
#endif
}
//==============================
// モード切替フォントの表示
//==============================
void DrawModeChange()
{
	// 取得
	MODE nMode = GetMode();

	// ローカル変数
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	char aString[256];
	char aString1[256];

	if (nMode == MODE_EDIT)
	{// 編集モードなら
		strcpy(aString1, "編集モード");
	}
	else if (nMode == MODE_PLAY)
	{// プレイモードなら
		strcpy(aString1, "プレイモード");
	}

	// 文字列に代入
	wsprintf(&aString[0], "現在のモード < %s >\n", &aString1[0]);

	// フォントの表示
	g_pFont->DrawText(NULL, &aString[0], -1, &rect, DT_RIGHT, D3DCOLOR_RGBA(255, 255, 255, 255));

}
//======================
// デバイスの取得
//======================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}
//====================
// モードの取得
//====================
MODE GetMode(void)
{
	return g_mode; // 現在のモードを返す
}
//=====================
// ワイヤーフレームON
//=====================
void onWireFrame()
{
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
}
//=====================
// ワイヤーフレームOFF
//=====================
void offWireFrame() 
{
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}
//=================================================
// ウィンドウをフルスクリーンに変える処理
//=================================================
void ToggleFullscreen(HWND hWnd)
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

