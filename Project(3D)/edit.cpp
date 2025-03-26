//==================================================================
//
//ステージエディット処理<edit.cpp>
//Author:Saeki Takuto
//
//==================================================================

#include "edit.h"
#include <stdio.h>
#include <string.h>
#include "input.h"

//マクロ定義
#define EDIT_MOVE (20)

//構造体
typedef struct
{
	D3DXVECTOR3 pos;//位置;
	D3DXVECTOR3 move;//動き
	float fWidth;//横幅
	float fHeight;//縦幅
	int nType;
	bool bUse;
}EditInfo;

//グローバル
LPDIRECT3DTEXTURE9 g_pTextureEdit[TYPE_BLOCK] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEdit = NULL;//頂点バッファへのポインタ
EditInfo g_EditInfo[MAX_BLOCK];//構造体変数
int g_nCntEdit;
int g_nCntTextFile;

void InitEdit(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCnt;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/block000.jpg",//通常ブロック
		&g_pTextureEdit[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/block001.png",//左ベルコンブロック
		&g_pTextureEdit[1]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/block002.png",//右ベルコンブロック
		&g_pTextureEdit[2]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/block003.png",//トランポリンブロック
		&g_pTextureEdit[3]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/block004.png",//左ベルコン強化ブロック
		&g_pTextureEdit[4]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/block005.png",//右ベルコン強化ブロック
		&g_pTextureEdit[5]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/block006.png",//クリアボタンブロック
		&g_pTextureEdit[6]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/block007.png",//クリアボタンブロック
		&g_pTextureEdit[7]);


	//弾の情報の初期化
	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		g_EditInfo[nCnt].pos = D3DXVECTOR3(20.0f, 20.0f, 0.0f);
		g_EditInfo[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_EditInfo[nCnt].fWidth = 40.0f;
		g_EditInfo[nCnt].fHeight = 40.0f;
		g_EditInfo[nCnt].nType = 0;
		g_EditInfo[nCnt].bUse = false;//使用していない状態にする
	}

	g_EditInfo[0].bUse = true;//最初の配置物を使用可能に

	g_nCntEdit = 0;

	g_nCntTextFile = 0;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEdit,
		NULL);

	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffEdit->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		pVtx[0].pos = D3DXVECTOR3(g_EditInfo[nCnt].pos.x - (g_EditInfo[nCnt].fWidth / 2), g_EditInfo[nCnt].pos.y - (g_EditInfo[nCnt].fHeight / 2), g_EditInfo[nCnt].pos.z);
		pVtx[1].pos = D3DXVECTOR3(g_EditInfo[nCnt].pos.x + (g_EditInfo[nCnt].fWidth / 2), g_EditInfo[nCnt].pos.y - (g_EditInfo[nCnt].fHeight / 2), g_EditInfo[nCnt].pos.z);
		pVtx[2].pos = D3DXVECTOR3(g_EditInfo[nCnt].pos.x - (g_EditInfo[nCnt].fWidth / 2), g_EditInfo[nCnt].pos.y + (g_EditInfo[nCnt].fHeight / 2), g_EditInfo[nCnt].pos.z);
		pVtx[3].pos = D3DXVECTOR3(g_EditInfo[nCnt].pos.x + (g_EditInfo[nCnt].fWidth / 2), g_EditInfo[nCnt].pos.y + (g_EditInfo[nCnt].fHeight / 2), g_EditInfo[nCnt].pos.z);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;//頂点データのポインタを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEdit->Unlock();
}

