//================================
//
//�@�E�C���h�E�\������[main.cpp]
//�@Author:kaiti
//
//================================
#include "main.h"
#include "input.h"
#include "fade.h"
#include "title.h"
#include "result.h"
#include "game.h"
#include "ranking.h"
#include "sound.h"
#include "camera.h"
//#include "player.h"
#include "mouse.h"
#include "titleinfo.h"
#include "pause.h"
#include <crtdbg.h>
#include <stdio.h>

//�O���[�o���ϐ��錾
LPDIRECT3D9 g_pD3D = NULL;
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;
MODE g_mode = MODE_TITLE;
LPD3DXFONT g_pFont;
RECT g_windowRect; // �E�B���h�E�؂�ւ��ϐ�
bool bDispFont = true;
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

	case WM_MOUSEWHEEL:
	{
		GAMESTATE state = GetGameSatate();
		int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
		//SetMouseWheel(zDelta);
		//�g�p���閂�@�̎�ނ�ύX
		if (g_mode == MODE_TITLE)
		{
			UpdateTitleInfo(zDelta);
		}
		if(state == GAMESTATE_PAUSE)
		{
			UpdatePause(zDelta);
		}
	}
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

	if (FAILED(InitMouse(hInstance, hWnd)))
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
	//
	InitRanking();
	//�t�F�[�h
	InitFade(g_mode);

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

	UninitMouse();

	UninitJoypad();

	//
	UninitFade();

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
	UpdateMouse();
	UpdateKeyboard();
	UpdateJoypad();

	switch (g_mode)
	{
	case MODE_TITLE:
		UpdateTitle();
		break;
	case MODE_STAGEONE:
		UpdateGame();
		break;
	case MODE_STAGETWO:
		UpdateGame();
		break;
	case MODE_STAGETHREE:
		UpdateGame();
		break;
	case MODE_BOSSMOVIE:
		break;
	case MODE_STAGEFOUR:
		UpdateGame();
		break;
	case MODE_RESULT:
		UpdateResult();
		break;
	case MODE_RANK:
		UpdateRanking();
		break;
	default:
		break;
	}
	//
	UpdateFade();

#ifdef _DEBUG
	//���C���[
	if (GetKeyboardPress(DIK_F1) == true)
	{
		onWireFrame();
	}
	else if (GetKeyboardPress(DIK_F2) == true)
	{
		offWireFrame();
	}

	//�f�o�b�N�t�H���g�̕\����\��
	if (KeyboardTrigger(DIK_F4) == true)
	{
		bDispFont = bDispFont ? false : true;
	}

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
		//-----------------------------
		//�e��I�u�W�F�N�g�̕`�揈��
		//-----------------------------
		switch (g_mode)
		{
		case MODE_TITLE:
			DrawTitle();
			break;
		case MODE_STAGEONE:
			DrawGame();
			break;
		case MODE_STAGETWO:
			DrawGame();
			break;
		case MODE_STAGETHREE:
			DrawGame();
			break;
		case MODE_STAGEFOUR:
			DrawGame();
			break;
		case MODE_BOSSMOVIE:
			break;
		case MODE_RESULT:
			DrawResult();
			break;
		case MODE_RANK:
			DrawRanking();
			break;
		default:
			break;
		}

#ifdef _DEBUG

		if (bDispFont == true)
		{
			DrawPlayerCollision();
			//DrawEffectEditer();
			DrawCameraInfo();
			//DrawPlayerInfo();
			//DrawTestInfo();
			//DrawBossInfo();
		}

#endif // DEBUG

		DrawFade();

		//�`��I��
		g_pD3DDevice->EndScene();
	}
	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//==============
// ���[�h�̐ݒ�
//==============
void SetMode(MODE mode)
{
	//���݂̉�ʂ̏I������
	switch (g_mode)
	{
	case MODE_TITLE:
		UninitTitle();
		break;
	case MODE_STAGEONE:
		UninitGame();
		break;
	case MODE_STAGETWO:
		UninitGame();
		break;
	case MODE_STAGETHREE:
		UninitGame();
		break;
	case MODE_STAGEFOUR:
		UninitGame();
		break;
	case MODE_BOSSMOVIE:
		break;
	case MODE_RESULT:
		UninitResult();
		break;
	case MODE_RANK:
		UninitRanking();
		break;
	default:
		break;
	}
	MODE OldMode = g_mode;
	g_mode = mode;

	//�V������ʂ̏���������
	switch (mode)
	{
	case MODE_TITLE:
		InitTitle();
		break;
	case MODE_STAGEONE:
		InitGame();
		break;
	case MODE_STAGETWO:
		InitGame();
		break;
	case MODE_STAGETHREE:
		InitGame();
		break;
	case MODE_STAGEFOUR:
		InitGame();
		break;
	case MODE_BOSSMOVIE:
		break;
	case MODE_RESULT:
		InitResult(OldMode);
		break;
	case MODE_RANK:
		InitRanking();
		break;
	case MODE_END:
		break;
	default:
		break;
	}
}

