//==============================
//
//  選択処理[titleinfo.h]
//  Author:kaiti
//
//==============================
#include "titleinfo.h"
#include "game.h"
#include "input.h"
#include "ranking.h"
#include "fade.h"
//#include "tutorial.h"
#include "sound.h"
//#include "player.h"
#include "mouse.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturetitleinfo[TITLE_MAX] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufftitleinfo = NULL;
D3DXVECTOR3 g_Selectpos;
//ロゴ
LPDIRECT3DTEXTURE9 g_pTexturetitleinfoLogo = NULL;
LPDIRECT3DVERTEXBUFFER9 g_pVtxBufftitleinfoLogo = NULL;
D3DXVECTOR3 g_Logopos;

int g_titleinfoMenu;
//============
//初期化処理
//============
void InitTitleInfo(void)
{
	g_titleinfoMenu = TITLE_START;

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	g_Selectpos = D3DXVECTOR3(640.0f, 360.0f, 0.0f);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\gamestart.png", &g_pTexturetitleinfo[0]); //START
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\titleranking.png", &g_pTexturetitleinfo[1]); //ランキング
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\exit.png", &g_pTexturetitleinfo[2]); //ランキング

	//頂点バッファの生成・頂点情報の設定
	VERTEX_2D* pVtx;
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TITLE_MAX,
		                        D3DUSAGE_WRITEONLY,
		                        FVF_VERTEX_2D,
		                        D3DPOOL_MANAGED,
		                        &g_pVtxBufftitleinfo,
		                        NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBufftitleinfo->Lock(0, 0, (void**)&pVtx, 0);
	for (int nCnt = 0; nCnt < TITLE_MAX; nCnt++)
	{//メニュー
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_Selectpos.x - TITLESELECT_WIDTH / 2, g_Selectpos.y - TITLESELECT_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Selectpos.x + TITLESELECT_WIDTH / 2, g_Selectpos.y - TITLESELECT_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Selectpos.x - TITLESELECT_WIDTH / 2, g_Selectpos.y + TITLESELECT_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Selectpos.x + TITLESELECT_WIDTH / 2, g_Selectpos.y + TITLESELECT_HEIGHT / 2, 0.0f);
		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		g_Selectpos.y += 120.0f;
		pVtx += 4;
	}
	//頂点バッファをアンロック
	g_pVtxBufftitleinfo->Unlock();


	//ロゴ
	g_Logopos = D3DXVECTOR3(640.0f, 0.0f, 0.0f);

	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\title2.png", &g_pTexturetitleinfoLogo); //START
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBufftitleinfoLogo,
		NULL);
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBufftitleinfoLogo->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_Logopos.x - TITLELOGO_WIDTH / 2, g_Logopos.y - TITLELOGO_HEIGHT / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Logopos.x + TITLELOGO_WIDTH / 2, g_Logopos.y - TITLELOGO_HEIGHT / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Logopos.x - TITLELOGO_WIDTH / 2, g_Logopos.y + TITLELOGO_HEIGHT / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Logopos.x + TITLELOGO_WIDTH / 2, g_Logopos.y + TITLELOGO_HEIGHT / 2, 0.0f);
	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxBufftitleinfoLogo->Unlock();
}
//==========
//終了処理
//==========
void UninitTitleInfo(void)
{
	// テクスチャの破棄
	for (int nCnt = 0; nCnt < TITLE_MAX; nCnt++)
	{
		UninitTexture(g_pTexturetitleinfo[nCnt]);
	}
	UninitTexture(g_pTexturetitleinfoLogo);

	// バッファの破棄
	UninitBuffer(g_pVtxBufftitleinfo);
	UninitBuffer(g_pVtxBufftitleinfoLogo);
}
//==========
//更新処理
//==========
void UpdateTitleInfo(int zDelta)
{
	FADE g_fade;
	g_fade = GetFade();

	//Player* pPlayer = GetPlayer();

	UpdateSelect(zDelta);

	UpdateLighting();

	if ((KeyboardTrigger(DIK_RETURN) == true|| GetJoypadTrigger(JOYKEY_A) == true || OnMouseTriggerDown(0) == true) && g_fade == FADE_NONE)
	{//ENTERが押された
		if (g_Logopos.y < LOGO_END_Y)
		{//ロゴが下りてきている時
			g_Logopos.y += LOGO_END_Y;
		}
		else
		{//ロゴがおり切ったとき
			StopSound();
			//PlaySound(SOUND_LABEL_DESICION);
			//メニューに合わせてモードの切り替え
			if (g_titleinfoMenu == TITLE_START)
			{	//STARTにいる場合
				SetFade(MODE_STAGEONE);
				//pPlayer->bfirst = true;

				//PlaySound(SOUND_LABEL_GAME);
			}
			else if (g_titleinfoMenu == TITLE_RANK)
			{	//RANKにいる場合
				SetFade(MODE_RANK);
				SetRankMode(RANKMODE_SELECT);

				//PlaySound(SOUND_LABEL_RANKING);
			}
			else if (g_titleinfoMenu == TITLE_FIN)
			{	//FINにいる場合
				SetFade(MODE_END);
			}
		}
	}

	VERTEX_2D* pVtx = NULL;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBufftitleinfoLogo->Lock(0, 0, (void**)&pVtx, 0);

	//ロゴを下げる
	if (g_Logopos.y < LOGO_END_Y)
	{
		g_Logopos.y++;
	}
	//ロゴのY制限
	if (g_Logopos.y >= LOGO_END_Y)
	{
		g_Logopos.y = LOGO_END_Y;
	}

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_Logopos.x - TITLELOGO_WIDTH / 2, g_Logopos.y - TITLELOGO_HEIGHT / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_Logopos.x + TITLELOGO_WIDTH / 2, g_Logopos.y - TITLELOGO_HEIGHT / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_Logopos.x - TITLELOGO_WIDTH / 2, g_Logopos.y + TITLELOGO_HEIGHT / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_Logopos.x + TITLELOGO_WIDTH / 2, g_Logopos.y + TITLELOGO_HEIGHT / 2, 0.0f);


	//頂点バッファをアンロック
	g_pVtxBufftitleinfoLogo->Unlock();
}
//===========
//描画処理
//===========
void DrawTitleInfo(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBufftitleinfo, 0, sizeof(VERTEX_2D));

	int nCnt;
	for (nCnt = 0; nCnt < TITLE_MAX; nCnt++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturetitleinfo[nCnt]);
		//プレイヤーの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
	}

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBufftitleinfoLogo, 0, sizeof(VERTEX_2D));
	//テクスチャの設定
	pDevice->SetTexture(0, g_pTexturetitleinfoLogo);
	//プレイヤーの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}

