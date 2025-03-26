//============================================================
//
//プレイヤー[player.cpp]
//Author:Rio Ohno
//
//============================================================

#include"player.h"
#include"input.h"
#include"bullet.h"
//#include"exploosion.h

//マクロ
#define PLAYER_SPEED ((float)1.0f)

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;                                     //テクスチャへのポインタ
LPDIRECT3DTEXTURE9 g_pTexturePlayerPos = NULL;                                  //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer=NULL;                                  //頂点情報を格納
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayerPos = NULL;                             //頂点情報を格納
Player g_Player;

int g_nCounterAnimPlayer = 0;                                                   //アニメーションカウンタ
int g_nPatternAnimPlayer = 0;                                                   //アニメーションパターンNo.
static int nData = 0;                                                           //方向検知用

//=============================================================================================================
//プレイヤーの初期化処理
//=============================================================================================================
void InitPlayer()
{

	LPDIRECT3DDEVICE9 pDevice;													//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		/*"data\\TEXTURE\\00000402.png"*/NULL,
		&g_pTexturePlayer);

	g_nCounterAnimPlayer = 0;													//カウンターを初期化する
	g_nPatternAnimPlayer = 0;													//パターンNO.を初期化する

	g_Player.pos = D3DXVECTOR3(640.0f,625.0f,0);								//位置を初期化する
	g_Player.move = D3DXVECTOR3(0.0f,0.0f,0.0f);								//移動量を初期化する
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								//向きを初期化する
	g_Player.state = PLAYERSTATE_NORMAL;										//敵の状態の初期化
	g_Player.nCountState = 0;													//状態管理用カウンターの初期化
	g_Player.nCntPlayer = 2;													//残機数を初期化

	//対角線の長さを算出する
	g_Player.fLength = sqrtf(NUM_WIDTH* NUM_WIDTH + NUM_HEIGHT * NUM_HEIGHT) / 2.0f;

	//対角線の角度を算出する
	g_Player.fAngle = atan2f(NUM_WIDTH, NUM_HEIGHT);

	g_Player.fLife = PLAYERLIFE;												//体力の初期化
	g_Player.bUse = true;														//自機を使用状態にする
	g_Player.bDisp = true;

	//頂点バッファのの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,                               
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);

	VERTEX_2D* pVtx;								                           //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = g_Player.pos.x + sinf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[0].pos.y = g_Player.pos.y + cosf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[1].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (0.0f - g_Player.fAngle)) * g_Player.fLength;
	pVtx[2].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (0.0f - g_Player.fAngle)) * g_Player.fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_Player.pos.x + sinf(g_Player.rot.z - (0.0f - g_Player.fAngle)) * g_Player.fLength;
	pVtx[3].pos.y = g_Player.pos.y + cosf(g_Player.rot.z - (0.0f - g_Player.fAngle)) * g_Player.fLength;
	pVtx[3].pos.z = 0.0f;


	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(100, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(100, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(100, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(100, 255, 255, 255);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}

//=============================================================================================================
//プレイヤーの描画処理
//=============================================================================================================
void DrawPlayer()
{
	LPDIRECT3DDEVICE9 pDevice;														//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	if (g_Player.bDisp == true)
	{
		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePlayer);

		//プレイヤーの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	}

	//DrawPlayerを一個で済ませたいなら設定->描画->設定->描画の順

	////テクスチャの設定
	//pDevice->SetTexture(0, g_pTexturePlayer2);

	////プレイヤーの描画
	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);

}

//=============================================================================================================
//プレイヤーの更新処理
//=============================================================================================================
void UpdatePlayer()
{
	if (g_Player.bUse == true)
	{
		//プレイヤーの入力
		ActionPlayer();

		//位置を更新
		g_Player.pos.x += g_Player.move.x;
		g_Player.pos.y += g_Player.move.y;

		//移動量を更新（減衰）
		g_Player.move.x += (0.0f - g_Player.move.x) * 0.09f;
		g_Player.move.y += (0.0f - g_Player.move.y) * 0.09f;//あんまりいらないから数字でかめにしてる

		//画面外に出ないようにする処理
		if ((g_Player.pos.x + sinf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength) < 0.0f)
		{
			g_Player.pos.x = NUM_WIDTH / 2;
		}
		if ((g_Player.pos.x + sinf(g_Player.rot.z + (D3DX_PI - g_Player.fAngle)) * g_Player.fLength) > SCREEN_WIDTH)
		{
			g_Player.pos.x = SCREEN_WIDTH - (NUM_WIDTH / 2);
		}
		if ((g_Player.pos.y + cosf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength) < 0.0f)
		{
			g_Player.pos.y = NUM_HEIGHT / 2;
		}
		if ((g_Player.pos.y + cosf(g_Player.rot.z - (0.0f - g_Player.fAngle)) * g_Player.fLength) > SCREEN_HEIGHT)
		{
			g_Player.pos.y = SCREEN_HEIGHT - (NUM_HEIGHT / 2);
		}

		VERTEX_2D* pVtx;//頂点情報へのポインタ

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		pVtx[0].pos.x = g_Player.pos.x + sinf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
		pVtx[0].pos.y = g_Player.pos.y + cosf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
		pVtx[1].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (0.0f - g_Player.fAngle)) * g_Player.fLength;
		pVtx[2].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (0.0f - g_Player.fAngle)) * g_Player.fLength;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_Player.pos.x + sinf(g_Player.rot.z - (0.0f - g_Player.fAngle)) * g_Player.fLength;
		pVtx[3].pos.y = g_Player.pos.y + cosf(g_Player.rot.z - (0.0f - g_Player.fAngle)) * g_Player.fLength;
		pVtx[3].pos.z = 0.0f;

		//頂点バッファをアンロックする
		g_pVtxBuffPlayer->Unlock();
	}
}

