//================================
//
//  �Q�[����ʕ\������[game.cpp]
//  Author:kaiti
//
//================================

#include "game.h"
#include "result.h"
#include "camera.h"
#include "light.h"
#include "input.h"
//#include "player.h"
#include "shadow.h"
#include "meshwall.h"
#include "meshfield.h"
#include "fade.h"
#include "pause.h"
#include "sound.h"
#include <time.h>
#include "mouse.h"
#include "meshsphere.h"
#include "score.h"
#include <cassert>
#include "resultinfo.h"

bool isState(GAMESTATE State);
bool isUpdateGameCondition();

//void UpdateEditer();

//�O���[�o���ϐ�
GAMESTATE g_gamestate = GAMESTATE_NONE;
int g_nCounterGameState = 0;
bool g_bPause = false;  //�|�[�Y�����ǂ���
bool bAbo = false;//�S�Ńt���O
bool g_bTutorial = true;
bool g_bOption = false;

//=============
// ����������
//=============
void InitGame(void)
{
	MODE Mode = GetMode();
	//���b�V���t�B�[���h�̏�����
	InitMeshfield();
	SetMeshfield(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 10, 0, 10, 100, 100);

	// ���̂̏�����
	InitSphere();

	//�e�̏�����
	InitShadow();

	//���b�V���ǂ̏�����
	InitMeshWall();

	//�J�����̏�����
	InitCamera();

	//���C�g�̏�����
	InitLight();

	//�|�[�Y�̏�����
	InitPause();

	// ���̂̐ݒ菈��
	SetSphere(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, 16, 5, 2000.0f, false, true);

	////�v���C���[�̏��擾
	//Player* pPlayer = GetPlayer();

	//�e������
	g_gamestate = GAMESTATE_NORMAL;		//�Q�[���X�e�[�g
	g_nCounterGameState = 0;			//�X�e�[�g�J�E���^�[
	srand((int)time(0));				//�V�[�h�l(�A�C�e��rand)
	g_bPause = false;					//�|�[�Y���Ă��Ȃ���Ԃ�
}

//===========
// �I������
//===========
void UninitGame(void)
{
	//�|�[�Y�̏I������
	UninitPause();

	//�e�̏I������
	UninitShadow();

	//���b�V���ǂ̏I������
	UninitMeshWall();

	//�J�����̏I������
	UninitCamera();

	//���C�g�̏I������
	UninitLight();

	//���b�V���t�B�[���h�̏I������
	UninitMeshfield();

	UninitSphere();
}

