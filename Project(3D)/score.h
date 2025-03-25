//==============================
//
//  スコア処理[score.h]
//  Author:kaiti
//
//==============================
#ifndef _SCORE_H_
#define _SCORE_H_

#include"main.h"

#define MAX_SCORE (6)
#define SCOREDATA (100000)
#define SCORE_SIZE (20)
#define SCORE_SPEED (60)

//プロトタイプ宣言
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void SetScore(int nScore);
void AddScore(int nValue);
int GetScore(void);

#endif#pragma once
