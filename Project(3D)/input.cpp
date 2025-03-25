//===========================
//
//  キー入力処理[input.cpp]
//  Author:kaiti
//
//===========================
#include "input.h"
//マクロ定義
#define NUM_KEY_MAX (256)
//グローバル変数
XINPUT_STATE g_joyKeyState;
XINPUT_STATE g_joyKeyStateTrigger;//ジョイパッドのトリガー

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
//キーボードの初期化処理
//=======================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	//キーボード初期化
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pInput, NULL)))
	{
		return E_FAIL;
	}
	//
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}
	//データフォーマットを設定
	if (FAILED(g_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}
	//協調モードを設定
	if (FAILED(g_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//キーボードへのアクセス権を獲得
	g_pDevKeyboard->Acquire();

	return S_OK;
}
//=======================
//キーボードの終了処理
//=======================
void UninitKeyboard(void)
{
	//入力デバイスの破棄
	if (g_pDevKeyboard != NULL)
	{
		g_pDevKeyboard->Unacquire();
		g_pDevKeyboard->Release();
		g_pDevKeyboard = NULL;
	}
	//DirectInputオブジェクトの破棄
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}
//=======================
//キーボードの更新処理
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
//キーボードのプレス情報を取得
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
//コントローラーの初期化処理
//============================
HRESULT InitJoypad(void)
{
	//メモリのクリア
	memset(&g_joyKeyState, 0, sizeof(XINPUT_STATE));
	memset(&g_joyKeyStateTrigger, 0, sizeof(XINPUT_STATE));

	//XInputのステートを設定(有効)
	XInputEnable(true);

	return S_OK;
}
//============================
//コントローラーの終了処理
//============================
void UninitJoypad(void)
{
	//XInputのステートを設定(無効)
	XInputEnable(false);
}
//============================
//コントローラーの更新処理
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
//コントローラーのプレス情報を取得
//=================================
bool GetJoypadPress(JOYKEY key)
{
	return (g_joyKeyState.Gamepad.wButtons &(0x01 << key)) ? true : false;
}
//===================================
//ジョイパッドのトリガー情報を取得
//===================================
bool GetJoypadTrigger(JOYKEY key)
{
	return(g_joyKeyStateTrigger.Gamepad.wButtons & (0x01 << key)) ? true : false;
}
//===================================
//ジョイパッドのトリガー情報を取得
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
//ジョイパッドのトリガー情報を取得
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