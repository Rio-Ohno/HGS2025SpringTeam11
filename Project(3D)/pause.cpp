//==============================
//
//  ポーズ処理[pause.h]
//  Author:kaiti
//
//==============================
#include "pause.h"
#include "game.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
//#include "player.h"
#include "mouse.h"

#define MAX_TEXTURE (5)

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturePause[MAX_TEXTURE] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;
PAUSE_MENU g_pauseMenu = PAUSE_MENU_CONTNUE;
D3DXVECTOR3 g_Pausepos;
//============
//初期化処理
//============
void InitPause(void)
{
	g_pauseMenu = PAUSE_MENU_CONTNUE;

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\settei01.png", &g_pTexturePause[1]); //設定
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\quit01.png", &g_pTexturePause[2]); //戻る
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\retry2.png", &g_pTexturePause[3]); //やり直し
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\exit2.png", &g_pTexturePause[4]); //タイトルに戻る

	g_Pausepos = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - PAUSESELECT_HEIGHT * 1.5f, 0.0f);

	//頂点バッファの生成・頂点情報の設定
	VERTEX_2D* pVtx;
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TEXTURE,
		                        D3DUSAGE_WRITEONLY,
		                        FVF_VERTEX_2D,
		                        D3DPOOL_MANAGED,
		                        &g_pVtxBuffPause,
		                        NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_TEXTURE ; nCnt++)
	{//ポーズメニュー
		if (nCnt == 0)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);
			//頂点カラーの設定
			pVtx[0].col = D3DCOLOR_RGBA(1, 1, 1, 190);
			pVtx[1].col = D3DCOLOR_RGBA(1, 1, 1, 190);
			pVtx[2].col = D3DCOLOR_RGBA(1, 1, 1, 190);
			pVtx[3].col = D3DCOLOR_RGBA(1, 1, 1, 190);

		}
		else
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_Pausepos.x - (PAUSESELECT_WIDTH / 2), g_Pausepos.y - (PAUSESELECT_HEIGHT / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Pausepos.x + (PAUSESELECT_WIDTH / 2), g_Pausepos.y - (PAUSESELECT_HEIGHT / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Pausepos.x - (PAUSESELECT_WIDTH / 2), g_Pausepos.y + (PAUSESELECT_HEIGHT / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Pausepos.x + (PAUSESELECT_WIDTH / 2), g_Pausepos.y + (PAUSESELECT_HEIGHT / 2), 0.0f);
			g_Pausepos.y += PAUSESELECT_HEIGHT * 1.25;

			//頂点カラーの設定
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	//頂点バッファをアンロック
	g_pVtxBuffPause->Unlock();
}
//==========
//終了処理
//==========
void UninitPause(void)
{
	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		//テクスチャの破棄
		if (g_pTexturePause[nCnt] != NULL)
		{
			g_pTexturePause[nCnt]->Release();
			g_pTexturePause[nCnt] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}
//==========
//更新処理
//==========
void UpdatePause(int zDelta)
{
	FADE g_fade;
	g_fade = GetFade();

	//Player* pPlayer = GetPlayer();

	VERTEX_2D* pVtx;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);
		
	//メニューの選択(上下)
	//Contnueにいる場合
	if (KeyboardTrigger(DIK_W) == true || GetJoypadTrigger(JOYKEY_UP) == true || zDelta > 0)
	{
		//PlaySound(SOUND_LABEL_SELECT);
		switch (g_pauseMenu)
		{
		case PAUSE_MENU_OPTION:
			g_pauseMenu = PAUSE_MENU_QUIT;
			break;
		case PAUSE_MENU_CONTNUE:
			g_pauseMenu = PAUSE_MENU_OPTION;
			break;
		case PAUSE_MENU_RETRY:
			g_pauseMenu = PAUSE_MENU_CONTNUE;
			break;
		case PAUSE_MENU_QUIT:
			g_pauseMenu = PAUSE_MENU_RETRY;
			break;
		default:
			break;
		}
	}
	else if (KeyboardTrigger(DIK_S) == true || GetJoypadTrigger(JOYKEY_DOWN) == true || zDelta < 0)
	{
		//PlaySound(SOUND_LABEL_SELECT);
		switch (g_pauseMenu)
		{
		case PAUSE_MENU_OPTION:
			g_pauseMenu = PAUSE_MENU_CONTNUE;
			break;
		case PAUSE_MENU_CONTNUE:
			g_pauseMenu = PAUSE_MENU_RETRY;
			break;
		case PAUSE_MENU_RETRY:
			g_pauseMenu = PAUSE_MENU_QUIT;
			break;
		case PAUSE_MENU_QUIT:
			g_pauseMenu = PAUSE_MENU_OPTION;
			break;
		default:
			break;
		}
	}

	pVtx += 4;
	for (int nCnt = 0; nCnt < PAUSE_MENU_MAX; nCnt++)
	{
		if (nCnt == g_pauseMenu)
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 1, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 1, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 1, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 1, 255);
		}
		else
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffPause->Unlock();

	if ((KeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A) == true || OnMouseTriggerDown(0) == true) && g_fade == FADE_NONE)
	{
		//PlaySound(SOUND_LABEL_DESICION);
		//メニューに合わせてモードの切り替え
		if (g_pauseMenu == PAUSE_MENU_CONTNUE)
		{	//Contnueにいる場合
			SetEnablePause(false);
			SetGameState(GAMESTATE_NORMAL);
		}
		else if (g_pauseMenu == PAUSE_MENU_RETRY)
		{	//RETRYにいる場合
			SetFade(MODE_STAGEONE);
			//pPlayer->bfirst = true;

			StopSound();
			//PlaySound(SOUND_LABEL_GAME);
		}
		else if (g_pauseMenu == PAUSE_MENU_QUIT)
		{	//QUITにいる場合
			SetFade(MODE_TITLE);

			StopSound();
			//PlaySound(SOUND_LABEL_TITLE);
		}
		else if (g_pauseMenu == PAUSE_MENU_OPTION)
		{	//QUITにいる場合
			SetEnableOption(true);
			SetEnablePause(false);
			SetGameState(GAMESTATE_OPTION);
		}
	}
}
//===========
//描画処理
//===========
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	for (int nCnt = 0; nCnt < MAX_TEXTURE; nCnt++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePause[nCnt]);
		//プレイヤーの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}
}