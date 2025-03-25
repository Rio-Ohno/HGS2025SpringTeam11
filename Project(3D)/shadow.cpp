//================================
//
// �e�\������[shadow.cpp]
// Author:kaiti
//
//================================

#include "shadow.h"
#include "light.h"

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_apTextureShadow = NULL;
Shadow g_shadow[MAX_SHADOW];

//=======================
// �e�̏���������
//=======================
void InitShadow(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++)
	{
		g_shadow[nCnt].pos = D3DXVECTOR3(0.0f, 0.1f, 0.0f);			//�ʒu�̏�����
		g_shadow[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�����̏�����
		g_shadow[nCnt].fRadius = 0.0f;								//���a�̏�����
		g_shadow[nCnt].bUse = false;								//�g�p���Ă��Ă��Ȃ���Ԃɂ���
	}

	//�e�N�X�`���̓Ǎ�
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\shadow000.jpg", &g_apTextureShadow); //1
}

//====================
// �e�̏I������
//====================
void UninitShadow(void)
{
	//�e�N�X�`���̔j��
	if (g_apTextureShadow != NULL)
	{
		g_apTextureShadow->Release();
		g_apTextureShadow = NULL;
	}

	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++)
	{
		//���_�o�b�t�@�̔j��
		if (g_shadow[nCnt].pVtxBuffShadow != NULL)
		{
			g_shadow[nCnt].pVtxBuffShadow->Release();
			g_shadow[nCnt].pVtxBuffShadow = NULL;
		}
	}
}

//=====================
// �e�̍X�V����
//=====================
void UpdateShadow(void)
{

}

//====================
// �e�̕`�揈��
//====================
void DrawShadow(void)
{
	////�f�o�C�X�̎擾
	//LPDIRECT3DDEVICE9 pDevice = GetDevice();

	////���Z�����̐ݒ�
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++)
	//{
	//	//�g�p���Ă����ԂȂ�
	//	if (g_shadow[nCnt].bUse == true)
	//	{
	//		//�v�Z�p�}�g���b�N�X
	//		D3DXMATRIX mtxRot, mtxTrans, mtxShadow;

	//		//���[���h�}�g���b�N�X�̏�����
	//		D3DXMatrixIdentity(&g_shadow[nCnt].mtxWorld);

	//		//�����𔽓]
	//		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_shadow[nCnt].rot.y, g_shadow[nCnt].rot.x, g_shadow[nCnt].rot.z);
	//		D3DXMatrixMultiply(&g_shadow[nCnt].mtxWorld, &g_shadow[nCnt].mtxWorld, &mtxRot);

	//		//�ʒu�𔽉f
	//		D3DXMatrixTranslation(&mtxTrans, g_shadow[nCnt].pos.x, 0.1f, g_shadow[nCnt].pos.z);
	//		D3DXMatrixMultiply(&g_shadow[nCnt].mtxWorld, &g_shadow[nCnt].mtxWorld, &mtxTrans);

	//		D3DXVECTOR4 LightPos = D3DXVECTOR4(0.0f, 10.0f, 0.0f, 0.0f);
	//		D3DXPLANE Plane = D3DXPLANE(0.0f, 1.0f, 0.0f, 0.0f);

	//		D3DXMatrixShadow(&mtxShadow, &LightPos, &Plane);
	//		D3DXMatrixMultiply(&g_shadow[nCnt].mtxWorld, &g_shadow[nCnt].mtxWorld, &mtxShadow);

	//		//���[���h�}�g���b�N�X�̐ݒ�
	//		pDevice->SetTransform(D3DTS_WORLD, &g_shadow[nCnt].mtxWorld);

	//		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	//		pDevice->SetStreamSource(0, g_shadow[nCnt].pVtxBuffShadow, 0, sizeof(VERTEX_3D));

	//		//���_�t�H�[�}�b�g�̐ݒ�
	//		pDevice->SetFVF(FVF_VERTEX_3D);

	//		//�e�N�X�`���̐ݒ�
	//		pDevice->SetTexture(0, g_apTextureShadow);

	//		//�|���S����`��
	//		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	//	}
	//}

	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	////�ݒ�����ɖ߂�
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void DrwaShadowPlayer(int Indx, D3DXMATRIX mtxWorld)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���Z�����̐ݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//�g�p���Ă����ԂȂ�
	if (g_shadow[Indx].bUse == true)
	{
		//�v�Z�p�}�g���b�N�X
		D3DXMATRIX mtxRot, mtxTrans, mtxShadow;

		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_shadow[Indx].mtxWorld);

		D3DXVECTOR4 LightPos = D3DXVECTOR4(0.0f, 10.0f, 0.0f, 0.0f);
		D3DXPLANE Plane = D3DXPLANE(0.0f, 1.0f, 0.0f, 0.0f);

		D3DXMatrixShadow(&mtxShadow, &LightPos, &Plane);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxShadow);
		D3DXMatrixMultiply(&g_shadow[Indx].mtxWorld, &g_shadow[Indx].mtxWorld, &mtxWorld);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_shadow[Indx].mtxWorld);

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_shadow[Indx].pVtxBuffShadow, 0, sizeof(VERTEX_3D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureShadow);

		//�|���S����`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}

	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//�ݒ�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}
