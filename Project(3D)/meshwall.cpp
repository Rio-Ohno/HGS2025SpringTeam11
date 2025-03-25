//==============================================
//
// ���b�V���Ǖ\������[meshWall.cpp]
// Author:kaiti
//
//==============================================

#include "meshwall.h"
#include "meshfield.h"

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureMeshWall = NULL;		//�e�N�X�`���ւ̃|�C���^

MeshWall g_MeshWall[MAX_MESHWALL];									//�|���S��(�c)�̍\����
static char walltexName[MAX_TEX_WALL][32];							//�e�N�X�`���t�@�C�����ۑ��p

//=================================
// ���b�V���ǂ̏���������
//=================================
void InitMeshWall(void)
{

	for (int nCnt = 0; nCnt < MAX_MESHWALL; nCnt++)
	{
		g_MeshWall[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_MeshWall[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_MeshWall[nCnt].IdxBuffMeshWall = { NULL };
		g_MeshWall[nCnt].VtxBuffMeshWall = { NULL };
		g_MeshWall[nCnt].textype = 0;
		g_MeshWall[nCnt].nDiviX = 0;
		g_MeshWall[nCnt].nDiviY = 0;
		g_MeshWall[nCnt].nDiviZ = 0;
		g_MeshWall[nCnt].nWidth = 0;
		g_MeshWall[nCnt].nHeight = 0;
		g_MeshWall[nCnt].nMaxVtx = 0;
		g_MeshWall[nCnt].nPolyNum = 0;
		g_MeshWall[nCnt].bUse = false;
	}
}

//===============================
// ���b�V���ǂ̏I������
//===============================
void UninitMeshWall(void)
{
	
	//�e�N�X�`���̔j��
	if (g_pTextureMeshWall != NULL)
	{
		g_pTextureMeshWall->Release();
		g_pTextureMeshWall = NULL;
	}

	for (int nCnt = 0; nCnt < MAX_MESHWALL; nCnt++)
	{
		//���_�o�b�t�@�̔j��
		if (g_MeshWall[nCnt].VtxBuffMeshWall != NULL)
		{
			g_MeshWall[nCnt].VtxBuffMeshWall->Release();
			g_MeshWall[nCnt].VtxBuffMeshWall = NULL;
		}

		//�C���f�b�N�X�o�b�t�@�̔j��
		if (g_MeshWall[nCnt].IdxBuffMeshWall != NULL)
		{
			g_MeshWall[nCnt].IdxBuffMeshWall->Release();
			g_MeshWall[nCnt].IdxBuffMeshWall = NULL;
		}
	}
}

//===============================
// ���b�V���ǂ̍X�V����
//===============================
void UpdateMeshWall(void)
{

}

//===============================
// ���b�V���ǂ̕`�揈��
//===============================
void DrawMeshWall(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;

	for (int nCnt = 0; nCnt < MAX_MESHWALL; nCnt++)
	{
		if (g_MeshWall[nCnt].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_MeshWall[nCnt].mtxWorld);

			//�����𔽓]
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_MeshWall[nCnt].rot.y, g_MeshWall[nCnt].rot.x, g_MeshWall[nCnt].rot.z);
			D3DXMatrixMultiply(&g_MeshWall[nCnt].mtxWorld, &g_MeshWall[nCnt].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_MeshWall[nCnt].pos.x, g_MeshWall[nCnt].pos.y, g_MeshWall[nCnt].pos.z);
			D3DXMatrixMultiply(&g_MeshWall[nCnt].mtxWorld, &g_MeshWall[nCnt].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_MeshWall[nCnt].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_MeshWall[nCnt].VtxBuffMeshWall, 0, sizeof(VERTEX_3D));

			//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(g_MeshWall[nCnt].IdxBuffMeshWall);

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureMeshWall);
			//���b�V���ǂ�`��
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, g_MeshWall[nCnt].nMaxVtx, 0, g_MeshWall[nCnt].nPolyNum);
		}
	}
}

