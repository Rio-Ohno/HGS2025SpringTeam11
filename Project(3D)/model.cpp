//********************************
// モデルを管理する[model.cpp]
// Hatori Kensaku
//
//********************************
#include "model.h"
//#include "player.h"
#include "camera.h"
#include "collision.h"
#include <cassert>

// グローバル変数
MODELORIGIN g_ModelOrigin[MODELTYPE_MAX];
STAGEMODEL g_StageModel[MAX_STAGEMODEL];

//******************************
/// ステージに出すモデルの初期化
//******************************
void InitStageModel()
{
	for (int ModelCount = 0; ModelCount < MAX_STAGEMODEL; ModelCount++)
	{
		g_StageModel[ModelCount].bUse = false;
		g_StageModel[ModelCount].bHitRayCamera = false;
		g_StageModel[ModelCount].btest = true;
		g_StageModel[ModelCount].nType = MODELTYPE_ZERO;
		g_StageModel[ModelCount].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_StageModel[ModelCount].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_StageModel[ModelCount].ObbModel.CenterPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_StageModel[ModelCount].ObbModel.fLength[0] = 0.0f;
		g_StageModel[ModelCount].ObbModel.fLength[1] = 0.0f;
		g_StageModel[ModelCount].ObbModel.fLength[2] = 0.0f;

		g_StageModel[ModelCount].ObbModel.RotVec[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_StageModel[ModelCount].ObbModel.RotVec[1] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_StageModel[ModelCount].ObbModel.RotVec[2] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_StageModel[ModelCount].Max = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_StageModel[ModelCount].Min = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	for (int Origin = 0; Origin < MODELTYPE_MAX; Origin++)
	{
		g_ModelOrigin[Origin].dwNumMat = NULL;
		g_ModelOrigin[Origin].pBuffMat = NULL;
		g_ModelOrigin[Origin].pMesh = NULL;
		for (int TexCount = 0; TexCount < MAX_TEX; TexCount++)
		{
			g_ModelOrigin[Origin].pTexture[TexCount] = NULL;
		}
	}
}
//********************************
/// ステージに出すモデルの終了処理
//********************************
void UninitStageModel()
{
	for (int ModelCount = 0; ModelCount < MODELTYPE_MAX; ModelCount++)
	{
		for (int TexCount = 0; TexCount < MAX_TEX; TexCount++)
		{
			UninitTexture(g_ModelOrigin[ModelCount].pTexture[TexCount]);
		}
		UninitBuffMat(g_ModelOrigin[ModelCount].pBuffMat);
		UninitMesh(g_ModelOrigin[ModelCount].pMesh);
	}
}
//********************************
/// ステージに出すモデルの更新処理
//********************************
void UpdateStageModel()
{
	for (int ModelCount = 0; ModelCount < MAX_STAGEMODEL; ModelCount++)
	{
		if (g_StageModel[ModelCount].bUse == true)
		{
			//Player* pPlayer = GetPlayer();
			Camera* pCamera = GetCamera();
			D3DXVECTOR3 LocalRayPos,LocalRayDirection;
			LocalRayPos = pCamera->posV;
			LocalRayDirection = D3DXVECTOR3(pCamera->posR.x, pCamera->posR.y, pCamera->posR.z) - pCamera->posV;
			D3DXVECTOR3 obb1Center;
			//obb1Center.x = pPlayer->pos.x;
			//obb1Center.y = pPlayer->pos.y + (pPlayer->size.y * 0.5f);
			//obb1Center.z = pPlayer->pos.z;
			SetObbInfo(ModelCount);
			//CollOBBs(g_StageModel[ModelCount].ObbModel, obb1Center, ModelCount);
		}
	}
}
//********************************
/// ステージに出すモデルの描画処理
//********************************
void DrawStageModel()
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;
	//現在のマテリアル保存用
	D3DMATERIAL9 matDef;
	//マテリアルデータへのポインタ
	D3DXMATERIAL* pMat;

	for (int ModelCount = 0; ModelCount < MAX_STAGEMODEL; ModelCount++)
	{
		if (g_StageModel[ModelCount].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_StageModel[ModelCount].mtxWorld);

			//向きを反転
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_StageModel[ModelCount].rot.y, g_StageModel[ModelCount].rot.x, g_StageModel[ModelCount].rot.z);
			D3DXMatrixMultiply(&g_StageModel[ModelCount].mtxWorld, &g_StageModel[ModelCount].mtxWorld, &mtxRot);

			g_StageModel[ModelCount].ObbModel.RotVec[0] = D3DXVECTOR3(mtxRot._11, mtxRot._12, mtxRot._13);
			g_StageModel[ModelCount].ObbModel.RotVec[1] = D3DXVECTOR3(mtxRot._21, mtxRot._22, mtxRot._23);
			g_StageModel[ModelCount].ObbModel.RotVec[2] = D3DXVECTOR3(mtxRot._31, mtxRot._32, mtxRot._33);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_StageModel[ModelCount].pos.x, g_StageModel[ModelCount].pos.y, g_StageModel[ModelCount].pos.z);
			D3DXMatrixMultiply(&g_StageModel[ModelCount].mtxWorld, &g_StageModel[ModelCount].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_StageModel[ModelCount].mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_ModelOrigin[g_StageModel[ModelCount].nType].pBuffMat->GetBufferPointer();

			for (int ModelMatCount = 0; ModelMatCount < (int)g_ModelOrigin[g_StageModel[ModelCount].nType].dwNumMat; ModelMatCount++)
			{
				if (g_StageModel[ModelCount].bHitRayCamera == true)
				{
					pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		// Zバッファには書き込まない
					D3DXMATERIAL CameraRayHit;
					CameraRayHit = pMat[ModelMatCount];
					CameraRayHit.MatD3D.Diffuse.a = 0.5f;
					//マテリアルの設定
					pDevice->SetMaterial(&CameraRayHit.MatD3D);
				}
				else
				{
					pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		// Zバッファには書き込まない
					//マテリアルの設定
					pDevice->SetMaterial(&pMat[ModelMatCount].MatD3D);
				}
				//テクスチャの設定
				pDevice->SetTexture(0, g_ModelOrigin[g_StageModel[ModelCount].nType].pTexture[ModelMatCount]);
				//モデルの描画
				g_ModelOrigin[g_StageModel[ModelCount].nType].pMesh->DrawSubset(ModelMatCount);
			}
			pDevice->SetMaterial(&matDef);
		}
	}
}
STAGEMODEL* GetModel()
{
	return &g_StageModel[0];
}
//********************************
/// ステージに出すモデルの設定処理
//********************************
void SetStageModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, MODELTYPE nType)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	for (int ModelCount = 0; ModelCount < MAX_STAGEMODEL; ModelCount++)
	{
		if (g_StageModel[ModelCount].bUse == false)
		{
			g_StageModel[ModelCount].bUse = true;
			g_StageModel[ModelCount].nType = nType;
			g_StageModel[ModelCount].pos = pos;
			g_StageModel[ModelCount].rot = rot;
			g_StageModel[ModelCount].btest = true;
			break;
		}
	}
}
//*****************************************
/// ステージに出すモデルのOBB情報の設定処理
//*****************************************
void SetObbInfo(int Indx)
{
	LPDIRECT3DDEVICE9 pDevice;
	//デバイスの取得
	pDevice = GetDevice();

	int nNumVtx;   //頂点数
	DWORD sizeFVF; //頂点フォーマットのサイズ
	BYTE* pVtxBuff;//頂点バッファへのポインタ

	//頂点数取得
	nNumVtx = g_ModelOrigin[g_StageModel[Indx].nType].pMesh->GetNumVertices();
	//頂点フォーマットのサイズ取得
	sizeFVF = D3DXGetFVFVertexSize(g_ModelOrigin[g_StageModel[Indx].nType].pMesh->GetFVF());
	//頂点バッファのロック
	g_ModelOrigin[g_StageModel[Indx].nType].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	if (g_StageModel[Indx].btest == true)
	{
		for (int nCnt = 0; nCnt < nNumVtx; nCnt++)
		{
			//頂点座標の代入
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			//頂点座標を比較してプレイヤーの最小値、最大値を取得
			//最小値
			if (vtx.x < g_StageModel[Indx].Min.x)
			{
				g_StageModel[Indx].Min.x = vtx.x;
			}
			if (vtx.y < g_StageModel[Indx].Min.y)
			{
				g_StageModel[Indx].Min.y = vtx.y;
			}
			if (vtx.z < g_StageModel[Indx].Min.z)
			{
				g_StageModel[Indx].Min.z = vtx.z;
			}
			//最大値
			if (vtx.x > g_StageModel[Indx].Max.x)
			{
				g_StageModel[Indx].Max.x = vtx.x;
			}
			if (vtx.y > g_StageModel[Indx].Max.y)
			{
				g_StageModel[Indx].Max.y = vtx.y;
			}
			if (vtx.z > g_StageModel[Indx].Max.z)
			{
				g_StageModel[Indx].Max.z = vtx.z;
			}
			//頂点フォーマットのサイズ分ポインタを進める
			pVtxBuff += sizeFVF;
		}
	}

	//頂点バッファのアンロック
	g_ModelOrigin[g_StageModel[Indx].nType].pMesh->UnlockVertexBuffer();

	D3DXMATRIX mtxRotModel, mtxTransModel;
	D3DXMATRIX mtxParent;
	D3DXMatrixIdentity(&g_StageModel[Indx].ObbModel.CenterMtx);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_StageModel[Indx].rot.y, g_StageModel[Indx].rot.x, g_StageModel[Indx].rot.z);
	D3DXMatrixMultiply(&g_StageModel[Indx].ObbModel.CenterMtx, &g_StageModel[Indx].ObbModel.CenterMtx, &mtxRotModel);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTransModel, 0.0f, g_StageModel[Indx].Max.y * 0.5f, 0.0f);
	D3DXMatrixMultiply(&g_StageModel[Indx].ObbModel.CenterMtx, &g_StageModel[Indx].ObbModel.CenterMtx, &mtxTransModel);

	mtxParent = g_StageModel[Indx].mtxWorld;

	D3DXMatrixMultiply(&g_StageModel[Indx].ObbModel.CenterMtx,
		&g_StageModel[Indx].ObbModel.CenterMtx,
		&mtxParent);

	pDevice->SetTransform(D3DTS_WORLD,
		&g_StageModel[Indx].ObbModel.CenterMtx);

	// 中心点
	g_StageModel[Indx].ObbModel.CenterPos.x = g_StageModel[Indx].ObbModel.CenterMtx._41;
	g_StageModel[Indx].ObbModel.CenterPos.y = g_StageModel[Indx].ObbModel.CenterMtx._42;
	g_StageModel[Indx].ObbModel.CenterPos.z = g_StageModel[Indx].ObbModel.CenterMtx._43;

	// 各座標軸の傾きを表すベクトルを計算
	g_StageModel[Indx].ObbModel.RotVec[0] = D3DXVECTOR3(mtxRotModel._11, mtxRotModel._12, mtxRotModel._13);
	g_StageModel[Indx].ObbModel.RotVec[1] = D3DXVECTOR3(mtxRotModel._21, mtxRotModel._22, mtxRotModel._23);
	g_StageModel[Indx].ObbModel.RotVec[2] = D3DXVECTOR3(mtxRotModel._31, mtxRotModel._32, mtxRotModel._33);

	// 中心点から面への距離の半分
	g_StageModel[Indx].ObbModel.fLength[0] = fabsf(g_StageModel[Indx].Max.x - g_StageModel[Indx].Min.x) * 0.5f;
	g_StageModel[Indx].ObbModel.fLength[1] = fabsf(g_StageModel[Indx].Max.y - g_StageModel[Indx].Min.y) * 0.5f;
	g_StageModel[Indx].ObbModel.fLength[2] = fabsf(g_StageModel[Indx].Max.z - g_StageModel[Indx].Min.z) * 0.5f;
}
//******************************************
/// ステージに出すモデルのバッファー設定処理
//******************************************
void SetStageModelInfo(char *ModelPath[], int PathType)
{
	HRESULT Hresult;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	const char* pFile = ModelPath[0];

	//Xファイルの読み込み
	Hresult = D3DXLoadMeshFromX(pFile,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_ModelOrigin[PathType].pBuffMat,
		NULL,
		&g_ModelOrigin[PathType].dwNumMat,
		&g_ModelOrigin[PathType].pMesh);
	
	if (FAILED(Hresult))
	{
		return;
	}

	D3DXMATERIAL* pMat;//マテリアルへのポインタ
	pMat = (D3DXMATERIAL*)g_ModelOrigin[PathType].pBuffMat->GetBufferPointer();
	for (int nCntBlockMat = 0; nCntBlockMat < (int)g_ModelOrigin[PathType].dwNumMat; nCntBlockMat++)
	{
		if (pMat[nCntBlockMat].pTextureFilename != NULL)
		{
			D3DXCreateTextureFromFile(pDevice, 
				pMat[nCntBlockMat].pTextureFilename, 
				&g_ModelOrigin[PathType].pTexture[nCntBlockMat]); //1
		}
	}
}
//*************************
/// OBBと天の当たり判定処理
//*************************
void LenOBBToPoint(OBB& obb, D3DXVECTOR3& p)
{
	D3DXVECTOR3 Vec(0, 0, 0);   // 最終的に長さを求めるベクトル

   // 各軸についてはみ出た部分のベクトルを算出
	for (int DimensionCount = 0; DimensionCount < 3; DimensionCount++)
	{
		FLOAT L = obb.fLength[DimensionCount];
		if (L <= 0) continue;  // L=0は計算できない
		D3DXVECTOR3 test = p - obb.CenterPos;
		FLOAT s = D3DXVec3Dot(&test, &obb.RotVec[DimensionCount]) / L;

		// sの値から、はみ出した部分があればそのベクトルを加算
		s = (float)fabs(s);
		if (s > 1)
			Vec += (1 - s) * L * obb.RotVec[DimensionCount];   // はみ出した部分のベクトル算出
	}
	FLOAT fDistance = D3DXVec3Length(&Vec);   // 長さを出力
	if (fDistance < 10)
	{
		//Player* pPlayer = GetPlayer();
		//pPlayer->pos = pPlayer->posOld;
	}
}
////**************************
///// OBBとOBBの当たり判定処理
////**************************
//void CollOBBs(OBB& obb, D3DXVECTOR3& p,int Indx)
//{
//	Player* pPlayer = GetPlayer();
//	// 各方向ベクトルの確保
//	   // （N***:標準化方向ベクトル）
//	D3DXVECTOR3 NAe1 = obb.RotVec[0], Ae1 = NAe1 * obb.fLength[0];	// X軸
//	D3DXVECTOR3 NAe2 = obb.RotVec[1], Ae2 = NAe2 * obb.fLength[1];	// Y軸
//	D3DXVECTOR3 NAe3 = obb.RotVec[2], Ae3 = NAe3 * obb.fLength[2];	// Z軸
//	D3DXVECTOR3 NBe1 = D3DXVECTOR3(1.0f, 0.0f, 0.0f), Be1 = NBe1 * pPlayer->size.x * 0.5f;
//	D3DXVECTOR3 NBe2 = D3DXVECTOR3(0.0f, 1.0f, 0.0f), Be2 = NBe2 * pPlayer->size.y * 0.5f;
//	D3DXVECTOR3 NBe3 = D3DXVECTOR3(0.0f, 0.0f, 1.0f), Be3 = NBe3 * pPlayer->size.z * 0.5f;
//
//	D3DXVECTOR3 r;
//	D3DXVECTOR3 MathX;
//	D3DXVECTOR3 MathY;
//	D3DXVECTOR3 MathZ;
//	D3DXVECTOR3 MathPos;
//	MathPos = obb.CenterPos - p;
//	MathX = obb.RotVec[0] * obb.fLength[0];
//	MathY = obb.RotVec[1] * obb.fLength[1];
//	MathZ = obb.RotVec[2] * obb.fLength[2];
//	r.x = (float)fabs(D3DXVec3Dot(&MathX, &NBe1));
//	r.y = (float)fabs(D3DXVec3Dot(&MathY, &NBe2));
//	r.z = (float)fabs(D3DXVec3Dot(&MathZ, &NBe3));
//
//	D3DXVECTOR3 Interval = MathPos;
//	bool bCollision = true;
//
//	// 分離軸 : Ae1
//	FLOAT rA = D3DXVec3Length(&Ae1);
//	FLOAT rB = LenSegOnSeparateAxis(&NAe1, &Be1, &Be2, &Be3);
//	FLOAT L = (float)fabs(D3DXVec3Dot(&Interval, &NAe1));
//	if (L > rA + rB)
//	{
//		bCollision = false;
//	}
//
//	// 分離軸 : Ae2
//	rA = D3DXVec3Length(&Ae2);
//	rB = LenSegOnSeparateAxis(&NAe2, &Be1, &Be2, &Be3);
//	L = (float)fabs(D3DXVec3Dot(&Interval, &NAe2));
//	if (L > rA + rB)
//	{
//		bCollision = false;
//	}
//	// 分離軸 : Ae3
//	rA = D3DXVec3Length(&Ae3);
//	rB = LenSegOnSeparateAxis(&NAe3, &Be1, &Be2, &Be3);
//	L = (float)fabs(D3DXVec3Dot(&Interval, &NAe3));
//	if (L > rA + rB)
//	{
//		bCollision = false;
//	}
//
//	// 分離軸 : Be1
//	rA = LenSegOnSeparateAxis(&NBe1, &Ae1, &Ae2, &Ae3);
//	rB = D3DXVec3Length(&Be1);
//	L = (float)fabs(D3DXVec3Dot(&Interval, &NBe1));
//	if (L > rA + rB)
//	{
//		bCollision = false;
//	}
//
//	// 分離軸 : Be2
//	rA = LenSegOnSeparateAxis(&NBe2, &Ae1, &Ae2, &Ae3);
//	rB = D3DXVec3Length(&Be2);
//	L = (float)fabs(D3DXVec3Dot(&Interval, &NBe2));
//	if (L > rA + rB)
//	{
//		bCollision = false;
//	}
//
//	// 分離軸 : Be3
//	rA = LenSegOnSeparateAxis(&NBe3, &Ae1, &Ae2, &Ae3);
//	rB = D3DXVec3Length(&Be3);
//	L = (float)fabs(D3DXVec3Dot(&Interval, &NBe3));
//	if (L > rA + rB)
//	{
//		bCollision = false;
//	}
//
//	// 分離軸 : C11
//	D3DXVECTOR3 Cross;
//	D3DXVec3Cross(&Cross, &NAe1, &NBe1);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
//	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
//	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//	{
//		bCollision = false;
//	}
//
//	// 分離軸 : C12
//	D3DXVec3Cross(&Cross, &NAe1, &NBe2);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
//	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
//	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//	{
//		bCollision = false;
//	}
//
//	// 分離軸 : C13
//	D3DXVec3Cross(&Cross, &NAe1, &NBe3);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
//	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
//	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//	{
//		bCollision = false;
//	}
//
//	// 分離軸 : C21
//	D3DXVec3Cross(&Cross, &NAe2, &NBe1);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
//	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
//	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//	{
//		bCollision = false;
//	}
//
//	// 分離軸 : C22
//	D3DXVec3Cross(&Cross, &NAe2, &NBe2);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
//	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
//	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//	{
//		bCollision = false;
//	}
//
//	// 分離軸 : C23
//	D3DXVec3Cross(&Cross, &NAe2, &NBe3);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
//	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
//	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//	{
//		bCollision = false;
//	}
//
//	// 分離軸 : C31
//	D3DXVec3Cross(&Cross, &NAe3, &NBe1);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
//	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
//	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//	{
//		bCollision = false;
//	}
//
//	// 分離軸 : C32
//	D3DXVec3Cross(&Cross, &NAe3, &NBe2);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
//	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
//	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//	{
//		bCollision = false;
//	}
//
//	// 分離軸 : C33
//	D3DXVec3Cross(&Cross, &NAe3, &NBe3);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
//	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
//	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//	{
//		bCollision = false;
//	}
//
//	if (bCollision == true)
//	{
//		// ↓あってるかわかんない
//		// X軸の面に当たっているか
//		D3DXVECTOR3 IntervalX;
//		D3DXVECTOR3 norX;
//		FLOAT fDotX;
//		D3DXVECTOR3 IntervalX1;
//		D3DXVECTOR3 norX1;
//		D3DXVECTOR3 Math;
//		FLOAT fDotX1;
//
//		// X軸のマイナス方向のベクトル
//		Math = -g_StageModel[Indx].ObbModel.RotVec[0];
//		// X軸の方向ベクトル(法線)
//		D3DXVec3Normalize(&norX, &g_StageModel[Indx].ObbModel.RotVec[0]);
//		// X軸のマイナス方向の法線ベクトル
//		D3DXVec3Normalize(&norX1, &Math);
//
//		// プレイヤーぽすからX軸の面の中心ポスを引いた値
//		IntervalX = p - g_StageModel[Indx].ObbModel.CenterPos + 
//			(norX * g_StageModel[Indx].ObbModel.fLength[0]);
//		// プレイヤーのポスからX軸のマイナス方向の中心ポスを引いた値
//		IntervalX1 = p - g_StageModel[Indx].ObbModel.CenterPos +
//			(norX1 * g_StageModel[Indx].ObbModel.fLength[0]);
//
//		// 内積(マイナスだと当たっている)
//		fDotX = D3DXVec3Dot(&IntervalX, &norX);
//		// 内積(マイナスだと当たっている)
//		fDotX1 = D3DXVec3Dot(&IntervalX1, &norX1);
//
//		// Y軸の面に当たっているか
//		D3DXVECTOR3 IntervalY;
//		D3DXVECTOR3 norY;
//		FLOAT fDotY;
//		D3DXVECTOR3 IntervalY1;
//		D3DXVECTOR3 norY1;
//		D3DXVECTOR3 MathY;
//		FLOAT fDotY1;
//
//		// Y軸のマイナス方向のベクトル
//		MathY = g_StageModel[Indx].ObbModel.RotVec[1] * -1.0f;
//		// Y軸の方向ベクトル(法線)
//		D3DXVec3Normalize(&norY, &g_StageModel[Indx].ObbModel.RotVec[1]);
//		// Y軸のマイナス方向のベクトル(法線)
//		D3DXVec3Normalize(&norY1, &MathY);
//
//		// プレイヤーぽすからY軸の面の中心ポスを引いた値
//		IntervalY = p - g_StageModel[Indx].ObbModel.CenterPos +
//			(norY * g_StageModel[Indx].ObbModel.fLength[1]);
//		// プレイヤーぽすからY軸のマイナス方向の面の中心ポスを引いた値
//		IntervalY1 = p - g_StageModel[Indx].ObbModel.CenterPos +
//			(norY1 * g_StageModel[Indx].ObbModel.fLength[1]);
//
//		// 内積(マイナスだと当たっている)
//		fDotY = D3DXVec3Dot(&IntervalY, &norY);
//		// 内積(マイナスだと当たっている)
//		fDotY1 = D3DXVec3Dot(&IntervalY1, &norY1);
//
//		// Z軸の面に当たっているか
//		D3DXVECTOR3 IntervalZ;
//		D3DXVECTOR3 norZ;
//		FLOAT fDotZ;
//		D3DXVECTOR3 IntervalZ1;
//		D3DXVECTOR3 norZ1;
//		D3DXVECTOR3 MathZ;
//		FLOAT fDotZ1;
//
//		// Z軸のマイナス方向のベクトル
//		MathZ = -g_StageModel[Indx].ObbModel.RotVec[2];
//		// Z軸の方向ベクトル(法線)
//		D3DXVec3Normalize(&norZ, &g_StageModel[Indx].ObbModel.RotVec[2]);
//		// Z軸のマイナスの方向ベクトル(法線)
//		D3DXVec3Normalize(&norZ1, &MathZ);
//
//		// プレイヤーぽすからZ軸の面の中心ポスを引いた値
//		IntervalZ = p - g_StageModel[Indx].ObbModel.CenterPos + 
//			(norZ * g_StageModel[Indx].ObbModel.fLength[2]);
//		// プレイヤーぽすからZ軸のマイナス方向の面の中心ポスを引いた値
//		IntervalZ1 = p - g_StageModel[Indx].ObbModel.CenterPos +
//			(norZ1 * g_StageModel[Indx].ObbModel.fLength[2]);
//
//		// 内積(マイナスだと当たっている)
//		fDotZ = D3DXVec3Dot(&IntervalZ, &norZ);
//		// 内積(マイナスだと当たっている)
//		fDotZ1 = D3DXVec3Dot(&IntervalZ1, &norZ1);
//
//		if (fDotY >= 0)
//		{
//			if (fDotX < fDotX1 && fDotX < fDotZ && fDotX < fDotZ1)
//			{
//				D3DXVECTOR3 pVec = pPlayer->posOld - pPlayer->pos;
//				D3DXVECTOR3 nor = norX;
//				D3DXVec3Normalize(&nor, &nor);
//				FLOAT osiete = D3DXVec3Dot(&pVec, &nor);
//				D3DXVECTOR3 test1 = nor * D3DXVec3Dot(&pVec, &nor);
//				if (osiete <= 0.0f)
//				{
//					pPlayer->pos += test1;
//				}
//			}
//			if (fDotX1 < fDotX && fDotX1 < fDotZ && fDotX1 < fDotZ1)
//			{
//				D3DXVECTOR3 pVec = pPlayer->posOld - pPlayer->pos;
//				D3DXVECTOR3 nor = norX1;
//				D3DXVec3Normalize(&nor, &nor);
//				FLOAT osiete = D3DXVec3Dot(&pVec, &nor);
//				D3DXVECTOR3 test1 = nor * D3DXVec3Dot(&pVec, &nor);
//				if (osiete <= 0.0f)
//				{
//					pPlayer->pos += test1;
//				}
//			}
//			if (fDotZ < fDotZ1 && fDotZ <= fDotX && fDotZ <= fDotX1)
//			{
//				D3DXVECTOR3 pVec = pPlayer->posOld - pPlayer->pos;
//				D3DXVECTOR3 nor = norZ;
//				D3DXVec3Normalize(&nor, &nor);
//				FLOAT osiete = D3DXVec3Dot(&pVec, &nor);
//				D3DXVECTOR3 test1 = nor * D3DXVec3Dot(&pVec, &nor);
//				if (osiete <= 0.0f)
//				{
//					pPlayer->pos += test1;
//				}
//			}
//			if (fDotZ1 < fDotZ && fDotZ1 < fDotX && fDotZ1 < fDotX1)
//			{
//				D3DXVECTOR3 pVec = pPlayer->posOld - pPlayer->pos;
//				D3DXVECTOR3 nor = norZ1;
//				D3DXVec3Normalize(&nor, &nor);
//				FLOAT osiete = D3DXVec3Dot(&pVec, &nor);
//				D3DXVECTOR3 test1 = nor * D3DXVec3Dot(&pVec, &nor);
//				if (osiete <= 0.0f)
//				{
//					pPlayer->pos += test1;
//				}
//			}
//		}
//		// 下から上
//		if (fDotY <= 1.0f && fDotX >= -2.0f && fDotX1 >= -2.0f && fDotZ >= -2.0f && fDotZ1 >= -2.0f)
//		{
//			D3DXVECTOR3 pVec = pPlayer->posOld - pPlayer->pos;
//			D3DXVECTOR3 nor = norY1;
//			D3DXVec3Normalize(&nor, &nor);
//			FLOAT osiete = D3DXVec3Dot(&pVec, &nor);
//			D3DXVECTOR3 test1 = nor * D3DXVec3Dot(&pVec, &nor);
//			if (osiete >= 0.0f)
//			{
//				pPlayer->pos += test1;
//			}
//		}
//		// ↑から↓
//		if (fDotY1 <= 1.0f && fDotX >= -2.0f && fDotX1 >= -2.0f && fDotZ >= -2.0f && fDotZ1 >= -2.0f)
//		{
//			D3DXVECTOR3 pVec = pPlayer->posOld - pPlayer->pos;
//			D3DXVECTOR3 nor = norY;
//			D3DXVec3Normalize(&nor, &nor);
//			FLOAT osiete = D3DXVec3Dot(&pVec, &nor);
//			D3DXVECTOR3 test1 = nor * D3DXVec3Dot(&pVec, &nor);
//			if (osiete >= 0.0f)
//			{
//				pPlayer->pos = pPlayer->pos + test1;
//				pPlayer->bLanding = true;
//			}
//		}
//		else
//		{
//			pPlayer->bLanding = false;
//		}
//		//else
//		//{
//		//	if (fDotY >= 0)
//		//	{
//				//D3DXVECTOR3 pVec = pPlayer->posOld - pPlayer->pos;
//				//D3DXVECTOR3 nor = norY;
//				//D3DXVec3Normalize(&nor, &nor);
//				//D3DXVECTOR3 test1 = nor * D3DXVec3Dot(&pVec, &nor);
//				//pPlayer->pos += test1;
//		//	}
//		//	else if (fDotY1 >= 0)
//		//	{
//		//		D3DXVECTOR3 pVec = pPlayer->posOld - pPlayer->pos;
//		//		D3DXVECTOR3 nor = norY1;
//		//		D3DXVec3Normalize(&nor, &nor);
//		//		D3DXVECTOR3 test1 = nor * D3DXVec3Dot(&pVec, &nor);
//		//		pPlayer->pos += test1;
//		//	}
//		//}
//	}
//}
// 分離軸に投影された軸成分から投影線分長を算出
FLOAT LenSegOnSeparateAxis(D3DXVECTOR3* Sep, D3DXVECTOR3* e1, D3DXVECTOR3* e2, D3DXVECTOR3* e3)
{
	// 3つの内積の絶対値の和で投影線分長を計算
	// 分離軸Sepは標準化されていること
	FLOAT r1 = (float)fabs(D3DXVec3Dot(Sep, e1));
	FLOAT r2 = (float)fabs(D3DXVec3Dot(Sep, e2));
	FLOAT r3 = e3 ? (float)fabs(D3DXVec3Dot(Sep, e3)) : 0;
	return r1 + r2 + r3;
}