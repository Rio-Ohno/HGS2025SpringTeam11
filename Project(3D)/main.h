//==============================
//
//�@�E�C���h�E�\������[main.h]
//�@Author:kaiti
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
//���C�u�����̃����N
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"xinput.lib")

//�}�N����`
#define CLASS_NAME "WindowsClass"
#define WINDOW_NAME "DEVASTATED CITY"
#define SCREEN_WIDTH (1600)
#define SCREEN_HEIGHT (900)
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)//���W�E�@���E�J���[�E�e�N�X�`��
#define MAX_WORD (4096)
#define TOPI (float)(0.0174f)

//���_���(2D)�̍\����
typedef struct
{
	D3DXVECTOR3 pos;//���_���W
	float rhw;      //���W�ϊ��p�W��
	D3DCOLOR col;   //���_�J���[
	D3DXVECTOR2 tex;//�e�N�X�`�����W
}VERTEX_2D;

//���_���(3D)�̍\����
typedef struct
{
	D3DXVECTOR3 pos;//���_���W
	D3DXVECTOR3 nor;//�@���x�N�g��
	D3DCOLOR col;   //���_�J���[
	D3DXVECTOR2 tex;//�e�N�X�`�����W
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

//�v���g�^�C�v�錾
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
D3DXVECTOR3 *PitoRadian(D3DXVECTOR3 *rot);		// ���W�A���l����PI�ɕϊ����鏈��
void UninitTexture(LPDIRECT3DTEXTURE9 &Texture);
void UninitBuffer(LPDIRECT3DVERTEXBUFFER9 &Buffer);
void UninitMesh(LPD3DXMESH &Mesh);
void UninitBuffMat(LPD3DXBUFFER &Buffer);
void UninitIndxBuffer(LPDIRECT3DINDEXBUFFER9 &IndxBuffer);
void DrawEditBlock(int File, D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight, int nType, int nTotal);


// �v�Z�n�̊֐�
float Vector(float StartPoint, float EndPoint);
float Angle(D3DXVECTOR2 Vec);
void NearRot(float Angle,float &OutAngle);
// ����ƌn�̊֐�
void SetAngle(float fAngle,float &OutAngle);

////�G�f�B�^�[�̃X�e�[�W�ǂݍ���
//void LoadStage();
#endif