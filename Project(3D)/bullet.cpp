//============================================================
//
//弾[bullet.cpp]
//Author:Rio Ohno
//
//============================================================

#include"bullet.h"
//#include"exploosion.h"
#include"player.h"
#include"score.h"

//マクロ定義
#define MAX_BULLET (256)                                   //弾の最大数
#define BULLET_WIDTH (10)                                  //ポリゴンの幅
#define BULLET_HEIGHT (10)                                 //ポリゴンの高さ
#define BULLET_SPEED ((float)1.0f)						   //弾のスピード

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;				   //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;           //頂点バッファへのポインタ
Bullet g_aBullet[MAX_BULLET];                              //弾の情報
int nNumBullet = 0;											

//=============================================================================================================
//初期化処理
//=============================================================================================================
void InitBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBullet = NULL;

	//デバイスに取得
	pDevice = GetDevice();

	//テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bullet000.png",
		&g_pTextureBullet);


	//弾の情報の初期化
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);									//位置の初期化
		g_aBullet[nCntBullet].move = D3DXVECTOR3(-BULLET_SPEED*0.5f, BULLET_SPEED, 0.0f);									//移動量の初期化
		g_aBullet[nCntBullet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);									//角度の初期化

		//対角線の長さを算出する
		g_aBullet[nCntBullet].fLengthBullet = sqrtf(BULLET_WIDTH * BULLET_WIDTH + BULLET_HEIGHT * BULLET_HEIGHT) / 2.0f;

		//対角線の角度を算出する
		g_aBullet[nCntBullet].fAngleBullet = atan2f(BULLET_WIDTH, BULLET_HEIGHT);

		g_aBullet[nCntBullet].fLife = 0;															//弾の寿命の初期化
		g_aBullet[nCntBullet].bUse = false;															//使用していない状態にする
	}

	nNumBullet = 0;																					//弾数の初期化

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D)*4 * MAX_BULLET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_2D * pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		//頂点座標の設定
 		pVtx[0].pos.x = 0.0f;
		pVtx[0].pos.y = 0.0f;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = 0.0f;
		pVtx[1].pos.y = 0.0f;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = 0.0f;
		pVtx[2].pos.y = 0.0f;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = 0.0f;
		pVtx[3].pos.y = 0.0f;
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

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f,0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f,0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f,1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f,1.0f);

		pVtx += 4;//頂点データのポインタを4つ分進める
	}

	//頂点バッファのアンロック
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================================================
//弾の終了処理
//=============================================================================================================
void UninitBullet()
{
	//テクスチャの破棄
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//=============================================================================================================
//弾の更新処理
//=============================================================================================================
void UpdateBullet()
{
	int nCntBullet = NULL;
	int nCnt = 0;

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用されているとき

			Player* pPlayer;																//プレイヤーへのポインタ

			//プレイヤーの取得
			pPlayer = GetPlayer();

			if (pPlayer->bUse == true)
			{
				if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - NUM_WIDTH
					&& g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + NUM_WIDTH
					&& g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - NUM_HEIGHT
					&& g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + NUM_HEIGHT
					)
				{
					ReflectBullet(nCntBullet);
				}
			}
		}

		//g_aBullet[nCntBullet].move.y += g_aBullet[nCntBullet].move.y * sinf(g_aBullet[nCntBullet].rot.z);
		//g_aBullet[nCntBullet].move.x += g_aBullet[nCntBullet].move.x * cosf(g_aBullet[nCntBullet].rot.z);

		//前の位置の保存
		g_aBullet[nCntBullet].oldpos = g_aBullet[nCntBullet].pos;

		//弾の位置更新
		g_aBullet[nCntBullet].pos.y += g_aBullet[nCntBullet].move.y;
		g_aBullet[nCntBullet].pos.x += g_aBullet[nCntBullet].move.x;


		//頂点座標の設定
		pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z - (D3DX_PI - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
		pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z - (D3DX_PI - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (D3DX_PI - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
		pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (D3DX_PI - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (0.0f - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
		pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (0.0f - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z - (0.0f - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
		pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z - (0.0f - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
		pVtx[3].pos.z = 0.0f;

		//if (g_aBullet[nCntBullet].pos.x<=0.0f|| g_aBullet[nCntBullet].pos.x >= SCREEN_WIDTH
		//	                                  || g_aBullet[nCntBullet].pos.y <= 0.0f|| g_aBullet[nCntBullet].pos.y >= SCREEN_HEIGHT)//弾が画面外にでた
		//{
		//}

		//g_aBullet[nCntBullet].fLife --;//寿命デクリメント

		if (g_aBullet[nCntBullet].fLife <= 0)//寿命が尽きた
		{
			g_aBullet[nCntBullet].bUse = false;//使用していない状態にする
			nNumBullet--;
		}
		pVtx += 4;
	}
	//アンロック
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================================================
//弾の描画処理
//=============================================================================================================
void DrawBullet()
{
	int nCntBullet;
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{//弾が使用されている

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBullet);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,nCntBullet*4,2);
		}
	}
}

//=============================================================================================================
//弾の設定処理
//=============================================================================================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fLength,float fLife)
{
	VERTEX_2D* pVtx;
	int nCntBullet;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{
			//弾が使用されていないとき
			g_aBullet[nCntBullet].pos = pos;
			g_aBullet[nCntBullet].rot = rot;
			g_aBullet[nCntBullet].fLife = fLife;

			//目標の移動方向（角度）の補正
			if (g_aBullet[nCntBullet].rot.z > D3DX_PI)
			{
				g_aBullet[nCntBullet].rot.z -= D3DX_PI * 2.0f;
			}
			else if (g_aBullet[nCntBullet].rot.z < -D3DX_PI)
			{
				g_aBullet[nCntBullet].rot.z += D3DX_PI * 2.0f;
			}

			//頂点座標の設定
			pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z - (D3DX_PI - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
			pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z - (D3DX_PI - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (D3DX_PI - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
			pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (D3DX_PI - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + (0.0f - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
			pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + (0.0f - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z - (0.0f - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
			pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z - (0.0f - g_aBullet[nCntBullet].fAngleBullet)) * (g_aBullet[nCntBullet].fLengthBullet / 5);
			pVtx[3].pos.z = 0.0f;

			g_aBullet[nCntBullet].fLengthBullet = fLength;
			g_aBullet[nCntBullet].bUse = true;//使用してる状態にする
			nNumBullet++;//弾の数カウント
			break;
		}
		pVtx += 4;
	}
	//アンロック
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================================================
//弾の数の取得
//=============================================================================================================
int GetNumBullet()
{
	return nNumBullet;
}

//=============================================================================================================
//弾の反射処理
//=============================================================================================================
void ReflectBullet(int indx)
{//プレイヤーとの当たり判定
	//プレイヤーの取得
	Player *pPlayer = GetPlayer();

		D3DXVECTOR3 vecplayer = D3DXVECTOR3(pPlayer->pos.x - NUM_WIDTH - pPlayer->pos.x + NUM_WIDTH, pPlayer->pos.y - NUM_HEIGHT - pPlayer->pos.y + NUM_HEIGHT, 0.0f);
		D3DXVECTOR3 vecBullet = g_aBullet[indx].pos - g_aBullet[indx].oldpos;
		D3DXVECTOR3 nor;
		D3DXVECTOR3 c, e, d, f, g = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		float Dot;

		D3DXVec3Cross(&nor, &vecplayer, &vecBullet);

		//逆ベクトルにする
		vecBullet *= -1.0f;

		//内積
		Dot = D3DXVec3Dot(&vecBullet, &nor);
		c = vecBullet + Dot * nor;
		e = vecBullet + 2 * Dot * nor;

		//当たり判定(ｙ軸)
	if (g_aBullet[indx].pos.x >= pPlayer->pos.x - NUM_WIDTH
		&& g_aBullet[indx].pos.x <= pPlayer->pos.x + NUM_WIDTH)
	{
		if (g_aBullet[indx].oldpos.y <=  pPlayer->pos.y - NUM_HEIGHT && g_aBullet[indx].pos.y > pPlayer->pos.y - NUM_HEIGHT)
		{
			g_aBullet[indx].move.y = e.y;
		}
		else if (g_aBullet[indx].oldpos.y >= pPlayer->pos.y + NUM_HEIGHT && g_aBullet[indx].pos.y < pPlayer->pos.y + NUM_HEIGHT)
		{
			g_aBullet[indx].move.y = e.y;
		}
	}
	//当たり判定(ｘ軸)
	if (g_aBullet[indx].pos.y >= pPlayer->pos.y - NUM_HEIGHT
		&& g_aBullet[indx].pos.y <= pPlayer->pos.y + NUM_HEIGHT)
	{
		if (g_aBullet[indx].oldpos.x <= pPlayer->pos.x - NUM_WIDTH && g_aBullet[indx].pos.x > pPlayer->pos.x - NUM_WIDTH)
		{
			g_aBullet[indx].move.x = e.x;
			g_aBullet[indx].move.y = e.y;
		}
		else if (g_aBullet[indx].oldpos.x >= pPlayer->pos.x + NUM_WIDTH && g_aBullet[indx].pos.x < pPlayer->pos.x + NUM_WIDTH)
		{
			g_aBullet[indx].move.x = e.x;
			g_aBullet[indx].move.y = e.y;
		}
	}
}