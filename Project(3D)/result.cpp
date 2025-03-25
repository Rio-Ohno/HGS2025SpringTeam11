//================================
//
//  リザルト表示処理[result.cpp]
//  Author:kaiti
//
//================================
#include "result.h"
#include "resultinfo.h"
#include "camera.h"
#include "light.h"
#include "sound.h"
//#include "loadstage.h"
//#include "loadmotion.h"
#include "model.h"
#include "meshfield.h"
//#include "player.h"

//===========
//初期化処理
//===========
void InitResult(MODE OldMode)
{
	InitMeshfield();
	SetMeshfield(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 10, 0, 10, 100, 100);

	InitCamera();

	InitLight();
}
//==========
//終了処理
//==========
void UninitResult(void)
{
	UninitMeshfield();

	UninitCamera();

	UninitLight();
}
//==========
//更新処理
//==========
void UpdateResult(void)
{
	UpdateMeshfield();

	UpdateCamera();

	UpdateLight();
}
//==========
//描画処理
//==========
void DrawResult(void)
{
	SetCamera();

	DrawMeshfield();
}