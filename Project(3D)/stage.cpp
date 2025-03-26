//==================================================================
//
//ステージ処理<stage.cpp>
//Author:Saeki Takuto
//
//==================================================================
#include "stage.h"
#include "block.h"
#include "player.h"

Stage g_stage[MAX_STAGE];
int g_nowstage;

void InitStage(void)
{
	for (int nCnt = 0; nCnt < MAX_STAGE; nCnt++)
	{
		g_stage[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_stage[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_stage[nCnt].fWidth = 0.0f;
		g_stage[nCnt].fHeight = 0.0f;
		g_stage[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	g_nowstage = 0;
}

void UninitStage(void)
{
}

void UpdateStage(void)
{
}

void LoadStage(int nStage)
{
	FILE* pFile;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	float fWidth;
	float fHeight;
	int ntype;
	char stagedata[MAX_WORD];
	char stagefilename[MAX_WORD];
	Player* pPlayer = GetPlayer();

	//if (GetModeMenu() == MODE_MENU_NORMALMODE)
	//{
	//	sprintf(stagefilename, "data/STAGE/Normal/stage%d.txt", nStage);
	//}
	//else if (GetModeMenu() == MODE_MENU_WASUKEMODE)
	//{
	//	sprintf(stagefilename, "data/STAGE/Yamada/stage%d.txt", nStage);
	//}

	pFile = fopen(stagefilename, "r");

	if (pFile != NULL)
	{
		while (1)
		{
			fscanf(pFile, "%s", &stagedata[0]);

			if (strcmp(stagedata, "BLOCKSET") == 0)
			{

				while (1)
				{

					fscanf(pFile, "%s", &stagedata[0]);

					if (strcmp(stagedata, "POS") == 0)
					{
						fscanf(pFile, "%f", &pos.x);
						fscanf(pFile, "%f", &pos.y);
						fscanf(pFile, "%f", &pos.z);
					}
					else if (strcmp(stagedata, "MOVE") == 0)
					{
						fscanf(pFile, "%f", &move.x);
						fscanf(pFile, "%f", &move.y);
						fscanf(pFile, "%f", &move.z);
					}
					else if (strcmp(stagedata, "WIDTH") == 0)
					{
						fscanf(pFile, "%f", &fWidth);
					}
					else if (strcmp(stagedata, "HEIGHT") == 0)
					{
						fscanf(pFile, "%f", &fHeight);
					}
					else if (strcmp(stagedata, "TYPE") == 0)
					{
						fscanf(pFile, "%d", &ntype);
					}
					else if (strcmp(stagedata, "END_BLOCKSET") == 0)
					{
						//データを代入
						SetBlock(pos, move,fWidth, fHeight,ntype);
						break;
					}
				}
			}

			if (strcmp(stagedata, "END_SCRIPT") == 0)
			{
				break;
			}
		}



		fclose(pFile);

	}
	else
	{
		return;
	}
}
