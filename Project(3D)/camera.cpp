//==========================
//
// �J�������� [camera.cpp]
// Author: Rio Ohno
//
//==========================

// �C���N���[�h�t�@�C���錾
#include "camera.h"
#include "input.h"
#include "mouse.h"

// �}�N����`
#define MAX_VIEWUP (3.00f) // �J�����̐���
#define MAX_VIEWDOWN (0.1f) // �J�����̐���

// �O���[�o���ϐ��錾
Camera g_camera;			// �J�������
D3DXVECTOR3 zoom;			// �J�����Y�[��
bool isrot;					// �������񂩔ۂ�
bool isPlayer;

//=========================
// �J�����̏���������
//=========================
void InitCamera(void)
{
	g_camera.posV = D3DXVECTOR3(0.0f, 350.0f, -1600.0f);		 // �J�����̈ʒu
	g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			 // �J�����̌��Ă���ʒu
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			 // ������x�N�g��
	g_camera.rot = D3DXVECTOR3(D3DX_PI * 0.65f, 0.0f, 0.0f); // �p�x
	g_camera.g_CameraMode = CAMERAMODE_NONE;				 // �������

	// ���W�̍������v�Z
	float fRotx = g_camera.posV.x - g_camera.posR.x;
	float fRoty = g_camera.posV.y - g_camera.posR.y;
	float fRotz = g_camera.posV.z - g_camera.posR.z;

	// ���_���璍���_�܂ł̋���
	g_camera.fDistance = sqrtf((fRotx * fRotx) + (fRoty * fRoty) + (fRotz * fRotz));

	// �O���[�o���ϐ��̏�����
	zoom = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	isrot = false;
	isPlayer = false;
}
//=========================
// �J�����̏I������
//=========================
void UninitCamera(void)
{
	// ����
}
//=========================
// �J�����̍X�V����
//=========================
void UpdateCamera(void)
{
	// �v���C���[���擾
	// PLAYER* pPlayer = GetPlayer();

	// ���݂̃��[�h���擾
	MODE mode = GetMode();

	//�ҏW���[�h���̃J�����ړ�
	MouseEditMode();
#if 0
	if (GetKeyboardPress(DIK_LEFT))
	{
		// ��]
		g_camera.rot.y += 0.009f;

		// �J�����̎��_�̏��
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.y = g_camera.posR.y - cosf(g_camera.rot.x) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;

		// ���K��
		if (g_camera.rot.y > D3DX_PI)
		{// D3DX_PI���傫���Ȃ�����
			g_camera.rot.y -= D3DX_PI * 2.0f;
		}

		// �p�x�̐��K��
		if (g_camera.rot.y < -D3DX_PI)
		{// D3DX_PI��菬�����Ȃ�����
			g_camera.rot.y += D3DX_PI * 2.0f;
		}
	}
	else if (GetKeyboardPress(DIK_RIGHT))
	{
		// ��]
		g_camera.rot.y -= 0.009f;

		// �J�����̎��_�̏��
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.y = g_camera.posR.y - cosf(g_camera.rot.x) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;

		// ���K��
		if (g_camera.rot.y > D3DX_PI)
		{// D3DX_PI���傫���Ȃ�����
			g_camera.rot.y -= D3DX_PI * 2.0f;
		}

		// �p�x�̐��K��
		if (g_camera.rot.y < -D3DX_PI)
		{// D3DX_PI��菬�����Ȃ�����
			g_camera.rot.y += D3DX_PI * 2.0f;
		}
	}

	if (GetKeyboardPress(DIK_UP))
	{
		// ��]
		g_camera.rot.x += 0.009f;

		// �p�x�̐��K��
		if (g_camera.rot.x < -D3DX_PI)
		{// D3DX_PI��菬�����Ȃ�����
			g_camera.rot.x += D3DX_PI * 2.0f;
		}
		if (g_camera.rot.x > D3DX_PI)
		{// D3DX_PI���傫���Ȃ�����
			g_camera.rot.x -= D3DX_PI * 2.0f;
		}

		// �J�����̎��_�̏��
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.y = g_camera.posR.y - cosf(g_camera.rot.x) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;

	}
	else if (GetKeyboardPress(DIK_DOWN))
	{
		// ��]
		g_camera.rot.x -= 0.009f;

		// �p�x�̐��K��
		if (g_camera.rot.x < -D3DX_PI)
		{// D3DX_PI��菬�����Ȃ�����
			g_camera.rot.x += D3DX_PI * 2.0f;
		}
		if (g_camera.rot.x > D3DX_PI)
		{// D3DX_PI���傫���Ȃ�����
			g_camera.rot.x -= D3DX_PI * 2.0f;
		}

		// �J�����̎��_�̏��
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.y = g_camera.posR.y - cosf(g_camera.rot.x) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;
	}
#endif
	if (KeyboardTrigger(DIK_F10))
	{// F10�L�[
		// ����N��
		isrot = true;
	}
	else if (KeyboardTrigger(DIK_F11))
	{// F11�L�[
		// ����I��
		isrot = false;
	}

	if (KeyboardTrigger(DIK_N))
	{
		// �Ǐ]
		isPlayer = true;
	}
	else if (KeyboardTrigger(DIK_M))
	{
		// �Ǐ]
		isPlayer = false;
	}

	if (OnMouseTriggerDown(2) || KeyboardTrigger(DIK_TAB))
	{
		InitCamera();
	}

#if 1
	if (mode == MODE_PLAY)
	{// ��������

		if (isrot)
		{
			g_camera.posV.z = -800.0f;
			g_camera.posV.y = 800.0f;

			g_camera.rot.y += 0.005f; // �J�����̎��_�̏��

			g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * 1800.0f;
			g_camera.posV.y = g_camera.posR.y - cosf(g_camera.rot.x) * 1800.0f;
			g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * 1800.0f;
		}

	}
#endif
	// �v���C���̃J�����̍X�V
	if (mode != MODE_EDIT && isPlayer)
	{
		// �}�E�X���_�ړ�
		MouseView();
#if 0
		//****************************************
		//      �v���C���[�Ǐ]
		//****************************************
		g_camera.posRDest.x = pPlayer->pos.x + sinf(pPlayer->rotDestPlayer.y) * 1.0f;
		g_camera.posRDest.y = pPlayer->pos.y + cosf(pPlayer->rotDestPlayer.y) * 1.0f;
		g_camera.posRDest.z = pPlayer->pos.z + cosf(pPlayer->rotDestPlayer.y) * 1.0f;

		g_camera.posVDest.x = pPlayer->pos.x - sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posVDest.y = pPlayer->pos.y - cosf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posVDest.z = pPlayer->pos.z - cosf(g_camera.rot.y) * g_camera.fDistance;

		g_camera.posR.x += ((g_camera.posRDest.x - g_camera.posR.x) * 0.3f);
		g_camera.posR.y += ((g_camera.posRDest.y - g_camera.posR.y) * 0.3f);
		g_camera.posR.z += ((g_camera.posRDest.z - g_camera.posR.z) * 0.3f);

		g_camera.posV.x += ((g_camera.posVDest.x - g_camera.posV.x) * 0.3f);
		g_camera.posV.z += ((g_camera.posVDest.z - g_camera.posV.z) * 0.3f);

	}

#endif
	}

	if (g_camera.rot.x <= D3DX_PI * 0.35f)
	{// �J�����̉���

		g_camera.rot.x = D3DX_PI * 0.35f;
	}
	else if (g_camera.rot.x >= D3DX_PI * 0.9f)
	{// �J�����̏��

		g_camera.rot.x = D3DX_PI * 0.9f;
	}
}
//=========================
// �J�����̐ݒ菈��
//=========================
void SetCamera(void)
{
	// �f�o�C�X�|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxView);

	// �r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_camera.mtxView,
		&g_camera.posV,
		&g_camera.posR,
		&g_camera.vecU);

	// �r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);

	// �v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	// �v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
		D3DXToRadian(45.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		4000.0f);

	// �v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

}
//=========================
// �J�����̎擾����
//=========================
Camera* GetCamera()
{
	return &g_camera;
}
//=================================
// �J�����̂��߂̉E�X�e�B�b�N����
//=================================
void StickCamera(void)
{
	// �X�e�B�b�N
	XINPUT_STATE* pStick = GetJoyStickAngle();
#if 1
	if (GetJoyStick())
	{
		float RStickAngleY = pStick->Gamepad.sThumbRY;
		float RStickAngleX = pStick->Gamepad.sThumbRX;

		float DeadZone = 10920.0f;
		float fMag = sqrtf((RStickAngleX * RStickAngleX) + (RStickAngleY * RStickAngleY));

		if (fMag > DeadZone)
		{
			if (pStick->Gamepad.sThumbRX < -10920.0f)
			{
				// �p�x
				g_camera.rot.y -= 0.03f;

				g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * sinf(g_camera.rot.y) * g_camera.fDistance;
				g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.y) * cosf(g_camera.rot.y) * g_camera.fDistance;

			}
			else if (pStick->Gamepad.sThumbRX > 10920.0f)
			{
				// �p�x
				g_camera.rot.y += 0.03f;

				g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.y) * sinf(g_camera.rot.y) * g_camera.fDistance;
				g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.y) * cosf(g_camera.rot.y) * g_camera.fDistance;

			}


			// �p�x�̐��K��
			if (g_camera.rot.y > D3DX_PI)
			{// D3DX_PI���傫���Ȃ�����
				g_camera.rot.y -= D3DX_PI * 2.0f;
			}

			// �p�x�̐��K��
			if (g_camera.rot.y < -D3DX_PI)
			{// D3DX_PI��菬�����Ȃ�����
				g_camera.rot.y += D3DX_PI * 2.0f;
			}
		}
	}
