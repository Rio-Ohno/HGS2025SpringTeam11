//================================
//
// 影表示処理[shadow.cpp]
// Author:kaiti
//
//================================

#include "shadow.h"
#include "light.h"

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_apTextureShadow = NULL;
Shadow g_shadow[MAX_SHADOW];

//=======================
// 影の初期化処理
//=======================
void InitShadow(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++)
	{
		g_shadow[nCnt].pos = D3DXVECTOR3(0.0f, 0.1f, 0.0f);			//位置の初期化
		g_shadow[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//向きの初期化
		g_shadow[nCnt].fRadius = 0.0f;								//半径の初期化
		g_shadow[nCnt].bUse = false;								//使用してしていない状態にする
	}

	//テクスチャの読込
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\shadow000.jpg", &g_apTextureShadow); //1
}

//====================
// 影の終了処理
//====================
void UninitShadow(void)
{
	//テクスチャの破棄
	if (g_apTextureShadow != NULL)
	{
		g_apTextureShadow->Release();
		g_apTextureShadow = NULL;
	}

	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++)
	{
		//頂点バッファの破棄
		if (g_shadow[nCnt].pVtxBuffShadow != NULL)
		{
			g_shadow[nCnt].pVtxBuffShadow->Release();
			g_shadow[nCnt].pVtxBuffShadow = NULL;
		}
	}
}

//=====================
// 影の更新処理
//=====================
void UpdateShadow(void)
{

}

//====================
// 影の描画処理
//====================
void DrawShadow(void)
{
	////デバイスの取得
	//LPDIRECT3DDEVICE9 pDevice = GetDevice();

	////減算合成の設定
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++)
	//{
	//	//使用している状態なら
	//	if (g_shadow[nCnt].bUse == true)
	//	{
	//		//計算用マトリックス
	//		D3DXMATRIX mtxRot, mtxTrans, mtxShadow;

	//		//ワールドマトリックスの初期化
	//		D3DXMatrixIdentity(&g_shadow[nCnt].mtxWorld);

	//		//向きを反転
	//		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_shadow[nCnt].rot.y, g_shadow[nCnt].rot.x, g_shadow[nCnt].rot.z);
	//		D3DXMatrixMultiply(&g_shadow[nCnt].mtxWorld, &g_shadow[nCnt].mtxWorld, &mtxRot);

	//		//位置を反映
	//		D3DXMatrixTranslation(&mtxTrans, g_shadow[nCnt].pos.x, 0.1f, g_shadow[nCnt].pos.z);
	//		D3DXMatrixMultiply(&g_shadow[nCnt].mtxWorld, &g_shadow[nCnt].mtxWorld, &mtxTrans);

	//		D3DXVECTOR4 LightPos = D3DXVECTOR4(0.0f, 10.0f, 0.0f, 0.0f);
	//		D3DXPLANE Plane = D3DXPLANE(0.0f, 1.0f, 0.0f, 0.0f);

	//		D3DXMatrixShadow(&mtxShadow, &LightPos, &Plane);
	//		D3DXMatrixMultiply(&g_shadow[nCnt].mtxWorld, &g_shadow[nCnt].mtxWorld, &mtxShadow);

	//		//ワールドマトリックスの設定
	//		pDevice->SetTransform(D3DTS_WORLD, &g_shadow[nCnt].mtxWorld);

	//		//頂点バッファをデータストリームに設定
	//		pDevice->SetStreamSource(0, g_shadow[nCnt].pVtxBuffShadow, 0, sizeof(VERTEX_3D));

	//		//頂点フォーマットの設定
	//		pDevice->SetFVF(FVF_VERTEX_3D);

	//		//テクスチャの設定
	//		pDevice->SetTexture(0, g_apTextureShadow);

	//		//ポリゴンを描画
	//		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	//	}
	//}

	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	////設定を元に戻す
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void DrwaShadowPlayer(int Indx, D3DXMATRIX mtxWorld)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//減算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//使用している状態なら
	if (g_shadow[Indx].bUse == true)
	{
		//計算用マトリックス
		D3DXMATRIX mtxRot, mtxTrans, mtxShadow;

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_shadow[Indx].mtxWorld);

		D3DXVECTOR4 LightPos = D3DXVECTOR4(0.0f, 10.0f, 0.0f, 0.0f);
		D3DXPLANE Plane = D3DXPLANE(0.0f, 1.0f, 0.0f, 0.0f);

		D3DXMatrixShadow(&mtxShadow, &LightPos, &Plane);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxShadow);
		D3DXMatrixMultiply(&g_shadow[Indx].mtxWorld, &g_shadow[Indx].mtxWorld, &mtxWorld);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_shadow[Indx].mtxWorld);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_shadow[Indx].pVtxBuffShadow, 0, sizeof(VERTEX_3D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureShadow);

		//ポリゴンを描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//設定を元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
