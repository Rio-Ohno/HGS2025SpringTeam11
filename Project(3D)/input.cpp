//===========================
//
//  �L�[���͏���[input.cpp]
//  Author:kaiti
//
//===========================
#include "input.h"
//�}�N����`
#define NUM_KEY_MAX (256)
//�O���[�o���ϐ�
XINPUT_STATE g_joyKeyState;
XINPUT_STATE g_joyKeyStateTrigger;//�W���C�p�b�h�̃g���K�[

LPDIRECTINPUT8 g_pInput = NULL;
LPDIRECTINPUTDEVICE8 g_pDevKeyboard = NULL;
BYTE g_aKeyState[NUM_KEY_MAX];
BYTE g_aOldState[NUM_KEY_MAX];

bool g_Vib;
int g_VibCounter;
int g_VibFrame;

bool isVib();
bool isGreaterVib();

void EndVib();
//=======================
//�L�[�{�[�h�̏���������
//=======================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//�L�[�{�[�h������
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}
	//
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}
	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}
	//�������[�h��ݒ�
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	g_pDevKeyboard->Acquire();

	return S_OK;
}
//=======================
//�L�[�{�[�h�̏I������
//=======================
void UninitKeyboard(void)
{
	//���̓f�o�C�X�̔j��
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}
	//DirectInput�I�u�W�F�N�g�̔j��
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}
//=======================
//�L�[�{�[�h�̍X�V����
//=======================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];
	int nCntKey;

	for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		g_aOldState[nCntKey] = g_aKeyState[nCntKey];
	}

	//
	if (SUCCEEDED(g_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_aKeyState[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{
		g_pDevKeyboard->Acquire();
	}
}
//==============================
//�L�[�{�[�h�̃v���X�����擾
//==============================
bool GetKeyboardPress(int nKey)
{
	return(g_aKeyState[nKey] & 0x080) ? true : false;
}

bool KeyboardTrigger(int nKey)
{
	bool Trigger = false;
	if (g_aKeyState[nKey] & 0x80 && !(g_aOldState[nKey] & 0x80))
	{
		Trigger = true;
	}
	return Trigger;
}
bool KeyboardRelease(int nKey)
{
	bool Trigger = false;
	if (g_aOldState[nKey] & 0x80 && !(g_aKeyState[nKey] & 0x80))
	{
		Trigger = true;
	}
	return Trigger;
}
bool KeyboardRepeat(int nKey)
{
	bool Trigger = false;
	if (g_aOldState[nKey] & 0x80 && (g_aKeyState[nKey] & 0x80))
	{
		Trigger = true;
	}
	return Trigger;
}

//============================
//�R���g���[���[�̏���������
//============================
HRESULT InitJoypad(void)
{
	//�������̃N���A
	memset(&g_joyKeyState, 0, sizeof(XINPUT_STATE));
	memset(&g_joyKeyStateTrigger, 0, sizeof(XINPUT_STATE));

	//XInput�̃X�e�[�g��ݒ�(�L��)
	XInputEnable(true);

	return S_OK;
}
//============================
//�R���g���[���[�̏I������
//============================
void UninitJoypad(void)
{
	//XInput�̃X�e�[�g��ݒ�(����)
	XInputEnable(false);
}
//============================
//�R���g���[���[�̍X�V����
//============================
void UpdateJoypad(void)
{
	XINPUT_STATE joykeyState;
	//
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{
		WORD Button = joykeyState.Gamepad.wButtons;
		WORD OldButton = g_joyKeyState.Gamepad.wButtons;

		g_joyKeyStateTrigger.Gamepad.wButtons = Button & ~OldButton;

		g_joyKeyState = joykeyState;
	}
	if (isVib() == true)
	{
		g_VibCounter++;
		if (isGreaterVib() == true)
		{
			EndVib();
		}
	}
}
//=================================
//�R���g���[���[�̃v���X�����擾
//=================================
bool GetJoypadPress(JOYKEY key)
{
	return (g_joyKeyState.Gamepad.wButtons &(0x01 << key)) ? true : false;
}
//===================================
//�W���C�p�b�h�̃g���K�[�����擾
//===================================
bool GetJoypadTrigger(JOYKEY key)
{
	return(g_joyKeyStateTrigger.Gamepad.wButtons & (0x01 << key)) ? true : false;
}
//===================================
//�W���C�p�b�h�̃g���K�[�����擾
//===================================
bool GetJoyStickL()
{
	bool joykey = false;

	if (g_joyKeyState.Gamepad.sThumbLX >= 100 
		|| g_joyKeyState.Gamepad.sThumbLX <= -100 
		|| g_joyKeyState.Gamepad.sThumbLY >= 100 
		|| g_joyKeyState.Gamepad.sThumbLY <= -100)
	{
		joykey = true;
	}
	return joykey;
}
//===================================
//�W���C�p�b�h�̃g���K�[�����擾
//===================================
bool GetJoyStickR()
{
	bool joykey = false;

	if (g_joyKeyState.Gamepad.sThumbRX >= 100
		|| g_joyKeyState.Gamepad.sThumbRX <= -100
		|| g_joyKeyState.Gamepad.sThumbRY >= 100
		|| g_joyKeyState.Gamepad.sThumbRY <= -100)
	{
		joykey = true;
	}
	return joykey;
}
XINPUT_STATE* GetJoyStickAngle(void)
{
	return &g_joyKeyState;
}

bool isVib()
{
	return g_Vib;
}

bool isGreaterVib()
{
	return g_VibCounter >= g_VibFrame;
}

void SetVibRation(int RightVib, int LeftVib, int Frame)
{
	g_Vib = true;
	g_VibFrame = Frame;
	g_VibCounter = 0;
	XINPUT_VIBRATION LocalVib = {};
	LocalVib.wRightMotorSpeed = RightVib;
	LocalVib.wLeftMotorSpeed = LeftVib;
	XInputSetState(0, &LocalVib);
}

void EndVib()
{
	g_VibCounter = 0;
	g_VibFrame = 0;
	g_Vib = false;
	XINPUT_VIBRATION ResetVib = {};
	XInputSetState(0,&ResetVib);
}