//=============
// ���[�h�擾
//=============
MODE GetMode(void)
{
	return g_mode;
}

//===========================
//�v���C���[�̓����蔻��\��
//===========================
void DrawPlayerCollision()
{
	//PARTICLEEDITER* pEditer = GetParticleInfo();
	//RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	//char aStr[256];
	//Player* pPlayer = GetPlayer();

	//// ������ɑ��
	//sprintf(&aStr[0], "�������Ă��邩:%d\n�v���C���[�̃|�X:%3.2f,%3.2f,%3.2f",pPlayer->btest,pPlayer->pos.x, pPlayer->pos.y, pPlayer->pos.z);

	//// �e�L�X�g�\��
	//g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(200, 255, 0, 255));
}

////============================
////�G�t�F�N�g�G�f�B�^�[�̕\��
////============================
//void DrawEffectEditer()
//{
//	PARTICLEEDITER* pEditer = GetParticleInfo();
//	RECT rect = { 0,40,SCREEN_WIDTH,SCREEN_HEIGHT };
//	char aStr[256];
//
//	// ������ɑ��
//	sprintf(&aStr[0], "*****************PARTICLEEDITER*****************\n�J���[�ύX:R,G,B\n���ˊp�ύX:X,Y,Z\n���q�̐��ύX:L\n�����ύX:H\n���ꂼ��SHIFT�����Ȃ�����ƌ���");
//
//	// �e�L�X�g�\��
//	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(200, 255, 0, 255));
//
//	rect = { 0,160,SCREEN_WIDTH,SCREEN_HEIGHT };
//
//	// ������ɑ��
//	sprintf(&aStr[0], "R,G,B:%3.2f,%3.2f,%3.2f\nX,Y,Z:%3.2f,%3.2f,%3.2f\n�G�t�F�N�g�̍ő吔:%d\n�G�t�F�N�g�̎���:%d\n���q�̑傫��X,Y,Z:%3.2f,%3.2f,%3.2f", pEditer->ParticleInfo.col.r,
//		pEditer->ParticleInfo.col.g, 
//		pEditer->ParticleInfo.col.b,
//		pEditer->ParticleInfo.dir.x,
//		pEditer->ParticleInfo.dir.y, 
//		pEditer->ParticleInfo.dir.z,
//		pEditer->ParticleInfo.forlimit,
//		pEditer->ParticleInfo.nLife,
//		pEditer->ParticleInfo.Scale.x,
//		pEditer->ParticleInfo.Scale.y, 
//		pEditer->ParticleInfo.Scale.z);
//
//	// �e�L�X�g�\��
//	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(200, 255, 0, 255));
//}

