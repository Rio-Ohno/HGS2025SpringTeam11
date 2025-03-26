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
#define MAX_SIZE (400.0f)                                                       //ポリゴンの最大(対角線)
#define MIN_SIZE (50.0f)                                                        //ポリゴンの最小(対角線)
#define NUM_WIDTH (60)											                //ポリゴンの幅
#define NUM_HEIGHT (20)	                                                        //ポリゴンの高さ
#define PLAYERLIFE (1000.0f)													//プレイヤーのライフ

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

//============================================================================================================================================
//デバック用
//============================================================================================================================================
int GetNumBullet();
#endif // !_BULLET_H