//=============
// �e�̐ݒ�
//=============
int SetShadow(D3DXVECTOR3 pos, D3DXVECTOR3 rot,float fRadius)
{
	int nCntShadow;
	VERTEX_3D* pVtx = NULL;

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (nCntShadow = 0; nCntShadow < MAX_SHADOW; nCntShadow++)
	{
		if (g_shadow[nCntShadow].bUse == false)
		{
			//���_�o�b�t�@�̐���
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_shadow[nCntShadow].pVtxBuffShadow,
				NULL);

			//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			g_shadow[nCntShadow].pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

			g_shadow[nCntShadow].pos = pos;											//�ʒu
			g_shadow[nCntShadow].rot = rot;											//����
			g_shadow[nCntShadow].fRadius = fRadius;									//���a

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_shadow[nCntShadow].fRadius, g_shadow[nCntShadow].pos.y + 0.1f, +g_shadow[nCntShadow].fRadius);
			pVtx[1].pos = D3DXVECTOR3(+g_shadow[nCntShadow].fRadius, g_shadow[nCntShadow].pos.y + 0.1f, +g_shadow[nCntShadow].fRadius);
			pVtx[2].pos = D3DXVECTOR3(-g_shadow[nCntShadow].fRadius, g_shadow[nCntShadow].pos.y + 0.1f, -g_shadow[nCntShadow].fRadius);
			pVtx[3].pos = D3DXVECTOR3(+g_shadow[nCntShadow].fRadius, g_shadow[nCntShadow].pos.y + 0.1f, -g_shadow[nCntShadow].fRadius);

			//	//�@���x�N�g���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0, 1.0, 1.0, 1.0);
			pVtx[1].col = D3DXCOLOR(1.0, 1.0, 1.0, 1.0);
			pVtx[2].col = D3DXCOLOR(1.0, 1.0, 1.0, 1.0);
			pVtx[3].col = D3DXCOLOR(1.0, 1.0, 1.0, 1.0);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			g_shadow[nCntShadow].bUse = true;				//�g�p���Ă����Ԃɂ���
			break;
		}
		//���_�o�b�t�@���A�����b�N
		g_shadow[nCntShadow].pVtxBuffShadow->Unlock();
	}
	return nCntShadow; //�e�̔ԍ�(index)��Ԃ�
}

//=====================
// �e�̈ʒu�̍X�V����
//=====================
void SetPositionShadow(int nIdxShadow, D3DXVECTOR3 pos, bool bUse)
{
	//�����Ŏw�肳�ꂽ�ԍ��̉e��pos��ݒ�
	g_shadow[nIdxShadow].pos = pos;
	g_shadow[nIdxShadow].bUse = bUse;
}

//=====================
// �e�̃T�C�Y�̍X�V����
//=====================
void SetSizeShadow(D3DXVECTOR3 pos, int nIndx)
{
	if (g_shadow[nIndx].bUse == true)
	{
		float posY = pos.y;//���[�U�[�̍������i�[
		float fRadeius = 0;//���a
		D3DXCOLOR fAlpha = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		VERTEX_3D* pVtx = NULL;

		//posY�̐���
		if (posY <= 1.0f)
		{
			posY = 1.0f;
		}
		else if (posY >= g_shadow[nIndx].fRadius * 0.5f)
		{
			posY = -g_shadow[nIndx].fRadius * 0.1f;
		}

		//���a�̐ݒ�
		fRadeius = -5.0f * (1.0f / posY) + g_shadow[nIndx].fRadius;

		if (posY <= 0.0f)
		{
			posY *= -1;
		}

		//���l�̐ݒ�
		fAlpha.a = (1.0f / (posY + 0.5f));

		//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		g_shadow[nIndx].pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-fRadeius, 0.1f, +fRadeius);
		pVtx[1].pos = D3DXVECTOR3(+fRadeius, 0.1f, +fRadeius);
		pVtx[2].pos = D3DXVECTOR3(-fRadeius, 0.1f, -fRadeius);
		pVtx[3].pos = D3DXVECTOR3(+fRadeius, 0.1f, -fRadeius);

		//���_�J���[�̐ݒ�
		pVtx[0].col = fAlpha;
		pVtx[1].col = fAlpha;
		pVtx[2].col = fAlpha;
		pVtx[3].col = fAlpha;

		//���_�o�b�t�@���A�����b�N
		g_shadow[nIndx].pVtxBuffShadow->Unlock();
	}
}

//=====================
// �e�̃T�C�Y�̎擾����
//=====================
Shadow* GetShadow(void)
{
	return &g_shadow[0];
}
