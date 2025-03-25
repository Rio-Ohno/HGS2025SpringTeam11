//==============================================
//
// ���b�V�����\������[meshfield.cpp]
// Author: Rio Ohno
//
//==============================================

#include "meshfield.h"
#include <string.h>
#include <cassert>

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureMeshfield[MAX_TEX_FIELD] = { NULL };		//�e�N�X�`���ւ̃|�C���^

MeshField g_Meshfield[MESH_NUM_MAX];									//�|���S��(��)�̍\����
static char fieldtexName[MAX_TEX_FIELD][256] = { NULL };					//�e�N�X�`���t�@�C�����ۑ��p

//=================================
// ���b�V�����̏���������
//=================================
void InitMeshfield(void)
{
	for (int nCnt = 0; nCnt < MESH_NUM_MAX; nCnt++)
	{
		//�\���̂̏�����
		g_Meshfield[nCnt].pos = D3DXVECTOR3(-900.0f, 0.0f, 900.0f);		//�ʒu
		g_Meshfield[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//����
		g_Meshfield[nCnt].IdxBuffMeshField = { NULL };					//�C���f�b�N�X�o�b�t�@
		g_Meshfield[nCnt].pVtxBuffMeshfield = { NULL };					//���_�o�b�t�@
		g_Meshfield[nCnt].textype = 0;									//�e�N�X�`���^�C�v
		g_Meshfield[nCnt].nDiviX = 0;									//������x(0�ɂ�����nullptr)
		g_Meshfield[nCnt].nDiviY = 0;									//������y
		g_Meshfield[nCnt].nDiviZ = 0;									//������z(0�ɂ�����nullptr)
		g_Meshfield[nCnt].nWidth = 0;									//��
		g_Meshfield[nCnt].nHeight = 0;									//����
		g_Meshfield[nCnt].nMaxVtx = 0;									//���_��
		g_Meshfield[nCnt].nPolyNum = 0;									//�|���S����
		g_Meshfield[nCnt].bUse = false;									//�g�p���Ă��Ȃ���Ԃɂ���
	}
}

//===============================
// ���b�V�����̏I������
//===============================
void UninitMeshfield(void)
{
	//�e�N�X�`���̔j��
	for (int nCnt = 0; nCnt < MAX_TEX_FIELD; nCnt++)
	{
		if (g_pTextureMeshfield[nCnt] != NULL)
		{
			g_pTextureMeshfield[nCnt]->Release();
			g_pTextureMeshfield[nCnt] = NULL;
		}
	}

	for (int nCnt = 0; nCnt < MESH_NUM_MAX; nCnt++)
	{
		//���_�o�b�t�@�̔j��
		if (g_Meshfield[nCnt].pVtxBuffMeshfield != NULL)
		{
			g_Meshfield[nCnt].pVtxBuffMeshfield->Release();
			g_Meshfield[nCnt].pVtxBuffMeshfield = NULL;
		}

		//�C���f�b�N�X�o�b�t�@�̔j��
		if (g_Meshfield[nCnt].IdxBuffMeshField != NULL)
		{
			g_Meshfield[nCnt].IdxBuffMeshField->Release();
			g_Meshfield[nCnt].IdxBuffMeshField = NULL;
		}
	}
}

//===============================
// ���b�V�����̍X�V����
//===============================
void UpdateMeshfield(void)
{

}

//===============================
// ���b�V�����̕`�揈��
//===============================
void DrawMeshfield(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	
	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	for (int nCnt = 0; nCnt < MESH_NUM_MAX; nCnt++)
	{
		if (g_Meshfield[nCnt].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_Meshfield[nCnt].mtxWorld);

			//�����𔽓]
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Meshfield[nCnt].rot.y, g_Meshfield[nCnt].rot.x, g_Meshfield[nCnt].rot.z);
			D3DXMatrixMultiply(&g_Meshfield[nCnt].mtxWorld, &g_Meshfield[nCnt].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_Meshfield[nCnt].pos.x, g_Meshfield[nCnt].pos.y, g_Meshfield[nCnt].pos.z);
			D3DXMatrixMultiply(&g_Meshfield[nCnt].mtxWorld, &g_Meshfield[nCnt].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_Meshfield[nCnt].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_Meshfield[nCnt].pVtxBuffMeshfield, 0, sizeof(VERTEX_3D));

			//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(g_Meshfield[nCnt].IdxBuffMeshField);

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureMeshfield[g_Meshfield[nCnt].textype]);

			//���b�V������`��
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_Meshfield[nCnt].nMaxVtx, 0, g_Meshfield[nCnt].nPolyNum);
		}
	}
}

