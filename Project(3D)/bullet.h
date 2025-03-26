//============================================================
//
//シューティングゲー[bullet.h]
//Author:Rio Ohno
//
//============================================================

#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

//マクロ定義
#define MAX_BULLET (256)                                   //弾の最大数
#define BULLET_WIDTH (10)                                  //ポリゴンの幅
#define BULLET_HEIGHT (10)                                 //ポリゴンの高さ
#define BULLET_SPEED ((float)2.0f)						   //弾のスピード

//弾のユーザー
typedef enum
{
	BULLETUSER_NULL=0,
	BULLETUSER_PLAYER,
	BULLETUSER_ENEMY,
	BULLETUSER_MAX,
}BULLETUSER;

//弾の種類
typedef enum
{
	BULLETTYPE_NULL=0,
	BULLETTYPE_HOMING,
	BULLETTYPE_RADIAL,
	BULLETTYPE_RAIN,
	BULLETTYPE_MAX,
}BULLETTYPE;

//プレイヤーの弾構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;                                       //位置
	D3DXVECTOR3 oldpos;                                    //前の位置
	D3DXVECTOR3 move;                                      //移動量
	D3DXVECTOR3 rot;                                       //向き
	float fLengthBullet;                                   //ポリゴンの対角線の長さ
	float fAngleBullet;                                    //ポリゴンの角度
	float fLife;                                           //寿命
	bool bUse;                                             //使用しているかどう
}Bullet;

//プロトタイプ宣言
void InitBullet();
void UninitBullet();
void UpdateBullet();
void DrawBullet();
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fLength,float nLife);
void ReflectBullet(int indx);
Bullet* GetBullet();

//============================================================================================================================================
//デバック用
//============================================================================================================================================
int GetNumBullet();
#endif // !_BULLET_H