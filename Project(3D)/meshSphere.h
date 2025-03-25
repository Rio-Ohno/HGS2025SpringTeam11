//============================================================
//
//���b�V���X�t�B�A[meshSphere.h]
//Author:Rio Ohno
//
//============================================================

#ifndef _MESHSPHERE_H_
#define _MESHSPHERE_H_

#include "main.h"

#define MAX_MESHSPHERE (1)																						// ���̍ő吔
																												   
//���̍\����																									 �@ 
typedef struct																									   
{																												   
	D3DXVECTOR3 pos;																							// �ʒu
	D3DXVECTOR3 rot;																							// ����
	D3DXCOLOR col;																								// �F
	D3DXMATRIX mtxWorld;																						// ���[���h�}�g���b�N�X
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff;																			// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 pIndxBuff;																			// �C���f�N�X�o�b�t�@�ւ̃|�C���^
	int textype;																								// �e�N�X�`���̎��
	int DiviX;																									// ������
	int DiviY;																									// ������
	int nMaxVtx;																								// ���_�� 
	int nPolyNum;																								// �|���S����
	float  fRadius;																								// ���a
	bool bHalf;																									// �������ǂ���
	bool bHead;																									// �\���ǂ���
	bool bUse;																									// �g�p���Ă��邩�ǂ���
}MeshSphere;																									   
																												   
//�v���g�^�C�v�錾																								   
void InitSphere(void);																							// ����������
void UninitSphere(void);																						// �I������
void UpdateSphere(void);																						// �X�V����
void DrawSphere(void);																							// �`�揈��
int SetSphere(D3DXVECTOR3 pos, int textype, int DiviX, int DiviY, float fRadius, bool bHead, bool bHalf);		// �ݒ菈��
void SetSpherePos(int indx,D3DXVECTOR3 pos);																	// �ʒu�̐ݒ菈��
void SetSphereTexture(int indx);																				// �e�N�X�`���̐ݒ菈��
void DeleteSphere(int indx);																					// ��������
#endif // !_MESHSPHERE_H_
