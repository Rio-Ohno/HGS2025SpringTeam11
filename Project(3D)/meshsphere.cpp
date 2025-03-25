//============================================================
//
//���b�V���X�t�B�A[meshSphere.cpp]
//Author:Rio Ohno
//
//============================================================

// �C���N���[�h
#include "meshSphere.h"
#include "meshfield.h"
#include <cassert>

// �O���[�o���ϐ��錾
MeshSphere g_Sphere[MAX_MESHSPHERE];									// ���̍\����
LPDIRECT3DTEXTURE9 g_pTexture;											// �e�N�X�`���p�|�C���^

//======================================
//���̏���������
//======================================
void InitSphere(void)
{
	// �e�평����
	for (int nCnt = 0; nCnt < MAX_MESHSPHERE; nCnt++)
	{
		g_Sphere[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �ʒu
		g_Sphere[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// ����
		g_Sphere[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// �F
		g_Sphere[nCnt].pVtxBuff = NULL;									// ���_���̃|�C���^
		g_Sphere[nCnt].pIndxBuff = NULL;								// �C���f�b�N�X�o�b�t�@
		g_Sphere[nCnt].textype = 0;										// �e�N�X�`���̎��
		g_Sphere[nCnt].DiviX = 0;										// x�̕�����
		g_Sphere[nCnt].DiviY = 0;										// y�̕�����
		g_Sphere[nCnt].fRadius = 0.0f;									// ���a
		g_Sphere[nCnt].bHalf = false;									// �����ł͂Ȃ���Ԃɂ���
		g_Sphere[nCnt].bHead = true;									// �@����\�Ɍ�����
		g_Sphere[nCnt].bUse = false;									// �g�p���Ă��Ȃ���Ԃɂ���
	}
	g_pTexture = NULL;													// �e�N�X�`���|�C���^
}

//======================================
// ���̏I������
//======================================
void UninitSphere(void)
{
	for (int nCnt = 0; nCnt < MAX_MESHSPHERE; nCnt++)
	{
		// ���_���̔j��
		if (g_Sphere[nCnt].pVtxBuff != NULL)
		{
			g_Sphere[nCnt].pVtxBuff->Release();
			g_Sphere[nCnt].pVtxBuff = NULL;
		}

		// �e�N�X�`���̔j��
		if (g_pTexture != NULL)
		{
			g_pTexture->Release();
			g_pTexture = NULL;
		}

		// �C���f�b�N�X�o�b�t�@�̔j��
		if (g_Sphere[nCnt].pIndxBuff != NULL)
		{
			g_Sphere[nCnt].pIndxBuff->Release();
			g_Sphere[nCnt].pIndxBuff = NULL;
		}
	}
}

//======================================
// ���̍X�V����
//======================================
void UpdateSphere(void)
{

}

//======================================
// ���̕`�揈��
//======================================
void DrawSphere(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// �v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	// ���C�g��؂�
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int nCnt = 0; nCnt < MAX_MESHSPHERE; nCnt++)
	{
		if (g_Sphere[nCnt].bUse == true)
		{
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_Sphere[nCnt].mtxWorld);

			//�����𔽓]
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Sphere[nCnt].rot.y, g_Sphere[nCnt].rot.x, g_Sphere[nCnt].rot.z);
			D3DXMatrixMultiply(&g_Sphere[nCnt].mtxWorld, &g_Sphere[nCnt].mtxWorld, &mtxRot);

			// �ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_Sphere[nCnt].pos.x, g_Sphere[nCnt].pos.y, g_Sphere[nCnt].pos.z);
			D3DXMatrixMultiply(&g_Sphere[nCnt].mtxWorld, &g_Sphere[nCnt].mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_Sphere[nCnt].mtxWorld);

			// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_Sphere[nCnt].pVtxBuff, 0, sizeof(VERTEX_3D));

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̃|�C���^���擾
			LPDIRECT3DTEXTURE9 pTexture = GetTexture2(g_Sphere[nCnt].textype);

			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, pTexture);

			// �|���S����`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, g_Sphere[nCnt].DiviX);

			// ��i�ȏ�Ȃ�
			if (g_Sphere[nCnt].DiviY >= 2)
			{
				// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
				pDevice->SetIndices(g_Sphere[nCnt].pIndxBuff);

				// ���b�V���ǂ�`��
				pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 1, 1, g_Sphere[nCnt].nMaxVtx, 0, g_Sphere[nCnt].nPolyNum);
			}
		}
	}
	// ���C�g������
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//======================================
// ���̐ݒ菈��
//======================================
int SetSphere(D3DXVECTOR3 pos, int textype, int DiviX, int DiviY, float fRadius, bool bHead, bool bHalf)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	VERTEX_3D* pVtx = NULL;
	int nCnt = 0;

	for (nCnt = 0; nCnt < MAX_MESHSPHERE; nCnt++)
	{
		if (g_Sphere[nCnt].bUse == false)
		{
			//�ϐ��錾
			int indx = 0;																		// ���_�C���f�b�N�X
			//float q, q2 = 0;																	// �v�Z���ʕۑ��p
			D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);									// �x�N�g���̕ۑ��p
			D3DXVECTOR3 VtxPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);									// ���_���W�ۑ��p

			//�e��ݒ�
			g_Sphere[nCnt].pos = pos;															// �ʒu
			g_Sphere[nCnt].textype = textype;													// �e�N�X�`���̎��
			g_Sphere[nCnt].DiviX = DiviX;														// ������
			g_Sphere[nCnt].DiviY = DiviY;														// ������
			g_Sphere[nCnt].fRadius = fRadius;													// ���a
			g_Sphere[nCnt].bHead = bHead;														// �@��
			g_Sphere[nCnt].bHalf = bHalf;														// �������ǂ���

			g_Sphere[nCnt].nMaxVtx = (g_Sphere[nCnt].DiviX + 1) * (g_Sphere[nCnt].DiviY) + 1;	// ���_��

			//���_�o�b�t�@�̐���
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) *g_Sphere[nCnt].nMaxVtx,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_Sphere[nCnt].pVtxBuff,
				NULL);

			//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			g_Sphere[nCnt].pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			for (int nCntY = 0; nCntY <= g_Sphere[nCnt].DiviY; nCntY++)
			{
				for (int nCntX = 0; nCntX <= g_Sphere[nCnt].DiviX; nCntX++)
				{
					int i = nCntX;

					// �����ł͂Ȃ��Ȃ�
					if (g_Sphere[nCnt].bHead == false)
					{
						i = g_Sphere[nCnt].DiviX - nCntX;
					}
				
					//�p�x�i�[
					float fAngle = ((D3DX_PI * 2 / g_Sphere[nCnt].DiviX) * i);									// y��
					float fAngle2 = (D3DX_PI / g_Sphere[nCnt].DiviY) * (g_Sphere[nCnt].DiviY - nCntY);			// z��

					//�����Ȃ�
					if (g_Sphere[nCnt].bHalf == true)
					{
						fAngle2 = (((D3DX_PI * 0.5f) / g_Sphere[nCnt].DiviY)* nCntY);
					}

					//���_�̈ʒu�̐ݒ�

					VtxPos = D3DXVECTOR3(
						(float)g_Sphere[nCnt].fRadius * sinf(fAngle2) * sinf(fAngle), 
						(float)g_Sphere[nCnt].fRadius * cosf(fAngle2), 
						(float)g_Sphere[nCnt].fRadius * sinf(fAngle2) * cosf(fAngle));

					pVtx[indx].pos = VtxPos;

					//�O���ւ̃x�N�g��
					vec = pVtx[indx].pos - g_Sphere[nCnt].pos;

					//�x�N�g���̐��K��,�e���_�̖@���̐ݒ�
					D3DXVec3Normalize(&pVtx[indx].nor, &vec);

					//���_�J���[�̐ݒ�
					pVtx[indx].col = g_Sphere[nCnt].col;

					//�e�N�X�`�����W�̐ݒ�
					pVtx[indx].tex = D3DXVECTOR2((1.0f / g_Sphere[nCnt].DiviX) * nCntX, (1.0f / g_Sphere[nCnt].DiviY) * nCntY);

					// 0�Ԗڂ�������
					if (indx == 0)
					{
						//�e�N�X�`�����W�̐ݒ�
						pVtx[indx].tex = D3DXVECTOR2((1.0f / g_Sphere[nCnt].DiviX) * (g_Sphere[nCnt].DiviX * 0.5f), 0.0f);
					}

					//���_�C���f�b�N�X��i�߂�
					indx++;
					assert(indx >= 0 && indx <= g_Sphere[nCnt].nMaxVtx);

					//���_��0�Ԗڂ������Ȃ�
					if (indx == 1)
					{
						//���[�v�𔲂���
						break;
					}
				}
			}

			//���_�o�b�t�@���A�����b�N
			g_Sphere[nCnt].pVtxBuff->Unlock();

			//�C���f�b�N�X�o�b�t�@�̐ݒ�
			if (g_Sphere[nCnt].DiviY >= 2)
			{
				//�C���f�b�N�X�ւ̃|�C���^
				WORD* pIdx = NULL;
				int indexNum = (2 * ((g_Sphere[nCnt].DiviY - 1) * (2 + g_Sphere[nCnt].DiviX) - 1));													// �C���f�b�N�X
				g_Sphere[nCnt].nPolyNum = (2 * g_Sphere[nCnt].DiviX * (g_Sphere[nCnt].DiviY - 1) + (g_Sphere[nCnt].DiviY - 2) * 4);					// �|���S����

				//�C���f�b�N�X�o�b�t�@�̐���
				pDevice->CreateIndexBuffer(sizeof(WORD) * indexNum,
					D3DUSAGE_WRITEONLY,
					D3DFMT_INDEX16,
					D3DPOOL_MANAGED,
					&g_Sphere[nCnt].pIndxBuff,
					NULL);

				//�C���f�b�N�X�o�b�t�@�����b�N
  				g_Sphere[nCnt].pIndxBuff->Lock(0, 0, (void**)&pIdx, 0);

				int nCntX = 0;
				int Indx = 0;

				for (int nCntY = 0; nCntY < g_Sphere[nCnt].DiviY-1; nCntY++)
				{
					for (nCntX = g_Sphere[nCnt].DiviX; nCntX >= 0; nCntX--)
					{
						//�C���f�b�N�X�̐ݒ�
						pIdx[0] = (g_Sphere[nCnt].DiviX + 1) * (nCntY + 1) + nCntX;
						pIdx[1] = nCntX + (nCntY * (g_Sphere[nCnt].DiviX + 1));

						pIdx += 2;
						Indx += 2;
					}

					//���ރ|���S����
					if (nCntY < g_Sphere[nCnt].DiviY - 2)
					{
						//�C���f�b�N�X�̐ݒ�
						pIdx[0] = (nCntX + 1) + (nCntY * (g_Sphere[nCnt].DiviX + 1));
						pIdx[1] = (g_Sphere[nCnt].DiviX + 1) * (nCntY + 2) + g_Sphere[nCnt].DiviX;

						pIdx += 2;
						Indx += 2;
					}

					assert(Indx <= indexNum);
					assert(nCntY >= 0 && nCntY <= g_Sphere[nCnt].DiviY);
				}

				//�C���f�b�N�X�o�b�t�@���A�����b�N
				g_Sphere[nCnt].pIndxBuff->Unlock();
			}

			//�����g�p���Ă��Ԃɂ���
			g_Sphere[nCnt].bUse = true;
			break;
		}
	}
	return nCnt;						//���̃C���f�b�N�X��Ԃ�
}

//======================================
// ���̈ʒu�̐ݒ菈��
//======================================
void SetSpherePos(int indx, D3DXVECTOR3 pos)
{
	g_Sphere[indx].pos = pos;
}

//======================================
// ���b�V���ǂ̃e�N�X�`���ݒ�
//======================================
void SetSphereTexture(int indx)
{
	//�e�N�X�`���̃|�C���^���擾
	LPDIRECT3DTEXTURE9 pTexture = GetTexture2(g_Sphere[indx].textype);

	if (pTexture != NULL)
	{
		g_pTexture = pTexture;
	}
}

//======================================
// ���̈ʒu�̐ݒ菈��
//======================================
void DeleteSphere(int indx)
{
	//�g�p���Ă��Ȃ���Ԃɂ���
	g_Sphere[indx].bUse = false;
}