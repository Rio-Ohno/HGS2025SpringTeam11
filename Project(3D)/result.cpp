//================================
//
//  ���U���g�\������[result.cpp]
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
//����������
//===========
void InitResult(MODE OldMode)
{
	InitMeshfield();
	SetMeshfield(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 10, 0, 10, 100, 100);

	InitCamera();

	InitLight();
}
//==========
//�I������
//==========
void UninitResult(void)
{
	UninitMeshfield();

	UninitCamera();

	UninitLight();
}
//==========
//�X�V����
//==========
void UpdateResult(void)
{
	UpdateMeshfield();

	UpdateCamera();

	UpdateLight();
}
//==========
//�`�揈��
//==========
void DrawResult(void)
{
	SetCamera();

	DrawMeshfield();
}