#endif
}
//=================================
// �}�E�X�z�C�[������
//=================================
void MouseWheel(int zdelta)
{
	// ���[�J���ϐ�
	zoom = g_camera.posV - g_camera.posR;
	D3DXVec3Normalize(&zoom, &zoom);

	if (zdelta > 0)
	{// �}�E�X�̒l���� (���ɓ�������)
		g_camera.fDistance += 10.0f;
	}
	else if (zdelta < 0)
	{// �}�E�X�̒l���� (��O�ɓ�������)
		g_camera.fDistance -= 10.0f;
	}

	if (g_camera.fDistance <= 300.0f)
	{// 300.0f�ȉ��Ȃ�
		g_camera.fDistance = 300.0f;
	}

	// �J�����̎��_�̏��
	g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * g_camera.fDistance;
	g_camera.posV.y = g_camera.posR.y - cosf(g_camera.rot.x) * g_camera.fDistance;
	g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;

}
//=========================
// �}�E�X�̎��_
//========================
void MouseView(void)
{
	MODE mode = GetMode();

	DIMOUSESTATE mouseState;

	if (mode != MODE_PLAY && GetMouseState(&mouseState))
	{
		static POINT prevCursorPos = { (long)SCREEN_WIDTH / (long)1.5f,(long)SCREEN_HEIGHT / (long)1.5f };

		POINT cursorPos;
		GetCursorPos(&cursorPos);

		float X = (float)cursorPos.x - prevCursorPos.x;
		float Y = (float)cursorPos.y - prevCursorPos.y;

		const float mouseSensitivity = 0.00025f;

		X *= mouseSensitivity;
		Y *= mouseSensitivity;

		g_camera.rot.y += X;
		g_camera.rot.x += Y;

		if (g_camera.rot.y < -D3DX_PI)
		{
			g_camera.rot.y += D3DX_PI * 2.0f;
		}
		else if (g_camera.rot.y > D3DX_PI)
		{
			g_camera.rot.y += -D3DX_PI * 2.0f;
		}

		if (g_camera.rot.x < -D3DX_PI)
		{
			g_camera.rot.x += D3DX_PI * 2.0f;
		}
		else if (g_camera.rot.x > D3DX_PI)
		{
			g_camera.rot.x += -D3DX_PI * 2.0f;
		}

		if (g_camera.rot.x > MAX_VIEWUP)
		{
			g_camera.rot.x -= Y;
		}
		else if (g_camera.rot.x < MAX_VIEWDOWN)
		{
			g_camera.rot.x -= Y;
		}

		SetCursorPos((long)SCREEN_WIDTH / (long)1.5f, (long)SCREEN_HEIGHT / (long)1.5f);

		prevCursorPos.x = (long)SCREEN_WIDTH / (long)1.5f;
		prevCursorPos.y = (long)SCREEN_HEIGHT / (long)1.5f;

		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.y = g_camera.posV.y + cosf(g_camera.rot.x) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;

	}

}
//============================
// �ҏW���[�h�̎��̃}�E�X�ړ�
//============================
void MouseEditMode(void)
{
	if (OnMousePress(RIGHT_MOUSE) && GetKeyboardPress(DIK_LSHIFT))
	{
		D3DXVECTOR2 Move = GetMouseVelocity();
		D3DXVECTOR2 MoveOld = GetMouseOldVelocity();

		D3DXVECTOR2 fAngle = Move - MoveOld;

		//��]�ʂ��X�V
		g_camera.rot.y += fAngle.x * 0.01f;
		g_camera.rot.x += fAngle.y * 0.01f;

		//��]�ʂ𐧌�
		if (g_camera.rot.x > MAX_VIEWUP)
		{
			g_camera.rot.x -= fAngle.y * 0.01f;
		}
		else if (g_camera.rot.x < MAX_VIEWDOWN)
		{
			g_camera.rot.x -= fAngle.y * 0.01f;
		}

		//�J�������W���X�V
		g_camera.posR.x = g_camera.posV.x + sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posR.y = g_camera.posV.y + cosf(g_camera.rot.x) * g_camera.fDistance;
		g_camera.posR.z = g_camera.posV.z + sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;
	}
	else if (OnMousePress(LEFT_MOUSE) && GetKeyboardPress(DIK_LSHIFT))
	{
		D3DXVECTOR2 Move = GetMouseVelocity();
		D3DXVECTOR2 MoveOld = GetMouseOldVelocity();

		D3DXVECTOR2 fAngle = Move - MoveOld;

		//��]�ʂ��X�V
		g_camera.rot.y += fAngle.x * 0.01f;
		g_camera.rot.x += fAngle.y * 0.01f;

		//��]�ʂ𐧌�
		if (g_camera.rot.x > MAX_VIEWUP)
		{
			g_camera.rot.x -= fAngle.y * 0.01f;
		}
		else if (g_camera.rot.x < MAX_VIEWDOWN)
		{
			g_camera.rot.x -= fAngle.y * 0.01f;
		}

		// �J�����̎��_�̏��
		g_camera.posV.x = g_camera.posR.x - sinf(g_camera.rot.x) * sinf(g_camera.rot.y) * g_camera.fDistance;
		g_camera.posV.y = g_camera.posR.y - cosf(g_camera.rot.x) * g_camera.fDistance;
		g_camera.posV.z = g_camera.posR.z - sinf(g_camera.rot.x) * cosf(g_camera.rot.y) * g_camera.fDistance;
	}

	//���K��
	if (g_camera.rot.y < -D3DX_PI)
	{
		g_camera.rot.y += D3DX_PI * 2.0f;
	}
	else if (g_camera.rot.y > D3DX_PI)
	{
		g_camera.rot.y += -D3DX_PI * 2.0f;
	}
	if (g_camera.rot.x < -D3DX_PI)
	{
		g_camera.rot.x += D3DX_PI * 2.0f;
	}
	else if (g_camera.rot.x > D3DX_PI)
	{
		g_camera.rot.x += -D3DX_PI * 2.0f;
	}

}

