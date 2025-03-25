//==============================
//
//  �Q�[����ʕ\������[game.h]
//  Author:kaiti
//
//==============================
#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

#define STAGE_SIZE (float)(945.0f)

//�Q�[���̏��
typedef enum
{
	GAMESTATE_NONE = 0,	//�������Ă��Ȃ����
	GAMESTATE_NORMAL,	//�ʏ��ԁi�Q�[���v���C���j
	GAMESTATE_PAUSE,	//
	GAMESTATE_OPTION,	//
	GAMESTATE_CLEAR,	//�N���A
	GAMESTATE_GAMEOVER,	//�Q�[���I�[�o�[
	GAMESTATE_EFFECTEDITER,	//�Q�[���I�[�o�[
	GAMESTATE_CAMERAEDITER,
	GAMESTATE_MAX
}GAMESTATE;

//�X�e�[�W
typedef enum
{
	STAGE_NO_0 = 0,
	STAGE_NO_1,
	STAGE_NO_2,
	STAGE_NO_3,
	STAGE_NO_MAX,
}STAGE_NO;

//�v���g�^�C�v�錾
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(void);
void SetGameState(GAMESTATE state);
GAMESTATE GetGameSatate(void);
void SetEnablePause(bool bPause);
void SetEnableTutorial(bool bTutorial);
void SetEnableOption(bool bOption);

#endif