//********************************************
//
// モーション情報を読み込む処理[loadmotion.h]
// Hatori Kensaku
//
//*********************************************
#ifndef LOADMOTION_H_
#define LOADMOTION_H_

#include "main.h"
//#include "loadstage.h"
#include "key.h"
#include "model.h"

#define MOTIONFILE_PLAYER  ("data\\MOTION\\motion00.txt")//プレイヤーのモーションファイル
#define MOTIONFILE_ENEMYONE  ("data\\MOTION\\Enemymotion00.txt")//プレイヤーのモーションファイル

// 読み込むテキストの種類
typedef enum
{
	LOADTYPE_PLAYER = 0,
	LOADTYPE_ENEMYONE,
	LOADTYPE_ENEMYTWO,
	LOADTYPE_ENEMYTHREE,
	LOADTYPE_ENEMYFOUR,
	LOADTYPE_MAX
}LoadType;

// 読み込むための情報を格納する構造体
typedef struct
{
	MODELINFO PartsInfo[MAX_PARTS];
	MOTIONINFO MotionInfo[MOTIONTYPE_MAX];
	char cPartsPath[MAX_PARTS][256];
	int nNumParts;
	int PathCount,PartsCount,MotionCount,KeyCount,KeyPartsCount;
}LoadInfo;

void InitMotion();
void LoadMotionViewer(char *cMotionPath);	// スクリプト以前を読み込む処理
void LoadMotionStart(FILE* pFile);			// スクリプト以降の読み込み処理

void LoadCharactorInfo(FILE* pFile,int *nCharactor, int* nParts, MODELINFO*Model);				// キャラクター情報を読み込む処理
void LoadPartsInfo(FILE* pFile,int *nParts);													// パーツ情報を読み込む処理
void LoadMotionInfo(FILE* pFile,int *nMotion, int* nKey, int* nKeyParts,MOTIONINFO *Motion);	// モーション情報を読み込む処理
void LoadKeyInfo(FILE* pFile, int* nKey, int* nKeyParts, MOTIONINFO* Motion);					// キー情報を読み込む処理
void LoadKeyPartsInfo(FILE* pFile,int *nKey,int *nKeyParts, MOTIONINFO* Motion);				// パーツごとのキー情報を読み込む処理
#endif // !LOADMOTION_H_