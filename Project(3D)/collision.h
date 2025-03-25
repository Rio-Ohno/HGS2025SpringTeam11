//*************************************
//
// 当たり判定をまとめたh[collision.h]
// Author Kensaku Hatori
//
//*************************************
#ifndef COLLISION_H_
#define COLLISION_H_
#include "main.h"
#include "model.h"

// 立方体の当たり判定に必要な構造体
typedef struct
{
	D3DXVECTOR3 FacePos[4];	// 面の中心ポス
	D3DXVECTOR3 Nor[4];		// 面の法線
}BoxCollision;

bool collisionboxtopoint(D3DXVECTOR3 Pos,BoxCollision BoxInfo);							// 点と矩形の当たり判定
bool collisioncircle(D3DXVECTOR3 Pos, float fRadius, D3DXVECTOR3 Pos1, float fRadius1);	// 球と球の当たり判定
bool collisionobb(OBB obb,OBB obb1,D3DXVECTOR3 Pos,D3DXVECTOR3 Pos1);					// OBBとOBBの当たり判定
D3DXVECTOR3 collisionobbfacedot(OBB obb, D3DXVECTOR3 Pos, D3DXVECTOR3 VecMove);			// OBBの当たった面の法線をリターンする関数
void PushPosition(D3DXVECTOR3 *Pos,D3DXVECTOR3 VecMove,D3DXVECTOR3 Nor);				// 壁刷り関数
bool CollisionRaytoObb(D3DXVECTOR3& rayOrigin, D3DXVECTOR3& rayDirection, OBB Obb);		// れいとおBB
#endif // !COLLISION_H_