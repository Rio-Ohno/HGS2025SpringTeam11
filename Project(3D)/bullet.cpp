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
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);									//移動量の初期化
		g_aBullet[nCntBullet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);									//角度の初期化
		g_aBullet[nCntBullet].user = BULLETUSER_NULL;												//ユーザーの初期化
		g_aBullet[nCntBullet].type = BULLETTYPE_NULL;												//弾の種類の初期化
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
			int nCntEnemy;

			//前の位置の保存
			g_aBullet[nCntBullet].oldpos = g_aBullet[nCntBullet].pos;

			//プレイヤーの取得
			pPlayer = GetPlayer();

			if (g_aBullet[nCntBullet].user == BULLETUSER_ENEMY)					//敵の弾
			{	
				if (pPlayer->bUse == true)
				{
					//プレイヤーとの当たり判定
					if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x - 5.0f
						&& g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + 5.0f
						&& g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y - 5.0f
						&& g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + 5.0f 
						&& pPlayer->state != PLAYERSTATE_APPEAR)
					{

						g_aBullet[nCntBullet].bUse = false;
					}
					//プレイヤーとの当たり判定(グレイズ)
					else if (g_aBullet[nCntBullet].pos.x >= pPlayer->pos.x + sinf(pPlayer->rot.z - (D3DX_PI - pPlayer->fAngle)) * pPlayer->fLength
							&& g_aBullet[nCntBullet].pos.x <= pPlayer->pos.x + sinf(pPlayer->rot.z + (D3DX_PI - pPlayer->fAngle)) * pPlayer->fLength
							&& g_aBullet[nCntBullet].pos.y >= pPlayer->pos.y + cosf(pPlayer->rot.z - (D3DX_PI - pPlayer->fAngle)) * pPlayer->fLength
							&& g_aBullet[nCntBullet].pos.y <= pPlayer->pos.y + cosf(pPlayer->rot.z - (  0.0f  - pPlayer->fAngle)) * pPlayer->fLength
							&& pPlayer->state != PLAYERSTATE_APPEAR)
					{
						//スコアに5を加算
						AddScore(5);
					}
				}
			}
		}
		
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

		if (g_aBullet[nCntBullet].pos.x<=0.0f|| g_aBullet[nCntBullet].pos.x >= SCREEN_WIDTH
			                                  || g_aBullet[nCntBullet].pos.y <= 0.0f|| g_aBullet[nCntBullet].pos.y >= SCREEN_HEIGHT)//弾が画面外にでた
		{
			//g_aBullet[nCntBullet].bUse = false;//使用していない状態にする
			//nNumBullet--;
		}

		g_aBullet[nCntBullet].fLife --;//寿命デクリメント

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
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, float fLength,float fLife,BULLETUSER user,BULLETTYPE type)
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
			g_aBullet[nCntBullet].user = user;
			g_aBullet[nCntBullet].type = type;

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
			g_aBullet[nCntBullet].move = move;
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
{

}