//=============================================================================================================
//プレイヤーの終了処理
//=============================================================================================================
void UninitPlayer()
{
	//テクスチャの破棄
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//=============================================================================================================
//弾が当たった時の処理
//=============================================================================================================
void HitPlayer(float fDamage)
{
	g_Player.fLife -= fDamage;

	if (g_Player.fLife <= 0)
	{
		if (g_Player.nCntPlayer <= 0)
		{
			g_Player.nCountState = 5;
			g_Player.bUse = false;
			g_Player.state = PLAYERSTATE_DEATH;
		}
		else if(g_Player.nCntPlayer > 0)
		{
			g_Player.nCountState = 20;
			g_Player.bUse = false;
			g_Player.bDisp = false;
			g_Player.state = PLAYERSTATE_APPEAR;
		}
	}

	else
	{
		g_Player.nCountState = 5;												//ダメージ状態を保つ時間を設定
		g_Player.state = PLAYERSTATE_DAMAGE;

		VERTEX_2D* pVtx;

		//頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);

		//頂点バッファをアンロックする
		g_pVtxBuffPlayer->Unlock();

	}
}

//=============================================================================================================
//プレイヤーの行動
//=============================================================================================================
void ActionPlayer()
{
	if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true)
	{
#ifdef _DEBUG
		if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)
		{//Wキー||上（ゲームパッド）が押された
			g_Player.move.y += cosf(-D3DX_PI * 0.75f) * PLAYER_SPEED;
			g_Player.move.x += sinf(-D3DX_PI * 0.75f) * PLAYER_SPEED;
		}
		else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
		{//Sキー||下（ゲームパッド）が押された
			g_Player.move.y += cosf(-D3DX_PI * 0.25) * PLAYER_SPEED;
			g_Player.move.x += sinf(-D3DX_PI * 0.25f) * PLAYER_SPEED;
		}
		else
		{//Aキー||左（ゲームパッド）だけ
			g_Player.move.x -= PLAYER_SPEED;
		}
#endif
#ifdef _RELEASE
		if (GetKeyboardPress(DIK_A))
		{
			g_Player.move.x -= PLAYER_SPEED;
		}
#endif
	}
	else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true)
	{//Dキーが押された
#ifdef _DEBUG
		if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)
		{//Wキーが押された
			g_Player.move.y += cosf(D3DX_PI * 0.75f) * PLAYER_SPEED;
			g_Player.move.x += sinf(D3DX_PI * 0.75f) * PLAYER_SPEED;
		}
		else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
		{//Sキーが押された
			g_Player.move.y += cosf(D3DX_PI * 0.25f) * PLAYER_SPEED;
			g_Player.move.x += sinf(D3DX_PI * 0.25f) * PLAYER_SPEED;
		}
		else
		{//Dキーだけ
			g_Player.move.x += PLAYER_SPEED;
		}
#endif
#ifdef _RELEASE
		if (GetKeyboardPress(DIK_D) == true)
		{//Dキーだけ
			g_Player.move.x += PLAYER_SPEED;
		}
#endif // 


	}