void UninitEdit(void)
{
	for (int nCnt = 0; nCnt < TYPE_BLOCK; nCnt++)
	{
		//テクスチャの破棄
		if (g_pTextureEdit[nCnt] != NULL)
		{
			g_pTextureEdit[nCnt]->Release();
			g_pTextureEdit[nCnt] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffEdit != NULL)
	{
		g_pVtxBuffEdit->Release();
		g_pVtxBuffEdit = NULL;
	}

}

void UpdateEdit(void)
{

	if (KeyboardTrigger(DIK_A) == true)
	{
		g_EditInfo[g_nCntEdit].pos.x -= EDIT_MOVE;
	}
	else if (KeyboardTrigger(DIK_D) == true)
	{
		g_EditInfo[g_nCntEdit].pos.x += EDIT_MOVE;
	}
	else if (KeyboardTrigger(DIK_W) == true)
	{
		g_EditInfo[g_nCntEdit].pos.y -= EDIT_MOVE;
	}
	else if (KeyboardTrigger(DIK_S) == true)
	{
		g_EditInfo[g_nCntEdit].pos.y += EDIT_MOVE;
	}
	else if (KeyboardTrigger(DIK_Q) == true)
	{
		g_EditInfo[g_nCntEdit].nType +=1;
	}
	else if (KeyboardTrigger(DIK_E) == true)
	{
		g_EditInfo[g_nCntEdit].nType -= 1;
	}
	else if (KeyboardTrigger(DIK_UP) == true)
	{
		g_EditInfo[g_nCntEdit].fHeight += 20.0f;
	}
	else if (KeyboardTrigger(DIK_DOWN) == true)
	{
		g_EditInfo[g_nCntEdit].fHeight -= 20.0f;
	}
	else if (KeyboardTrigger(DIK_LEFT) == true)
	{
		g_EditInfo[g_nCntEdit].fWidth += 20.0f;
	}
	else if (KeyboardTrigger(DIK_RIGHT) == true)
	{
		g_EditInfo[g_nCntEdit].fWidth -= 20.0f;
	}
	else if (KeyboardTrigger(DIK_Z) == true)
	{
		g_EditInfo[g_nCntEdit].move.x -= 1.0f;
	}
	else if (KeyboardTrigger(DIK_X) == true)
	{
		g_EditInfo[g_nCntEdit].move.x += 1.0f;
	}
	else if (KeyboardTrigger(DIK_C) == true)
	{
		g_EditInfo[g_nCntEdit].move.y -= 1.0f;
	}
	else if (KeyboardTrigger(DIK_V) == true)
	{
		g_EditInfo[g_nCntEdit].move.y += 1.0f;
	}
	else if (KeyboardTrigger(DIK_O) == true)
	{
		g_nCntTextFile++;
	}
	else if (KeyboardTrigger(DIK_P) == true)
	{
		g_nCntTextFile--;
	}
	else if (KeyboardTrigger(DIK_B) == true)
	{
		SaveEdit();
	}
	else if (KeyboardTrigger(DIK_N) == true)
	{
		LoadEdit();
	}


	if (KeyboardTrigger(DIK_RETURN) == true)
	{
		g_EditInfo[g_nCntEdit+1].pos = g_EditInfo[g_nCntEdit].pos;
		g_EditInfo[g_nCntEdit + 1].nType = g_EditInfo[g_nCntEdit].nType;
		//使用判定する
		g_EditInfo[g_nCntEdit+1].bUse=true;
		//インクリメント
		g_nCntEdit++;
	}


	if (KeyboardTrigger(DIK_F10) == true)
	{
		//使用判定する
		g_EditInfo[g_nCntEdit].bUse = false;
		//インクリメント
		g_nCntEdit--;
	}



	VERTEX_2D* pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffEdit->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		if (g_EditInfo[nCnt].bUse == true)
		{
			pVtx[0].pos = D3DXVECTOR3(g_EditInfo[nCnt].pos.x - (g_EditInfo[nCnt].fWidth / 2), g_EditInfo[nCnt].pos.y - (g_EditInfo[nCnt].fHeight / 2), g_EditInfo[nCnt].pos.z);
			pVtx[1].pos = D3DXVECTOR3(g_EditInfo[nCnt].pos.x + (g_EditInfo[nCnt].fWidth / 2), g_EditInfo[nCnt].pos.y - (g_EditInfo[nCnt].fHeight / 2), g_EditInfo[nCnt].pos.z);
			pVtx[2].pos = D3DXVECTOR3(g_EditInfo[nCnt].pos.x - (g_EditInfo[nCnt].fWidth / 2), g_EditInfo[nCnt].pos.y + (g_EditInfo[nCnt].fHeight / 2), g_EditInfo[nCnt].pos.z);
			pVtx[3].pos = D3DXVECTOR3(g_EditInfo[nCnt].pos.x + (g_EditInfo[nCnt].fWidth / 2), g_EditInfo[nCnt].pos.y + (g_EditInfo[nCnt].fHeight / 2), g_EditInfo[nCnt].pos.z);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(g_EditInfo[nCnt].fWidth / WIDTHBLOCK, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, g_EditInfo[nCnt].fHeight / HEIGHTBLOCK);
			pVtx[3].tex = D3DXVECTOR2(g_EditInfo[nCnt].fWidth / WIDTHBLOCK, g_EditInfo[nCnt].fHeight / HEIGHTBLOCK);

		}
		pVtx += 4;//頂点データのポインタを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEdit->Unlock();

}

void DrawEdit(void)
{
	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの処理
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEdit, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCnt = 0; nCnt < MAX_BLOCK; nCnt++)
	{
		if (g_EditInfo[nCnt].bUse == true)
		{//弾が使用されている

			if (g_EditInfo[nCnt].nType == BLOCKTYPE_NORMAL)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureEdit[0]);
			}
			else if (g_EditInfo[nCnt].nType == BLOCKTYPE_CONVEYOR_L)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureEdit[1]);
			}
			else if (g_EditInfo[nCnt].nType == BLOCKTYPE_CONVEYOR_R)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureEdit[2]);
			}
			else if (g_EditInfo[nCnt].nType == BLOCKTYPE_TRAMPOLINE)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureEdit[3]);
			}
			else if (g_EditInfo[nCnt].nType == BLOCKTYPE_CONVEYOR_L_HIGH)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureEdit[4]);
			}
			else if (g_EditInfo[nCnt].nType == BLOCKTYPE_CONVEYOR_R_HIGH)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureEdit[5]);
			}
			else if (g_EditInfo[nCnt].nType == BLOCKTYPE_CLEARBUTTON)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureEdit[6]);
			}
			else if (g_EditInfo[nCnt].nType == BLOCKTYPE_SOUL)
			{
				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureEdit[7]);
			}


			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				4 * nCnt,//頂点インデックス
				2);
		}
	}

	DrawEditBlock(g_nCntTextFile,g_EditInfo[g_nCntEdit].pos, g_EditInfo[g_nCntEdit].move, g_EditInfo[g_nCntEdit].fWidth, g_EditInfo[g_nCntEdit].fHeight, g_EditInfo[g_nCntEdit].nType, g_nCntEdit);
}

