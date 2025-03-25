//=============================================================================
//
// サウンド処理 [sound.h]
// Author : 
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンド一覧
//*****************************************************************************
typedef enum
{
	//BGM
	SOUND_LABEL_GAME = 0,
	SOUND_LABEL_GAMECLEAR,
	SOUND_LABEL_GAMEOVER,
	SOUND_LABEL_RANKING,
	SOUND_LABEL_TITLE,
	SOUND_LABEL_BOSS,
	SOUND_LABEL_BOSSMOVIE,

	//SE
	SOUND_LABEL_DESICION,
	SOUND_LABEL_MAGIC,
	SOUND_LABEL_MAGIC1,
	SOUND_LABEL_POTION,
	SOUND_LABEL_SELECT,
	SOUND_LABEL_SKELETON,
	SOUND_LABEL_SLASH,
	SOUND_LABEL_ZOMBI,
	SOUND_LABEL_EXPROSION,
	SOUND_LABEL_ROLLING,
	SOUND_LABEL_LOCKON,
	SOUND_LABEL_ITEMGET,
	SOUND_LABEL_MAHOUJIN,
	SOUND_LABEL_ZENMETU,
	SOUND_LABEL_STAGECHANGE,

	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);
bool SetMasterVolume(float Volume);
bool SetSourceVolume(float Volume, SOUND_LABEL label);
#endif