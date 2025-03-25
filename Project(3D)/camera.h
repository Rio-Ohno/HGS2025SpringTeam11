//================================
//
//�@�J��������[camera.h]
//�@Author:kaiti
//
//================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#define CAMERA_DISTANCESPEED (float)(0.1f)
#define CAMERASHAKE_VALUE (20)
#include "main.h"

typedef struct
{
	D3DXVECTOR3 posV;//���_
	D3DXVECTOR3 posR;//�����_

	D3DXVECTOR3 posVDest;//���_
	D3DXVECTOR3 posRDest;//�����_
	
	D3DXVECTOR3 vecU;//������x�N�g��
	D3DXMATRIX mtxProjection;//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX mtxView;//�r���[�}�g���b�N�X
	D3DXVECTOR3 rot;//����
	int ShakeCount;
	float fDistance;//���_���璍���_�̋���
	bool bResete;
	bool bShake;
}Camera;

//�v���g�^�C�v�錾
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

// �������̊֐���
bool isShake();
bool isGreaterCount(int Counter);

// ����Ƃ̊֐���
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