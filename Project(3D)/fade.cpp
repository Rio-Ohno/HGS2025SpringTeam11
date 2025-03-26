//==============================
//
//  フェード表示処理[fade.cpp]
//  Author:kaiti
//
//==============================
#include "fade.h"

LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;
FADE g_Fade;
MODE g_modeNext;
D3DXCOLOR g_colorFade;
//===========
//初期化処理^^unti
//===========
void InitFade(MODE modeNext)
{
	g_Fade = FADE_IN;
	g_modeNext = modeNext;
	g_colorFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();
	//頂点バッファの生成・頂点情報の設定
	VERTEX_2D* pVtx;
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		                        D3DUSAGE_WRITEONLY,
		                        FVF_VERTEX_2D,
		                        D3DPOOL_MANAGED,
		                        &g_pVtxBuffFade,
		                        NULL);
	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);
	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	//頂点バッファをアンロック
	g_pVtxBuffFade->Unlock();

	//モードの設定
	SetMode(g_modeNext);
}
//==========
//終了処理
//==========
void UninitFade(void)
{
	//頂点バッファの破棄
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}
//==========
//更新処理
//==========
void UpdateFade(void)
{
	if (g_Fade != FADE_NONE)
	{
		VERTEX_2D* pVtx;
		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

		if (g_Fade == FADE_IN)
		{//フェードイン
			g_colorFade.a -= 0.04f;
			if (g_colorFade <= 0.0f)
			{
				g_colorFade.a = 0.0f;
				g_Fade = FADE_NONE;
			}
		}
		else if (g_Fade == FADE_OUT)
		{//フェードアウト
			g_colorFade.a += 0.04f;
			if (g_colorFade.a >= 1.0f)
			{
				g_colorFade.a = 1.0f;
				g_Fade = FADE_IN;
				//モードの設定
				SetMode(g_modeNext);
			}
		}
		//頂点カラーの設定
		pVtx[0].col = g_colorFade;
		pVtx[1].col = g_colorFade;
		pVtx[2].col = g_colorFade;
		pVtx[3].col = g_colorFade;

		//頂点バッファをアンロック
		g_pVtxBuffFade->Unlock();
	}
}
//==========
//描画処理
//==========
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	//テクスチャの設定
	pDevice->SetTexture(0, NULL);
	//プレイヤーの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
//==============
//フェード設定
//==============
void SetFade(MODE modeNext)
{
	g_Fade = FADE_OUT;
	g_modeNext = modeNext;	
}
//====================
//フェード状態の取得
//====================
FADE GetFade(void)
{
	return g_Fade;
}