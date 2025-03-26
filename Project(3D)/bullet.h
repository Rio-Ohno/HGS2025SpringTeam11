//============================================================
//
//�V���[�e�B���O�Q�[[bullet.h]
//Author:Rio Ohno
//
//============================================================

#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

//�e�̃��[�U�[
typedef enum
{
	BULLETUSER_NULL=0,
	BULLETUSER_PLAYER,
	BULLETUSER_ENEMY,
	BULLETUSER_MAX,
}BULLETUSER;

//�e�̎��
typedef enum
{
	BULLETTYPE_NULL=0,
	BULLETTYPE_HOMING,
	BULLETTYPE_RADIAL,
	BULLETTYPE_RAIN,
	BULLETTYPE_MAX,
}BULLETTYPE;

//�v���C���[�̒e�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;                                       //�ʒu
	D3DXVECTOR3 oldpos;                                    //�O�̈ʒu
	D3DXVECTOR3 move;                                      //�ړ���
	D3DXVECTOR3 rot;                                       //����
	BULLETUSER user;								       //�N���g�Ă邩
	BULLETTYPE type;                                       //���
	float fLengthBullet;                                   //�|���S���̑Ίp���̒���
	float fAngleBullet;                                    //�|���S���̊p�x
	float fLife;                                           //����
	bool bUse;                                             //�g�p���Ă��邩�ǂ�
}Bullet;

//�v���g�^�C�v�錾
void InitBullet();
void UninitBullet();
void UpdateBullet();
void DrawBullet();
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, float fLength,float nLife,BULLETUSER user,BULLETTYPE type);
void ReflectBullet(int indx);

//============================================================================================================================================
//�f�o�b�N�p
//============================================================================================================================================
int GetNumBullet();
#endif // !_BULLET_H