void LoadEdit(void)
{
	FILE* pFile;
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 move= D3DXVECTOR3(0.0f,0.0f,0.0f);
	float fWidth;
	float fHeight;
	int nType;
	char stagedata[MAX_WORD];
	char stagefilename[MAX_WORD];

	g_nCntEdit = 0;

	sprintf(stagefilename, "data/STAGE/stage%d.txt", g_nCntTextFile);

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
						fscanf(pFile, "%d", &nType);
					}
					else if (strcmp(stagedata, "END_BLOCKSET") == 0)
					{
						g_EditInfo[g_nCntEdit].pos = pos;
						g_EditInfo[g_nCntEdit].move = move;
						g_EditInfo[g_nCntEdit].fWidth = fWidth;
						g_EditInfo[g_nCntEdit].fHeight = fHeight;
						g_EditInfo[g_nCntEdit].nType = nType;

						g_EditInfo[g_nCntEdit + 1].pos = g_EditInfo[g_nCntEdit].pos;
						g_EditInfo[g_nCntEdit + 1].nType = g_EditInfo[g_nCntEdit].nType;
						//使用判定する
						g_EditInfo[g_nCntEdit + 1].bUse = true;
						//インクリメント
						g_nCntEdit++;
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

void SaveEdit(void)
{
	FILE* pFile;

	char stagefilename[MAX_WORD];

	sprintf(stagefilename, "data/STAGE/stage%d.txt", g_nCntTextFile);

	pFile = fopen(stagefilename, "w");

	if (pFile != NULL)
	{
		fwrite(&g_nCntEdit, sizeof(int), 0, pFile);

		for (int nCnt = 0; nCnt < g_nCntEdit; nCnt++)//配置ぶん回す
		{
			if (g_EditInfo[nCnt].bUse == true)
			{//使用されている
				fprintf(pFile, "BLOCKSET\n");

				fprintf(pFile, "POS %f %f %f\n", g_EditInfo[nCnt].pos.x, g_EditInfo[nCnt].pos.y, g_EditInfo[nCnt].pos.z);

				fprintf(pFile, "MOVE %f %f %f\n", g_EditInfo[nCnt].move.x, g_EditInfo[nCnt].move.y, g_EditInfo[nCnt].move.z);

				fprintf(pFile, "WIDTH %f\n", g_EditInfo[nCnt].fWidth);

				fprintf(pFile, "HEIGHT %f\n", g_EditInfo[nCnt].fHeight);

				fprintf(pFile, "TYPE %d\n", g_EditInfo[nCnt].nType);

				fprintf(pFile, "END_BLOCKSET\n");
			}
		}
		fprintf(pFile, "END_SCRIPT\n");
		fclose(pFile);
	}

}
