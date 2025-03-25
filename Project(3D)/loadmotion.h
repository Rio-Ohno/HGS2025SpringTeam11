//********************************************
//
// ���[�V��������ǂݍ��ޏ���[loadmotion.h]
// Hatori Kensaku
//
//*********************************************
#ifndef LOADMOTION_H_
#define LOADMOTION_H_

#include "main.h"
//#include "loadstage.h"
#include "key.h"
#include "model.h"

#define MOTIONFILE_PLAYER  ("data\\MOTION\\motion00.txt")//�v���C���[�̃��[�V�����t�@�C��
#define MOTIONFILE_ENEMYONE  ("data\\MOTION\\Enemymotion00.txt")//�v���C���[�̃��[�V�����t�@�C��

// �ǂݍ��ރe�L�X�g�̎��
typedef enum
{
	LOADTYPE_PLAYER = 0,
	LOADTYPE_ENEMYONE,
	LOADTYPE_ENEMYTWO,
	LOADTYPE_ENEMYTHREE,
	LOADTYPE_ENEMYFOUR,
	LOADTYPE_MAX
}LoadType;

// �ǂݍ��ނ��߂̏����i�[����\����
typedef struct
{
	MODELINFO PartsInfo[MAX_PARTS];
	MOTIONINFO MotionInfo[MOTIONTYPE_MAX];
	char cPartsPath[MAX_PARTS][256];
	int nNumParts;
	int PathCount,PartsCount,MotionCount,KeyCount,KeyPartsCount;
}LoadInfo;

void InitMotion();
void LoadMotionViewer(char *cMotionPath);	// �X�N���v�g�ȑO��ǂݍ��ޏ���
void LoadMotionStart(FILE* pFile);			// �X�N���v�g�ȍ~�̓ǂݍ��ݏ���

void LoadCharactorInfo(FILE* pFile,int *nCharactor, int* nParts, MODELINFO*Model);				// �L�����N�^�[����ǂݍ��ޏ���
void LoadPartsInfo(FILE* pFile,int *nParts);													// �p�[�c����ǂݍ��ޏ���
void LoadMotionInfo(FILE* pFile,int *nMotion, int* nKey, int* nKeyParts,MOTIONINFO *Motion);	// ���[�V��������ǂݍ��ޏ���
void LoadKeyInfo(FILE* pFile, int* nKey, int* nKeyParts, MOTIONINFO* Motion);					// �L�[����ǂݍ��ޏ���
void LoadKeyPartsInfo(FILE* pFile,int *nKey,int *nKeyParts, MOTIONINFO* Motion);				// �p�[�c���Ƃ̃L�[����ǂݍ��ޏ���
#endif // !LOADMOTION_H_