//===========
// �X�V����
//===========
void UpdateGame(void)
{
	MODE Mode = GetMode();
	//Option* pOption = GetOption();

	//if (SetMasterVolume(pOption->Sound) == false)
	//{
	//	HWND hWnd = GetActiveWindow();
	//	MessageBox(hWnd, "�}�X�^�[�{�����[���̐ݒ�Ɏ��s", "�x���I", MB_ICONWARNING);
	//}

	if ((KeyboardTrigger(DIK_P) == true || GetJoypadTrigger(JOYKEY_START) == true) && g_bOption == false)
	{//�|�[�Y�L�[�������ꂽ
		//PlaySound(SOUND_LABEL_DESICION);
		g_bPause = g_bPause ? false : true;
	}

	if (isUpdateGameCondition() == true)
	{
		if (g_bPause == true)
		{//�|�[�Y��
			//�|�[�Y�̍X�V����
			g_gamestate = GAMESTATE_PAUSE;
			UpdatePause(0);
			g_bTutorial = false;
		}

		else if (g_bPause == false && g_bTutorial == false && g_bOption == false)
		{
			//���b�V���t�B�[���h�̍X�V����
			UpdateMeshfield();

			UpdateSphere();

			//�e�̍X�V����
			UpdateShadow();


#ifdef _DEBUG

			//���U���g�ɔ��
			if (KeyboardTrigger(DIK_1) == true)
			{//Clear
				SetFade(MODE_RESULT);
				SetResult(RESULT_CLEAR);
			}
			if (KeyboardTrigger(DIK_2) == true)
			{//over
				SetFade(MODE_RESULT);
				SetResult(RESULT_GAMEOVER);
			}

#endif // DEBUG

			//���b�V���ǂ̍X�V����
			UpdateMeshWall();

			//�J�����̍X�V����
			UpdateCamera();

			UpdateGameCamera();

			//���C�g�̍X�V����
			UpdateLight();

			//�G�̐����擾����
			MODE mode = GetMode();

			////�S�Ă�wave���I������Ȃ�
			//if (GetFinish() == true)
			//{
			//	g_gamestate = GAMESTATE_CLEAR;
			//}

			//�Q�[���X�e�[�g�Ǘ�
			switch (g_gamestate)
			{
			case GAMESTATE_NORMAL:

				break;

			case GAMESTATE_CLEAR:

				g_nCounterGameState++;
				if (g_nCounterGameState >= 30)
				{
					g_gamestate = GAMESTATE_NONE;
					//���[�h�����U���g�ɂ���
					SetFade(MODE_RESULT);
					SetResult(RESULT_CLEAR);

					//if (pPlayer->Status.fHP == PLAYER_HP)
					//{
					//	AddScore(10000);
					//}
					//else if (pPlayer->Status.fHP >= PLAYER_HP / 2)
					//{
					//	AddScore(5000);
					//}

					StopSound();
					//PlaySound(SOUND_LABEL_GAMECLEAR);
				}
				break;

			case GAMESTATE_GAMEOVER:

				g_nCounterGameState++;
				if (g_nCounterGameState >= 40)
				{
					g_gamestate = GAMESTATE_NONE;
					//���[�h�����U���g�ɂ���
					SetFade(MODE_RESULT);
					SetResult(RESULT_GAMEOVER);

					StopSound();
					//PlaySound(SOUND_LABEL_GAMEOVER);
				}
				break;

			case GAMESTATE_EFFECTEDITER:

				break;
			}
		}
	}
}

//===========
// �`�揈��
//===========
void DrawGame(void)
{
	//�J�����̐ݒ菈��
	SetCamera();

	//���b�V���t�B�[���h�̕`�揈��
	DrawMeshfield();

	DrawSphere();

	DrawSphere();

	//�e�̕`�揈��
	DrawShadow();

	//���b�V���ǂ̕`�揈��
	DrawMeshWall();

	//�|�[�Y���Ă���Ȃ�
	if (g_bPause == true)
	{//�|�[�Y��
		//�|�[�Y�̕`�揈��
		DrawPause();
	}
}

//===============
// �Q�[���̏��
//===============
void SetGameState(GAMESTATE state)
{
	g_gamestate = state;
}

//=====================
// �Q�[���̏�Ԃ̎擾
//=====================
GAMESTATE GetGameSatate(void)
{
	return g_gamestate;
}

//=======================
// �|�[�Y�̗L�������ݒ�
//=======================
void SetEnablePause(bool bPause)
{
	g_bPause = bPause;
}

//
//
//
void SetEnableTutorial(bool bTutorial)
{
	g_bTutorial = bTutorial;
}

bool isState(GAMESTATE State)
{
	return g_gamestate == State;
}

bool isUpdateGameCondition()
{
	return isState(GAMESTATE_EFFECTEDITER) == false && isState(GAMESTATE_CAMERAEDITER) == false;
}

//void UpdateEditer()
//{
//	if (isState(GAMESTATE_EFFECTEDITER) == true)
//	{
//		UpdateEffect();
//	}
//	else if (isState(GAMESTATE_CAMERAEDITER) == true)
//	{
//		UpdateCameraEditer();
//		CameraMove();
//	}
//}
//
//
//
void SetEnableOption(bool bOption)
{
	g_bOption = bOption;
}