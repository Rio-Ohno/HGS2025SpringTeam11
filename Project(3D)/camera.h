//================================
//
//　カメラ処理[camera.h]
//　Author:kaiti
//
//================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#define CAMERA_DISTANCESPEED (float)(0.1f)
#define CAMERASHAKE_VALUE (20)
#include "main.h"

typedef struct
{
	D3DXVECTOR3 posV;//視点
	D3DXVECTOR3 posR;//注視点

	D3DXVECTOR3 posVDest;//視点
	D3DXVECTOR3 posRDest;//注視点
	
	D3DXVECTOR3 vecU;//上方向ベクトル
	D3DXMATRIX mtxProjection;//プロジェクションマトリックス
	D3DXMATRIX mtxView;//ビューマトリックス
	D3DXVECTOR3 rot;//向き
	int ShakeCount;
	float fDistance;//視点から注視点の距離
	bool bResete;
	bool bShake;
}Camera;

//プロトタイプ宣言
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void UpdateCameratoMousePos(void);
void UpdateCameratoJoyPadPos(void);
void SetCamera(void);
Camera * GetCamera(void);
void SetMouseWheel(int zDelta);
void ResetCameraPos(D3DXVECTOR3 posV, D3DXVECTOR3 posR);
void lockOnCamera(void);

// 条件文の関数化
bool isShake();
bool isGreaterCount(int Counter);

// 実作業の関数化
void SetShake(int ShakeCount);
void UpdateShakeCounter();
void UpdateShake();
void UpdateGameCamera();
void SetCameraDistance(float Distance);
void SetCameraRotX(float Rot);
void SetCameraRotY(float Rot);
void CameraMove();
void UpdateMovie();
void SetNumKey(int AnimCount, int NumKey);
void SetLoop(int AnimCount, int Loop);
void SetCameraWorkInfo(D3DXVECTOR3 PosV, D3DXVECTOR3 PosR, D3DXVECTOR3 Rot,int Frame, int AnimCount, int KeyCount);
#endif