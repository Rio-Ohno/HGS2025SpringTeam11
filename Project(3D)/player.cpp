//============================================================
//
//�v���C���[[player.cpp]
//Author:Rio Ohno
//
//============================================================

#include"player.h"
#include"input.h"
#include"bullet.h"
//#include"exploosion.h

//�}�N��
#define PLAYER_SPEED ((float)1.0f)

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL;                                     //�e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTexturePlayerPos = NULL;                                  //�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer=NULL;                                  //���_�����i�[
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayerPos = NULL;                             //���_�����i�[
Player g_Player;

int g_nCounterAnimPlayer = 0;                                                   //�A�j���[�V�����J�E���^
int g_nPatternAnimPlayer = 0;                                                   //�A�j���[�V�����p�^�[��No.
static int nData = 0;                                                           //�������m�p

//=============================================================================================================
//�v���C���[�̏���������
//=============================================================================================================
void InitPlayer()
{

	LPDIRECT3DDEVICE9 pDevice;													//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		/*"data\\TEXTURE\\00000402.png"*/NULL,
		&g_pTexturePlayer);

	g_nCounterAnimPlayer = 0;													//�J�E���^�[������������
	g_nPatternAnimPlayer = 0;													//�p�^�[��NO.������������

	g_Player.pos = D3DXVECTOR3(640.0f,625.0f,0);								//�ʒu������������
	g_Player.move = D3DXVECTOR3(0.0f,0.0f,0.0f);								//�ړ��ʂ�����������
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								//����������������
	g_Player.state = PLAYERSTATE_NORMAL;										//�G�̏�Ԃ̏�����
	g_Player.nCountState = 0;													//��ԊǗ��p�J�E���^�[�̏�����
	g_Player.nCntPlayer = 2;													//�c�@����������

	//�Ίp���̒������Z�o����
	g_Player.fLength = sqrtf(NUM_WIDTH* NUM_WIDTH + NUM_HEIGHT * NUM_HEIGHT) / 2.0f;

	//�Ίp���̊p�x���Z�o����
	g_Player.fAngle = atan2f(NUM_WIDTH, NUM_HEIGHT);

	g_Player.fLife = PLAYERLIFE;												//�̗͂̏�����
	g_Player.bUse = true;														//���@���g�p��Ԃɂ���
	g_Player.bDisp = true;

	//���_�o�b�t�@�̂̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,                               
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);

	VERTEX_2D* pVtx;								                           //���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos.x = g_Player.pos.x + sinf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[0].pos.y = g_Player.pos.y + cosf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[1].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (0.0f - g_Player.fAngle)) * g_Player.fLength;
	pVtx[2].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (0.0f - g_Player.fAngle)) * g_Player.fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_Player.pos.x + sinf(g_Player.rot.z - (0.0f - g_Player.fAngle)) * g_Player.fLength;
	pVtx[3].pos.y = g_Player.pos.y + cosf(g_Player.rot.z - (0.0f - g_Player.fAngle)) * g_Player.fLength;
	pVtx[3].pos.z = 0.0f;


	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(100, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(100, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(100, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(100, 255, 255, 255);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayer->Unlock();
}

//=============================================================================================================
//�v���C���[�̕`�揈��
//=============================================================================================================
void DrawPlayer()
{
	LPDIRECT3DDEVICE9 pDevice;														//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	if (g_Player.bDisp == true)
	{
		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePlayer);

		//�v���C���[�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	}

	//DrawPlayer����ōς܂������Ȃ�ݒ�->�`��->�ݒ�->�`��̏�

	////�e�N�X�`���̐ݒ�
	//pDevice->SetTexture(0, g_pTexturePlayer2);

	////�v���C���[�̕`��
	//pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4, 2);

}

//=============================================================================================================
//�v���C���[�̍X�V����
//=============================================================================================================
void UpdatePlayer()
{
	if (g_Player.bUse == true)
	{
		//�v���C���[�̓���
		ActionPlayer();

		//�ʒu���X�V
		g_Player.pos.x += g_Player.move.x;
		g_Player.pos.y += g_Player.move.y;

		//�ړ��ʂ��X�V�i�����j
		g_Player.move.x += (0.0f - g_Player.move.x) * 0.09f;
		g_Player.move.y += (0.0f - g_Player.move.y) * 0.09f;//����܂肢��Ȃ����琔���ł��߂ɂ��Ă�

		//��ʊO�ɏo�Ȃ��悤�ɂ��鏈��
		if ((g_Player.pos.x + sinf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength) < 0.0f)
		{
			g_Player.pos.x = NUM_WIDTH / 2;
		}
		if ((g_Player.pos.x + sinf(g_Player.rot.z + (D3DX_PI - g_Player.fAngle)) * g_Player.fLength) > SCREEN_WIDTH)
		{
			g_Player.pos.x = SCREEN_WIDTH - (NUM_WIDTH / 2);
		}
		if ((g_Player.pos.y + cosf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength) < 0.0f)
		{
			g_Player.pos.y = NUM_HEIGHT / 2;
		}
		if ((g_Player.pos.y + cosf(g_Player.rot.z - (0.0f - g_Player.fAngle)) * g_Player.fLength) > SCREEN_HEIGHT)
		{
			g_Player.pos.y = SCREEN_HEIGHT - (NUM_HEIGHT / 2);
		}

		VERTEX_2D* pVtx;//���_���ւ̃|�C���^

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

		//���_���W�̐ݒ�
		pVtx[0].pos.x = g_Player.pos.x + sinf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
		pVtx[0].pos.y = g_Player.pos.y + cosf(g_Player.rot.z - (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
		pVtx[1].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (D3DX_PI - g_Player.fAngle)) * g_Player.fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_Player.pos.x + sinf(g_Player.rot.z + (0.0f - g_Player.fAngle)) * g_Player.fLength;
		pVtx[2].pos.y = g_Player.pos.y + cosf(g_Player.rot.z + (0.0f - g_Player.fAngle)) * g_Player.fLength;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_Player.pos.x + sinf(g_Player.rot.z - (0.0f - g_Player.fAngle)) * g_Player.fLength;
		pVtx[3].pos.y = g_Player.pos.y + cosf(g_Player.rot.z - (0.0f - g_Player.fAngle)) * g_Player.fLength;
		pVtx[3].pos.z = 0.0f;

		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffPlayer->Unlock();
	}
}

//=============================================================================================================
//�v���C���[�̏I������
//=============================================================================================================
void UninitPlayer()
{
	//�e�N�X�`���̔j��
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//=============================================================================================================
//�e�������������̏���
//=============================================================================================================
void HitPlayer(float fDamage)
{
	g_Player.fLife -= fDamage;

	if (g_Player.fLife <= 0)
	{
		if (g_Player.nCntPlayer <= 0)
		{
			g_Player.nCountState = 5;
			g_Player.bUse = false;
			g_Player.state = PLAYERSTATE_DEATH;
		}
		else if(g_Player.nCntPlayer > 0)
		{
			g_Player.nCountState = 20;
			g_Player.bUse = false;
			g_Player.bDisp = false;
			g_Player.state = PLAYERSTATE_APPEAR;
		}
	}

	else
	{
		g_Player.nCountState = 5;												//�_���[�W��Ԃ�ۂ��Ԃ�ݒ�
		g_Player.state = PLAYERSTATE_DAMAGE;

		VERTEX_2D* pVtx;

		//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);

		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffPlayer->Unlock();

	}
}

//=============================================================================================================
//�v���C���[�̍s��
//=============================================================================================================
void ActionPlayer()
{
	if (GetKeyboardPress(DIK_A) == true || GetJoypadPress(JOYKEY_LEFT) == true)
	{
#ifdef _DEBUG
		if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)
		{//W�L�[||��i�Q�[���p�b�h�j�������ꂽ
			g_Player.move.y += cosf(-D3DX_PI * 0.75f) * PLAYER_SPEED;
			g_Player.move.x += sinf(-D3DX_PI * 0.75f) * PLAYER_SPEED;
		}
		else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
		{//S�L�[||���i�Q�[���p�b�h�j�������ꂽ
			g_Player.move.y += cosf(-D3DX_PI * 0.25) * PLAYER_SPEED;
			g_Player.move.x += sinf(-D3DX_PI * 0.25f) * PLAYER_SPEED;
		}
		else
		{//A�L�[||���i�Q�[���p�b�h�j����
			g_Player.move.x -= PLAYER_SPEED;
		}
#endif
#ifdef _RELEASE
		if (GetKeyboardPress(DIK_A))
		{
			g_Player.move.x -= PLAYER_SPEED;
		}
#endif
	}
	else if (GetKeyboardPress(DIK_D) == true || GetJoypadPress(JOYKEY_RIGHT) == true)
	{//D�L�[�������ꂽ
#ifdef _DEBUG
		if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)
		{//W�L�[�������ꂽ
			g_Player.move.y += cosf(D3DX_PI * 0.75f) * PLAYER_SPEED;
			g_Player.move.x += sinf(D3DX_PI * 0.75f) * PLAYER_SPEED;
		}
		else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
		{//S�L�[�������ꂽ
			g_Player.move.y += cosf(D3DX_PI * 0.25f) * PLAYER_SPEED;
			g_Player.move.x += sinf(D3DX_PI * 0.25f) * PLAYER_SPEED;
		}
		else
		{//D�L�[����
			g_Player.move.x += PLAYER_SPEED;
		}
#endif
#ifdef _RELEASE
		if (GetKeyboardPress(DIK_D) == true)
		{//D�L�[����
			g_Player.move.x += PLAYER_SPEED;
		}
#endif // 


	}
#ifdef _DEBUG
	else if (GetKeyboardPress(DIK_W) == true || GetJoypadPress(JOYKEY_UP) == true)
	{//W�L�[�������ꂽ
		g_Player.move.y -= PLAYER_SPEED;
	}
	else if (GetKeyboardPress(DIK_S) == true || GetJoypadPress(JOYKEY_DOWN) == true)
	{//s�L�[�������ꂽ
		g_Player.move.y += PLAYER_SPEED;
	}
	if (KeyboardTrigger(DIK_SPACE) == true || GetJoypadPress(JOYKEY_X) == true)
	{//�X�y�[�X�L�[||X�L�[�i�Q�[���p�b�h�j
		SetBullet(g_Player.pos, g_Player.rot, g_Player.fLength, 100);
	}
#endif

}

//=============================================================================================================
//�v���C���[�̎擾
//=============================================================================================================
Player*GetPlayer()
{
	return &g_Player;
}

//=============================================================================================================
//�v���C���[pos�̏���������
//=============================================================================================================
void InitPos()
{
	LPDIRECT3DDEVICE9 pDevice;													//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\effect000.jpg",
		&g_pTexturePlayerPos);

	//���_�o�b�t�@�̂̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayerPos,
		NULL);

	VERTEX_2D* pVtx1;								                           //���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayerPos->Lock(0, 0, (void**)&pVtx1, 0);

		//���_���W�̏�����
		pVtx1[0].pos.x = g_Player.pos.x - 5.0f;
		pVtx1[0].pos.y = g_Player.pos.y - 5.0f;
		pVtx1[0].pos.z = 0.0f;

		pVtx1[1].pos.x = g_Player.pos.x + 5.0f;
		pVtx1[1].pos.y = g_Player.pos.y - 5.0f;
		pVtx1[1].pos.z = 0.0f;

		pVtx1[2].pos.x = g_Player.pos.x - 5.0f;
		pVtx1[2].pos.y = g_Player.pos.y + 5.0f;
		pVtx1[2].pos.z = 0.0f;

		pVtx1[3].pos.x = g_Player.pos.x + 5.0f;
		pVtx1[3].pos.y = g_Player.pos.y + 5.0f;
		pVtx1[3].pos.z = 0.0f;

		//rhw�̐ݒ�
		pVtx1[0].rhw = 1.0f;
		pVtx1[1].rhw = 1.0f;
		pVtx1[2].rhw = 1.0f;
		pVtx1[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx1[0].col = D3DXCOLOR(1.0f, 0.5f, 1.0f, 1.0f);
		pVtx1[1].col = D3DXCOLOR(1.0f, 0.5f, 1.0f, 1.0f);
		pVtx1[2].col = D3DXCOLOR(1.0f, 0.5f, 1.0f, 1.0f);
		pVtx1[3].col = D3DXCOLOR(1.0f, 0.5f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx1[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx1[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx1[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx1[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayerPos->Unlock();

}

//=============================================================================================================
//�v���C���[pos�̏I������
//=============================================================================================================
void UninitPos()
{
	//�e�N�X�`���̔j��
	if (g_pTexturePlayerPos != NULL)
	{
		g_pTexturePlayerPos->Release();
		g_pTexturePlayerPos = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPlayerPos != NULL)
	{
		g_pVtxBuffPlayerPos->Release();
		g_pVtxBuffPlayerPos = NULL;
	}
}

//=============================================================================================================
//�v���C���[pos�̍X�V����
//=============================================================================================================
void UpdatePos()
{
	VERTEX_2D* pVtx1;								                           //���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPlayerPos->Lock(0, 0, (void**)&pVtx1, 0);

		//���_���W�̏�����
		pVtx1[0].pos.x = g_Player.pos.x - 5.0f;
		pVtx1[0].pos.y = g_Player.pos.y - 5.0f;
		pVtx1[0].pos.z = 0.0f;

		pVtx1[1].pos.x = g_Player.pos.x + 5.0f;
		pVtx1[1].pos.y = g_Player.pos.y - 5.0f;
		pVtx1[1].pos.z = 0.0f;

		pVtx1[2].pos.x = g_Player.pos.x - 5.0f;
		pVtx1[2].pos.y = g_Player.pos.y + 5.0f;
		pVtx1[2].pos.z = 0.0f;

		pVtx1[3].pos.x = g_Player.pos.x + 5.0f;
		pVtx1[3].pos.y = g_Player.pos.y + 5.0f;
		pVtx1[3].pos.z = 0.0f;


	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPlayerPos->Unlock();
}

//=============================================================================================================
//�v���C���[pos�̕`�揈��
//=============================================================================================================
void DrawPos()
{
	LPDIRECT3DDEVICE9 pDevice;														//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	if (g_Player.bUse == true)
	{

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffPlayerPos, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//���u�����f�B���O�����Z�����ɐݒ�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePlayerPos);

		//�v���C���[�̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		//���u�����f�B���O�����ɖ߂�
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}