//====================================
//
// �T���v�����f���r���[���[ [main.h]
// Author: Rio Ohno
//
//=====================================

#ifndef _MAIN_H_ // ���̃}�N����`������ĂȂ�������
#define _MAIN_H_ // 2�d�C���N���[�h�h�~�̃}�N����`

// �C���N���[�h�t�@�C���錾
#include <windows.h>
#define DIRECTINPUT_VERSION (0x0800)// �r���h���x���Ώ��p�̃}�N��
#include "d3dx9.h"					// �`�揈���ɕK�v
#include "dinput.h"					// ���͏����ɕK�v
#include "Xinput.h"					// �W���C�p�b�h�����ɕK�v
#include "xaudio2.h"

// ���C�u�����̃����N�錾
#pragma comment(lib,"d3d9.lib")		// �`�揈���ɕK�v
#pragma comment(lib,"d3dx9.lib")	// [d3d9.lib]�̊g�����C�u����
#pragma comment(lib,"dxguid.lib")	// DirectX�R���|�[�l���g(���i)�g�p�ɕK�v
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib,"xinput.lib")	// �W���C�p�b�h�����ɕK�v
#pragma comment(lib,"Winmm.lib")

// �}�N����`
#define CLASS_NAME "WindowClass"          // �E�C���h�E�N���X�̖��O
#define WINDOW_NAME "����"	  // �E�C���h�E�̖��O(�L���v�V�����ɕ\��)
#define SCREEN_WIDTH (1280)               // �E�C���h�E�̕�
#define SCREEN_HEIGHT (720)               // �E�C���h�E�̍���
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1) // ���_�t�H�[�}�b�g[2D]
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL |  D3DFVF_DIFFUSE | D3DFVF_TEX1) // ���W,�@��,�J���[,�e�N�X�`��
#define FVF_VERTEX_3D_MULTI (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2) // ���W,�@��,�J���[,�}���`�e�N�X�`��

// ���_���[2D]�̍\����
typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	float rhw;			// ���W�ϊ��p�W��(1.0f�ŌŒ�)
	D3DCOLOR col;		// ���_�J���[
	D3DXVECTOR2 tex;	// �e�N�X�`�����W
}VERTEX_2D;

// ���_���[3D]�̍\����
typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	D3DXVECTOR3 nor;	// �@���x�N�g��
	D3DCOLOR col;		// ���_�J���[
	D3DXVECTOR2 tex;	// �e�N�X�`�����W
}VERTEX_3D;

// ���_���[3D_�}���`]�̍\����
typedef struct
{
	D3DXVECTOR3 pos;	// ���_���W
	D3DXVECTOR3 nor;	// �@���x�N�g��
	D3DCOLOR col;		// ���_�J���[
	D3DXVECTOR2 tex;	// �e�N�X�`�����W
	D3DXVECTOR2 texM;	// �}���`�e�N�X�`��
}VERTEX_3D_MULTI;

// ���[�h�̎��
typedef enum
{
	MODE_TITLE = 0,		// �^�C�g��
	MODE_TUTORIAL,		// �`���[�g���A��
	MODE_GAME,			// �Q�[��
	MODE_RESULT,		// ���U���g
	MODE_EDIT,			// �G�f�B�b�g
	MODE_RANKING,		// �����L���O
	MODE_PLAY,			// �v���C���[�h
	MODE_MAX,
}MODE;

// �v���g�^�C�v�錾
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	//�@�E�B���h�E�v���V�[�W��
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);							//�@����������
void Uninit(void);																	//�@�I������
void Update(void);																	//�@�X�V����
void Draw(void);																	//�@�`�揈��
LPDIRECT3DDEVICE9 GetDevice(void);													//�@�f�o�C�X�̎擾
MODE GetMode(void);																	//�@���[�h�̎擾
void DrawFPS(void);																	//�@�f�o�b�O�\��


#endif