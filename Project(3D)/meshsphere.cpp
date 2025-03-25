//============================================================
//
//メッシュスフィア[meshSphere.cpp]
//Author:Rio Ohno
//
//============================================================

// インクルード
#include "meshSphere.h"
#include "meshfield.h"
#include <cassert>

// グローバル変数宣言
MeshSphere g_Sphere[MAX_MESHSPHERE];									// 球体構造体
LPDIRECT3DTEXTURE9 g_pTexture;											// テクスチャ用ポインタ

//======================================
//球の初期化処理
//======================================
void InitSphere(void)
{
	// 各種初期化
	for (int nCnt = 0; nCnt < MAX_MESHSPHERE; nCnt++)
	{
		g_Sphere[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 位置
		g_Sphere[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// 向き
		g_Sphere[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// 色
		g_Sphere[nCnt].pVtxBuff = NULL;									// 頂点情報のポインタ
		g_Sphere[nCnt].pIndxBuff = NULL;								// インデックスバッファ
		g_Sphere[nCnt].textype = 0;										// テクスチャの種類
		g_Sphere[nCnt].DiviX = 0;										// xの分割数
		g_Sphere[nCnt].DiviY = 0;										// yの分割数
		g_Sphere[nCnt].fRadius = 0.0f;									// 半径
		g_Sphere[nCnt].bHalf = false;									// 半球ではない状態にする
		g_Sphere[nCnt].bHead = true;									// 法線を表に向ける
		g_Sphere[nCnt].bUse = false;									// 使用していない状態にする
	}
	g_pTexture = NULL;													// テクスチャポインタ
}

//======================================
// 球の終了処理
//======================================
void UninitSphere(void)
{
	for (int nCnt = 0; nCnt < MAX_MESHSPHERE; nCnt++)
	{
		// 頂点情報の破棄
		if (g_Sphere[nCnt].pVtxBuff != NULL)
		{
			g_Sphere[nCnt].pVtxBuff->Release();
			g_Sphere[nCnt].pVtxBuff = NULL;
		}

		// テクスチャの破棄
		if (g_pTexture != NULL)
		{
			g_pTexture->Release();
			g_pTexture = NULL;
		}

		// インデックスバッファの破棄
		if (g_Sphere[nCnt].pIndxBuff != NULL)
		{
			g_Sphere[nCnt].pIndxBuff->Release();
			g_Sphere[nCnt].pIndxBuff = NULL;
		}
	}
}

//======================================
// 球の更新処理
//======================================
void UpdateSphere(void)
{

}

//======================================
// 球の描画処理
//======================================
void DrawSphere(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	// ライトを切る
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int nCnt = 0; nCnt < MAX_MESHSPHERE; nCnt++)
	{
		if (g_Sphere[nCnt].bUse == true)
		{
			// ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Sphere[nCnt].mtxWorld);

			//向きを反転
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Sphere[nCnt].rot.y, g_Sphere[nCnt].rot.x, g_Sphere[nCnt].rot.z);
			D3DXMatrixMultiply(&g_Sphere[nCnt].mtxWorld, &g_Sphere[nCnt].mtxWorld, &mtxRot);

			// 位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Sphere[nCnt].pos.x, g_Sphere[nCnt].pos.y, g_Sphere[nCnt].pos.z);
			D3DXMatrixMultiply(&g_Sphere[nCnt].mtxWorld, &g_Sphere[nCnt].mtxWorld, &mtxTrans);

			// ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Sphere[nCnt].mtxWorld);

			// 頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_Sphere[nCnt].pVtxBuff, 0, sizeof(VERTEX_3D));

			// 頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			// テクスチャのポインタを取得
			LPDIRECT3DTEXTURE9 pTexture = GetTexture2(g_Sphere[nCnt].textype);

			// テクスチャの設定
			pDevice->SetTexture(0, pTexture);

			// ポリゴンを描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, g_Sphere[nCnt].DiviX);

			// 二段以上なら
			if (g_Sphere[nCnt].DiviY >= 2)
			{
				// インデックスバッファをデータストリームに設定
				pDevice->SetIndices(g_Sphere[nCnt].pIndxBuff);

				// メッシュ壁を描画
				pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 1, 1, g_Sphere[nCnt].nMaxVtx, 0, g_Sphere[nCnt].nPolyNum);
			}
		}
	}
	// ライトをつける
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//======================================
// 球の設定処理
//======================================
int SetSphere(D3DXVECTOR3 pos, int textype, int DiviX, int DiviY, float fRadius, bool bHead, bool bHalf)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_3D* pVtx = NULL;
	int nCnt = 0;

	for (nCnt = 0; nCnt < MAX_MESHSPHERE; nCnt++)
	{
		if (g_Sphere[nCnt].bUse == false)
		{
			//変数宣言
			int indx = 0;																		// 頂点インデックス
			//float q, q2 = 0;																	// 計算結果保存用
			D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);									// ベクトルの保存用
			D3DXVECTOR3 VtxPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);									// 頂点座標保存用

			//各種設定
			g_Sphere[nCnt].pos = pos;															// 位置
			g_Sphere[nCnt].textype = textype;													// テクスチャの種類
			g_Sphere[nCnt].DiviX = DiviX;														// 分割数
			g_Sphere[nCnt].DiviY = DiviY;														// 分割数
			g_Sphere[nCnt].fRadius = fRadius;													// 半径
			g_Sphere[nCnt].bHead = bHead;														// 法線
			g_Sphere[nCnt].bHalf = bHalf;														// 半球かどうか

			g_Sphere[nCnt].nMaxVtx = (g_Sphere[nCnt].DiviX + 1) * (g_Sphere[nCnt].DiviY) + 1;	// 頂点数

			//頂点バッファの生成
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) *g_Sphere[nCnt].nMaxVtx,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_Sphere[nCnt].pVtxBuff,
				NULL);

			//頂点バッファをロックし、頂点情報へのポインタを取得
			g_Sphere[nCnt].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			for (int nCntY = 0; nCntY <= g_Sphere[nCnt].DiviY; nCntY++)
			{
				for (int nCntX = 0; nCntX <= g_Sphere[nCnt].DiviX; nCntX++)
				{
					int i = nCntX;

					// 半球ではないなら
					if (g_Sphere[nCnt].bHead == false)
					{
						i = g_Sphere[nCnt].DiviX - nCntX;
					}
				
					//角度格納
					float fAngle = ((D3DX_PI * 2 / g_Sphere[nCnt].DiviX) * i);									// y軸
					float fAngle2 = (D3DX_PI / g_Sphere[nCnt].DiviY) * (g_Sphere[nCnt].DiviY - nCntY);			// z軸

					//半球なら
					if (g_Sphere[nCnt].bHalf == true)
					{
						fAngle2 = (((D3DX_PI * 0.5f) / g_Sphere[nCnt].DiviY)* nCntY);
					}

					//頂点の位置の設定

					VtxPos = D3DXVECTOR3(
						(float)g_Sphere[nCnt].fRadius * sinf(fAngle2) * sinf(fAngle), 
						(float)g_Sphere[nCnt].fRadius * cosf(fAngle2), 
						(float)g_Sphere[nCnt].fRadius * sinf(fAngle2) * cosf(fAngle));

					pVtx[indx].pos = VtxPos;

					//外側へのベクトル
					vec = pVtx[indx].pos - g_Sphere[nCnt].pos;

					//ベクトルの正規化,各頂点の法線の設定
					D3DXVec3Normalize(&pVtx[indx].nor, &vec);

					//頂点カラーの設定
					pVtx[indx].col = g_Sphere[nCnt].col;

					//テクスチャ座標の設定
					pVtx[indx].tex = D3DXVECTOR2((1.0f / g_Sphere[nCnt].DiviX) * nCntX, (1.0f / g_Sphere[nCnt].DiviY) * nCntY);

					// 0番目だったら
					if (indx == 0)
					{
						//テクスチャ座標の設定
						pVtx[indx].tex = D3DXVECTOR2((1.0f / g_Sphere[nCnt].DiviX) * (g_Sphere[nCnt].DiviX * 0.5f), 0.0f);
					}

					//頂点インデックスを進める
					indx++;
					assert(indx >= 0 && indx <= g_Sphere[nCnt].nMaxVtx);

					//頂点が0番目だったなら
					if (indx == 1)
					{
						//ループを抜ける
						break;
					}
				}
			}

			//頂点バッファをアンロック
			g_Sphere[nCnt].pVtxBuff->Unlock();

			//インデックスバッファの設定
			if (g_Sphere[nCnt].DiviY >= 2)
			{
				//インデックスへのポインタ
				WORD* pIdx = NULL;
				int indexNum = (2 * ((g_Sphere[nCnt].DiviY - 1) * (2 + g_Sphere[nCnt].DiviX) - 1));													// インデックス
				g_Sphere[nCnt].nPolyNum = (2 * g_Sphere[nCnt].DiviX * (g_Sphere[nCnt].DiviY - 1) + (g_Sphere[nCnt].DiviY - 2) * 4);					// ポリゴン数

				//インデックスバッファの生成
				pDevice->CreateIndexBuffer(sizeof(WORD) * indexNum,
					D3DUSAGE_WRITEONLY,
					D3DFMT_INDEX16,
					D3DPOOL_MANAGED,
					&g_Sphere[nCnt].pIndxBuff,
					NULL);

				//インデックスバッファをロック
  				g_Sphere[nCnt].pIndxBuff->Lock(0, 0, (void**)&pIdx, 0);

				int nCntX = 0;
				int Indx = 0;

				for (int nCntY = 0; nCntY < g_Sphere[nCnt].DiviY-1; nCntY++)
				{
					for (nCntX = g_Sphere[nCnt].DiviX; nCntX >= 0; nCntX--)
					{
						//インデックスの設定
						pIdx[0] = (g_Sphere[nCnt].DiviX + 1) * (nCntY + 1) + nCntX;
						pIdx[1] = nCntX + (nCntY * (g_Sphere[nCnt].DiviX + 1));

						pIdx += 2;
						Indx += 2;
					}

					//衰退ポリゴン分
					if (nCntY < g_Sphere[nCnt].DiviY - 2)
					{
						//インデックスの設定
						pIdx[0] = (nCntX + 1) + (nCntY * (g_Sphere[nCnt].DiviX + 1));
						pIdx[1] = (g_Sphere[nCnt].DiviX + 1) * (nCntY + 2) + g_Sphere[nCnt].DiviX;

						pIdx += 2;
						Indx += 2;
					}

					assert(Indx <= indexNum);
					assert(nCntY >= 0 && nCntY <= g_Sphere[nCnt].DiviY);
				}

				//インデックスバッファをアンロック
				g_Sphere[nCnt].pIndxBuff->Unlock();
			}

			//球を使用してる状態にする
			g_Sphere[nCnt].bUse = true;
			break;
		}
	}
	return nCnt;						//球のインデックスを返す
}

//======================================
// 球の位置の設定処理
//======================================
void SetSpherePos(int indx, D3DXVECTOR3 pos)
{
	g_Sphere[indx].pos = pos;
}

//======================================
// メッシュ壁のテクスチャ設定
//======================================
void SetSphereTexture(int indx)
{
	//テクスチャのポインタを取得
	LPDIRECT3DTEXTURE9 pTexture = GetTexture2(g_Sphere[indx].textype);

	if (pTexture != NULL)
	{
		g_pTexture = pTexture;
	}
}

//======================================
// 球の位置の設定処理
//======================================
void DeleteSphere(int indx)
{
	//使用していない状態にする
	g_Sphere[indx].bUse = false;
}