//=============
// 影の設定
//=============
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot,float fRadius)
{
	int nCntShadow;
	VERTEX_3D* pVtx = NULL;

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_shadow[nCntShadow].bUse == false)
		{
			//頂点バッファの生成
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_shadow[nCntShadow].pVtxBuffShadow,
				NULL);

			//頂点バッファをロックし、頂点情報へのポインタを取得
			g_shadow[nCntShadow].pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

			g_shadow[nCntShadow].pos = pos;											//位置
			g_shadow[nCntShadow].rot = rot;											//向き
			g_shadow[nCntShadow].fRadius = fRadius;									//半径

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_shadow[nCntShadow].fRadius, g_shadow[nCntShadow].pos.y + 0.1f, +g_shadow[nCntShadow].fRadius);
			pVtx[1].pos = D3DXVECTOR3(+g_shadow[nCntShadow].fRadius, g_shadow[nCntShadow].pos.y + 0.1f, +g_shadow[nCntShadow].fRadius);
			pVtx[2].pos = D3DXVECTOR3(-g_shadow[nCntShadow].fRadius, g_shadow[nCntShadow].pos.y + 0.1f, -g_shadow[nCntShadow].fRadius);
			pVtx[3].pos = D3DXVECTOR3(+g_shadow[nCntShadow].fRadius, g_shadow[nCntShadow].pos.y + 0.1f, -g_shadow[nCntShadow].fRadius);

			//	//法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0, 1.0, 1.0, 1.0);
			pVtx[1].col = D3DXCOLOR(1.0, 1.0, 1.0, 1.0);
			pVtx[2].col = D3DXCOLOR(1.0, 1.0, 1.0, 1.0);
			pVtx[3].col = D3DXCOLOR(1.0, 1.0, 1.0, 1.0);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			g_shadow[nCntShadow].bUse = true;				//使用している状態にする
			break;
		}
		//頂点バッファをアンロック
		g_shadow[nCntShadow].pVtxBuffShadow->Unlock();
	}
	return nCntShadow; //影の番号(index)を返す
}

//=====================
// 影の位置の更新処理
//=====================
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, bool bUse)
{
	//引数で指定された番号の影のposを設定
	g_shadow[nIdxShadow].pos = pos;
	g_shadow[nIdxShadow].bUse = bUse;
}

//=====================
// 影のサイズの更新処理
//=====================
void SetSizeShadow(D3DXVECTOR3 pos, int nIndx)
{
	if (g_shadow[nIndx].bUse == true)
	{
		float posY = pos.y;//ユーザーの高さを格納
		float fRadeius = 0;//半径
		D3DXCOLOR fAlpha = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		VERTEX_3D* pVtx = NULL;

		//posYの制限
		if (posY <= 1.0f)
		{
			posY = 1.0f;
		}
		else if (posY >= g_shadow[nIndx].fRadius * 0.5f)
		{
			posY = -g_shadow[nIndx].fRadius * 0.1f;
		}

		//半径の設定
		fRadeius = -5.0f * (1.0f / posY) + g_shadow[nIndx].fRadius;

		if (posY <= 0.0f)
		{
			posY *= -1;
		}

		//α値の設定
		fAlpha.a = (1.0f / (posY + 0.5f));

		//頂点バッファをロックし、頂点情報へのポインタを取得
		g_shadow[nIndx].pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-fRadeius, 0.1f, +fRadeius);
		pVtx[1].pos = D3DXVECTOR3(+fRadeius, 0.1f, +fRadeius);
		pVtx[2].pos = D3DXVECTOR3(-fRadeius, 0.1f, -fRadeius);
		pVtx[3].pos = D3DXVECTOR3(+fRadeius, 0.1f, -fRadeius);

		//頂点カラーの設定
		pVtx[0].col = fAlpha;
		pVtx[1].col = fAlpha;
		pVtx[2].col = fAlpha;
		pVtx[3].col = fAlpha;

		//頂点バッファをアンロック
		g_shadow[nIndx].pVtxBuffShadow->Unlock();
	}
}

//=====================
// 影のサイズの取得処理
//=====================
Shadow* GetShadow(void)
{
	return &g_shadow[0];
}
