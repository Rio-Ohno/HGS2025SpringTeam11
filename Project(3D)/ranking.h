//==============================
//
//  ランキング処理[ranking.h]
//  Author:kaiti
//
//==============================
#ifndef _RANKING_H_
#define _RANKING_H_

#include"main.h"

#define MAX_RANKING (5) //表示順位数
#define RANKING_TIME (600)//ランキングの時間
#define RANKSCORE_TIME (25)

#define RANK_SIZE (40)

typedef enum
{
	RANKMODE_RESULT = 0,
	RANKMODE_TITLE,
	RANKMODE_SELECT,
	RANKMODE_MAX,
}RANK_MODE;

//プロトタイプ宣言
void InitRanking(void);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(void);
void ResetRanking(void);
void SetRanking(int nScore);
void SetRankMode(RANK_MODE mode);
void MoveRanking(void);
void Ranking(void);
#endif