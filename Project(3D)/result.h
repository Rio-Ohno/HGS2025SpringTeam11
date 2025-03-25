//==============================
//
//  ���U���g�\������[result.h]
//  Author:kaiti
//
//==============================
#ifndef _RESULT_H_
#define _RESULT_H_

#include"main.h"

typedef enum
{
	RESULT_NONE = 0,
	RESULT_CLEAR,
	RESULT_GAMEOVER,
	RESULT_MAX,
}RESULT;

//�v���g�^�C�v�錾
void InitResult(MODE OldMode);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);

#endif