//===============================
// ���b�V�����̐ݒ菈��
//===============================
void SetMeshfield(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int textype, int nDiviX,int nDiviY, int nDiviZ, int nWidth, int nHeight)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_���ւ̃|�C���^
	VERTEX_3D* pVtx = NULL;

	//�C���f�b�N�X�ւ̃|�C���^
	WORD* pIdx = NULL;

	for (int nCnt = 0; nCnt < MESH_NUM_MAX; nCnt++)
	{
		if (g_Meshfield[nCnt].bUse==false)
		{
			g_Meshfield[nCnt].pos = pos;						//�ʒu
			g_Meshfield[nCnt].rot = rot;						//����
			g_Meshfield[nCnt].textype = textype;				//�e�N�X�`���^�C�v
			g_Meshfield[nCnt].nDiviX = nDiviX;					//������x
			g_Meshfield[nCnt].nDiviY = nDiviY;					//������y
			g_Meshfield[nCnt].nDiviZ = nDiviZ;					//������z
			g_Meshfield[nCnt].nWidth = nWidth;					//��
			g_Meshfield[nCnt].nHeight = nHeight;				//����
			g_Meshfield[nCnt].bUse = true;						//�g�p���Ă����Ԃɂ���(2 * g_MeshWall[nCnt].nDiviX * g_MeshWall[nCnt].nDiviY + (g_MeshWall[nCnt].nDiviY - 1) * 4)

			g_Meshfield[nCnt].nMaxVtx = (g_Meshfield[nCnt].nDiviX + 1) * (g_Meshfield[nCnt].nDiviZ + 1);											//���_��
			g_Meshfield[nCnt].nPolyNum = (2 * g_Meshfield[nCnt].nDiviX * g_Meshfield[nCnt].nDiviZ + (g_Meshfield[nCnt].nDiviZ - 1) * 4);			//�|���S����
			int flindexNum = (2 * (g_Meshfield[nCnt].nDiviZ * (2 + g_Meshfield[nCnt].nDiviX) - 1));													//�C���f�b�N�X

			//���_�o�b�t�@�̐���
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_Meshfield[nCnt].nMaxVtx,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_Meshfield[nCnt].pVtxBuffMeshfield,
				NULL);

			//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			g_Meshfield[nCnt].pVtxBuffMeshfield->Lock(0, 0, (void**)&pVtx, 0);

			int nCntVtx = 0;

			//�����ւ��炷
			float centerX = g_Meshfield[nCnt].nWidth * (g_Meshfield[nCnt].nDiviX - 2) * 0.5f;
			float centerY = g_Meshfield[nCnt].nHeight * (g_Meshfield[nCnt].nDiviZ - 2) * 0.5f;

			//���_���̐ݒ�
			for (int nCntZ = 0; nCntZ <= g_Meshfield[nCnt].nDiviZ; nCntZ++)
			{
				for (int nCntX = 0; nCntX <= g_Meshfield[nCnt].nDiviX; nCntX++)
				{
					//���_���W�̐ݒ�
					pVtx[nCntVtx].pos = D3DXVECTOR3((float)(-g_Meshfield[nCnt].nWidth + (g_Meshfield[nCnt].nWidth * nCntX) - centerX), 0.0f, (float)(g_Meshfield[nCnt].nHeight - (g_Meshfield[nCnt].nHeight * nCntZ) + centerY));

					//�@���x�N�g���̐ݒ�
					pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

					//���_�J���[�̐ݒ�
					pVtx[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					//�e�N�X�`�����W�̐ݒ�
					pVtx[nCntVtx].tex = D3DXVECTOR2(1.0f * nCntX, 1.0f * nCntZ);

					nCntVtx++;
				}
			}
			assert(nCntVtx <= g_Meshfield[nCnt].nMaxVtx);

			//���_�o�b�t�@���A�����b�N�@
			g_Meshfield[nCnt].pVtxBuffMeshfield->Unlock();

				//�C���f�b�N�X�o�b�t�@�̐���
			pDevice->CreateIndexBuffer(sizeof(WORD) * flindexNum,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_Meshfield[nCnt].IdxBuffMeshField,
				NULL);

			//�C���f�b�N�X�o�b�t�@�����b�N
			g_Meshfield[nCnt].IdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

			int nCntX;
			int Indx = 0;
			for (int nCntZ = 0; nCntZ < g_Meshfield[nCnt].nDiviZ; nCntZ++)
			{
				for (nCntX = 0; nCntX <= g_Meshfield[nCnt].nDiviX; nCntX++)
				{
					//�C���f�b�N�X�̐ݒ�
					pIdx[0] = (g_Meshfield[nCnt].nDiviX + 1) * (nCntZ + 1) + nCntX;
					pIdx[1] = nCntX + (nCntZ * (g_Meshfield[nCnt].nDiviX + 1));

					pIdx += 2;
					Indx += 2;
				}

				if (nCntZ < g_Meshfield[nCnt].nDiviZ - 1)
				{
					pIdx[0] = (nCntX - 1) + (nCntZ * (g_Meshfield[nCnt].nDiviX + 1));
					pIdx[1] = nCntX + ((nCntZ + 1) * (g_Meshfield[nCnt].nDiviX + 1));

					pIdx += 2;
					Indx += 2;
				}
			}
			assert(Indx <= flindexNum);

			//�C���f�b�N�X�o�b�t�@�̃A�����b�N
			g_Meshfield[nCnt].IdxBuffMeshField->Unlock();

			break;
		}
	}
}

//===============================
// ���b�V�����̃e�N�X�`���ݒ菈��
//===============================
void SetfieldTexture(char* pFileName, int TexIndx)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	HRESULT hresult;

	strcpy(fieldtexName[TexIndx], pFileName);

	//�e�N�X�`���̓Ǎ�
	hresult = D3DXCreateTextureFromFile(pDevice,
		&fieldtexName[TexIndx][0],
		&g_pTextureMeshfield[TexIndx]);

	if (FAILED(hresult))
	{
		return;
	}
}

////===============================
//// ���b�V�����̃e�N�X�`���擾����
////===============================
//LPDIRECT3DTEXTURE9* GetfieldTexture()
//{
//	return &g_pTextureMeshfield[0];
//}

//================================
//�e�N�X�`���̎擾����
//================================
LPDIRECT3DTEXTURE9 GetTexture2(int TexType)
{
	return g_pTextureMeshfield[(int)TexType];
}
