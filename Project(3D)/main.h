//==============================
//
//　ウインドウ表示処理[main.h]
//　Author:kaiti
//
//==============================
#ifndef _MAIN_H_
#define _MAIN_H_

#include<Windows.h>
#include "d3dx9.h"

#include<stdio.h>

#define DIRECTINPUT_VERSION (0x0800)
#include "dinput.h"
#include "Xinput.h"
#include "xaudio2.h"

#include<time.h>
#include "string.h"
#include "stdlib.h"
//ライブラリのリンク
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")

//マクロ定義
#define CLASS_NAME "WindowsClass"
#define WINDOW_NAME "DEVASTATED CITY"
#define SCREEN_WIDTH (1600)
#define SCREEN_HEIGHT (900)
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)//座標・法線・カラー・テクスチャ
#define MAX_WORD (4096)
#define TOPI (float)(0.0174f)

//頂点情報(2D)の構造体
typedef struct
{
	D3DXVECTOR3 pos;//頂点座標
	float rhw;      //座標変換用係数
	D3DCOLOR col;   //頂点カラー
	D3DXVECTOR2 tex;//テクスチャ座標
}VERTEX_2D;

//頂点情報(3D)の構造体
typedef struct
{
	D3DXVECTOR3 pos;//頂点座標
	D3DXVECTOR3 nor;//法線ベクトル
	D3DCOLOR col;   //頂点カラー
	D3DXVECTOR2 tex;//テクスチャ座標
}VERTEX_3D;

typedef enum
{
	MODE_TITLE = 0,
	MODE_STAGEONE,
	MODE_STAGETWO,
	MODE_STAGETHREE,
	MODE_BOSSMOVIE,
	MODE_STAGEFOUR,
	MODE_RESULT,
	MODE_RANK,
	MODE_END,
	MODE_MAX
}MODE;

//プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void onWireFrame();
void offWireFrame();
void SetMode(MODE mode);
MODE GetMode(void);
LPDIRECT3DDEVICE9 GetDevice(void);
D3DXVECTOR3 *PitoRadian(D3DXVECTOR3 *rot);		// ラジアン値からPIに変換する処理
void UninitTexture(LPDIRECT3DTEXTURE9 &Texture);
void UninitBuffer(LPDIRECT3DVERTEXBUFFER9 &Buffer);
void UninitMesh(LPD3DXMESH &Mesh);
void UninitBuffMat(LPD3DXBUFFER &Buffer);
void UninitIndxBuffer(LPDIRECT3DINDEXBUFFER9 &IndxBuffer);
void DrawEditBlock(int File, D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight, int nType, int nTotal);


// 計算系の関数
float Vector(float StartPoint, float EndPoint);
float Angle(D3DXVECTOR2 Vec);
void NearRot(float Angle,float &OutAngle);
// 実作業系の関数
void SetAngle(float fAngle,float &OutAngle);

////エディターのステージ読み込み
//void LoadStage();
#endif