//===================
// ���b�V���ǂ̐ݒ�
//===================
void SetMeshWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int textype, int nDiviX, int nDiviZ, int nDiviY, int nWidth, int nHeight)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	WORD* pIdx = NULL;

	for (int nCnt = 0; nCnt < MAX_MESHWALL; nCnt++)
	{
		if (g_MeshWall[nCnt].bUse == false)
		{
			g_MeshWall[nCnt].pos = pos;						//�ʒu
			g_MeshWall[nCnt].rot = rot;						//����
			g_MeshWall[nCnt].textype = textype;				//�e�N�X�`���^�C�v
			g_MeshWall[nCnt].nDiviX = nDiviX;				//������x
			g_MeshWall[nCnt].nDiviY = nDiviY;				//������y
			g_MeshWall[nCnt].nDiviZ = nDiviZ;				//������z
			g_MeshWall[nCnt].nWidth = nWidth;				//��
			g_MeshWall[nCnt].nHeight = nHeight;				//����
			g_MeshWall[nCnt].bUse = true;					//�g�p���Ă����Ԃɂ���

			g_MeshWall[nCnt].nMaxVtx = (g_MeshWall[nCnt].nDiviX + 1) * (g_MeshWall[nCnt].nDiviY + 1);										//���_��
			g_MeshWall[nCnt].nPolyNum = (2 * g_MeshWall[nCnt].nDiviX * g_MeshWall[nCnt].nDiviY + (g_MeshWall[nCnt].nDiviY - 1) * 4);		//�|���S����
			int indexNum = (2 * (g_MeshWall[nCnt].nDiviY * (2 + g_MeshWall[nCnt].nDiviX) - 1));												//�C���f�b�N�X

			//�e�N�X�`���̐ݒ�
			SetwallTexture(nCnt);

			//���_�o�b�t�@�̐���
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_MeshWall[nCnt].nMaxVtx,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_3D,
				D3DPOOL_MANAGED,
				&g_MeshWall[nCnt].VtxBuffMeshWall,
				NULL);

			VERTEX_3D* pVtx = NULL;

			//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			g_MeshWall[nCnt].VtxBuffMeshWall->Lock(0, 0, (void**)&pVtx, 0);

			int nCntVtx = 0;

			//�����ւ��炷
			float centerX = g_MeshWall[nCnt].nWidth * (g_MeshWall[nCnt].nDiviX - 2) * 0.5f;

			for (int nCntY = 0; nCntY <= g_MeshWall[nCnt].nDiviY; nCntY++)
			{
				for (int nCntX = 0; nCntX <= g_MeshWall[nCnt].nDiviX; nCntX++)
				{
					//���_���W�̐ݒ�
					pVtx[nCntVtx].pos = D3DXVECTOR3((float)(-g_MeshWall[nCnt].nWidth + (g_MeshWall[nCnt].nWidth * nCntX) - centerX), (float)(g_MeshWall[nCnt].nHeight - (g_MeshWall[nCnt].nHeight * nCntY)),0.0f);

					//�@���x�N�g���̐ݒ�
					pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

					//���_�J���[�̐ݒ�
					pVtx[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					//�e�N�X�`�����W�̐ݒ�
					pVtx[nCntVtx].tex = D3DXVECTOR2(1.0f * nCntX, (1.0f / g_MeshWall[nCnt].nDiviY) * nCntY);

					nCntVtx++;
				}
			}

			//���_�o�b�t�@���A�����b�N�@
			g_MeshWall[nCnt].VtxBuffMeshWall->Unlock();

			//�C���f�b�N�X�o�b�t�@�̐���
			pDevice->CreateIndexBuffer(sizeof(WORD) * indexNum,
				D3DUSAGE_WRITEONLY,
				D3DFMT_INDEX16,
				D3DPOOL_MANAGED,
				&g_MeshWall[nCnt].IdxBuffMeshWall,
				NULL);

			//�C���f�b�N�X�o�b�t�@�����b�N
			g_MeshWall[nCnt].IdxBuffMeshWall->Lock(0, 0, (void**)&pIdx, 0);

			int nCntX, nCntY;
			for (nCntY = 0; nCntY < g_MeshWall[nCnt].nDiviY; nCntY++)
			{
				for (nCntX = 0; nCntX <= g_MeshWall[nCnt].nDiviX; nCntX++)
				{
					//�C���f�b�N�X�̐ݒ�
					pIdx[0] = (g_MeshWall[nCnt].nDiviX + 1) * (nCntY + 1) + nCntX;
					pIdx[1] = nCntX + (nCntY * (g_MeshWall[nCnt].nDiviX + 1));

					pIdx += 2;
				}

				if (nCntY <= g_MeshWall[nCnt].nDiviY - 1)
				{
					pIdx[0] = (nCntX - 1) + (nCntY * (g_MeshWall[nCnt].nDiviX + 1));
					pIdx[1] = nCntX + ((nCntY + 1) * (g_MeshWall[nCnt].nDiviX + 1));

					pIdx += 2;
				}
			}

			//�C���f�b�N�X�o�b�t�@�̃A�����b�N
			g_MeshWall[nCnt].IdxBuffMeshWall->Unlock();

			break;
		}
	}
}

//===================
// ���b�V���ǂ̃e�N�X�`���ݒ�
//===================
void SetwallTexture(int indx)
{
	//�e�N�X�`���̃|�C���^���擾
	LPDIRECT3DTEXTURE9 pTexture = GetTexture2(g_MeshWall[indx].textype);

	if (pTexture != NULL)
	{
		g_pTextureMeshWall = pTexture;
	}
}