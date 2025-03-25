//================================
//
// �e�\������[shadow.h]
// Author:kaiti
//
//================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"

#define MAX_SHADOW (128)		//�e�̍ő吔
#define LIMI_SIZE (0.25f)		//�T�C�Y�̉���(���{��)

//�e�̍\����
typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	D3DXVECTOR3 rot;			//����
	D3DXMATRIX mtxWorld;		//���[���h�}�g���b�N�X
	LPDIRECT3DVERTEXBUFFER9 pVtxBuffShadow;//���_���
	float fRadius;				//���a
	bool bUse;					//�g�p���Ă��邩�ǂ���
}Shadow;

//�v���g�^�C�v�錾
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
void DrwaShadowPlayer(int Indx,D3DXMATRIX mtxWorld);
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot,float fRadius);					//�e�̏����ʒu�A�����A���a�̐ݒ�
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos,bool bUse);				//�e�̈ʒu�X�V
void SetSizeShadow(D3DXVECTOR3 pos,int nIndx);						//�e�̃T�C�Y�X�V
Shadow* GetShadow(void);
#endif