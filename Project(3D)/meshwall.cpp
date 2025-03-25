//==============================================
//
// メッシュ壁表示処理[meshWall.cpp]
// Author:kaiti
//
//==============================================

#include "meshwall.h"
#include "meshfield.h"

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureMeshWall = NULL;		//テクスチャへのポインタ

MeshWall g_MeshWall[MAX_MESHWALL];									//ポリゴン(縦)の構造体
static char walltexName[MAX_TEX_WALL][32];							//テクスチャファイル名保存用

//=================================
// メッシュ壁の初期化処理
//=================================
void InitMeshWall(void)
{

	for (int nCnt = 0; nCnt < MAX_MESHWALL; nCnt++)
	{
		g_MeshWall[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_MeshWall[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_MeshWall[nCnt].IdxBuffMeshWall = { NULL };
		g_MeshWall[nCnt].VtxBuffMeshWall = { NULL };
		g_MeshWall[nCnt].textype = 0;
		g_MeshWall[nCnt].nDiviX = 0;
		g_MeshWall[nCnt].nDiviY = 0;
		g_MeshWall[nCnt].nDiviZ = 0;
		g_MeshWall[nCnt].nWidth = 0;
		g_MeshWall[nCnt].nHeight = 0;
		g_MeshWall[nCnt].nMaxVtx = 0;
		g_MeshWall[nCnt].nPolyNum = 0;
		g_MeshWall[nCnt].bUse = false;
	}
}

//===============================
// メッシュ壁の終了処理
//===============================
void UninitMeshWall(void)
{
	
	//テクスチャの破棄
	if (g_pTextureMeshWall != NULL)
	{
		g_pTextureMeshWall->Release();
		g_pTextureMeshWall = NULL;
	}

	for (int nCnt = 0; nCnt < MAX_MESHWALL; nCnt++)
	{
		//頂点バッファの破棄
		if (g_MeshWall[nCnt].VtxBuffMeshWall != NULL)
		{
			g_MeshWall[nCnt].VtxBuffMeshWall->Release();
			g_MeshWall[nCnt].VtxBuffMeshWall = NULL;
		}

		//インデックスバッファの破棄
		if (g_MeshWall[nCnt].IdxBuffMeshWall != NULL)
		{
			g_MeshWall[nCnt].IdxBuffMeshWall->Release();
			g_MeshWall[nCnt].IdxBuffMeshWall = NULL;
		}
	}
}

//===============================
// メッシュ壁の更新処理
//===============================
void UpdateMeshWall(void)
{

}

//===============================
// メッシュ壁の描画処理
//===============================
void DrawMeshWall(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	for (int nCnt = 0; nCnt < MAX_MESHWALL; nCnt++)
	{
		if (g_MeshWall[nCnt].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_MeshWall[nCnt].mtxWorld);

			//向きを反転
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshWall[nCnt].rot.y, g_MeshWall[nCnt].rot.x, g_MeshWall[nCnt].rot.z);
			D3DXMatrixMultiply(&g_MeshWall[nCnt].mtxWorld, &g_MeshWall[nCnt].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_MeshWall[nCnt].pos.x, g_MeshWall[nCnt].pos.y, g_MeshWall[nCnt].pos.z);
			D3DXMatrixMultiply(&g_MeshWall[nCnt].mtxWorld, &g_MeshWall[nCnt].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_MeshWall[nCnt].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_MeshWall[nCnt].VtxBuffMeshWall, 0, sizeof(VERTEX_3D));

			//インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_MeshWall[nCnt].IdxBuffMeshWall);

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureMeshWall);
			//メッシュ壁を描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_MeshWall[nCnt].nMaxVtx, 0, g_MeshWall[nCnt].nPolyNum);
		}
	}
}

