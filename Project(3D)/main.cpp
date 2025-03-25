//====================================
//
// �}�b�v�G�f�B�^�[����� [main.cpp]
// Author: Asuma Nishio
//
//====================================

//****************************
// �C���N���[�h�t�@�C���錾
//****************************
#include "main.h"
#include <time.h>
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>
#include "camera.h"
#include "light.h"
#include "input.h"
#include "meshfield.h"
#include "mouse.h"

//*****************************
// �O���[�o���ϐ��錾
//*****************************
LPDIRECT3D9 g_pD3D = NULL;				// Direct3D�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;	// Direct3D�f�o�C�X�ւ̃|�C���^
MODE g_mode = MODE_EDIT;				// ���݂̃��[�h
bool g_isFullscreen = false;			// �E�B���h�E��؂�ւ��邽�߂̃t���O
RECT g_windowRect;						// �E�B���h�E��؂�ւ��邽�߂̕ϐ�
LPD3DXFONT g_pFont = NULL;				// �t�H���g�ւ̃|�C���^
int g_nCountFPS = 0;					// FPS�J�E���^
int nGetNumber = 0;						// �ԍ�
int Filenamepass;						// �t�@�C����
int nBlockSet = 0;						// �z�u����u���b�N��

//******************************
// �v���g�^�C�v�錾
//******************************
void ToggleFullscreen(HWND hWnd);	// �E�B���h�E���t���X�N���[���ɂ�����@
void DrawModeChange();				// ���[�h�؂�ւ�
void DebugEditModelInfo();			// �z�u���f�����
void DrawCameraPos(void);			// �J�����̍��W�\��
void onWireFrame();					// ���C���[�t���[���N��
void offWireFrame();				// ���C���[�t���[���I��

//===============================
// ���C���֐�
//===============================
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hInstancePrev, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// ���������[�N���m�p�̃t���O
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);		

	DWORD dwCurrentTime;			// ���ݎ���
	DWORD dwExecLastTime;			// �I������

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),				 // WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,						 // �E�C���h�E�̃X�^�C��
		WindowProc,						 // �E�C���h�E�v���V�[�W��
		0,								 // 0�ɂ���
		0,								 // 0�ɂ���
		hInstance,						 // �C���X�^���X�n���h��
		LoadIcon(NULL,IDI_APPLICATION),  // �^�X�N�o�[�A�C�R��
		LoadCursor(NULL,IDC_ARROW),      // �}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),      // �N���C�A���g�̈�̔w�i�F
		NULL,                            // ���j���[�o�[
		CLASS_NAME,                      // �E�C���h�E�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION),  // �t�@�C���A�C�R��
	};


	HWND hWnd = {};  // �E�C���h�E�n���h��(���ʎq)
	MSG msg;    //���b�Z�[�W���i�[����ϐ�

	// ��ʃT�C�Y�̍\��
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	// �t���X�N���[�����[�h
	BOOL bWindow = FALSE;

	// �E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �N���C�A���g�̈�
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// �E�C���h�E�𐶐�
	hWnd = CreateWindowEx(0,		// �g���E�C���h�E�X�^�C��
		CLASS_NAME,					// �E�C���h�E�N���X�̖��O
		WINDOW_NAME,				// �E�C���h�E�̖��O
		WS_OVERLAPPEDWINDOW,		// �E�C���h�E�X�^�C��
		CW_USEDEFAULT,				// �E�C���h�E�̍����X���W
		CW_USEDEFAULT,				// �E�C���h�E�̍����Y���W
		(rect.right - rect.left),   // �E�C���h�E��
		(rect.bottom - rect.top),   // �E�C���h�E�̍���
		NULL,						// NULL
		NULL,						// NULL
		hInstance,					// �C���X�^���X�n���h��
		NULL);						// �E�C���h�E�쐬�f�[�^


	// ����������
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{
		// ���������������s�����Ƃ�
		return -1;
	}

	// ����\�������Ă�
	timeBeginPeriod(1);
	dwCurrentTime = 0;					// ������
	dwExecLastTime = timeGetTime();		// ���ݎ�����ۑ�

	// �E�C���h�E�̕\��
	ShowWindow(hWnd, nCmdShow); // �E�C���h�E�̕\����Ԃ̐ݒ�
	UpdateWindow(hWnd);				    // �N���C�A���g�̈�̍X�V

	// ������
	DWORD dwFrameCount;					// �t���[���J�E���g
	DWORD dwFPSLastTime;				// �Ō��FPS���v����������

	dwFrameCount = 0;					// �t���[���J�E���g��������
	dwFPSLastTime = timeGetTime();		// ���݂̎������擾

	// ���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			// Windows�̏���
			if (msg.message == WM_QUIT)
			{
				// WM_QUIT���b�Z�[�W���󂯎�����烋�[�v������
				break;
			}
			else
			{
				// ���b�Z�[�W�ݒ�
				TranslateMessage(&msg);	// ���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�

				DispatchMessage(&msg);	// �E�C���h�E�v���V�[�W���փ��b�Z�[�W�𑗏o
			}
		}

		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (!GetMessage(&msg, NULL, 0, 0)) 
			{
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
			
		}
		else
		{// DirectX�̏���
			dwCurrentTime = timeGetTime();		// ���ݎ����̎擾
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{
				// 0.5�b�o��
				// FPS���v��
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;	// �ۑ�

				dwFrameCount = 0;				// 0�ɖ߂�

			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				// 60/1�o��
				dwExecLastTime = dwCurrentTime;	// �����J�n�̎���

				dwFrameCount++;					// �t���[���J�E���g�����Z

				// �X�V����
				Update();

				// �`�揈��
				Draw();

			}
		}
	}

	// �I������
	Uninit();

	// ����\��߂�
	timeEndPeriod(1);

	// �E�C���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// ���b�Z�[�W��Ԃ�
	return(int)msg.wParam;
}