//*********************
// 下のモードを選べるか
//*********************
bool isSelectUpCondition(int zDelta)
{
	FADE g_fade;
	g_fade = GetFade();
	return KeyboardTrigger(DIK_S) == true || KeyboardTrigger(DIK_DOWN) == true || GetJoypadTrigger(JOYKEY_DOWN) == true || zDelta < 0;
}

//*********************
// 上のモードを選べるか
//*********************
bool isSelectDownCondition(int zDelta)
{
	FADE g_fade;
	g_fade = GetFade();
	return KeyboardTrigger(DIK_W) == true || KeyboardTrigger(DIK_UP) == true || GetJoypadTrigger(JOYKEY_UP) == true || zDelta > 0;
}

//*********************************
// 引数が現在のモードと同じだったら
//*********************************
bool isTitleMode(TITLEINFO TitleType)
{
	return g_titleinfoMenu == TitleType;
}

//*******************************
// 引数が現在のモード以下だったら
//*******************************
bool isLessMode(TITLEINFO TitleType)
{
	return g_titleinfoMenu < TitleType;
}

//*******************************
// 引数が現在のモード以上だったら
//*******************************
bool isGreaterMode(TITLEINFO TitleType)
{
	return g_titleinfoMenu > TitleType;
}

//*******************
// 選んでる項目を更新
//*******************
void UpdateSelect(int zDelta)
{
	//メニューの選択(上下)
	if (isSelectDownCondition(zDelta) == true)
	{
		//PlaySound(SOUND_LABEL_SELECT);

		g_titleinfoMenu--;
		if (isLessMode(TITLE_START) == true)
		{
			g_titleinfoMenu = TITLE_FIN;
		}
	}
	else if (isSelectUpCondition(zDelta) == true)
	{
		//PlaySound(SOUND_LABEL_SELECT);

		g_titleinfoMenu++;
		if (isGreaterMode(TITLE_FIN) == true)
		{
			g_titleinfoMenu = TITLE_START;
		}
	}
}

//*****************************
// 選んでる項目を目立たせる処理
//*****************************
void UpdateLighting()
{
	VERTEX_2D* pVtx = NULL;
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBufftitleinfo->Lock(0, 0, (void**)&pVtx, 0);

	g_Selectpos = D3DXVECTOR3(640.0f, 360.0f, 0.0f);

	for (int TypeCount = 0; TypeCount < TITLE_MAX; TypeCount++)
	{
		if (isTitleMode((TITLEINFO)TypeCount) == true)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_Selectpos.x - TITLESELECT_WIDTH / 2 * ZOOM_SELECT, g_Selectpos.y - TITLESELECT_HEIGHT / 2 * ZOOM_SELECT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Selectpos.x + TITLESELECT_WIDTH / 2 * ZOOM_SELECT, g_Selectpos.y - TITLESELECT_HEIGHT / 2 * ZOOM_SELECT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Selectpos.x - TITLESELECT_WIDTH / 2 * ZOOM_SELECT, g_Selectpos.y + TITLESELECT_HEIGHT / 2 * ZOOM_SELECT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Selectpos.x + TITLESELECT_WIDTH / 2 * ZOOM_SELECT, g_Selectpos.y + TITLESELECT_HEIGHT / 2 * ZOOM_SELECT, 0.0f);

			pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

		}
		else
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_Selectpos.x - TITLESELECT_WIDTH / 2, g_Selectpos.y - TITLESELECT_HEIGHT / 2, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Selectpos.x + TITLESELECT_WIDTH / 2, g_Selectpos.y - TITLESELECT_HEIGHT / 2, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Selectpos.x - TITLESELECT_WIDTH / 2, g_Selectpos.y + TITLESELECT_HEIGHT / 2, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Selectpos.x + TITLESELECT_WIDTH / 2, g_Selectpos.y + TITLESELECT_HEIGHT / 2, 0.0f);

			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		g_Selectpos.y += 120.0f;
		pVtx += 4;
	}
	//頂点バッファをアンロック
	g_pVtxBufftitleinfo->Unlock();
}