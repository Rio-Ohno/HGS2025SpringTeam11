//=============================
//
//�@�J��������[camera.cpp]
//�@Author:kaiti
//
//=============================

//�C���N���[�h
#include "camera.h"
#include "input.h"
#include "mouse.h"
#include "sound.h"

//�O���[�o���ϐ�
Camera g_camera;

void UpdateGameCamera();
void UpdateSequenceCamera();
void UpdateMousePosEditer();
void UpdateCameraPositionR();
//=================
// �J�����̏�����
//=================
void InitCamera(void)
{
	//���_�E�����_�E�������ݒ肷��
	g_camera.posV = D3DXVECTOR3(0.0f, 200.0f, -300.0f);
	g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.fDistance = sqrtf(((g_camera.posV.x - g_camera.posR.x) * (g_camera.posV.x - g_camera.posR.x))
							 + ((g_camera.posV.y - g_camera.posR.y) * (g_camera.posV.y - g_camera.posR.y))
							 + ((g_camera.posV.z - g_camera.posR.z) * (g_camera.posV.z - g_camera.posR.z)));

	g_camera.bResete = false;
	g_camera.ShakeCount = 0;
	g_camera.bShake = false;

}

//===================
// �J�����̏I������
//===================
void UninitCamera(void)
{

}

//====================
// �J�����̍X�V����
//====================
void UpdateCamera(void)
{
	//�e����擾
	MODE pMode = GetMode();						//�Q�[�����[�h
	XINPUT_STATE* pStick = GetJoyStickAngle();

	//ResetCameraPos(D3DXVECTOR3(350.0f, 200.0f, 1245.0f), pPlayer->pos);

	//�^�C�g���ƃ��U���g�̎�
	if (pMode == MODE_TITLE || pMode == MODE_RESULT)
	{//�J�����̎�����]
		UpdateSequenceCamera();
	}
}

//=============================
//�}�E�X�ɂ��J�������쏈��
//=============================
void UpdateCameratoMousePos(void)
{
	//Option* pOption = GetOption();

	static POINT SetMousePos = { (LONG)SCREEN_WIDTH / (LONG)2.0f,(LONG)SCREEN_HEIGHT / (LONG)2.0f };
	POINT MousePos;
	GetCursorPos(&MousePos);
	D3DXVECTOR2 DiffMouse = D3DXVECTOR2((FLOAT)MousePos.x - (FLOAT)SetMousePos.x,
		(FLOAT)MousePos.y - (FLOAT)SetMousePos.y);
	
	g_camera.rot.x += DiffMouse.y;
	g_camera.rot.y += DiffMouse.x;

	SetCursorPos((int)SetMousePos.x, (int)SetMousePos.y);
}

//====================================
//�W���C�p�b�h�ɂ��J�������쏈��
//====================================
void UpdateCameratoJoyPadPos(void)
{
	XINPUT_STATE* pStick = GetJoyStickAngle();
	if (GetJoyStickR() == true)
	{
		float fStickAngleX = (float)pStick->Gamepad.sThumbRX * pStick->Gamepad.sThumbRX;
		float fStickAngleY = (float)pStick->Gamepad.sThumbRY * pStick->Gamepad.sThumbRY;

		float DeadZone = 10920.0f;
		float fMag = sqrtf(fStickAngleX + fStickAngleY);

		if (fMag > DeadZone)
		{
			if (pStick->Gamepad.sThumbRX < -DeadZone)
			{
				//g_camera.rot.y -= pOption->cameraSP / 10;
			}
			else if (pStick->Gamepad.sThumbRX > DeadZone)
			{
				//g_camera.rot.y += pOption->cameraSP / 10;
			}
		}
	}
}
////=========================
//// ���b�N�I�����̃J����
////=========================
//void lockOnCamera(void)
//{
//	Player* pPlayer = GetPlayer();				//�v���C���[
//
//	float Dis = ((pPlayer->pos.x - pLockon->pos.x) * (pPlayer->pos.x - pLockon->pos.x))
//		      + ((pPlayer->pos.y - pLockon->pos.y) * (pPlayer->pos.y - pLockon->pos.y))
//		      + ((pPlayer->pos.z - pLockon->pos.z) * (pPlayer->pos.z - pLockon->pos.z));
//
//	if (Dis >= pPlayer->fSightRange * pPlayer->fSightRange * 2)
//	{
//		Dis = pPlayer->fSightRange * pPlayer->fSightRange * 2;
//	}
//	g_camera.fDistance = sqrtf(Dis) * 2.6f;
//
//	g_camera.posRDest.x = pLockon->pos.x + sinf(pPlayer->rot.x) * (pLockon->pos.x - g_camera.posR.x);
//	g_camera.posRDest.y = pLockon->pos.y;
//	g_camera.posRDest.z = pLockon->pos.z + cosf(pPlayer->rot.z) * (pLockon->pos.z - g_camera.posR.z);
//
//	g_camera.posVDest.x = pLockon->pos.x + sinf(g_camera.rot.y - D3DX_PI) * g_camera.fDistance;
//	g_camera.posVDest.z = pLockon->pos.z + cosf(g_camera.rot.y - D3DX_PI) * g_camera.fDistance;
//}

