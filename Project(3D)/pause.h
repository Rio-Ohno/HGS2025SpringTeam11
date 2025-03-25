//==============================
//
//  �|�[�Y����[pause.h]
//  Author:kaiti
//
//==============================
#ifndef _PAUSE_H_
#define _PAUSE_H_

#include "main.h"
#define PAUSESELECT_HEIGHT (100)
#define PAUSESELECT_WIDTH (400)

//�|�[�Y���j���[
typedef enum
{
	PAUSE_MENU_OPTION=0,//�߂�
	PAUSE_MENU_CONTNUE,//�߂�
	PAUSE_MENU_RETRY,    //��蒼��
	PAUSE_MENU_QUIT,     //�^�C�g���ɖ߂�
	PAUSE_MENU_MAX,
}PAUSE_MENU;

//�v���g�^�C�v�錾
void InitPause(void);
void UninitPause(void);
void UpdatePause(int zDelta);
void DrawPause(void);
#endif