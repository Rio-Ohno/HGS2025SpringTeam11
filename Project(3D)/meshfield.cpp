//==============================================
//
// メッシュ床表示処理[meshfield.cpp]
// Author: Rio Ohno
//
//==============================================

#include "meshfield.h"
#include <string.h>
#include <cassert>

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureMeshfield[MAX_TEX_FIELD] = { NULL };		//テクスチャへのポインタ

MeshField g_Meshfield[MESH_NUM_MAX];									//ポリゴン(横)の構造体
static char fieldtexName[MAX_TEX_FIELD][256] = { NULL };					//テクスチャファイル名保存用

//=================================
// メッシュ床の初期化処理
//=================================
void InitMeshfield(void)
{
	for (int nCnt = 0; nCnt < MESH_NUM_MAX; nCnt++)
	{
		//構造体の初期化
		g_Meshfield[nCnt].pos = D3DXVECTOR3(-900.0f, 0.0f, 900.0f);		//位置
		g_Meshfield[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//向き
		g_Meshfield[nCnt].IdxBuffMeshField = { NULL };					//インデックスバッファ
		g_Meshfield[nCnt].pVtxBuffMeshfield = { NULL };					//頂点バッファ
		g_Meshfield[nCnt].textype = 0;									//テクスチャタイプ
		g_Meshfield[nCnt].nDiviX = 0;									//分割数x(0にしたらnullptr)
		g_Meshfield[nCnt].nDiviY = 0;									//分割数y
		g_Meshfield[nCnt].nDiviZ = 0;									//分割数z(0にしたらnullptr)
		g_Meshfield[nCnt].nWidth = 0;									//幅
		g_Meshfield[nCnt].nHeight = 0;									//高さ
		g_Meshfield[nCnt].nMaxVtx = 0;									//頂点数
		g_Meshfield[nCnt].nPolyNum = 0;									//ポリゴン数
		g_Meshfield[nCnt].bUse = false;									//使用していない状態にする
	}
}

//===============================
// メッシュ床の終了処理
//===============================
void UninitMeshfield(void)
{
	//テクスチャの破棄
	for (int nCnt = 0; nCnt < MAX_TEX_FIELD; nCnt++)
	{
		if (g_pTextureMeshfield[nCnt] != NULL)
		{
			g_pTextureMeshfield[nCnt]->Release();
			g_pTextureMeshfield[nCnt] = NULL;
		}
	}

	for (int nCnt = 0; nCnt < MESH_NUM_MAX; nCnt++)
	{
		//頂点バッファの破棄
		if (g_Meshfield[nCnt].pVtxBuffMeshfield != NULL)
		{
			g_Meshfield[nCnt].pVtxBuffMeshfield->Release();
			g_Meshfield[nCnt].pVtxBuffMeshfield = NULL;
		}

		//インデックスバッファの破棄
		if (g_Meshfield[nCnt].IdxBuffMeshField != NULL)
		{
			g_Meshfield[nCnt].IdxBuffMeshField->Release();
			g_Meshfield[nCnt].IdxBuffMeshField = NULL;
		}
	}
}

//===============================
// メッシュ床の更新処理
//===============================
void UpdateMeshfield(void)
{

}

//===============================
// メッシュ床の描画処理
//===============================
void DrawMeshfield(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	for (int nCnt = 0; nCnt < MESH_NUM_MAX; nCnt++)
	{
		if (g_Meshfield[nCnt].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_Meshfield[nCnt].mtxWorld);

			//向きを反転
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Meshfield[nCnt].rot.y, g_Meshfield[nCnt].rot.x, g_Meshfield[nCnt].rot.z);
			D3DXMatrixMultiply(&g_Meshfield[nCnt].mtxWorld, &g_Meshfield[nCnt].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_Meshfield[nCnt].pos.x, g_Meshfield[nCnt].pos.y, g_Meshfield[nCnt].pos.z);
			D3DXMatrixMultiply(&g_Meshfield[nCnt].mtxWorld, &g_Meshfield[nCnt].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_Meshfield[nCnt].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_Meshfield[nCnt].pVtxBuffMeshfield, 0, sizeof(VERTEX_3D));

			//インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_Meshfield[nCnt].IdxBuffMeshField);

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureMeshfield[g_Meshfield[nCnt].textype]);

			//メッシュ床を描画
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_Meshfield[nCnt].nMaxVtx, 0, g_Meshfield[nCnt].nPolyNum);
		}
	}
}