//================
// �J�����̐ݒ�
//================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxView);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_camera.mtxView,
					   &g_camera.posV,
					   &g_camera.posR,
					   &g_camera.vecU);

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
		D3DXToRadian(45.0f), //����p
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, //�A�X�y�N�g��
		10.0f,
		5000.0f);

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);
}

//=================
//�J�����̏��擾����
//=================
Camera * GetCamera(void)
{
	return &g_camera;
}

void SetMouseWheel(int zDelta)
{
	g_camera.fDistance += zDelta * CAMERA_DISTANCESPEED;
}

//=============================
//�J�����̎��_�A�����_�̐ݒ�
//=============================
void ResetCameraPos(D3DXVECTOR3 posV, D3DXVECTOR3 posR)
{
	if (g_camera.bResete == false)
	{
		g_camera.posV = posV;				//���_
		g_camera.posR = posR;				//�����_
		g_camera.posVDest = posV;			//���_
		g_camera.posRDest = posR;			//�����_
		D3DXVECTOR3 vec = (g_camera.posRDest- g_camera.posVDest);

		//�p�x�̎擾
		float fAngle = atan2f(vec.x, vec.z);

		g_camera.rot.y = fAngle;
		g_camera.bResete = true;
	}
}

//*******************
// �V�F�C�N�����ǂ���
//*******************
bool isShake()
{
	return g_camera.bShake;
}

//*************************************************
// �V�F�C�N�J�E���^�[���J�E���g�𒴂��Ă��邩�ǂ���
//*************************************************
bool isGreaterCount(int Counter)
{
	return Counter >= g_camera.ShakeCount ? true : false;
}

//*******************
// �V�F�C�N�̐ݒ菈��
//*******************
void SetShake(int ShakeCount)
{
	g_camera.ShakeCount = ShakeCount;
	g_camera.bShake = true;
}

//*************************
// �V�F�C�N�J�E���^�[���X�V
//*************************
void UpdateShakeCounter()
{
	static int ShakeCounter = 0;
	if (isShake() == true)
	{
		ShakeCounter++;
		if (isGreaterCount(ShakeCounter) == true)
		{
			ShakeCounter = 0;
			g_camera.bShake = false;
			ShakeCounter = 0;
		}
		else
		{
			UpdateShake();
		}
	}
}

//*******************************
// �V�F�C�N���̃J�����̈ʒu���X�V
//*******************************
void UpdateShake()
{
	const int ShakeValueMax = CAMERASHAKE_VALUE;
	const int ShakeValue = (rand() % (ShakeValueMax * 2)) - ShakeValueMax;
	g_camera.posV.x += cosf(g_camera.rot.y) * ShakeValue;
	g_camera.posV.z -= sinf(g_camera.rot.y) * ShakeValue;
}

//*****************
// �g�����킩��Ȃ�
//*****************
void SetCameraDistance(float Distance)
{
	g_camera.fDistance = Distance;
}
void SetCameraRotX(float Rot)
{
	g_camera.rot.x = Rot;
}
void SetCameraRotY(float Rot)
{
	g_camera.rot.y = Rot;
}

