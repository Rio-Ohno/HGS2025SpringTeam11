//==================================================================
//
//ブロック処理<block.cpp>
//Author:Saeki Takuto
//
//==================================================================

#include "block.h"
#include "main.h"
#include "Block.h"
#include "input.h"
#include "player.h"
#include "stage.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureBlock[TYPE_BLOCK] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;//頂点バッファへのポインタ
Block g_aBlock[MAX_BLOCK];					//弾の情報
int nNowStage;

//弾の初期化処理
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBlock;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/block000.jpg",//通常ブロック
		&g_pTextureBlock[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/block001.png",//左ベルコンブロック
		&g_pTextureBlock[1]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/block002.png",//右ベルコンブロック
		&g_pTextureBlock[2]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/block003.png",//トランポリンブロック
		&g_pTextureBlock[3]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/block004.png",//左ベルコン強化ブロック
		&g_pTextureBlock[4]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/block005.png",//右ベルコン強化ブロック
		&g_pTextureBlock[5]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/block006.png",//クリアボタンブロック
		&g_pTextureBlock[6]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/block007.png",//クリアボタンブロック
		&g_pTextureBlock[7]);

	//弾の情報の初期化
	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		g_aBlock[nCntBlock].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBlock[nCntBlock].fWidth = 0.0f;
		g_aBlock[nCntBlock].fHeight = 0.0f;
		g_aBlock[nCntBlock].bUse = false;//使用していない状態にする

	}
	nNowStage = 0;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlock,
		NULL);

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].fWidth / 2), g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].fHeight / 2), g_aBlock[nCntBlock].pos.z);
		pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth / 2), g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].fHeight / 2), g_aBlock[nCntBlock].pos.z);
		pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].fWidth / 2), g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHeight / 2), g_aBlock[nCntBlock].pos.z);
		pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth / 2), g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHeight / 2), g_aBlock[nCntBlock].pos.z);

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
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;//頂点データのポインタを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffBlock->Unlock();
}

//弾の終了処理
void UninitBlock(void)
{
	for (int nCnt = 0; nCnt < TYPE_BLOCK; nCnt++)
	{
		//テクスチャの破棄
		if (g_pTextureBlock[nCnt] != NULL)
		{
			g_pTextureBlock[nCnt]->Release();
			g_pTextureBlock[nCnt] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffBlock != NULL)
	{
		g_pVtxBuffBlock->Release();
		g_pVtxBuffBlock = NULL;
	}
}

//弾の更新処理
void UpdateBlock(void)
{
	int nCntBlock;
	Player* pPlayer = GetPlayer();

	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			g_aBlock[nCntBlock].pos += g_aBlock[nCntBlock].move;
		}

		//頂点バッファをロックし、頂点データへのポインタを取得
		pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].fWidth / 2), g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].fHeight / 2), g_aBlock[nCntBlock].pos.z);
		pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth / 2), g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].fHeight / 2), g_aBlock[nCntBlock].pos.z);
		pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].fWidth / 2), g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHeight / 2), g_aBlock[nCntBlock].pos.z);
		pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth / 2), g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHeight / 2), g_aBlock[nCntBlock].pos.z);

		pVtx += 4;
	}


	if (pPlayer->bGoal == true)
	{
		nNowStage++;
		if (nNowStage > MAX_STAGE)
		{
			SetFade(MODE_RESULT);
		}
		else
		{
			for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
			{
				g_aBlock[nCntBlock].bUse = false;
			}

			LoadStage(nNowStage);

			pPlayer->bGoal = false;
		}
	}
	else if (pPlayer->bFall == true)
	{
			for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
			{
				g_aBlock[nCntBlock].bUse = false;
			}

			LoadStage(nNowStage);

			pPlayer->bFall = false;
	}


	g_pVtxBuffBlock->Unlock();
}

//弾の描画処理
void DrawBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntBlock;

	//デバイスの処理
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBlock, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);


	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{//弾が使用されている

			if (g_aBlock[nCntBlock].ntype == BLOCKTYPE_NORMAL)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureBlock[0]);
			}
			else if (g_aBlock[nCntBlock].ntype == BLOCKTYPE_CONVEYOR_L)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureBlock[1]);
			}
			else if (g_aBlock[nCntBlock].ntype == BLOCKTYPE_CONVEYOR_R)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureBlock[2]);
			}
			else if (g_aBlock[nCntBlock].ntype == BLOCKTYPE_TRAMPOLINE)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureBlock[3]);
			}
			else if (g_aBlock[nCntBlock].ntype == BLOCKTYPE_CONVEYOR_L_HIGH)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureBlock[4]);
			}
			else if (g_aBlock[nCntBlock].ntype == BLOCKTYPE_CONVEYOR_R_HIGH)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureBlock[5]);
			}
			else if (g_aBlock[nCntBlock].ntype == BLOCKTYPE_CLEARBUTTON)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureBlock[6]);
			}
			else if (g_aBlock[nCntBlock].ntype == BLOCKTYPE_SOUL)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureBlock[7]);
			}


			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				4 * nCntBlock,//頂点インデックス
				2);
		}
	}
}