//==================
//�J�����̏��\��
//==================
void DrawCameraInfo()
{
	Camera* pCamera = GetCamera();

	RECT rect = { 0,260,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	// ������ɑ��
	sprintf(&aStr[0], "*****************�J�������*****************");

	// �e�L�X�g�\��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(200, 255, 0, 255));

	rect = { 0,280,SCREEN_WIDTH,SCREEN_HEIGHT };

	// ������ɑ��
	sprintf(&aStr[0], "�J�����̎��_�̌���:%3.2f,%3.2f,%3.2f", pCamera->rot.x,
		pCamera->rot.y,
		pCamera->rot.z);

	// �e�L�X�g�\��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(200, 255, 0, 255));



}

////======================
////�v���C���[�̏��\��
////======================
//void DrawPlayerInfo()
//{
//	Player* pPlayer = GetPlayer();
//
//	RECT rect = { 0,300,SCREEN_WIDTH,SCREEN_HEIGHT };
//	char aStr[256];
//
//	// ������ɑ��
//	sprintf(&aStr[0], "*****************�v���C���[���*****************");
//
//	// �e�L�X�g�\��
//	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(200, 255, 0, 255));
//
//	rect = { 0,320,SCREEN_WIDTH,SCREEN_HEIGHT };
//
//	// ������ɑ��
//	sprintf(&aStr[0], "�v���C���[�̌���:%3.2f,%3.2f,%3.2f", pPlayer->rot.x,
//		pPlayer->rot.y,
//		pPlayer->rot.z);
//
//	// �e�L�X�g�\��
//	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(200, 255, 0, 255));
//
//	rect = { 0,340,SCREEN_WIDTH,SCREEN_HEIGHT };
//
//	// ������ɑ��
//	sprintf(&aStr[0], "�v���C���[�̖ڕW�̌���:%3.2f,%3.2f,%3.2f", pPlayer->rotDest.x,
//		pPlayer->rotDest.y,
//		pPlayer->rotDest.z);
//
//	// �e�L�X�g�\��
//	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(200, 255, 0, 255));
//
//	rect = { 0,360,SCREEN_WIDTH,SCREEN_HEIGHT };
//
//	// ������ɑ��
//	sprintf(&aStr[0], "�v���C���[�̈ʒu:%3.2f,%3.2f,%3.2f", pPlayer->pos.x,
//		pPlayer->pos.y,
//		pPlayer->pos.z);
//
//	// �e�L�X�g�\��
//	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(200, 255, 0, 255));
//
//	rect = { 0,380,SCREEN_WIDTH,SCREEN_HEIGHT };
//
//	// ������ɑ��
//	sprintf(&aStr[0], "�v���C���[�̐̂̈ʒu:%3.2f,%3.2f,%3.2f", pPlayer->posOld.x,
//		pPlayer->posOld.y,
//		pPlayer->posOld.z);
//
//	// �e�L�X�g�\��
//	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(200, 255, 0, 255));
//
//	rect = { 0,400,SCREEN_WIDTH,SCREEN_HEIGHT };
//
//	//// ������ɑ��
//	//sprintf(&aStr[0], "�v���C���[�̎��̃L�[:%d", pPlayer->PlayerMotion.NextKey);
//
//		// ������ɑ��
//	sprintf(&aStr[0], "�v���C���[�̃X�e�[�^�X:%d", pPlayer->state);
//
//	// �e�L�X�g�\��
//	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(200, 255, 0, 255));
//
//	rect = { 0,420,SCREEN_WIDTH,SCREEN_HEIGHT };
//
//	// ������ɑ��
//	sprintf(&aStr[0], "�v���C���[�̍��̃��[�V����:%d", pPlayer->PlayerMotion.motionType);
//
//	// �e�L�X�g�\��
//	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(200, 255, 0, 255));
//}
//void DrawTestInfo()
//{
//	D3DXVECTOR2 test = Gettest();
//	RECT rect = { 0,440,SCREEN_WIDTH,SCREEN_HEIGHT };
//	char aStr[256];
//	int *Num = GetNumEnemy();
//	// ������ɑ��
//	sprintf(&aStr[0], "�G�̑���:%d", *Num);
//
//	// �e�L�X�g�\��
//	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(200, 255, 0, 255));
//}
//void DrawBossInfo()
//{
//	BOSS* pBoss = GetBoss();
//	D3DXVECTOR2 test = Gettest();
//	RECT rect = { 0,460,SCREEN_WIDTH,SCREEN_HEIGHT };
//	char aStr[256];
//
//	// ������ɑ��
//	sprintf(&aStr[0], "�{�X�̗D��x:%3.2f,%3.2f,%3.2f", pBoss->BossAi.primary[0], 
//		pBoss->BossAi.primary[1],
//		pBoss->BossAi.primary[2]);
//
//	// �e�L�X�g�\��
//	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(200, 255, 0, 255));
//
//	rect = { 0,480,SCREEN_WIDTH,SCREEN_HEIGHT };
//
//	// ������ɑ��
//	sprintf(&aStr[0], "�{�X�̍s���p�^�[��:%d", pBoss->ActionType);
//
//	// �e�L�X�g�\��
//	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(200, 255, 0, 255));
//}
//=============
// ���C���[
//=============
void onWireFrame()
{
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
}
void offWireFrame()
{
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

//*******************************
// ���W�A���l����PI�ɕϊ����鏈��
//*******************************
D3DXVECTOR3* PitoRadian(D3DXVECTOR3* rot)
{
	// ���W�A���l����PI�ɕϊ�
	rot->x = rot->x * TOPI;
	rot->y = rot->y * TOPI;
	rot->z = rot->z * TOPI;

	// �p�x�̐��K��(X)
	if (rot->x >= D3DX_PI)
	{
		rot->x -= D3DX_PI * 2.0f;
	}
	else if (rot->x <= -D3DX_PI)
	{
		rot->x += D3DX_PI * 2.0f;
	}
	// �p�x�̐��K��(Y)
	if (rot->y >= D3DX_PI)
	{
		rot->y -= D3DX_PI * 2.0f;
	}
	else if (rot->y <= -D3DX_PI)
	{
		rot->y += D3DX_PI * 2.0f;
	}
	// �p�x�̐��K��(Z)
	if (rot->z >= D3DX_PI)
	{
		rot->z -= D3DX_PI * 2.0f;
	}
	else if (rot->z <= -D3DX_PI)
	{
		rot->z += D3DX_PI * 2.0f;
	}

	return rot;
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