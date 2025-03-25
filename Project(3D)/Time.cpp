//========================================================
// 
// タイマー処理[Time.h]
// Author:Rio Ohno
// 
//========================================================

#include"Time.h"

//マクロ定義
#define MAX_POLY (2)														//最大ポリゴン数(Time)
#define MAX_TIME (90)														//制限時間(秒)
#define WIDTH_TIME (50.0f)													//ポリゴンの幅
#define HEIGHT_TIME (80.0f)													//ポリゴンの高さ

//グローバル宣言
D3DXVECTOR3 g_pos[MAX_POLY];												//位置
TIMETYPE g_TimeType;														//タイプ(カウントダウンorカウントアップ)
LPDIRECT3DTEXTURE9 g_pTextureTime = NULL;									//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;								//頂点バッファへのポインタ
int g_nCntFrame;															//フレームカウンタ
int g_nTime[3];																//時間(多分)(0=60分の1秒,1=秒,2=分)
int g_TimeLim = MAX_TIME;													//制限時間(秒)
bool g_TimeOver = false;													//タイムオーバーか否か

//========================================================
// タイマーの初期化処理
//========================================================
void InitTime(TIMETYPE type)
{
	LPDIRECT3DDEVICE9 pDevice;												//デバイスの取得
	int nCntTime = 0;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\number001.png",
		&g_pTextureTime);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_POLY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTime,
		NULL);

	for (nCntTime = 0; nCntTime < MAX_POLY; nCntTime++)
	{
		g_pos[nCntTime] = D3DXVECTOR3((WIDTH_TIME * MAX_POLY +600.0f) - (nCntTime * WIDTH_TIME), HEIGHT_TIME, 0.0f);//位置の初期化
	}

	for (nCntTime = 0; nCntTime < 3; nCntTime++)
	{
		g_nTime[nCntTime] = 0;												//時間の初期化(0=60分の1秒,1=秒,2=分)
	}

	g_nCntFrame = 0;														//フレームカウンタの初期化
	g_TimeType=type;														//タイプの設定
	g_TimeLim = MAX_TIME;													//制限時間(秒)
	g_TimeOver = false;

	//頂点情報へのポインタ
	VERTEX_2D* pVtx = {};

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);


	for (nCntTime = 0; nCntTime < MAX_POLY; nCntTime++)
	{
		//頂点座標の初期化
		pVtx[0].pos.x = g_pos[nCntTime].x - WIDTH_TIME / 2.0f;
		pVtx[0].pos.y = g_pos[nCntTime].y - HEIGHT_TIME / 2.0f;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_pos[nCntTime].x + WIDTH_TIME / 2.0f;
		pVtx[1].pos.y = g_pos[nCntTime].y - HEIGHT_TIME / 2.0f;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_pos[nCntTime].x - WIDTH_TIME / 2.0f;
		pVtx[2].pos.y = g_pos[nCntTime].y + HEIGHT_TIME / 2.0f;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_pos[nCntTime].x + WIDTH_TIME / 2.0f;
		pVtx[3].pos.y = g_pos[nCntTime].y + HEIGHT_TIME / 2.0f;
		pVtx[3].pos.z = 0.0f;

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の初期化
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;
	}
	//頂点バッファをアンロック
	g_pVtxBuffTime->Unlock();
}

//========================================================
// タイマーの終了処理
//========================================================
void UninitTime()
{
	//テクスチャの破棄
	if (g_pTextureTime != NULL)
	{			  
		g_pTextureTime->Release();
		g_pTextureTime = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTime != NULL)
	{			  
		g_pVtxBuffTime->Release();
		g_pVtxBuffTime = NULL;
	}
}

//========================================================
// タイマーの更新処理
//========================================================
void UpdateTime()
{

	switch (g_TimeType)
	{

	case TIME_CNTUP:

		//時間カウント
		g_nCntFrame++;

		g_nTime[0] = g_nCntFrame;

		if (g_nTime[0] == 60)
		{
			g_nTime[1]++;
			g_nTime[0] = 0;
			g_nCntFrame = 0;
		}
		if (g_nTime[1] == 60)
		{
			g_nTime[2]++;
			g_nTime[1] = 0;
		}

		break;

	case TIME_CNTDOWN:

		//時間カウント(フレーム)
		g_nCntFrame++;

		g_nTime[0] = g_TimeLim;

		if (g_nCntFrame == 60)//60フレームたったら
		{
			//一秒減らす
			g_TimeLim--;

			//フレームカウンタの初期化
			g_nCntFrame = 0;
		}

		if (g_TimeLim <= 0)//タイムが0になったら
		{
			//ゲームを終わらせる
			g_TimeOver = true;
		}

		break;

	default:

		break;
	}

	SetTime();
}

//========================================================
// タイマーの設定
//========================================================
void SetTime()
{
	VERTEX_2D* pVtx = {};

	int NumTime[MAX_POLY] = {};														//各数字格納用			
	int nCntTime1, nCntTime2 = 0;
	int Time[3];

	for (nCntTime1 = 0; nCntTime1 < MAX_POLY/2.0f; nCntTime1++)
	{
		Time[nCntTime1] = g_nTime[nCntTime1];
	}

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTime1 = 0; nCntTime1 < MAX_POLY; nCntTime1++)
	{
		NumTime[nCntTime1] = Time[nCntTime2] % 10;
		Time[nCntTime2] /= 10;

		//テクスチャ座標の更新
		pVtx[0].tex = D3DXVECTOR2(0.0f + 0.1f * NumTime[nCntTime1], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f + 0.1f * NumTime[nCntTime1], 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + 0.1f * NumTime[nCntTime1], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f + 0.1f * NumTime[nCntTime1], 1.0f);

		pVtx += 4;

		if (((nCntTime1+1) % 2) == 0)
		{
			nCntTime2++;
		}

	}

	//頂点バッファをアンロック
	g_pVtxBuffTime->Unlock();
}

//========================================================
// タイマーの描画処理
//========================================================
void DrawTime()
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTime = MAX_POLY-1; nCntTime >=0; nCntTime--)
	{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureTime);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTime * 4, 2);
	}
}

//========================================================
// 時間切れの処理
//========================================================
bool GetTimeOver()
{
	return g_TimeOver;
}