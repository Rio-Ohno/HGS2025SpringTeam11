
//=====================================
//
// ステータス[status.h]
// Author:kaiti
//
//=====================================
#ifndef STATUS_H_
#define STATUS_H_

#define GRAVITY (float)(0.3f)

typedef struct
{
	float fHP;
	float fPower;
	int nMP;
	float fSpeed;
}PlayerStatus;

typedef struct
{
	float fHP;
	float fPower;
	float fSpeed;
	int Score;
}EnemyStatus;

typedef struct
{
	float fHP;
	float fPower;
	float fSpeed;
	int Score;
}BossStatus;
#endif // !STATUS_H_