//===============================
// メッシュ床の設定処理
//===============================
void SetMeshfield(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int textype, int nDiviX,int nDiviY, int nDiviZ, int nWidth, int nHeight)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点情報へのポインタ
	VERTEX_3D* pVtx = NULL;

	//インデックスへのポインタ
	WORD* pIdx = NULL;

	for (int nCnt = 0; nCnt < MESH_NUM_MAX; nCnt++)
	{
		if (g_Meshfield[nCnt].bUse==false)
		{
			g_Meshfield[nCnt].pos = pos;						//位置
			g_Meshfield[nCnt].rot = rot;						//向き
			g_Meshfield[nCnt].textype = textype;				//テクスチャタイプ
			g_Meshfield[nCnt].nDiviX = nDiviX;					//分割数x
			g_Meshfield[nCnt].nDiviY = nDiviY;					//分割数y
			g_Meshfield[nCnt].nDiviZ = nDiviZ;					//分割数z
			g_Meshfield[nCnt].nWidth = nWidth;					//幅
			g_Meshfield[nCnt].nHeight = nHeight;				//高さ
			g_Meshfield[nCnt].bUse = true;						//使用している状態にする(2 * g_MeshWall[nCnt].nDiviX * g_MeshWall[nCnt].nDiviY + (g_MeshWall[nCnt].nDiviY - 1) * 4)

			g_Meshfield[nCnt].nMaxVtx = (g_Meshfield[nCnt].nDiviX + 1) * (g_Meshfield[nCnt].nDiviZ + 1);											//頂点数
			g_Meshfield[nCnt].nPolyNum = (2 * g_Meshfield[nCnt].nDiviX * g_Meshfield[nCnt].nDiviZ + (g_Meshfield[nCnt].nDiviZ - 1) * 4);			//ポリゴン数
			int flindexNum = (2 * (g_Meshfield[nCnt].nDiviZ * (2 + g_Meshfield[nCnt].nDiviX) - 1));													//インデックス

			//頂点バッファの生成
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_Meshfield[nCnt].nMaxVtx,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_Meshfield[nCnt].pVtxBuffMeshfield,
				NULL);

			//頂点バッファをロックし、頂点情報へのポインタを取得
			g_Meshfield[nCnt].pVtxBuffMeshfield->Lock(0, 0, (void**)&pVtx, 0);

			int nCntVtx = 0;

			//中央へずらす
			float centerX = g_Meshfield[nCnt].nWidth * (g_Meshfield[nCnt].nDiviX - 2) * 0.5f;
			float centerY = g_Meshfield[nCnt].nHeight * (g_Meshfield[nCnt].nDiviZ - 2) * 0.5f;

			//頂点情報の設定
			for (int nCntZ = 0; nCntZ <= g_Meshfield[nCnt].nDiviZ; nCntZ++)
			{
				for (int nCntX = 0; nCntX <= g_Meshfield[nCnt].nDiviX; nCntX++)
				{
					//頂点座標の設定
					pVtx[nCntVtx].pos = D3DXVECTOR3((float)(-g_Meshfield[nCnt].nWidth + (g_Meshfield[nCnt].nWidth * nCntX) - centerX), 0.0f, (float)(g_Meshfield[nCnt].nHeight - (g_Meshfield[nCnt].nHeight * nCntZ) + centerY));

					//法線ベクトルの設定
					pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

					//頂点カラーの設定
					pVtx[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					//テクスチャ座標の設定
					pVtx[nCntVtx].tex = D3DXVECTOR2(1.0f * nCntX, 1.0f * nCntZ);

					nCntVtx++;
				}
			}
			assert(nCntVtx <= g_Meshfield[nCnt].nMaxVtx);

			//頂点バッファをアンロック　
			g_Meshfield[nCnt].pVtxBuffMeshfield->Unlock();

				//インデックスバッファの生成
			pDevice->CreateIndexBuffer(sizeof(WORD) * flindexNum,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_Meshfield[nCnt].IdxBuffMeshField,
				NULL);

			//インデックスバッファをロック
			g_Meshfield[nCnt].IdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

			int nCntX;
			int Indx = 0;
			for (int nCntZ = 0; nCntZ < g_Meshfield[nCnt].nDiviZ; nCntZ++)
			{
				for (nCntX = 0; nCntX <= g_Meshfield[nCnt].nDiviX; nCntX++)
				{
					//インデックスの設定
					pIdx[0] = (g_Meshfield[nCnt].nDiviX + 1) * (nCntZ + 1) + nCntX;
					pIdx[1] = nCntX + (nCntZ * (g_Meshfield[nCnt].nDiviX + 1));

					pIdx += 2;
					Indx += 2;
				}

				if (nCntZ < g_Meshfield[nCnt].nDiviZ - 1)
				{
					pIdx[0] = (nCntX - 1) + (nCntZ * (g_Meshfield[nCnt].nDiviX + 1));
					pIdx[1] = nCntX + ((nCntZ + 1) * (g_Meshfield[nCnt].nDiviX + 1));

					pIdx += 2;
					Indx += 2;
				}
			}
			assert(Indx <= flindexNum);

			//インデックスバッファのアンロック
			g_Meshfield[nCnt].IdxBuffMeshField->Unlock();

			break;
		}
	}
}

//===============================
// メッシュ床のテクスチャ設定処理
//===============================
void SetfieldTexture(char* pFileName, int TexIndx)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	HRESULT hresult;

	strcpy(fieldtexName[TexIndx], pFileName);

	//テクスチャの読込
	hresult = D3DXCreateTextureFromFile(pDevice,
		&fieldtexName[TexIndx][0],
		&g_pTextureMeshfield[TexIndx]);

	if (FAILED(hresult))
	{
		return;
	}
}

////===============================
//// メッシュ床のテクスチャ取得処理
////===============================
//LPDIRECT3DTEXTURE9* GetfieldTexture()
//{
//	return &g_pTextureMeshfield[0];
//}

//================================
//テクスチャの取得処理
//================================
LPDIRECT3DTEXTURE9 GetTexture2(int TexType)
{
	return g_pTextureMeshfield[(int)TexType];
}