//===================
// メッシュ壁の設定
//===================
void SetMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int textype, int nDiviX, int nDiviZ, int nDiviY, int nWidth, int nHeight)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	WORD* pIdx = NULL;

	for (int nCnt = 0; nCnt < MAX_MESHWALL; nCnt++)
	{
		if (g_MeshWall[nCnt].bUse == false)
		{
			g_MeshWall[nCnt].pos = pos;						//位置
			g_MeshWall[nCnt].rot = rot;						//向き
			g_MeshWall[nCnt].textype = textype;				//テクスチャタイプ
			g_MeshWall[nCnt].nDiviX = nDiviX;				//分割数x
			g_MeshWall[nCnt].nDiviY = nDiviY;				//分割数y
			g_MeshWall[nCnt].nDiviZ = nDiviZ;				//分割数z
			g_MeshWall[nCnt].nWidth = nWidth;				//幅
			g_MeshWall[nCnt].nHeight = nHeight;				//高さ
			g_MeshWall[nCnt].bUse = true;					//使用している状態にする

			g_MeshWall[nCnt].nMaxVtx = (g_MeshWall[nCnt].nDiviX + 1) * (g_MeshWall[nCnt].nDiviY + 1);										//頂点数
			g_MeshWall[nCnt].nPolyNum = (2 * g_MeshWall[nCnt].nDiviX * g_MeshWall[nCnt].nDiviY + (g_MeshWall[nCnt].nDiviY - 1) * 4);		//ポリゴン数
			int indexNum = (2 * (g_MeshWall[nCnt].nDiviY * (2 + g_MeshWall[nCnt].nDiviX) - 1));												//インデックス

			//テクスチャの設定
			SetwallTexture(nCnt);

			//頂点バッファの生成
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_MeshWall[nCnt].nMaxVtx,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_MeshWall[nCnt].VtxBuffMeshWall,
				NULL);

			VERTEX_3D* pVtx = NULL;

			//頂点バッファをロックし、頂点情報へのポインタを取得
			g_MeshWall[nCnt].VtxBuffMeshWall->Lock(0, 0, (void**)&pVtx, 0);

			int nCntVtx = 0;

			//中央へずらす
			float centerX = g_MeshWall[nCnt].nWidth * (g_MeshWall[nCnt].nDiviX - 2) * 0.5f;

			for (int nCntY = 0; nCntY <= g_MeshWall[nCnt].nDiviY; nCntY++)
			{
				for (int nCntX = 0; nCntX <= g_MeshWall[nCnt].nDiviX; nCntX++)
				{
					//頂点座標の設定
					pVtx[nCntVtx].pos = D3DXVECTOR3((float)(-g_MeshWall[nCnt].nWidth + (g_MeshWall[nCnt].nWidth * nCntX) - centerX), (float)(g_MeshWall[nCnt].nHeight - (g_MeshWall[nCnt].nHeight * nCntY)),0.0f);

					//法線ベクトルの設定
					pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

					//頂点カラーの設定
					pVtx[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					//テクスチャ座標の設定
					pVtx[nCntVtx].tex = D3DXVECTOR2(1.0f * nCntX, (1.0f / g_MeshWall[nCnt].nDiviY) * nCntY);

					nCntVtx++;
				}
			}

			//頂点バッファをアンロック　
			g_MeshWall[nCnt].VtxBuffMeshWall->Unlock();

			//インデックスバッファの生成
			pDevice->CreateIndexBuffer(sizeof(WORD) * indexNum,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_MeshWall[nCnt].IdxBuffMeshWall,
				NULL);

			//インデックスバッファをロック
			g_MeshWall[nCnt].IdxBuffMeshWall->Lock(0, 0, (void**)&pIdx, 0);

			int nCntX, nCntY;
			for (nCntY = 0; nCntY < g_MeshWall[nCnt].nDiviY; nCntY++)
			{
				for (nCntX = 0; nCntX <= g_MeshWall[nCnt].nDiviX; nCntX++)
				{
					//インデックスの設定
					pIdx[0] = (g_MeshWall[nCnt].nDiviX + 1) * (nCntY + 1) + nCntX;
					pIdx[1] = nCntX + (nCntY * (g_MeshWall[nCnt].nDiviX + 1));

					pIdx += 2;
				}

				if (nCntY <= g_MeshWall[nCnt].nDiviY - 1)
				{
					pIdx[0] = (nCntX - 1) + (nCntY * (g_MeshWall[nCnt].nDiviX + 1));
					pIdx[1] = nCntX + ((nCntY + 1) * (g_MeshWall[nCnt].nDiviX + 1));

					pIdx += 2;
				}
			}

			//インデックスバッファのアンロック
			g_MeshWall[nCnt].IdxBuffMeshWall->Unlock();

			break;
		}
	}
}

//===================
// メッシュ壁のテクスチャ設定
//===================
void SetwallTexture(int indx)
{
	//テクスチャのポインタを取得
	LPDIRECT3DTEXTURE9 pTexture = GetTexture2(g_MeshWall[indx].textype);

	if (pTexture != NULL)
	{
		g_pTextureMeshWall = pTexture;
	}
}