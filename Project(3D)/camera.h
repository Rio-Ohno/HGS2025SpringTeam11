//==========================
//
// �J�������� [camera.h]
// Author: Asuma Nishio
//
//==========================
#ifndef _CAMERA_H_
#define _CAMERA_H_

//****************************
// �C���N���[�h�t�@�C���錾
//****************************
#include "main.h"

//****************************
// �J�����񋓌^���`
//****************************
typedef enum
{
	CAMERAMODE_NONE = 0,
	CAMERAMODE_MAX
}CAMERAMODE;

//****************************
// �J�����\���̂��`
//****************************
typedef struct
{
	D3DXVECTOR3 posV;			// ���_
	D3DXVECTOR3 posR;			// �����_
	D3DXVECTOR3 vecU;			// ������x�N�g��
	D3DXMATRIX mtxProjection;	// �v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX mtxView;			// �r���[�}�g���b�N�X
	D3DXVECTOR3 rot;			// �p�x
	float fDistance;			// ���_���璍���_�܂ł̋���
	D3DXVECTOR3 posVDest;		// �ړI�̎��_
	D3DXVECTOR3 posRDest;		// �ړI�̒����_
	CAMERAMODE g_CameraMode;
	D3DVIEWPORT9 viewport;		// �r���[�|�[�g
}Camera;

//**********************
// �v���g�^�C�v�錾
//**********************
void InitCamera(void);		// �J�����̏�����
void UninitCamera(void);	// �J�����̏I��
void UpdateCamera(void);	// �J�����̍X�V
void SetCamera(void);		// �J�����̐ݒ�
Camera* GetCamera();		// �J�����̎擾
void StickCamera(void);		// �E�X�e�B�b�N�̃J��������
void MouseWheel(int zdelta);
void MouseView(void);       // �Q�[���̎��̃}�E�X�̎��_�ړ�
void MouseEditMode(void); // �ҏW���[�h�̎��̃}�E�X�ړ�

#endif