//弾の設定処理
void SetBlock(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight, int ntype)
{
	int nCntBlock;
	VERTEX_2D* pVtx;							//頂点情報へのポインタ

	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);


	for (nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == false)
		{//弾が使用されていない
			g_aBlock[nCntBlock].pos = pos;
			g_aBlock[nCntBlock].move = move;
			g_aBlock[nCntBlock].fWidth = fWidth;
			g_aBlock[nCntBlock].fHeight = fHeight;
			g_aBlock[nCntBlock].ntype = ntype;

			g_aBlock[nCntBlock].bUse = true;//使用している状態にする

			//頂点バッファをロックし、頂点データへのポインタを取得
			pVtx[0].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].fWidth / 2), g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].fHeight / 2), g_aBlock[nCntBlock].pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth / 2), g_aBlock[nCntBlock].pos.y - (g_aBlock[nCntBlock].fHeight / 2), g_aBlock[nCntBlock].pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].fWidth / 2), g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHeight / 2), g_aBlock[nCntBlock].pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth / 2), g_aBlock[nCntBlock].pos.y + (g_aBlock[nCntBlock].fHeight / 2), g_aBlock[nCntBlock].pos.z);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(fWidth / WIDTHBLOCK, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, fHeight / HEIGHTBLOCK);
			pVtx[3].tex = D3DXVECTOR2(fWidth / WIDTHBLOCK, fHeight / HEIGHTBLOCK);

			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffBlock->Unlock();

}

//ブロック当たり判定(X)
void CollisionBlockX(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fHeight, float fWidth,Block **pBlock)
{
	bool bLanding = false;//着地しているかどうか

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			//プレイヤーのyの範囲がブロックに重なっている左右の当たり判定
			if (pPosOld->y > g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHeight / 2 && pPosOld->y - PLAYER_HEIGHT < g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight / 2)
			{
				//左から右にブロックがめり込んだ場合
				if (pPosOld->x + PLAYER_WIDTH / 2 + g_aBlock[nCntBlock].move.x <= g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].fWidth / 2) + pMove->x && pPos->x + PLAYER_WIDTH / 2 > g_aBlock[nCntBlock].pos.x - (g_aBlock[nCntBlock].fWidth / 2))
				{
					if (g_aBlock[nCntBlock].ntype == BLOCKTYPE_SOUL)
					{
						g_aBlock[nCntBlock].bUse = false;
					}
					else
					{
						pPos->x = g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fWidth / 2 - PLAYER_WIDTH / 2;
						pMove->x = 0.0f;
						*pBlock = &g_aBlock[nCntBlock];
					}
				}
				//右から左にブロックがめり込んだ場合
				else if (pPosOld->x - PLAYER_WIDTH / 2 + g_aBlock[nCntBlock].move.x >= g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth / 2) + pMove->x && pPos->x - PLAYER_WIDTH / 2 < g_aBlock[nCntBlock].pos.x + (g_aBlock[nCntBlock].fWidth / 2))
				{
					if (g_aBlock[nCntBlock].ntype == BLOCKTYPE_SOUL)
					{
						g_aBlock[nCntBlock].bUse = false;
					}
					else
					{
						pPos->x = g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth / 2 + PLAYER_WIDTH / 2;
						pMove->x = 0.0f;
						*pBlock = &g_aBlock[nCntBlock];
					}
				}
			}

		}
	}
}

//ブロックの当たり判定(Y)
bool CollisionBlockY(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, float fHeight, float fWidth,Block **pBlock)
{
	bool bLanding = false;//着地しているかどうか

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true)
		{
			//プレイヤーのxの範囲がブロックに重なっている上下の当たり判定
			if (pPos->x + PLAYER_WIDTH / 2 > g_aBlock[nCntBlock].pos.x - g_aBlock[nCntBlock].fWidth / 2 && pPos->x - PLAYER_WIDTH / 2 < g_aBlock[nCntBlock].pos.x + g_aBlock[nCntBlock].fWidth / 2)
			{
				//上から下にブロックがめり込んだ場合
				if (pPosOld->y <= g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHeight / 2 && pPos->y > g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHeight / 2)
				{
					if (g_aBlock[nCntBlock].ntype == BLOCKTYPE_SOUL)
					{
						g_aBlock[nCntBlock].bUse = false;
					}
					else
					{

						bLanding = true;
						pPos->y = g_aBlock[nCntBlock].pos.y - g_aBlock[nCntBlock].fHeight / 2;
						pMove->y = 0.0f;
						*pBlock = &g_aBlock[nCntBlock];
					}
				}
				//下から上にブロックがめり込んだ場合
				else if (pPosOld->y - PLAYER_HEIGHT >= g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight / 2 && pPos->y - PLAYER_HEIGHT < g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight / 2)
				{
					if (g_aBlock[nCntBlock].ntype == BLOCKTYPE_SOUL)
					{
						g_aBlock[nCntBlock].bUse = false;
					}
					else
					{
						pPos->y = g_aBlock[nCntBlock].pos.y + g_aBlock[nCntBlock].fHeight / 2 + PLAYER_HEIGHT;
						pMove->y = 0.0f;
					}
				}

			}

		}
	}
	return bLanding;
}

int GetNumSoul()
{
	int nCntSoul = 0;

	for (int nCntBlock = 0; nCntBlock < MAX_BLOCK; nCntBlock++)
	{
		if (g_aBlock[nCntBlock].bUse == true&&g_aBlock[nCntBlock].ntype==BLOCKTYPE_SOUL)
		{
			nCntSoul++;
		}
	}

	return nCntSoul;
}
