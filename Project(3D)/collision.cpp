//*************************************
//
// �����蔻����܂Ƃ߂�cpp[collision.h]
// Author Kensaku Hatori
//
//*************************************
#include "collision.h"
#include <algorithm>

//*********************
// �_�Ƌ�`�̓����蔻��
//*********************
bool collisionboxtopoint(D3DXVECTOR3 Pos, BoxCollision BoxInfo)
{
	for (int FaceCount = 0; FaceCount < 4; FaceCount++)
	{
		if (BoxInfo.Nor[FaceCount].x > 1.0f ||
			BoxInfo.Nor[FaceCount].y > 1.0f ||
			BoxInfo.Nor[FaceCount].z > 1.0f)
		{
			D3DXVec3Normalize(&BoxInfo.Nor[FaceCount], &BoxInfo.Nor[FaceCount]);
		}
		D3DXVECTOR3 pVec = BoxInfo.FacePos[FaceCount] - Pos;
		float fDot = D3DXVec3Dot(&pVec, &BoxInfo.Nor[FaceCount]);
		if (fDot < 0)
		{
			return true;
		}
	}
	return false;
}
//*******************
// ���Ƌ��̓����蔻��
//*******************
bool collisioncircle(D3DXVECTOR3 Pos, float fRadius, D3DXVECTOR3 Pos1, float fRadius1)
{
	D3DXVECTOR3 Math = Pos - Pos1;
	float fDistance = D3DXVec3Length(&Math);
	float fMathRadius = fRadius + fRadius1;
	fMathRadius = fMathRadius * fMathRadius;
	if (fDistance < fMathRadius)
	{
		return true;
	}
	return false;
}
//*********************
// OBB��OBB�̓����蔻��
//*********************
bool collisionobb(OBB obb, OBB obb1, D3DXVECTOR3 Pos, D3DXVECTOR3 Pos1)
{
	// �e�����x�N�g���̊m��
   // �iN***:�W���������x�N�g���j
	D3DXVECTOR3 NAe1 = obb.RotVec[0], Ae1 = NAe1 * obb.fLength[0];	// X��
	D3DXVECTOR3 NAe2 = obb.RotVec[1], Ae2 = NAe2 * obb.fLength[1];	// Y��
	D3DXVECTOR3 NAe3 = obb.RotVec[2], Ae3 = NAe3 * obb.fLength[2];	// Z��
	D3DXVECTOR3 NBe1 = obb1.RotVec[0], Be1 = NBe1 * obb1.fLength[0];
	D3DXVECTOR3 NBe2 = obb1.RotVec[1], Be2 = NBe2 * obb1.fLength[1];
	D3DXVECTOR3 NBe3 = obb1.RotVec[2], Be3 = NBe3 * obb1.fLength[2];

	D3DXVECTOR3 r;
	D3DXVECTOR3 MathX;
	D3DXVECTOR3 MathY;
	D3DXVECTOR3 MathZ;
	D3DXVECTOR3 MathPos;
	MathPos = Pos - Pos1;
	MathX = obb.RotVec[0] * obb.fLength[0];
	MathY = obb.RotVec[1] * obb.fLength[1];
	MathZ = obb.RotVec[2] * obb.fLength[2];
	r.x = (float)fabs(D3DXVec3Dot(&MathX, &NBe1));
	r.y = (float)fabs(D3DXVec3Dot(&MathY, &NBe2));
	r.z = (float)fabs(D3DXVec3Dot(&MathZ, &NBe3));

	FLOAT sX = D3DXVec3Dot(&MathPos, &NBe1);
	FLOAT sY = D3DXVec3Dot(&MathPos, &NBe2);
	FLOAT sZ = D3DXVec3Dot(&MathPos, &NBe3);

	D3DXVECTOR3 Interval = MathPos;
	bool bCollision = true;

	// ������ : Ae1
	FLOAT rA = D3DXVec3Length(&Ae1);
	FLOAT rB = LenSegOnSeparateAxis(&NAe1, &Be1, &Be2, &Be3);
	FLOAT L = (float)fabs(D3DXVec3Dot(&Interval, &NAe1));
	if (L > rA + rB)
	{
		return false;
	}

	// ������ : Ae2
	rA = D3DXVec3Length(&Ae2);
	rB = LenSegOnSeparateAxis(&NAe2, &Be1, &Be2, &Be3);
	L = (float)fabs(D3DXVec3Dot(&Interval, &NAe2));
	if (L > rA + rB)
	{
		return false;
	}
	// ������ : Ae3
	rA = D3DXVec3Length(&Ae3);
	rB = LenSegOnSeparateAxis(&NAe3, &Be1, &Be2, &Be3);
	L = (float)fabs(D3DXVec3Dot(&Interval, &NAe3));
	if (L > rA + rB)
	{
		return false;
	}

	// ������ : Be1
	rA = LenSegOnSeparateAxis(&NBe1, &Ae1, &Ae2, &Ae3);
	rB = D3DXVec3Length(&Be1);
	L = (float)fabs(D3DXVec3Dot(&Interval, &NBe1));
	if (L > rA + rB)
	{
		return false;
	}

	// ������ : Be2
	rA = LenSegOnSeparateAxis(&NBe2, &Ae1, &Ae2, &Ae3);
	rB = D3DXVec3Length(&Be2);
	L = (float)fabs(D3DXVec3Dot(&Interval, &NBe2));
	if (L > rA + rB)
	{
		return false;
	}

	// ������ : Be3
	rA = LenSegOnSeparateAxis(&NBe3, &Ae1, &Ae2, &Ae3);
	rB = D3DXVec3Length(&Be3);
	L = (float)fabs(D3DXVec3Dot(&Interval, &NBe3));
	if (L > rA + rB)
	{
		return false;
	}

	// ������ : C11
	D3DXVECTOR3 Cross;
	D3DXVec3Cross(&Cross, &NAe1, &NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
	{
		return false;
	}

	// ������ : C12
	D3DXVec3Cross(&Cross, &NAe1, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
	{
		return false;
	}

	// ������ : C13
	D3DXVec3Cross(&Cross, &NAe1, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
	{
		return false;
	}

	// ������ : C21
	D3DXVec3Cross(&Cross, &NAe2, &NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
	{
		return false;
	}

	// ������ : C22
	D3DXVec3Cross(&Cross, &NAe2, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
	{
		return false;
	}

	// ������ : C23
	D3DXVec3Cross(&Cross, &NAe2, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
	{
		return false;
	}

	// ������ : C31
	D3DXVec3Cross(&Cross, &NAe3, &NBe1);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
	{
		return false;
	}

	// ������ : C32
	D3DXVec3Cross(&Cross, &NAe3, &NBe2);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
	{
		return false;
	}

	// ������ : C33
	D3DXVec3Cross(&Cross, &NAe3, &NBe3);
	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
	if (L > rA + rB)
	{
		return false;
	}
	return true;
}
//**************************************************
// OBB�Ƃ̓����蔻��œ��������ʂ̖M�D���擾���鏈��
//**************************************************
D3DXVECTOR3 collisionobbfacedot(OBB obb, D3DXVECTOR3 Pos, D3DXVECTOR3 VecMove)
{
	// X���̖ʂɓ������Ă��邩
	D3DXVECTOR3 IntervalX;
	D3DXVECTOR3 norX;
	FLOAT fDotX;
	D3DXVECTOR3 IntervalX1;
	D3DXVECTOR3 norX1;
	D3DXVECTOR3 Math;
	FLOAT fDotX1;

	// X���̃}�C�i�X�����̃x�N�g��
	Math = -obb.RotVec[0];
	// X���̕����x�N�g��(�@��)
	D3DXVec3Normalize(&norX, &obb.RotVec[0]);
	// X���̃}�C�i�X�����̖@���x�N�g��
	D3DXVec3Normalize(&norX1, &Math);

	// �v���C���[�ۂ�����X���̖ʂ̒��S�|�X���������l
	IntervalX = Pos - obb.CenterPos +
		(norX * obb.fLength[0]);
	// �v���C���[�̃|�X����X���̃}�C�i�X�����̒��S�|�X���������l
	IntervalX1 = Pos - obb.CenterPos +
		(norX1 * obb.fLength[0]);

	// ����(�}�C�i�X���Ɠ������Ă���)
	fDotX = D3DXVec3Dot(&IntervalX, &norX);
	// ����(�}�C�i�X���Ɠ������Ă���)
	fDotX1 = D3DXVec3Dot(&IntervalX1, &norX1);

	// Y���̖ʂɓ������Ă��邩
	D3DXVECTOR3 IntervalY;
	D3DXVECTOR3 norY;
	FLOAT fDotY;
	D3DXVECTOR3 IntervalY1;
	D3DXVECTOR3 norY1;
	D3DXVECTOR3 MathY;
	FLOAT fDotY1;

	// Y���̃}�C�i�X�����̃x�N�g��
	MathY = obb.RotVec[1] * -1.0f;
	// Y���̕����x�N�g��(�@��)
	D3DXVec3Normalize(&norY, &obb.RotVec[1]);
	// Y���̃}�C�i�X�����̃x�N�g��(�@��)
	D3DXVec3Normalize(&norY1, &MathY);

	// �v���C���[�ۂ�����Y���̖ʂ̒��S�|�X���������l
	IntervalY = Pos - obb.CenterPos +
		(norY * obb.fLength[1]);
	// �v���C���[�ۂ�����Y���̃}�C�i�X�����̖ʂ̒��S�|�X���������l
	IntervalY1 = Pos - obb.CenterPos +
		(norY1 * obb.fLength[1]);

	// ����(�}�C�i�X���Ɠ������Ă���)
	fDotY = D3DXVec3Dot(&IntervalY, &norY);
	// ����(�}�C�i�X���Ɠ������Ă���)
	fDotY1 = D3DXVec3Dot(&IntervalY1, &norY1);

	// Z���̖ʂɓ������Ă��邩
	D3DXVECTOR3 IntervalZ;
	D3DXVECTOR3 norZ;
	FLOAT fDotZ;
	D3DXVECTOR3 IntervalZ1;
	D3DXVECTOR3 norZ1;
	D3DXVECTOR3 MathZ;
	FLOAT fDotZ1;

	// Z���̃}�C�i�X�����̃x�N�g��
	MathZ = -obb.RotVec[2];
	// Z���̕����x�N�g��(�@��)
	D3DXVec3Normalize(&norZ, &obb.RotVec[2]);
	// Z���̃}�C�i�X�̕����x�N�g��(�@��)
	D3DXVec3Normalize(&norZ1, &MathZ);

	// �v���C���[�ۂ�����Z���̖ʂ̒��S�|�X���������l
	IntervalZ = Pos - obb.CenterPos +
		(norZ * obb.fLength[2]);
	// �v���C���[�ۂ�����Z���̃}�C�i�X�����̖ʂ̒��S�|�X���������l
	IntervalZ1 = Pos - obb.CenterPos +
		(norZ1 * obb.fLength[2]);

	// ����(�}�C�i�X���Ɠ������Ă���)
	fDotZ = D3DXVec3Dot(&IntervalZ, &norZ);
	// ����(�}�C�i�X���Ɠ������Ă���)
	fDotZ1 = D3DXVec3Dot(&IntervalZ1, &norZ1);

	if (fDotY >= 0)
	{
		if (fDotX < fDotX1 && fDotX < fDotZ && fDotX < fDotZ1)
		{
			D3DXVECTOR3 pVec = VecMove;
			D3DXVECTOR3 nor = norX;
			D3DXVec3Normalize(&nor, &nor);
			FLOAT osiete = D3DXVec3Dot(&pVec, &nor);
			D3DXVECTOR3 test1 = nor * D3DXVec3Dot(&pVec, &nor);
			if (osiete <= 0.0f)
			{
				return nor;
			}
		}
		if (fDotX1 < fDotX && fDotX1 < fDotZ && fDotX1 < fDotZ1)
		{
			D3DXVECTOR3 pVec = VecMove;
			D3DXVECTOR3 nor = norX1;
			D3DXVec3Normalize(&nor, &nor);
			FLOAT osiete = D3DXVec3Dot(&pVec, &nor);
			D3DXVECTOR3 test1 = nor * D3DXVec3Dot(&pVec, &nor);
			if (osiete <= 0.0f)
			{
				return nor;
			}
		}
		if (fDotZ < fDotZ1 && fDotZ <= fDotX && fDotZ <= fDotX1)
		{
			D3DXVECTOR3 pVec = VecMove;
			D3DXVECTOR3 nor = norZ;
			D3DXVec3Normalize(&nor, &nor);
			FLOAT osiete = D3DXVec3Dot(&pVec, &nor);
			D3DXVECTOR3 test1 = nor * D3DXVec3Dot(&pVec, &nor);
			if (osiete <= 0.0f)
			{
				return nor;
			}
		}
		if (fDotZ1 < fDotZ && fDotZ1 < fDotX && fDotZ1 < fDotX1)
		{
			D3DXVECTOR3 pVec = VecMove;
			D3DXVECTOR3 nor = norZ1;
			D3DXVec3Normalize(&nor, &nor);
			FLOAT osiete = D3DXVec3Dot(&pVec, &nor);
			D3DXVECTOR3 test1 = nor * D3DXVec3Dot(&pVec, &nor);
			if (osiete <= 0.0f)
			{
				return nor;
			}
		}
	}
	// �������
	if (fDotY <= 1.0f && fDotX >= -2.0f && fDotX1 >= -2.0f && fDotZ >= -2.0f && fDotZ1 >= -2.0f)
	{
		D3DXVECTOR3 pVec = VecMove;
		D3DXVECTOR3 nor = norY1;
		D3DXVec3Normalize(&nor, &nor);
		FLOAT osiete = D3DXVec3Dot(&pVec, &nor);
		D3DXVECTOR3 test1 = nor * D3DXVec3Dot(&pVec, &nor);
		if (osiete >= 0.0f)
		{
			return nor;
		}
	}
	// �����火
	if (fDotY1 <= 1.0f && fDotX >= -2.0f && fDotX1 >= -2.0f && fDotZ >= -2.0f && fDotZ1 >= -2.0f)
	{
		D3DXVECTOR3 pVec = VecMove;
		D3DXVECTOR3 nor = norY;
		D3DXVec3Normalize(&nor, &nor);
		FLOAT osiete = D3DXVec3Dot(&pVec, &nor);
		D3DXVECTOR3 test1 = nor * D3DXVec3Dot(&pVec, &nor);
		if (osiete >= 0.0f)
		{
			return nor;
		}
	}
	return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
//*************
// �����o���֐�
//*************
void PushPosition(D3DXVECTOR3* Pos, D3DXVECTOR3 VecMove, D3DXVECTOR3 Nor)
{
	if (Pos == NULL)
	{
		return;
	}
	D3DXVECTOR3 PushMove = Nor * D3DXVec3Dot(&VecMove, &Nor);
	*Pos += PushMove;
}
//**********************
// ���C��OBB�̓����蔻��
//**********************
bool CollisionRaytoObb(D3DXVECTOR3& rayOrigin, D3DXVECTOR3& rayDirection, OBB Obb)
{
	// OBB�̒��S�ʒu�Ɖ�]�����[�J�����W�n�ɕϊ�
	D3DXVECTOR3 LocalRayOrigin, LocalRayDir;
	D3DXMATRIX InvMtxWorld;
	D3DXMatrixInverse(&InvMtxWorld, nullptr, &Obb.CenterMtx);

	LocalRayOrigin = rayOrigin;
	LocalRayDir = rayDirection;
	FLOAT tMin = -FLT_MAX;
	FLOAT tMax = FLT_MAX;
	BOOL bHit = false;

	for (int i = 0; i < 3; ++i)
	{
		// OBB�̃��[�J�����W�n�̎��x�N�g���i��]���j
		D3DXVECTOR3 axis = Obb.RotVec[i]; // ���ꂪ���[���h��Ԃ̃x�N�g�����Ɖ���


		// ���C�̃I���W������OBB�̒��S�܂ł̃x�N�g���i���[�J����ԁj
		D3DXVECTOR3 test = LocalRayOrigin - Obb.CenterPos; // OBB�̒��S�ʒu�����[�J����Ԃɕϊ��ς݂ł��邱�Ƃ�O��


		// ���C�Ǝ��x�N�g���Ƃ̓���
		FLOAT e = D3DXVec3Dot(&axis, &test);
		FLOAT f = D3DXVec3Dot(&axis, &LocalRayDir);

		if (fabs(f) > 0.001f)
		{
			// t1, t2�̌v�Z�i�����_�j
			FLOAT t1 = (e + Obb.fLength[i] * 1.0f) / f;
			FLOAT t2 = (e - Obb.fLength[i] * 1.0f) / f;

			if (t1 > t2)
			{
				std::swap(t1, t2);
			}

			if (t1 > tMin)
			{
				tMin = t1;
			}
			if (t2 < tMax)
			{
				tMax = t2;
			}

			if (tMin > tMax)
			{
				return false;
			}
		}
		else
		{
			if (-e - Obb.fLength[i] * 1.0f > 0.0f || -e + Obb.fLength[i] * 1.0f < 0.0f)
			{
				return false;
			}
		}
	}
	return true;
}
//*********************
// ���C�Ƌ��̓����蔻��
//*********************
bool CollisionRaytoSphere(D3DXVECTOR3& rayOrigin, D3DXVECTOR3& rayDirection, D3DXVECTOR3 Pos, FLOAT Length, D3DXMATRIX mtxWorld)
{
	D3DXVECTOR3 RayPos = Pos - rayOrigin;
	D3DXVECTOR3 RayDirection;

	D3DXVec3TransformCoord(&RayPos, &RayPos, &mtxWorld);
	D3DXVec3TransformNormal(&RayDirection, &rayDirection, &mtxWorld);

	float A = RayDirection.x * RayDirection.x + RayDirection.y * RayDirection.y + RayDirection.z * RayDirection.z;
	float B = RayDirection.x * RayPos.x + RayDirection.y * RayPos.y + RayDirection.z * RayPos.z;
	float C = RayPos.x * RayPos.x + RayPos.y * RayPos.y + RayPos.z * RayPos.z - Length * Length;

	if (A == 0.0f)
	{
		return false; // ���C�̒�����0
	}

	float s = B * B - A * C;
	if (s < 0.0f)
	{
		return false; // �Փ˂��Ă��Ȃ�
	}

	s = sqrtf(s);
	float a1 = (B - s) / A;
	float a2 = (B + s) / A;

	if (a1 < 0.0f || a2 < 0.0f)
	{
		return false; // ���C�̔��΂ŏՓ�
	}

	return true;
}