#ifdef _DEBUG
	else if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)
	{//Wキーが押された
		g_Player.move.y -= PLAYER_SPEED;
	}
	else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
	{//sキーが押された
		g_Player.move.y += PLAYER_SPEED;
	}
	if (KeyboardTrigger(DIK_SPACE) == true || GetJoypadPress(JOYKEY_X) == true)
	{//スペースキー||Xキー（ゲームパッド）
		SetBullet(g_Player.pos, g_Player.rot, g_Player.fLength, 100);
	}
#endif

}

//=============================================================================================================
//プレイヤーの取得
//=============================================================================================================
Player*GetPlayer()
{
	return &g_Player;
}

//=============================================================================================================
//プレイヤーposの初期化処理
//=============================================================================================================
void InitPos()
{
	LPDIRECT3DDEVICE9 pDevice;													//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\effect000.jpg",
		&g_pTexturePlayerPos);

	//頂点バッファのの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayerPos,
		NULL);

	VERTEX_2D* pVtx1;								                           //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayerPos->Lock(0, 0, (void**)&pVtx1, 0);

		//頂点座標の初期化
		pVtx1[0].pos.x = g_Player.pos.x - 5.0f;
		pVtx1[0].pos.y = g_Player.pos.y - 5.0f;
		pVtx1[0].pos.z = 0.0f;

		pVtx1[1].pos.x = g_Player.pos.x + 5.0f;
		pVtx1[1].pos.y = g_Player.pos.y - 5.0f;
		pVtx1[1].pos.z = 0.0f;

		pVtx1[2].pos.x = g_Player.pos.x - 5.0f;
		pVtx1[2].pos.y = g_Player.pos.y + 5.0f;
		pVtx1[2].pos.z = 0.0f;

		pVtx1[3].pos.x = g_Player.pos.x + 5.0f;
		pVtx1[3].pos.y = g_Player.pos.y + 5.0f;
		pVtx1[3].pos.z = 0.0f;

		//rhwの設定
		pVtx1[0].rhw = 1.0f;
		pVtx1[1].rhw = 1.0f;
		pVtx1[2].rhw = 1.0f;
		pVtx1[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx1[0].col = D3DXCOLOR(1.0f, 0.5f, 1.0f, 1.0f);
		pVtx1[1].col = D3DXCOLOR(1.0f, 0.5f, 1.0f, 1.0f);
		pVtx1[2].col = D3DXCOLOR(1.0f, 0.5f, 1.0f, 1.0f);
		pVtx1[3].col = D3DXCOLOR(1.0f, 0.5f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx1[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx1[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx1[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx1[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffPlayerPos->Unlock();

}

//=============================================================================================================
//プレイヤーposの終了処理
//=============================================================================================================
void UninitPos()
{
	//テクスチャの破棄
	if (g_pTexturePlayerPos != NULL)
	{
		g_pTexturePlayerPos->Release();
		g_pTexturePlayerPos = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPlayerPos != NULL)
	{
		g_pVtxBuffPlayerPos->Release();
		g_pVtxBuffPlayerPos = NULL;
	}
}

//=============================================================================================================
//プレイヤーposの更新処理
//=============================================================================================================
void UpdatePos()
{
	VERTEX_2D* pVtx1;								                           //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPlayerPos->Lock(0, 0, (void**)&pVtx1, 0);

		//頂点座標の初期化
		pVtx1[0].pos.x = g_Player.pos.x - 5.0f;
		pVtx1[0].pos.y = g_Player.pos.y - 5.0f;
		pVtx1[0].pos.z = 0.0f;

		pVtx1[1].pos.x = g_Player.pos.x + 5.0f;
		pVtx1[1].pos.y = g_Player.pos.y - 5.0f;
		pVtx1[1].pos.z = 0.0f;

		pVtx1[2].pos.x = g_Player.pos.x - 5.0f;
		pVtx1[2].pos.y = g_Player.pos.y + 5.0f;
		pVtx1[2].pos.z = 0.0f;

		pVtx1[3].pos.x = g_Player.pos.x + 5.0f;
		pVtx1[3].pos.y = g_Player.pos.y + 5.0f;
		pVtx1[3].pos.z = 0.0f;


	//頂点バッファをアンロックする
	g_pVtxBuffPlayerPos->Unlock();
}

//=============================================================================================================
//プレイヤーposの描画処理
//=============================================================================================================
void DrawPos()
{
	LPDIRECT3DDEVICE9 pDevice;														//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	if (g_Player.bUse == true)
	{

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffPlayerPos, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//αブレンディングを加算合成に設定
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePlayerPos);

		//プレイヤーの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		//αブレンディングを元に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}