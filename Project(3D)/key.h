#ifndef KEY_H_
#define KEY_H_

#define MAX_KEY (10)
#define MAX_MOTION (5)
#define MAX_ACTIONDIRECTION (3)

#include "main.h"
#include "model.h"

//���[�V�����̎��
typedef enum
{
	MOTIONTYPE_NEUTRAL = 0,
	MOTIONTYPE_MOVE,
	MOTIONTYPE_ACTION,
	MOTIONTYPE_JUMP,
	MOTIONTYPE_LANDING,

	MOTIONTYPE_LOCKON_R_MOVE,	// ���b�N�I�����̈ړ��@�E
	MOTIONTYPE_LOCKON_L_MOVE,	// ���b�N�I�����̈ړ��@��
	MOTIONTYPE_LOCKON_F_MOVE,	// ���b�N�I�����̈ړ��@�O��

	MOTIONTYPE_DOWN,			//�_�E��
	MOTIONTYPE_ACTION_EXPLOSION,//�U���i�ߋ����j
	MOTIONTYPE_ACTION_HORMING,	//�U���i�ǔ��j

	MOTIONTYPE_KAIHI_MAE,		//�O���
	MOTIONTYPE_KAIHI_USIRO,		//�����
	MOTIONTYPE_KAIHI_MIGI,		//�E���
	MOTIONTYPE_KAIHI_HIDARI,	//�����

	MOTIONTYPE_MAX
}MOTIONTYPE;

typedef struct
{
	float fPosX;
	float fPosY;
	float fPosZ;
	float fRotX;
	float fRotY;
	float fRotZ;
}KEY;

typedef struct
{
	int nFrame;
	KEY aKey[MAX_PARTS];
}KEYINFO;

typedef struct
{
	int nStartKey, nEndKey;
	int nStartFrame, nEndFrame;
	bool bActionStart,bFirst;
}ACTIONFRAME;

typedef struct
{
	bool bLoop;
	int nNumKey;
	KEYINFO aKeyInfo[MAX_KEY];
	ACTIONFRAME ActionFrameInfo[MAX_ACTIONDIRECTION];
}MOTIONINFO;

//�v���C���[�̍\����
typedef struct
{
	//���f��
	MODELINFO aModel[MAX_PARTS];
	int nNumModel;
	//���[�V����
	MOTIONINFO aMotionInfo[MOTIONTYPE_MAX];	// ���[�V�������
	int nNumMotion;							// ���[�V�����̑���
	MOTIONTYPE motionType;					// ���[�V�����̎��
	bool bLoopMotion;						// ���[�v���邩�ǂ���
	int nNumKey;							// �L�[�̑���
	int nKey;								// ���݂̃L�[No
	int NextKey;							// ���̃L�[
	int nCntMotion;							// ���[�V�����̃J�E���^�[

	bool bFinish;							// ���݂̃��[�V�������I�����Ă��邩�ǂ���
	bool bBlendMotion;						// �u�����h���[�V���������邩�ǂ���
	MOTIONTYPE motionTypeBlend;				// �u�����h���[�V�����̎��
	bool bLoopBlend;						// ���[�v���邩�ǂ���
	int nNumKeyBlend;						// �u�����h���[�V�����̃L�[�̑���
	int nKeyBlend;							// �u�����h���[�V�����̂̃L�[No
	int nNextKeyBlend;						// �u�����h���[�V�����̎��̃L�[
	int nFrameBlend;						// �u�����h�t���[��
	int nCntBlend;							// �u�����h�t���[���J�E���g
	int nCntMotionBlend;					// �u�����h���[�V�����̃J�E���^�[
}OBJECTINFO;

#endif // !KEY_H_