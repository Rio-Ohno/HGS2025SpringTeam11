//================================
//
//�@�E�C���h�E�\������[main.cpp]
//�@Author:kaiti
//
//================================
#include "main.h"
#include "input.h"
#include "sound.h"
#include "player.h"
#include <crtdbg.h>
#include <stdio.h>
#include "bullet.h"

//�O���[�o���ϐ��錾
LPDIRECT3D9 g_pD3D = NULL;
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;
MODE g_mode = MODE_TITLE;
LPD3DXFONT g_pFont;
RECT g_windowRect; // �E�B���h�E�؂�ւ��ϐ�
bool bDispFont = true;
bool g_bEdit = false;
bool g_isFullscreen = false; // ������Ԃ��t���X�N���[���ɂ��Ȃ�

// �v���g�^�C�v�錾
void ToggleFullScreen(HWND hWnd); // �t���X�N���[���ύX����

//=============
// ���C���֐�
//=============
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hInstancePrev, _In_ LPSTR ipCmdLine, _In_ int nCmdShow)
{
	// ���������[�N���m�p�̃t���O
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WindowProc,
		0,
		0,
		hInstance,
		LoadIcon(NULL,IDI_APPLICATION),
		LoadCursor(NULL,IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		LoadIcon(NULL,IDI_APPLICATION)
	};

	HWND hWnd;
	MSG msg;

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	//�E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	//�N���C�A���g�̈���w��̃T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//�E�C���h�E�𐶐�
	hWnd = CreateWindowEx(0,
		                  CLASS_NAME,
		                  WINDOW_NAME,
		                  WS_OVERLAPPEDWINDOW,
		                  CW_USEDEFAULT,
		                  CW_USEDEFAULT,
		                  (rect.right - rect.left),
		                  (rect.bottom - rect.top),
		                  NULL,
		                  NULL,
		                  hInstance,
		                  NULL);

	DWORD dwCurrentTime;
	DWORD dwExecLastTime;

	//����������
	//����������
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{// ���������������s�����Ƃ�
		return -1;
	}

	
	//����\��ݒ�
	timeBeginPeriod(1);
	dwCurrentTime = 0;
	dwExecLastTime = timeGetTime();	

	//�E�C���h�E�̕\�� (�t���X�N���[��)
	ShowWindow(hWnd, SW_SHOWMAXIMIZED);
	UpdateWindow(hWnd);

	//���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{//DirectX�̏���
			dwCurrentTime = timeGetTime();
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				dwExecLastTime = dwCurrentTime;

				if (g_mode == MODE_END)
				{
					break;
				}

				//�X�V����
				Update();
				//�`�揈��
				Draw();
			}
		}
	}
	//�I������
	Uninit();

	//
	timeEndPeriod(1);

	//�E�C���h�E�N���X�̉���
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;
}

//========================
// �E�C���h�E�v���V�[�W��
//========================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	switch (g_mode)
	{
	case MODE_END:
		DestroyWindow(hWnd);
		break;

	default:
		break;
	}

	switch (uMsg)
	{
	case WM_DESTROY:
		//WM_QUIT���b�Z�[�W�𑗂�
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN: //�L�[�����̃��b�Z�[�W
		switch (wParam)
		{
		case VK_ESCAPE://[ESC]�L�[�������ꂽ�ꍇ
			nID = MessageBox(hWnd, "�I�����܂���?", "�I�����b�Z�[�W", MB_YESNO);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd);
			}
			else
			{
				return 0;
			}
			break;

		case VK_F11:
			// F11�L�[�������ꂽ
			ToggleFullScreen(hWnd);
			break;

		default:
			break;
		}
		break;
		break;
	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//==================
// �f�o�C�X�̎擾
//==================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//=============
// ����������
//=============
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// �}�E�X�J�[�\���̔�\��
	ShowCursor(FALSE);

	D3DDISPLAYMODE d3ddm;
	D3DPRESENT_PARAMETERS d3dpp;
	//Direct3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}
	//���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.Windowed = bWindow;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	//Direct3D�f�o�C�X�̐���
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		                            D3DDEVTYPE_HAL,
		                            hWnd,
		                            D3DCREATE_HARDWARE_VERTEXPROCESSING,
		                            &d3dpp,
		                            &g_pD3DDevice)))
	{//Direct3D�f�o�C�X�̐���
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			                            D3DDEVTYPE_HAL,
			                            hWnd,
			                            D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			                            &d3dpp,
			                            &g_pD3DDevice)))
		{//Direct3D�f�o�C�X�̐���
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

	//�����_�[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_INVSRCALPHA);

	//�T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	
	//�e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2,D3DTA_CURRENT);

	//------------------------------
	//�e��I�u�W�F�N�g�̏���������
	//------------------------------
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	if (FAILED(InitJoypad()))
	{
		return E_FAIL;
	}

	// �f�o�b�N�\���p�t�H���g�̐���
	D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0,
		FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		"Terminal", &g_pFont);

	//�T�E���h�̏���������
	InitSound(hWnd);

	//�v���C���[�̏�����
	InitPlayer();

	//�e�̏�����
	InitBullet();

	g_bEdit = false;
	return S_OK;
}

