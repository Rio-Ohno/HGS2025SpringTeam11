//==================================================================
//
//�X�e�[�W����<stage.h>
//Author:Saeki Takuto
//
//==================================================================
#ifndef _STAGE_H_
#define _STAGE_H_

#include "main.h"

#define MAX_STAGE (6)
#define MAX_WORD (4096)

typedef struct
{
	D3DXVECTOR3 pos;//�ʒu;
	D3DXVECTOR3 move;//�ʒu;
	float fWidth;//����
	float fHeight;//�c��
	int nType;
}Stage;

//�v���g�^�C�v�錾
void InitStage(void);
void UninitStage(void);
void UpdateStage(void);
void LoadStage(int nStage);

#endif