void UpdateGameCamera()
{
	//Player* pPlayer = GetPlayer();				//�v���C���[
	
	// �p�x�̋ߓ�
	if (g_camera.rot.y >= D3DX_PI)
	{
		g_camera.rot.y -= D3DX_PI * 2.0f;
	}
	else if (g_camera.rot.y <= -D3DX_PI)
	{
		g_camera.rot.y += D3DX_PI * 2.0f;
	}
	// �p�x�̋ߓ�
	if (g_camera.rot.x >= D3DX_PI)
	{
		g_camera.rot.x -= D3DX_PI * 2.0f;
	}
	else if ((g_camera.rot.x) <= -D3DX_PI)
	{
		g_camera.rot.x += D3DX_PI * 2.0f;
	}

	////�v���C���[�����b�N�I�����Ă���Ȃ�
	//if (pPlayer->bLockOn == true)
	//{
	//	lockOnCamera();
	//}
	//else
	//{
		//g_camera.fDistance = sqrtf(((0.0f - 0.0f) * (0.0f - 0.0f))
		//	+ ((200.0f - 0.0f) * (200.0f - 0.0f))
		//	+ ((-300.0f - 0.0f) * (-300.0f - 0.0f)));

		//g_camera.posRDest.x = pPlayer->pos.x + sinf(pPlayer->rot.x) * (pPlayer->pos.x - g_camera.posR.x);
		//g_camera.posRDest.y = pPlayer->pos.y;
		//g_camera.posRDest.z = pPlayer->pos.z + cosf(pPlayer->rot.z) * (pPlayer->pos.z - g_camera.posR.z);

		//g_camera.posVDest.x = pPlayer->pos.x + sinf(g_camera.rot.y - D3DX_PI) * g_camera.fDistance;
		//g_camera.posVDest.z = pPlayer->pos.z + cosf(g_camera.rot.y - D3DX_PI) * g_camera.fDistance;
	//}

	g_camera.posR.x += (g_camera.posRDest.x - g_camera.posR.x) * 0.08f;
	g_camera.posR.y += (g_camera.posRDest.y - g_camera.posR.y) * 1.0f;
	g_camera.posR.z += (g_camera.posRDest.z - g_camera.posR.z) * 0.08f;

	g_camera.posV.x += (g_camera.posVDest.x - g_camera.posV.x) * 0.08f;
	g_camera.posV.z += (g_camera.posVDest.z - g_camera.posV.z) * 0.08f;

#ifdef _DEBUG

	//���_�̏㉺
	if (GetKeyboardPress(DIK_UP) == true)
	{//��
		if (g_camera.posV.y <= 500)
		{
			g_camera.posV.y += 5;
		}
	}
	if (GetKeyboardPress(DIK_DOWN) == true)
	{//��
		if (g_camera.posV.y >= -500)
		{
			g_camera.posV.y -= 5;
		}
	}
#endif

	//if (pPlayer->bLockOn == false)
	//{
	//	UpdateCameratoMousePos();
	//	UpdateCameratoJoyPadPos();
	//}
	//else
	//{
	//	SetCursorPos(640, 360);
	//}

	if (isShake() == false)
	{
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z - cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	else
	{
		UpdateShakeCounter();
	}
}
void UpdateSequenceCamera()
{
	//Player* pPlayer = GetPlayer();				//�v���C���[

	//pPlayer->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);//�v���C���[�̈ʒu���O��


	g_camera.posR.x += (g_camera.posRDest.x - g_camera.posR.x) * 1.0f;
	g_camera.posR.y += (g_camera.posRDest.y - g_camera.posR.y) * 1.0f;
	g_camera.posR.z += (g_camera.posRDest.z - g_camera.posR.z) * 1.0f;

	g_camera.posV.x += (g_camera.posVDest.x - g_camera.posV.x) * 1.0f;
	g_camera.posV.y += (g_camera.posVDest.y - g_camera.posV.y) * 1.0f;
	g_camera.posV.z += (g_camera.posVDest.z - g_camera.posV.z) * 1.0f;
}

void CameraMove()
{
	ShowCursor(TRUE);
	const float EditerSpeed = 5.0f;
	float fAngle = g_camera.rot.y;
	D3DXVECTOR3 Move;
	if (OnMousePress(1) == true)
	{
		UpdateMousePosEditer();
	}
	else
	{
		if (GetKeyboardPress(DIK_A) == true)
		{
			if (GetKeyboardPress(DIK_W) == true)
			{
				fAngle += -D3DX_PI * 0.25f;
			}
			else if (GetKeyboardPress(DIK_S) == true)
			{
				fAngle += -D3DX_PI * 0.75f;
			}
			else
			{
				fAngle += -D3DX_PI * 0.5f;
			}
			Move.x = sinf(fAngle) * EditerSpeed;
			Move.y = 0.0f;
			Move.z = cosf(fAngle) * EditerSpeed;
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			if (GetKeyboardPress(DIK_W) == true)
			{
				fAngle -= -D3DX_PI * 0.25f;
			}
			else if (GetKeyboardPress(DIK_S) == true)
			{
				fAngle -= -D3DX_PI * 0.75f;
			}
			else
			{
				fAngle -= -D3DX_PI * 0.5f;
			}
			Move.x = sinf(fAngle) * EditerSpeed;
			Move.y = 0.0f;
			Move.z = cosf(fAngle) * EditerSpeed;
		}
		else if (GetKeyboardPress(DIK_W) == true)
		{
			Move.x = sinf(fAngle) * EditerSpeed;
			Move.y = 0.0f;
			Move.z = cosf(fAngle) * EditerSpeed;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{
			fAngle += D3DX_PI;
			Move.x = sinf(fAngle) * EditerSpeed;
			Move.y = 0.0f;
			Move.z = cosf(fAngle) * EditerSpeed;
		}
		else if (GetKeyboardPress(DIK_Y) == true)
		{
			Move.x = 0.0f;
			Move.y = 1.0f;
			Move.z = 0.0f;
		}
		else if (GetKeyboardPress(DIK_N) == true)
		{
			Move.x = 0.0f;
			Move.y = -1.0f;
			Move.z = 0.0f;
		}
		else
		{
			Move.x = 0.0f;
			Move.y = 0.0f;
			Move.z = 0.0f;
		}
		g_camera.posV.x += Move.x;
		g_camera.posV.y += Move.y;
		g_camera.posV.z += Move.z;

		g_camera.posR.x += Move.x;
		g_camera.posR.y += Move.y;
		g_camera.posR.z += Move.z;
	}
}

void UpdateMousePosEditer()
{
	D3DXVECTOR2 Diff;
	Diff = GetMouseVelocity() - GetMouseOldVelocity();

	g_camera.rot.x += Diff.y * 0.001f;
	g_camera.rot.y += Diff.x * 0.001f;
	UpdateCameraPositionR();
}
void UpdateCameraPositionR()
{
	g_camera.posR.x = g_camera.posV.x + cosf(g_camera.rot.x) * sinf(g_camera.rot.y) * -g_camera.fDistance;
	g_camera.posR.y = g_camera.posV.y + sinf(g_camera.rot.x) * -g_camera.fDistance;
	g_camera.posR.z = g_camera.posV.z + cosf(g_camera.rot.x) * cosf(g_camera.rot.y) * -g_camera.fDistance;
}

//void UpdateMovie()
//{
//	static POINT SetMousePos = { (LONG)SCREEN_WIDTH / (LONG)2.0f,(LONG)SCREEN_HEIGHT / (LONG)2.0f };
//	SetCursorPos((int)SetMousePos.x, (int)SetMousePos.y);
//
//	g_camera.posR.x += (g_camera.posRDest.x - g_camera.posR.x) * 1.0f;
//	g_camera.posR.y += (g_camera.posRDest.y - g_camera.posR.y) * 1.0f;
//	g_camera.posR.z += (g_camera.posRDest.z - g_camera.posR.z) * 1.0f;
//
//	g_camera.posV.x += (g_camera.posVDest.x - g_camera.posV.x) * 1.0f;
//	g_camera.posV.y += (g_camera.posVDest.y - g_camera.posV.y) * 1.0f;
//	g_camera.posV.z += (g_camera.posVDest.z - g_camera.posV.z) * 1.0f;
//
//	if (g_camera.Anim.bFinish == true)
//	{
//		SetMode(MODE_STAGEFOUR);
//		StopSound();
//		PlaySound(SOUND_LABEL_BOSS);
//	}
//}
//void SetNumKey(int AnimCount, int NumKey)
//{
//	g_camera.Anim.Anim[AnimCount].nNumKey = NumKey;
//}
//void SetLoop(int AnimCount, int Loop)
//{
//	g_camera.Anim.Anim[AnimCount].bLoop = Loop;
//}
//void SetCameraWorkInfo(D3DXVECTOR3 PosV, D3DXVECTOR3 PosR, D3DXVECTOR3 Rot, int Frame, int AnimCount, int KeyCount)
//{
//	g_camera.Anim.Anim[AnimCount].KeyInfo[KeyCount].PosV = PosV;
//	g_camera.Anim.Anim[AnimCount].KeyInfo[KeyCount].PosR = PosR;
//	g_camera.Anim.Anim[AnimCount].KeyInfo[KeyCount].Rot = Rot;
//	g_camera.Anim.Anim[AnimCount].KeyInfo[KeyCount].nFrame = Frame;
//}
//void SetNameEffectInfo(D3DXVECTOR3 Pos, D3DXVECTOR3 Rot, D3DXVECTOR2 Scale, D3DXCOLOR Col, int AnimCount, int EffectCount, int StartFrame, int EndFrame, int nLife,MovieEffectType nType)
//{
//	g_camera.Anim.Anim[AnimCount].MovieEffect[EffectCount].SetNameEffect.Pos = Pos;
//	g_camera.Anim.Anim[AnimCount].MovieEffect[EffectCount].SetNameEffect.Rot = Rot;
//	g_camera.Anim.Anim[AnimCount].MovieEffect[EffectCount].SetNameEffect.Scale = Scale;
//	g_camera.Anim.Anim[AnimCount].MovieEffect[EffectCount].SetNameEffect.Col = Col;
//	g_camera.Anim.Anim[AnimCount].MovieEffect[EffectCount].SetNameEffect.nLife = nLife;
//	g_camera.Anim.Anim[AnimCount].MovieEffect[EffectCount].EffectType = nType;
//	g_camera.Anim.Anim[AnimCount].MovieEffect[EffectCount].StartFrame = StartFrame;
//	g_camera.Anim.Anim[AnimCount].MovieEffect[EffectCount].EndFrame = EndFrame;
//}