//==========
// �I������
//==========
void Uninit(void)
{
	//----------------------------
	//�e��I�u�W�F�N�g�̏I������
	//----------------------------
	//�T�E���h�̏I������
	UninitSound();

	UninitKeyboard();

	UninitJoypad();

	UninitPlayer();

	UninitBullet();

	//D3D�f�o�C�X�̔j��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}
	//D3D�I�u�W�F�N�g�̔j��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

}

//==========
// �X�V����
//==========
void Update(void)
{
	//----------------------------
	//�e��I�u�W�F�N�g�̍X�V����
	//----------------------------
	UpdateKeyboard();
	UpdateJoypad();

#ifdef _DEBUG
	if (KeyboardTrigger(DIK_F3) == true)
	{
		g_bEdit = g_bEdit ? false : true;
	}
#endif

	if (g_bEdit == true)
	{

	}
	else
	{
		////�u���b�N
		//UpdateBlock();

		//�v���C���[
		UpdatePlayer();

		//�e
		UpdateBullet();
	}

#ifdef _DEBUG
	////���C���[
	//if (GetKeyboardPress(DIK_F1) == true)
	//{
	//	onWireFrame();
	//}
	//else if (GetKeyboardPress(DIK_F2) == true)
	//{
	//	offWireFrame();
	//}

	////�f�o�b�N�t�H���g�̕\����\��
	//if (KeyboardTrigger(DIK_F4) == true)
	//{
	//	bDispFont = bDispFont ? false : true;
	//}

#endif 
}

//==========
// �`�揈��
//==========
void Draw(void)
{
	//��ʃN���A
	g_pD3DDevice->Clear(0, NULL,
		                (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		                D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f ,0);
	//�`��J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//�`��J�n������

		if (g_bEdit == false)
		{
			//�v���C���[�̕`�揈��
			DrawPlayer();

			//�e�̕`�揈��
			DrawBullet();
		}

#ifdef _DEBUG

		if (bDispFont == true)
		{
			//DrawEffectEditer();
			//DrawPlayerInfo();
			//DrawTestInfo();
			//DrawBossInfo();
		}

#endif // DEBUG

		//�`��I��
		g_pD3DDevice->EndScene();
	}
	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//****************************************
// �E�B���h�E���t���X�N���[���ɂ��鏈��
//****************************************
void ToggleFullScreen(HWND hWnd)
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

//*****************
// �e�N�X�`���̔j��
//*****************
void UninitTexture(LPDIRECT3DTEXTURE9 &Texture)
{
	if (Texture != NULL)
	{
		Texture->Release();
		Texture = NULL;
	}
}

//*********************
// ���_�o�b�t�@�[�̔j��
//*********************

void UninitBuffer(LPDIRECT3DVERTEXBUFFER9 &Buffer)
{
	if (Buffer != NULL)
	{
		Buffer->Release();
		Buffer = NULL;
	}
}
//***************
// ���b�V���̔j��
//***************

void UninitMesh(LPD3DXMESH &Mesh)
{
	//���b�V���̔j��
	if (Mesh != NULL)
	{
		Mesh->Release();
		Mesh = NULL;
	}
}
void UninitBuffMat(LPD3DXBUFFER &Buffer)
{
	//�}�e���A���̔j��
	if (Buffer != NULL)
	{
		Buffer->Release();
		Buffer = NULL;
	}
}
//***************************
// �C���f�b�N�X�o�b�t�@�̔j��
//***************************
void UninitIndxBuffer(LPDIRECT3DINDEXBUFFER9 &IndxBuffer)
{
	if (IndxBuffer != NULL)
	{
		IndxBuffer->Release();
		IndxBuffer = NULL;
	}
}

//********************
// 1�����x�N�g��������
//********************
float Vector(float StartPoint, float EndPoint)
{
	return EndPoint - StartPoint;
}

//*************************
// 2D�x�N�g���̊p�x�����߂�
//*************************
float Angle(D3DXVECTOR2 Vec)
{
	return atan2f(Vec.x, Vec.y);
}

//*********************
// �p�x�̋ߓ���������
//*********************
void NearRot(float Angle, float& OutAngle)
{
	// �p�x�̋ߓ�
	if (Angle >= D3DX_PI) SetAngle(OutAngle += D3DX_PI * 2.0f,OutAngle);
	// �p�x�̋ߓ�
	else if (Angle <= -D3DX_PI) SetAngle(OutAngle -= D3DX_PI * 2.0f,OutAngle);
}

//*****************************
// ���݂̌�����ݒ�
//*****************************
void SetAngle(float fAngle, float& OutAngle)
{
	OutAngle = fAngle;
}