//=======================================
// �E�C���h�E�v���V�[�W��
//=======================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	const RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT }; // �E�C���h�E�̗̈�
	int nID;

	switch (uMsg)
	{
	case WM_ACTIVATE:	// �A�N�e�B�u���F1�@��A�N�e�B�u���F0
		return 0L;

	case WM_DESTROY: // �E�C���h�E�j�����b�Z�[�W
		// WM_QUIT���b�Z�[�W�𑗂�
		PostQuitMessage(0); // �E�C���h�E�̔j��
		break;

	case WM_KEYDOWN: // �L�[�����̃��b�Z�[�W
		switch (wParam)
		{
		case VK_ESCAPE:	// [ESC]�L�[�������ꂽ

			nID = MessageBox(hWnd, "�I�����܂���?", "�I�����b�Z�[�W", MB_YESNO);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd); // �E�C���h�E�j�����b�Z�[�W
			}
			else
			{
				return 0;			 // �Ԃ�
			}

			break;
		}
		break;

	case WM_MOUSEWHEEL: // �}�E�X�̃z�C�[������

		// ���[�J���ϐ�
		int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);

		// �z�C�[�����
		MouseWheel(zDelta);
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam); // ����̏������J��Ԃ�
}
//=========================
// ����������
//=========================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			// �f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;	// �v���[���e�[�V�����p�����[�^

	// Direct3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}
	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �p�����[�^�[��0�N���A
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	d3dpp.BackBufferWidth = SCREEN_WIDTH;						// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;						// �o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;									// �o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// �_�u���o�b�t�@�̐؂�ւ�(�f���M���ɓ���)
	d3dpp.EnableAutoDepthStencil = TRUE;						// �f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// �f�v�X�o�b�t�@�Ƃ���16bit���g�p
	d3dpp.Windowed = bWindow;									// �E�C���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// ���t���b�V���V���[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��

	// Direct3D�f�o�C�X�̐���(�`�揈���ƒ��_�������n�[�h�E�F�A�ōs��)
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		// Direct3D�f�o�C�X�̐���(�`�揈���̓n�[�h�E�F�A,���_������CPU�ōs��)
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			// Direct3D�f�o�C�X�̐���(�`�揈��,���_������CPU�ōs��)
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	// �e�N�X�`���X�e�[�W�`���[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//==============================
	// ����������
	//==============================
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{// �L�[�{�[�h
		return E_FAIL;
	}
	if (FAILED(InitJoypad()))
	{// �p�b�h
		return E_FAIL;
	}

	// �f�o�b�O�\���p�̃t�H���g��ݒ�
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"Terminal", &g_pFont);

	// �}�E�X������
	InitMouse(hInstance,hWnd);

	// ���C�g
	InitLight();

	// �J����
	InitCamera();

	// ���b�V���̕`��
	InitMeshfield();
	SetMeshfield(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 10, 0, 10, 100, 100);

	return S_OK; // ���ʂ�Ԃ�

}
//==================
// �I������
//==================
void Uninit(void)
{
	//==================
	// �e��I������
	//==================

	// �L�[�{�[�h
	UninitKeyboard();

	// �}�E�X
	UninitMouse();

	// �p�b�h
	UninitJoypad();

	// ���C�g
	UninitLight();

	// ���b�V���̏I��
	UninitMeshfield();

	// �f�o�b�O�\���p�̃t�H���g�̔j��
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	// Direct3D�f�o�C�X�̔j��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

}
//===================
// �X�V����
//===================
void Update(void)
{
	//===========================
	// �e��I�u�W�F�N�g�X�V����
	//===========================
	// �L�[�{�[�h�̍X�V����
	UpdateKeyboard();

	// �}�E�X�̍X�V����
	UpdateMouse();

	// �Q�[���p�b�h�̍X�V����
	UpdateJoypad();

	// �J�����̍X�V
	UpdateCamera();

	if (KeyboardTrigger(DIK_F1))
	{// F3�L�[
		// ���C���[�t���[���N��
		onWireFrame();
	}
	else if (KeyboardTrigger(DIK_F2))
	{// F4�L�[
		// ���C���[�t���[������
		offWireFrame();
	}

	// ���[�h�؂�ւ�
	//if (KeyboardTrigger(DIK_F2))
	//{
	//	g_mode = MODE_EDIT;
	//}

}
//===================
// �`�揈��
//===================
void Draw(void)
{
	// �J�����擾
	Camera* pCamera = GetCamera();

	// ��ʃN���A(�o�b�N�o�b�t�@&Z�o�b�t�@�̃N���A)
	g_pD3DDevice->Clear(0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// �`��J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		// �`�搬����
		//================
		// �`�揈��
		//================
			
		// �J�������Z�b�g
		SetCamera();

		// ���b�V���̕`��
		DrawMeshfield();


		if (g_mode == MODE_EDIT)
		{
			DrawCameraPos();

			DebugEditModelInfo();
		}
		
		// �f�o�b�O�t�H���g�֌W
		DrawFPS();

		// ���[�h�ؑ�
		DrawModeChange();


		// �`��I��
		g_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);

}
//======================
// FPS�̕\��
//======================
void DrawFPS(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aString[256];

	//������ɑ��
	wsprintf(&aString[0], "//************************\n// �V���[�g�J�b�g�L�[���\n//************************\n");

	//�e�L�X�g�̕`��
	g_pFont->DrawText(NULL, &aString[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
}
//========================
// �J�����\��
//========================
void DrawCameraPos(void)
{
	// ���[�J���ϐ�
	RECT rect = { 0,140,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect1 = { 0,160,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect2 = { 0,60,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect3 = { 0,120,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect4 = { 0,180,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect5 = { 0,200,SCREEN_WIDTH,SCREEN_HEIGHT };

	// ������
	char aString[256];
	char aString1[256];
	char aString2[256];
	char aString3[256];
	char aString4[64];
	char aString5[64];

	// �J�����̎擾
	Camera* pCamera = GetCamera();

	// ������ɑ��
	wsprintf(&aString2[0], "//************************\n// �J�������\n//************************\n");
	sprintf(&aString[0],   "[ �J������rot.y ]: < %.2f >\n", pCamera->rot.y);
	sprintf(&aString1[0],  "[ �J������rot.x ]: < %.2f >\n", pCamera->rot.x);
	wsprintf(&aString3[0], "[ ���_ / �����_ ] : < Shift + �}�E�X���� >\n");
	wsprintf(&aString4[0], "[ �Y�[�� ] :   < �}�E�X�z�C�[�� >\n");
	wsprintf(&aString5[0], "[ ���Z�b�g ] : < �}�E�X�z�C�[���������� > / TAB�L�[\n");

	// �e�L�X�g�`��
	g_pFont->DrawText(NULL, &aString[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aString1[0], -1, &rect1, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aString2[0], -1, &rect2, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aString3[0], -1, &rect3, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aString4[0], -1, &rect4, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aString5[0], -1, &rect5, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));

}
//============================
// �G�f�B�^�[���f���֌W�̕\��
//============================
void DebugEditModelInfo()
{
#if 0
	// �擾�֌W
	int nModel = ReturnEdit(); // �z�u�J�E���g
	MAPMODELINFO* pEdit = MapInfo(); // �z�u���̏��
	EDITMODEL* pModelEdit = GetBlockInfo(pEdit[nModel].mapedit.nType); // ���f�����

	// ���[�J���ϐ�
	RECT rect = { 0,220,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect1 = { 0,320,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect2 = { 0,340,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect3 = { 0,360,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect4 = { 0,400,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect5 = { 0,420,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect6 = { 0,440,SCREEN_WIDTH,SCREEN_HEIGHT };
	RECT rect7 = { 0,480,SCREEN_WIDTH,SCREEN_HEIGHT };

	// ������
	char aString[256];
	char aString1[256];
	char aString2[256];
	char aString3[256];
	char aString4[256];
	char aString5[256];
	char aString6[256];
	char aString7[256];

	// ������ɑ��
	wsprintf(&aString[0], "//************************\n// �z�u���郂�f�����\n//************************\n");
	sprintf(&aString1[0], "[ ���f����X���W ] : < %.2f >\n", pEdit[nModel].mapedit.pos.x);
	sprintf(&aString2[0], "[ ���f����Y���W ] : < %.2f >\n", pEdit[nModel].mapedit.pos.y);
	sprintf(&aString3[0], "[ ���f����Z���W ] : < %.2f >\n", pEdit[nModel].mapedit.pos.z);
	sprintf(&aString4[0], "[ ���f����X�̊g�嗦 ] : < %.2f > \n", pEdit[nModel].mapedit.Scal.x);
	sprintf(&aString5[0], "[ ���f����Y�̊g�嗦 ] : < %.2f >\n", pEdit[nModel].mapedit.Scal.y);
	sprintf(&aString6[0], "[ ���f����Z�̊g�嗦 ] : < %.2f >\n", pEdit[nModel].mapedit.Scal.z);
	sprintf(&aString7[0], "[ �I�𒆂̃��f���̎�� ] : < %s > \n", pModelEdit->FileName);

	// �e�L�X�g�`��
	g_pFont->DrawText(NULL, &aString[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aString1[0], -1, &rect1, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aString2[0], -1, &rect2, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aString3[0], -1, &rect3, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aString4[0], -1, &rect4, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aString5[0], -1, &rect5, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aString6[0], -1, &rect6, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
	g_pFont->DrawText(NULL, &aString7[0], -1, &rect7, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
#endif
}
//==============================
// ���[�h�ؑփt�H���g�̕\��
//==============================
void DrawModeChange()
{
	// �擾
	MODE nMode = GetMode();

	// ���[�J���ϐ�
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	char aString[256];
	char aString1[256];

	if (nMode == MODE_EDIT)
	{// �ҏW���[�h�Ȃ�
		strcpy(aString1, "�ҏW���[�h");
	}
	else if (nMode == MODE_PLAY)
	{// �v���C���[�h�Ȃ�
		strcpy(aString1, "�v���C���[�h");
	}

	// ������ɑ��
	wsprintf(&aString[0], "���݂̃��[�h < %s >\n", &aString1[0]);

	// �t�H���g�̕\��
	g_pFont->DrawText(NULL, &aString[0], -1, &rect, DT_RIGHT, D3DCOLOR_RGBA(255, 255, 255, 255));

}
//======================
// �f�o�C�X�̎擾
//======================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}
//====================
// ���[�h�̎擾
//====================
MODE GetMode(void)
{
	return g_mode; // ���݂̃��[�h��Ԃ�
}
//=====================
// ���C���[�t���[��ON
//=====================
void onWireFrame()
{
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
}
//=====================
// ���C���[�t���[��OFF
//=====================
void offWireFrame() 
{
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}
//=================================================
// �E�B���h�E���t���X�N���[���ɕς��鏈��
//=================================================
void ToggleFullscreen(HWND hWnd)
{
	// ���݂̃E�B���h�E�X�^�C�����擾
	DWORD dwStyle = GetWindowLong(hWnd, GWL_STYLE);

	if (g_isFullscreen)
	{
		// �E�B���h�E���[�h�ɐ؂�ւ�
		SetWindowLong(hWnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, g_windowRect.left, g_windowRect.top,
			g_windowRect.right - g_windowRect.left, g_windowRect.bottom - g_windowRect.top,
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_NORMAL);
	}
	else
	{
		// �t���X�N���[�����[�h�ɐ؂�ւ�
		GetWindowRect(hWnd, &g_windowRect);
		SetWindowLong(hWnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
		SetWindowPos(hWnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(hWnd, SW_MAXIMIZE);
	}

	g_isFullscreen = !g_isFullscreen;
}

