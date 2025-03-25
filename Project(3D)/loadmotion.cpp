////*********************************************
//// 
//// モーション情報を読み込む処理[loadmotion.cpp]
//// Hatori Kensaku
////
////*********************************************
//#include "loadmotion.h"
//#include "enemy.h"
//#include "player.h"
//#include "boss.h"
//
//// グローバル変数
//LoadInfo g_LoadInfo[LOADTYPE_MAX];
//int nType;
//
////*************************************
//// モーションを読み込む用の変数の初期化
////*************************************
//void InitMotion()
//{
//	nType = 0;
//	for (int LoadCount = 0; LoadCount < LOADTYPE_MAX; LoadCount++)
//	{
//		g_LoadInfo[LoadCount] = {NULL};
//	}
//}
//
////*****************************
//// スクリプト以前を読み込む処理
////*****************************
//void LoadMotionViewer(char* cMotionPath)
//{
//	char cData[2] = { NULL };
//	char cData1[64] = { NULL };
//	FILE* pFile = fopen((const char*)cMotionPath, "r");
//
//	if (pFile != NULL)
//	{
//		while (1)
//		{
//			fgets(cData, 2, pFile);
//			if (*cData != '#')
//			{
//				strcat(cData1, cData);
//				if (strcmp(&cData1[0], "SCRIPT") == 0)
//				{
//					cData1[0] = { NULL };
//					SkipComment(pFile);
//					break;
//				}
//			}
//			else
//			{
//				cData1[0] = { NULL };
//				SkipComment(pFile);
//			}
//		}
//		LoadMotionStart(pFile);
//		fclose(pFile);
//	}
//}
//
////*****************************
//// スクリプト以降を読み込む処理
////*****************************
//void LoadMotionStart(FILE* pFile)
//{
//	char cData[2] = { NULL };
//	char cData1[128] = { NULL };
//	char* cData2[32] = { NULL };
//	int nData;
//	char* ModelPath[32] = { NULL };
//	int CharactorCount, PartsCount,PathCount;
//	CharactorCount = 0;
//	PartsCount = 0;
//	PathCount = 0;
//	int MotionCount, KeyCount, KeyPartsCount;
//	MotionCount = 0;
//	KeyCount = 0;
//	KeyPartsCount = 0;
//	MODELINFO ModelInfo[MAX_PARTS];
//	MOTIONINFO MotionInfo[MOTIONTYPE_MAX];
//
//	while (1)
//	{
//		fgets(cData, 2, pFile);
//		if (cData[0] != 0x0a && cData[0] != '#' && cData[0] != ' ' && cData[0] != 0x09)
//		{
//			strcat(cData1, cData);
//			if (strcmp(&cData1[0], "NUM_MODEL") == 0)
//			{
//				Player* pPlayer = GetPlayer();
//				cData1[0] = { NULL };
//				SkipEqual(pFile);
//				nData = LoadInt(pFile);
//				g_LoadInfo[nType].nNumParts = nData;
//			}
//			else if (strcmp(&cData1[0], "MODEL_FILENAME") == 0)
//			{
//				cData1[0] = { NULL };
//				SkipEqual(pFile);
//				ModelPath[0] = LoadPath(pFile,ModelPath[0]);
//				strcpy(&g_LoadInfo[nType].cPartsPath[g_LoadInfo[nType].PathCount][0], ModelPath[0]);
//				g_LoadInfo[nType].PathCount++;
//			}
//			else if (strcmp(&cData1[0], "CHARACTERSET") == 0)
//			{
//				g_LoadInfo[nType].PathCount = 0;
//				cData1[0] = { NULL };
//				LoadCharactorInfo(pFile,&CharactorCount,&PartsCount,&ModelInfo[0]);
//			}
//			else if (strcmp(&cData1[0], "MOTIONSET") == 0)
//			{
//				cData1[0] = { NULL };
//				LoadMotionInfo(pFile,&MotionCount,&KeyCount,&KeyPartsCount,&MotionInfo[MotionCount]);
//				g_LoadInfo[nType].MotionCount++;
//				MotionCount++;
//			}
//			else if (strcmp(&cData1[0], "END_SCRIPT") == 0)
//			{
//				switch (nType)
//				{
//				case LOADTYPE_PLAYER:
//					SetPartsInfo(g_LoadInfo[nType]);
//					break;
//				case LOADTYPE_ENEMYONE:
//					SetEnemyPartsInfo(g_LoadInfo[nType], nType);
//					break;
//				case LOADTYPE_ENEMYTWO:
//					SetEnemyPartsInfo(g_LoadInfo[nType], nType);
//					break;
//				case LOADTYPE_ENEMYTHREE:
//					SetEnemyPartsInfo(g_LoadInfo[nType], nType);
//					break;
//				case LOADTYPE_ENEMYFOUR:
//					SetBossPartsInfo(g_LoadInfo[nType]);
//					break;
//				default:
//					break;
//				}
//				g_LoadInfo[nType].MotionCount = 0;
//				nType++;
//				MotionCount = 0;
//				break;
//			}
//		}
//		else
//		{
//			cData1[0] = { NULL };
//			if (cData[0] == '#')
//			{
//				SkipComment(pFile);
//			}
//		}
//	}
//}
//
////*************************************
//// キャラクター情報を読み込む処理
////*************************************
//void LoadCharactorInfo(FILE* pFile,int *nCharactor,int *nParts,MODELINFO *ModelInfo)
//{
//	char cData[2] = { NULL };
//	char cData1[128] = { NULL };
//	char cData2[128] = { NULL };
//	char *cData3[64] = { NULL };
//	float fData;
//	int nData;
//
//	while (1)
//	{
//		fgets(cData, 2, pFile);
//		if (cData[0] == 0x20 || cData[0] == 0x09 || cData[0] == 0x23 || cData[0] == 0x0a)
//		{
//			strcat(cData2, cData);
//			if (*cData == '#')
//			{
//				SkipComment(pFile);
//			}
//		}
//		else
//		{
//			strcat(cData1, cData);
//			if (strcmp(&cData1[0], "NUM_PARTS") == 0)
//			{
//				cData1[0] = { NULL };
//				SkipEqual(pFile);
//				nData = LoadInt(pFile);
//				g_LoadInfo[nType].nNumParts = nData;
//			}
//			else if (strcmp(&cData1[0], "MOVE") == 0)
//			{
//				cData1[0] = { NULL };
//				SkipEqual(pFile);
//				fData = LoadFloat(pFile);
//			}
//			else if (strcmp(&cData1[0], "JUMP") == 0)
//			{
//				cData1[0] = { NULL };
//				SkipEqual(pFile);
//				fData = LoadFloat(pFile);
//			}
//			else if (strcmp(&cData1[0], "RADIUS") == 0)
//			{
//				cData1[0] = { NULL };
//				SkipEqual(pFile);
//				fData = LoadFloat(pFile);
//			}
//			else if (strcmp(&cData1[0], "HEIGHT") == 0)
//			{
//				cData1[0] = { NULL };
//				SkipEqual(pFile);
//				fData = LoadFloat(pFile);
//			}
//			else if (strcmp(&cData1[0], "PARTSSET") == 0)
//			{
//				cData1[0] = { NULL };
//				LoadPartsInfo(pFile,nParts);
//				g_LoadInfo[nType].PartsCount++;
//			}
//			else if (strcmp(&cData1[0], "END_CHARACTERSET") == 0)
//			{
//				g_LoadInfo[nType].PartsCount = 0;
//				break;
//			}
//		}
//	}
//}
//
////*************************
//// パーツ情報を読み込む処理
////*************************
//void LoadPartsInfo(FILE* pFile,int *Parts)
//{
//	MODELINFO ModelInfo[MAX_PARTS];
//	char cData[2] = { NULL };
//	char cData1[128] = { NULL };
//	char cData2[64] = { NULL };
//
//	while (1)
//	{
//		fgets(cData, 2, pFile);
//		if (cData[0] == 0x20 || cData[0] == 0x09 || cData[0] == 0x23 || cData[0] == 0x0a)
//		{
//			strcat(cData2, cData);
//			if (*cData == '#')
//			{
//				SkipComment(pFile);
//			}
//		}
//		else
//		{
//			strcat(cData1, cData);
//			if (strcmp(&cData1[0], "INDEX") == 0)
//			{
//				cData1[0] = { NULL };
//				SkipEqual(pFile);
//				g_LoadInfo[nType].PartsInfo[g_LoadInfo[nType].PartsCount].nIndx = LoadInt(pFile);
//			}
//			else if (strcmp(&cData1[0], "PARENT") == 0)
//			{
//				cData1[0] = { NULL };
//				SkipEqual(pFile);
//				g_LoadInfo[nType].PartsInfo[g_LoadInfo[nType].PartsCount].Parent = LoadInt(pFile);
//			}
//			else if (strcmp(&cData1[0], "POS") == 0)
//			{
//				cData1[0] = { NULL };
//				SkipEqual(pFile);
//				g_LoadInfo[nType].PartsInfo[g_LoadInfo[nType].PartsCount].pos.x = LoadFloat(pFile);
//				g_LoadInfo[nType].PartsInfo[g_LoadInfo[nType].PartsCount].pos.y = LoadFloat(pFile);
//				g_LoadInfo[nType].PartsInfo[g_LoadInfo[nType].PartsCount].pos.z = LoadFloat(pFile);
//				g_LoadInfo[nType].PartsInfo[g_LoadInfo[nType].PartsCount].OffSetPos = g_LoadInfo[nType].PartsInfo[g_LoadInfo[nType].PartsCount].pos;
//			}
//			else if (strcmp(&cData1[0], "ROT") == 0)
//			{
//				cData1[0] = { NULL };
//				SkipEqual(pFile);
//				g_LoadInfo[nType].PartsInfo[g_LoadInfo[nType].PartsCount].rot.x = LoadFloat(pFile);
//				g_LoadInfo[nType].PartsInfo[g_LoadInfo[nType].PartsCount].rot.y = LoadFloat(pFile);
//				g_LoadInfo[nType].PartsInfo[g_LoadInfo[nType].PartsCount].rot.z = LoadFloat(pFile);
//				g_LoadInfo[nType].PartsInfo[g_LoadInfo[nType].PartsCount].OffSetRot = g_LoadInfo[nType].PartsInfo[g_LoadInfo[nType].PartsCount].rot;
//			}
//			else if (strcmp(&cData1[0], "END_PARTSSET") == 0)
//			{
//				break;
//			}
//		}
//	}
//}
//
////*************************************
//// モーション情報を読み込む処理
////*************************************
//void LoadMotionInfo(FILE* pFile,int *Motion,int *Key, int* nKeyParts,MOTIONINFO *MotionInfo)
//{
//	char cData[2] = { NULL };
//	char cData1[128] = { NULL };
//	char cData2[64] = { NULL };
//	char *cData3[64] = { NULL };
//	float fData;
//	int nData;
//
//	while (1)
//	{
//		fgets(cData, 2, pFile);
//		if (cData[0] == 0x20 || cData[0] == 0x09 || cData[0] == 0x23 || cData[0] == 0x0a)
//		{
//			strcat(cData2, cData);
//			if (*cData == '#')
//			{
//				SkipComment(pFile);
//			}
//		}
//		else
//		{
//			strcat(cData1, cData);
//			if (strcmp(&cData1[0], "LOOP") == 0)
//			{
//				cData1[0] = { NULL };
//				SkipEqual(pFile);
//				g_LoadInfo[nType].MotionInfo[g_LoadInfo[nType].MotionCount].bLoop = (bool)LoadInt(pFile);
//			}
//			else if (strcmp(&cData1[0], "NUM_KEY") == 0)
//			{
//				cData1[0] = { NULL };
//				SkipEqual(pFile);
//				g_LoadInfo[nType].MotionInfo[g_LoadInfo[nType].MotionCount].nNumKey = LoadInt(pFile);
//			}
//			else if (strcmp(&cData1[0], "FOOTSTEP") == 0)
//			{
//				cData1[0] = { NULL };
//				SkipEqual(pFile);
//				nData = LoadInt(pFile);
//				nData = LoadInt(pFile);
//				fData = LoadFloat(pFile);
//				fData = LoadFloat(pFile);
//				fData = LoadFloat(pFile);
//				nData = LoadInt(pFile);
//				nData = LoadInt(pFile);
//				fData = LoadFloat(pFile);
//				fData = LoadFloat(pFile);
//				fData = LoadFloat(pFile);
//				fData = LoadFloat(pFile);
//			}
//			else if (strcmp(&cData1[0], "SHOT") == 0)
//			{
//				cData1[0] = { NULL };
//				SkipEqual(pFile);
//				nData = LoadInt(pFile);
//				fData = LoadFloat(pFile);
//				fData = LoadFloat(pFile);
//				fData = LoadFloat(pFile);
//				nData = LoadInt(pFile);
//				nData = LoadInt(pFile);
//				nData = LoadInt(pFile);
//				fData = LoadFloat(pFile);
//			}
//			else if (strcmp(&cData1[0], "SMOKE") == 0)
//			{
//				cData1[0] = { NULL };
//				SkipEqual(pFile);
//				nData = LoadInt(pFile);
//				fData = LoadFloat(pFile);
//				fData = LoadFloat(pFile);
//				fData = LoadFloat(pFile);
//				nData = LoadInt(pFile);
//				nData = LoadInt(pFile);
//				nData = LoadInt(pFile);
//				nData = LoadInt(pFile);
//			}
//			else if (strcmp(&cData1[0], "KEYSET") == 0)
//			{
//				cData1[0] = { NULL };
//				LoadKeyInfo(pFile,Key, nKeyParts, MotionInfo);
//				g_LoadInfo[nType].KeyCount++;
//			}
//			else if (strcmp(&cData1[0], "END_MOTIONSET") == 0)
//			{
//				g_LoadInfo[nType].KeyCount = 0;
//				break;
//			}
//		}
//	}
//}
//
////*****************************
//// キー情報を読み込む処理
////*****************************
//void LoadKeyInfo(FILE* pFile, int* nKey, int* nKeyParts, MOTIONINFO* Motion)
//{
//	char cData[2] = { NULL };
//	char cData1[128] = { NULL };
//	char cData2[64] = { NULL };
//	char *cData3[64] = { NULL };
//
//	while (1)
//	{
//		fgets(cData, 2, pFile);
//		if (cData[0] == 0x20 || cData[0] == 0x09 || cData[0] == 0x23 || cData[0] == 0x0a)
//		{
//			strcat(cData2, cData);
//			if (*cData == '#')
//			{
//				SkipComment(pFile);
//			}
//		}
//		else
//		{
//			strcat(cData1, cData);
//			if (strcmp(&cData1[0], "FRAME") == 0)
//			{
//				cData1[0] = { NULL };
//				SkipEqual(pFile);
//				g_LoadInfo[nType].MotionInfo[g_LoadInfo[nType].MotionCount].aKeyInfo[g_LoadInfo[nType].KeyCount].nFrame = LoadInt(pFile);
//			}
//			else if (strcmp(&cData1[0], "KEY") == 0)
//			{
//				cData1[0] = { NULL };
//				LoadKeyPartsInfo(pFile,nKey, nKeyParts, Motion);
//				g_LoadInfo[nType].KeyPartsCount++;
//			}
//			else if (strcmp(&cData1[0], "END_KEYSET") == 0)
//			{
//				g_LoadInfo[nType].KeyPartsCount = 0;
//				break;
//			}
//		}
//	}
//}
//
////***********************************
//// パーツごとのキー情報を読み込む処理
////***********************************
//void LoadKeyPartsInfo(FILE* pFile,int *nKey,int* nKeyParts,MOTIONINFO *MotionInfo)
//{
//	char cData[2] = { NULL };
//	char cData1[128] = { NULL };
//	char cData2[64] = { NULL };
//
//	while (1)
//	{
//		fgets(cData, 2, pFile);
//		if (cData[0] == 0x20 || cData[0] == 0x09 || cData[0] == 0x23 || cData[0] == 0x0a)
//		{
//			strcat(cData2, cData);
//			if (*cData == '#')
//			{
//				SkipComment(pFile);
//			}
//		}
//		else
//		{
//			strcat(cData1, cData);
//			if (strcmp(&cData1[0], "POS") == 0)
//			{
//				cData1[0] = { NULL };
//				SkipEqual(pFile);
//				g_LoadInfo[nType].MotionInfo[g_LoadInfo[nType].MotionCount].aKeyInfo[g_LoadInfo[nType].KeyCount].aKey[g_LoadInfo[nType].KeyPartsCount].fPosX = LoadFloat(pFile);
//				g_LoadInfo[nType].MotionInfo[g_LoadInfo[nType].MotionCount].aKeyInfo[g_LoadInfo[nType].KeyCount].aKey[g_LoadInfo[nType].KeyPartsCount].fPosY = LoadFloat(pFile);
//				g_LoadInfo[nType].MotionInfo[g_LoadInfo[nType].MotionCount].aKeyInfo[g_LoadInfo[nType].KeyCount].aKey[g_LoadInfo[nType].KeyPartsCount].fPosZ = LoadFloat(pFile);
//			}
//			else if (strcmp(&cData1[0], "ROT") == 0)
//			{
//				cData1[0] = { NULL };
//				SkipEqual(pFile);
//				g_LoadInfo[nType].MotionInfo[g_LoadInfo[nType].MotionCount].aKeyInfo[g_LoadInfo[nType].KeyCount].aKey[g_LoadInfo[nType].KeyPartsCount].fRotX = LoadFloat(pFile);
//				g_LoadInfo[nType].MotionInfo[g_LoadInfo[nType].MotionCount].aKeyInfo[g_LoadInfo[nType].KeyCount].aKey[g_LoadInfo[nType].KeyPartsCount].fRotY = LoadFloat(pFile);
//				g_LoadInfo[nType].MotionInfo[g_LoadInfo[nType].MotionCount].aKeyInfo[g_LoadInfo[nType].KeyCount].aKey[g_LoadInfo[nType].KeyPartsCount].fRotZ = LoadFloat(pFile);
//			}
//			else if (strcmp(&cData1[0], "END_KEY") == 0)
//			{
//				break;
//			}
//		}
//	}
//}