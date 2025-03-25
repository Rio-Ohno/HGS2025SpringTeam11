//*************************************
//
// �����蔻����܂Ƃ߂�h[collision.h]
// Author Kensaku Hatori
//
//*************************************
#ifndef COLLISION_H_
#define COLLISION_H_
#include "main.h"
#include "model.h"

// �����̂̓����蔻��ɕK�v�ȍ\����
typedef struct
{
	D3DXVECTOR3 FacePos[4];	// �ʂ̒��S�|�X
	D3DXVECTOR3 Nor[4];		// �ʂ̖@��
}BoxCollision;

bool collisionboxtopoint(D3DXVECTOR3 Pos,BoxCollision BoxInfo);							// �_�Ƌ�`�̓����蔻��
bool collisioncircle(D3DXVECTOR3 Pos, float fRadius, D3DXVECTOR3 Pos1, float fRadius1);	// ���Ƌ��̓����蔻��
bool collisionobb(OBB obb,OBB obb1,D3DXVECTOR3 Pos,D3DXVECTOR3 Pos1);					// OBB��OBB�̓����蔻��
D3DXVECTOR3 collisionobbfacedot(OBB obb, D3DXVECTOR3 Pos, D3DXVECTOR3 VecMove);			// OBB�̓��������ʂ̖@�������^�[������֐�
void PushPosition(D3DXVECTOR3 *Pos,D3DXVECTOR3 VecMove,D3DXVECTOR3 Nor);				// �Ǎ���֐�
bool CollisionRaytoObb(D3DXVECTOR3& rayOrigin, D3DXVECTOR3& rayDirection, OBB Obb);		// �ꂢ�Ƃ�BB
#endif // !COLLISION_H_