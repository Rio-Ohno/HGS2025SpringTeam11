//==============================
//
//  �I������[Title.h]
//  Author:kaiti
//
//==============================
#include "title.h"
#include "titleinfo.h"
#include "ranking.h"
#include "fade.h"
#include "sound.h"
#include "camera.h"
#include "light.h"
//#include "loadstage.h"
//#include "loadmotion.h"
#include "model.h"
#include "meshfield.h"
//#include "cameraediter.h"

//�O���[�o���ϐ�
int g_nTimeTitle; //�^�C�g�����烉���L���O�ւ̎���

//============
//����������
//============
void InitTitle(void)
{
	g_nTimeTitle = 0;

	InitMeshfield();
	SetMeshfield(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 10, 0, 10, 100, 100);

	InitCamera();

	InitLight();

	//InitMotion();

	//InitStageModel();

	//LoadModelViewer(MODE_STAGEONE);

	InitTitleInfo();

	//PlaySound(SOUND_LABEL_TITLE);

	//LoadCameraWork();

	Camera* pCamera = GetCamera();

	//SetCameraWork(&pCamera->Anim, ANIMTYPE_ONE);
}
//==========
//�I������
//==========
void UninitTitle(void)
{
	UninitMeshfield();

	//UninitStageModel();

	UninitCamera();

	UninitLight();

	UninitTitleInfo();
}
//==========
//�X�V����
//==========
void UpdateTitle(void)
{
	UpdateMeshfield();

	//UpdateStageModel();

	UpdateCamera();

	UpdateLight();

	UpdateTitleInfo(0);

	g_nTimeTitle++;
	if (g_nTimeTitle >= MAX_TITLE)
	{
		SetRankMode(RANKMODE_TITLE);
		SetFade(MODE_RANK);
		g_nTimeTitle = 0;
	}
}
//===========
//�`�揈��
//===========
void DrawTitle(void)
{
	SetCamera();

	DrawMeshfield();

	//DrawStageModel();

	DrawTitleInfo();
}