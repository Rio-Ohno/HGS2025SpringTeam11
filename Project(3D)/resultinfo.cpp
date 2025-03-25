//================================
//
//  リザルト表示処理[ResultInfoinfo.cpp]
//  Author:kaiti
//
//================================
#include "resultinfo.h"
#include "input.h"
#include "fade.h"
#include "game.h"
#include "ranking.h"
#include "score.h"
#include "sound.h"
#include "mouse.h"

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureResultInfo[RESULT_MAX] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultInfo = NULL;
RESULT g_Resultstate = RESULT_NONE;

D3DXVECTOR3 g_posResult;
//===========
//初期化処理
//===========
void InitResultInfo(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	g_posResult = D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\GAMECLEAR.png", &g_pTextureResultInfo[0]); //クリア
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\GAMEOVER.png", &g_pTextureResultInfo[1]); //ゲームオーバー
	
	//頂点バッファの生成・頂点情報の設定
	VERTEX_2D* pVtx;
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		                        D3DUSAGE_WRITEONLY,
		                        FVF_VERTEX_2D,
		                        D3DPOOL_MANAGED,
		                        &g_pVtxBuffResultInfo,
		                        NULL);
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultInfo->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_posResult.x - RESULT_X, g_posResult.y - RESULT_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_posResult.x + RESULT_X, g_posResult.y - RESULT_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_posResult.x - RESULT_X, g_posResult.y + RESULT_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_posResult.x + RESULT_X, g_posResult.y + RESULT_Y, 0.0f);
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
	g_pVtxBuffResultInfo->Unlock();
}
//==========
//終了処理
//==========
void UninitResultInfo(void)
{
	for (int nCnt = 0; nCnt < RESULT_MAX; nCnt++)
	{
		if (g_pTextureResultInfo[nCnt] != NULL)
		{
			//テクスチャの破棄
			g_pTextureResultInfo[nCnt]->Release();
			g_pTextureResultInfo[nCnt] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffResultInfo != NULL)
	{
		g_pVtxBuffResultInfo->Release();
		g_pVtxBuffResultInfo = NULL;
	}
}
//==========
//更新処理
//==========
void UpdateResultInfo(void)
{
	FADE g_fade;
	g_fade = GetFade();

	if ((KeyboardTrigger(DIK_RETURN) == true || GetJoypadTrigger(JOYKEY_A) == true || OnMouseTriggerDown(0) == true) && g_fade == FADE_NONE)
	{//決定キーが押された
		//ランキングの設定
		SetFade(MODE_RANK);

		SetRankMode(RANKMODE_RESULT);
		SetRanking(GetScore());

		StopSound();
		//PlaySound(SOUND_LABEL_RANKING);
	}
}
//==========
//描画処理
//==========
void DrawResultInfo(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResultInfo, 0, sizeof(VERTEX_2D));

	if (g_Resultstate == RESULT_CLEAR)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureResultInfo[0]);
	}
	else if (g_Resultstate == RESULT_GAMEOVER)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureResultInfo[1]);
	}


	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
//===============
//リザルト設定
//===============
void SetResult(RESULT result)
{
	g_Resultstate = result;
}