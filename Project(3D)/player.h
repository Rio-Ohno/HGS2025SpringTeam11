//============================================================
//
//�V���[�e�B���O�Q�[[player.h]
//Author:Rio Ohno
//
//============================================================

#ifndef _Player_H_
#define _Player_H_

#include"main.h"

//�v���C���[�̏��
typedef enum
{
	PLAYERSTATE_NORMAL = 0,								//�ʏ펞
	PLAYERSTATE_DAMAGE,									//�_���[�W���
	PLAYERSTATE_APPEAR,									//�������̖��G���
	PLAYERSTATE_DEATH,									//���S���
	PLAYERSTATE_MAX
}PLAYERSTATE;

//�v���C���[�̍\����`
typedef struct
{
	D3DXVECTOR3 pos;                                    //�ʒu
	D3DXVECTOR3 move;                                   //�ړ���
	D3DXVECTOR3 rot;                                    //����
	PLAYERSTATE state;                                  //�v���C���[�̏��
	int nCountState;                                    //��ԊǗ��J�E���^�[
	int nCntPlayer;										//�v���C���[�̎c�@��
	float fLength;                                      //�Ίp���̒���
	float fAngle;                                       //�Ίp���̊p�x
	float fLife;                                        //�̗�
	bool bUse;                                          //�g�p���Ă��邩�ǂ���
	bool bDisp;											//�`�悷�邩�ǂ���
}Player;

//�v���g�^�C�v�錾
void InitPlayer();
void UninitPlayer();
void UpdatePlayer();
void DrawPlayer();
void HitPlayer(float fDamage);
void StatePlayer();
void ActionPlayer();
Player* GetPlayer();

void InitPos();
void UninitPos();
void UpdatePos();
void DrawPos();

#endif // !_Player_H_
