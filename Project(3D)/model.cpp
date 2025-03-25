//********************************
// ���f�����Ǘ�����[model.cpp]
// Hatori Kensaku
//
//********************************
#include "model.h"
//#include "player.h"
#include "camera.h"
#include "collision.h"
#include <cassert>

// �O���[�o���ϐ�
MODELORIGIN g_ModelOrigin[MODELTYPE_MAX];
STAGEMODEL g_StageModel[MAX_STAGEMODEL];

//******************************
/// �X�e�[�W�ɏo�����f���̏�����
//******************************
void InitStageModel()
{
	for (int ModelCount = 0; ModelCount < MAX_STAGEMODEL; ModelCount++)
	{
		g_StageModel[ModelCount].bUse = false;
		g_StageModel[ModelCount].bHitRayCamera = false;
		g_StageModel[ModelCount].btest = true;
		g_StageModel[ModelCount].nType = MODELTYPE_ZERO;
		g_StageModel[ModelCount].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_StageModel[ModelCount].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_StageModel[ModelCount].ObbModel.CenterPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_StageModel[ModelCount].ObbModel.fLength[0] = 0.0f;
		g_StageModel[ModelCount].ObbModel.fLength[1] = 0.0f;
		g_StageModel[ModelCount].ObbModel.fLength[2] = 0.0f;

		g_StageModel[ModelCount].ObbModel.RotVec[0] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_StageModel[ModelCount].ObbModel.RotVec[1] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_StageModel[ModelCount].ObbModel.RotVec[2] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		g_StageModel[ModelCount].Max = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_StageModel[ModelCount].Min = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	for (int Origin = 0; Origin < MODELTYPE_MAX; Origin++)
	{
		g_ModelOrigin[Origin].dwNumMat = NULL;
		g_ModelOrigin[Origin].pBuffMat = NULL;
		g_ModelOrigin[Origin].pMesh = NULL;
		for (int TexCount = 0; TexCount < MAX_TEX; TexCount++)
		{
			g_ModelOrigin[Origin].pTexture[TexCount] = NULL;
		}
	}
}
//********************************
/// �X�e�[�W�ɏo�����f���̏I������
//********************************
void UninitStageModel()
{
	for (int ModelCount = 0; ModelCount < MODELTYPE_MAX; ModelCount++)
	{
		for (int TexCount = 0; TexCount < MAX_TEX; TexCount++)
		{
			UninitTexture(g_ModelOrigin[ModelCount].pTexture[TexCount]);
		}
		UninitBuffMat(g_ModelOrigin[ModelCount].pBuffMat);
		UninitMesh(g_ModelOrigin[ModelCount].pMesh);
	}
}
//********************************
/// �X�e�[�W�ɏo�����f���̍X�V����
//********************************
void UpdateStageModel()
{
	for (int ModelCount = 0; ModelCount < MAX_STAGEMODEL; ModelCount++)
	{
		if (g_StageModel[ModelCount].bUse == true)
		{
			//Player* pPlayer = GetPlayer();
			Camera* pCamera = GetCamera();
			D3DXVECTOR3 LocalRayPos,LocalRayDirection;
			LocalRayPos = pCamera->posV;
			LocalRayDirection = D3DXVECTOR3(pCamera->posR.x, pCamera->posR.y, pCamera->posR.z) - pCamera->posV;
			D3DXVECTOR3 obb1Center;
			//obb1Center.x = pPlayer->pos.x;
			//obb1Center.y = pPlayer->pos.y + (pPlayer->size.y * 0.5f);
			//obb1Center.z = pPlayer->pos.z;
			SetObbInfo(ModelCount);
			//CollOBBs(g_StageModel[ModelCount].ObbModel, obb1Center, ModelCount);
		}
	}
}
//********************************
/// �X�e�[�W�ɏo�����f���̕`�揈��
//********************************
void DrawStageModel()
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxRot, mtxTrans;
	//���݂̃}�e���A���ۑ��p
	D3DMATERIAL9 matDef;
	//�}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL* pMat;

	for (int ModelCount = 0; ModelCount < MAX_STAGEMODEL; ModelCount++)
	{
		if (g_StageModel[ModelCount].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_StageModel[ModelCount].mtxWorld);

			//�����𔽓]
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_StageModel[ModelCount].rot.y, g_StageModel[ModelCount].rot.x, g_StageModel[ModelCount].rot.z);
			D3DXMatrixMultiply(&g_StageModel[ModelCount].mtxWorld, &g_StageModel[ModelCount].mtxWorld, &mtxRot);

			g_StageModel[ModelCount].ObbModel.RotVec[0] = D3DXVECTOR3(mtxRot._11, mtxRot._12, mtxRot._13);
			g_StageModel[ModelCount].ObbModel.RotVec[1] = D3DXVECTOR3(mtxRot._21, mtxRot._22, mtxRot._23);
			g_StageModel[ModelCount].ObbModel.RotVec[2] = D3DXVECTOR3(mtxRot._31, mtxRot._32, mtxRot._33);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_StageModel[ModelCount].pos.x, g_StageModel[ModelCount].pos.y, g_StageModel[ModelCount].pos.z);
			D3DXMatrixMultiply(&g_StageModel[ModelCount].mtxWorld, &g_StageModel[ModelCount].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_StageModel[ModelCount].mtxWorld);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_ModelOrigin[g_StageModel[ModelCount].nType].pBuffMat->GetBufferPointer();

			for (int ModelMatCount = 0; ModelMatCount < (int)g_ModelOrigin[g_StageModel[ModelCount].nType].dwNumMat; ModelMatCount++)
			{
				if (g_StageModel[ModelCount].bHitRayCamera == true)
				{
					pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);		// Z�o�b�t�@�ɂ͏������܂Ȃ�
					D3DXMATERIAL CameraRayHit;
					CameraRayHit = pMat[ModelMatCount];
					CameraRayHit.MatD3D.Diffuse.a = 0.5f;
					//�}�e���A���̐ݒ�
					pDevice->SetMaterial(&CameraRayHit.MatD3D);
				}
				else
				{
					pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);		// Z�o�b�t�@�ɂ͏������܂Ȃ�
					//�}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[ModelMatCount].MatD3D);
				}
				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_ModelOrigin[g_StageModel[ModelCount].nType].pTexture[ModelMatCount]);
				//���f���̕`��
				g_ModelOrigin[g_StageModel[ModelCount].nType].pMesh->DrawSubset(ModelMatCount);
			}
			pDevice->SetMaterial(&matDef);
		}
	}
}
STAGEMODEL* GetModel()
{
	return &g_StageModel[0];
}
//********************************
/// �X�e�[�W�ɏo�����f���̐ݒ菈��
//********************************
void SetStageModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, MODELTYPE nType)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int ModelCount = 0; ModelCount < MAX_STAGEMODEL; ModelCount++)
	{
		if (g_StageModel[ModelCount].bUse == false)
		{
			g_StageModel[ModelCount].bUse = true;
			g_StageModel[ModelCount].nType = nType;
			g_StageModel[ModelCount].pos = pos;
			g_StageModel[ModelCount].rot = rot;
			g_StageModel[ModelCount].btest = true;
			break;
		}
	}
}
//*****************************************
/// �X�e�[�W�ɏo�����f����OBB���̐ݒ菈��
//*****************************************
void SetObbInfo(int Indx)
{
	LPDIRECT3DDEVICE9 pDevice;
	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	int nNumVtx;   //���_��
	DWORD sizeFVF; //���_�t�H�[�}�b�g�̃T�C�Y
	BYTE* pVtxBuff;//���_�o�b�t�@�ւ̃|�C���^

	//���_���擾
	nNumVtx = g_ModelOrigin[g_StageModel[Indx].nType].pMesh->GetNumVertices();
	//���_�t�H�[�}�b�g�̃T�C�Y�擾
	sizeFVF = D3DXGetFVFVertexSize(g_ModelOrigin[g_StageModel[Indx].nType].pMesh->GetFVF());
	//���_�o�b�t�@�̃��b�N
	g_ModelOrigin[g_StageModel[Indx].nType].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	if (g_StageModel[Indx].btest == true)
	{
		for (int nCnt = 0; nCnt < nNumVtx; nCnt++)
		{
			//���_���W�̑��
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			//���_���W���r���ăv���C���[�̍ŏ��l�A�ő�l���擾
			//�ŏ��l
			if (vtx.x < g_StageModel[Indx].Min.x)
			{
				g_StageModel[Indx].Min.x = vtx.x;
			}
			if (vtx.y < g_StageModel[Indx].Min.y)
			{
				g_StageModel[Indx].Min.y = vtx.y;
			}
			if (vtx.z < g_StageModel[Indx].Min.z)
			{
				g_StageModel[Indx].Min.z = vtx.z;
			}
			//�ő�l
			if (vtx.x > g_StageModel[Indx].Max.x)
			{
				g_StageModel[Indx].Max.x = vtx.x;
			}
			if (vtx.y > g_StageModel[Indx].Max.y)
			{
				g_StageModel[Indx].Max.y = vtx.y;
			}
			if (vtx.z > g_StageModel[Indx].Max.z)
			{
				g_StageModel[Indx].Max.z = vtx.z;
			}
			//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
			pVtxBuff += sizeFVF;
		}
	}

	//���_�o�b�t�@�̃A�����b�N
	g_ModelOrigin[g_StageModel[Indx].nType].pMesh->UnlockVertexBuffer();

	D3DXMATRIX mtxRotModel, mtxTransModel;
	D3DXMATRIX mtxParent;
	D3DXMatrixIdentity(&g_StageModel[Indx].ObbModel.CenterMtx);

	// �����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRotModel, g_StageModel[Indx].rot.y, g_StageModel[Indx].rot.x, g_StageModel[Indx].rot.z);
	D3DXMatrixMultiply(&g_StageModel[Indx].ObbModel.CenterMtx, &g_StageModel[Indx].ObbModel.CenterMtx, &mtxRotModel);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTransModel, 0.0f, g_StageModel[Indx].Max.y * 0.5f, 0.0f);
	D3DXMatrixMultiply(&g_StageModel[Indx].ObbModel.CenterMtx, &g_StageModel[Indx].ObbModel.CenterMtx, &mtxTransModel);

	mtxParent = g_StageModel[Indx].mtxWorld;

	D3DXMatrixMultiply(&g_StageModel[Indx].ObbModel.CenterMtx,
		&g_StageModel[Indx].ObbModel.CenterMtx,
		&mtxParent);

	pDevice->SetTransform(D3DTS_WORLD,
		&g_StageModel[Indx].ObbModel.CenterMtx);

	// ���S�_
	g_StageModel[Indx].ObbModel.CenterPos.x = g_StageModel[Indx].ObbModel.CenterMtx._41;
	g_StageModel[Indx].ObbModel.CenterPos.y = g_StageModel[Indx].ObbModel.CenterMtx._42;
	g_StageModel[Indx].ObbModel.CenterPos.z = g_StageModel[Indx].ObbModel.CenterMtx._43;

	// �e���W���̌X����\���x�N�g�����v�Z
	g_StageModel[Indx].ObbModel.RotVec[0] = D3DXVECTOR3(mtxRotModel._11, mtxRotModel._12, mtxRotModel._13);
	g_StageModel[Indx].ObbModel.RotVec[1] = D3DXVECTOR3(mtxRotModel._21, mtxRotModel._22, mtxRotModel._23);
	g_StageModel[Indx].ObbModel.RotVec[2] = D3DXVECTOR3(mtxRotModel._31, mtxRotModel._32, mtxRotModel._33);

	// ���S�_����ʂւ̋����̔���
	g_StageModel[Indx].ObbModel.fLength[0] = fabsf(g_StageModel[Indx].Max.x - g_StageModel[Indx].Min.x) * 0.5f;
	g_StageModel[Indx].ObbModel.fLength[1] = fabsf(g_StageModel[Indx].Max.y - g_StageModel[Indx].Min.y) * 0.5f;
	g_StageModel[Indx].ObbModel.fLength[2] = fabsf(g_StageModel[Indx].Max.z - g_StageModel[Indx].Min.z) * 0.5f;
}
//******************************************
/// �X�e�[�W�ɏo�����f���̃o�b�t�@�[�ݒ菈��
//******************************************
void SetStageModelInfo(char *ModelPath[], int PathType)
{
	HRESULT Hresult;
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	const char* pFile = ModelPath[0];

	//X�t�@�C���̓ǂݍ���
	Hresult = D3DXLoadMeshFromX(pFile,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_ModelOrigin[PathType].pBuffMat,
		NULL,
		&g_ModelOrigin[PathType].dwNumMat,
		&g_ModelOrigin[PathType].pMesh);
	
	if (FAILED(Hresult))
	{
		return;
	}

	D3DXMATERIAL* pMat;//�}�e���A���ւ̃|�C���^
	pMat = (D3DXMATERIAL*)g_ModelOrigin[PathType].pBuffMat->GetBufferPointer();
	for (int nCntBlockMat = 0; nCntBlockMat < (int)g_ModelOrigin[PathType].dwNumMat; nCntBlockMat++)
	{
		if (pMat[nCntBlockMat].pTextureFilename != NULL)
		{
			D3DXCreateTextureFromFile(pDevice, 
				pMat[nCntBlockMat].pTextureFilename, 
				&g_ModelOrigin[PathType].pTexture[nCntBlockMat]); //1
		}
	}
}
//*************************
/// OBB�ƓV�̓����蔻�菈��
//*************************
void LenOBBToPoint(OBB& obb, D3DXVECTOR3& p)
{
	D3DXVECTOR3 Vec(0, 0, 0);   // �ŏI�I�ɒ��������߂�x�N�g��

   // �e���ɂ��Ă͂ݏo�������̃x�N�g�����Z�o
	for (int DimensionCount = 0; DimensionCount < 3; DimensionCount++)
	{
		FLOAT L = obb.fLength[DimensionCount];
		if (L <= 0) continue;  // L=0�͌v�Z�ł��Ȃ�
		D3DXVECTOR3 test = p - obb.CenterPos;
		FLOAT s = D3DXVec3Dot(&test, &obb.RotVec[DimensionCount]) / L;

		// s�̒l����A�͂ݏo��������������΂��̃x�N�g�������Z
		s = (float)fabs(s);
		if (s > 1)
			Vec += (1 - s) * L * obb.RotVec[DimensionCount];   // �͂ݏo���������̃x�N�g���Z�o
	}
	FLOAT fDistance = D3DXVec3Length(&Vec);   // �������o��
	if (fDistance < 10)
	{
		//Player* pPlayer = GetPlayer();
		//pPlayer->pos = pPlayer->posOld;
	}
}
////**************************
///// OBB��OBB�̓����蔻�菈��
////**************************
//void CollOBBs(OBB& obb, D3DXVECTOR3& p,int Indx)
//{
//	Player* pPlayer = GetPlayer();
//	// �e�����x�N�g���̊m��
//	   // �iN***:�W���������x�N�g���j
//	D3DXVECTOR3 NAe1 = obb.RotVec[0], Ae1 = NAe1 * obb.fLength[0];	// X��
//	D3DXVECTOR3 NAe2 = obb.RotVec[1], Ae2 = NAe2 * obb.fLength[1];	// Y��
//	D3DXVECTOR3 NAe3 = obb.RotVec[2], Ae3 = NAe3 * obb.fLength[2];	// Z��
//	D3DXVECTOR3 NBe1 = D3DXVECTOR3(1.0f, 0.0f, 0.0f), Be1 = NBe1 * pPlayer->size.x * 0.5f;
//	D3DXVECTOR3 NBe2 = D3DXVECTOR3(0.0f, 1.0f, 0.0f), Be2 = NBe2 * pPlayer->size.y * 0.5f;
//	D3DXVECTOR3 NBe3 = D3DXVECTOR3(0.0f, 0.0f, 1.0f), Be3 = NBe3 * pPlayer->size.z * 0.5f;
//
//	D3DXVECTOR3 r;
//	D3DXVECTOR3 MathX;
//	D3DXVECTOR3 MathY;
//	D3DXVECTOR3 MathZ;
//	D3DXVECTOR3 MathPos;
//	MathPos = obb.CenterPos - p;
//	MathX = obb.RotVec[0] * obb.fLength[0];
//	MathY = obb.RotVec[1] * obb.fLength[1];
//	MathZ = obb.RotVec[2] * obb.fLength[2];
//	r.x = (float)fabs(D3DXVec3Dot(&MathX, &NBe1));
//	r.y = (float)fabs(D3DXVec3Dot(&MathY, &NBe2));
//	r.z = (float)fabs(D3DXVec3Dot(&MathZ, &NBe3));
//
//	D3DXVECTOR3 Interval = MathPos;
//	bool bCollision = true;
//
//	// ������ : Ae1
//	FLOAT rA = D3DXVec3Length(&Ae1);
//	FLOAT rB = LenSegOnSeparateAxis(&NAe1, &Be1, &Be2, &Be3);
//	FLOAT L = (float)fabs(D3DXVec3Dot(&Interval, &NAe1));
//	if (L > rA + rB)
//	{
//		bCollision = false;
//	}
//
//	// ������ : Ae2
//	rA = D3DXVec3Length(&Ae2);
//	rB = LenSegOnSeparateAxis(&NAe2, &Be1, &Be2, &Be3);
//	L = (float)fabs(D3DXVec3Dot(&Interval, &NAe2));
//	if (L > rA + rB)
//	{
//		bCollision = false;
//	}
//	// ������ : Ae3
//	rA = D3DXVec3Length(&Ae3);
//	rB = LenSegOnSeparateAxis(&NAe3, &Be1, &Be2, &Be3);
//	L = (float)fabs(D3DXVec3Dot(&Interval, &NAe3));
//	if (L > rA + rB)
//	{
//		bCollision = false;
//	}
//
//	// ������ : Be1
//	rA = LenSegOnSeparateAxis(&NBe1, &Ae1, &Ae2, &Ae3);
//	rB = D3DXVec3Length(&Be1);
//	L = (float)fabs(D3DXVec3Dot(&Interval, &NBe1));
//	if (L > rA + rB)
//	{
//		bCollision = false;
//	}
//
//	// ������ : Be2
//	rA = LenSegOnSeparateAxis(&NBe2, &Ae1, &Ae2, &Ae3);
//	rB = D3DXVec3Length(&Be2);
//	L = (float)fabs(D3DXVec3Dot(&Interval, &NBe2));
//	if (L > rA + rB)
//	{
//		bCollision = false;
//	}
//
//	// ������ : Be3
//	rA = LenSegOnSeparateAxis(&NBe3, &Ae1, &Ae2, &Ae3);
//	rB = D3DXVec3Length(&Be3);
//	L = (float)fabs(D3DXVec3Dot(&Interval, &NBe3));
//	if (L > rA + rB)
//	{
//		bCollision = false;
//	}
//
//	// ������ : C11
//	D3DXVECTOR3 Cross;
//	D3DXVec3Cross(&Cross, &NAe1, &NBe1);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
//	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
//	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//	{
//		bCollision = false;
//	}
//
//	// ������ : C12
//	D3DXVec3Cross(&Cross, &NAe1, &NBe2);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
//	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
//	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//	{
//		bCollision = false;
//	}
//
//	// ������ : C13
//	D3DXVec3Cross(&Cross, &NAe1, &NBe3);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae2, &Ae3);
//	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
//	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//	{
//		bCollision = false;
//	}
//
//	// ������ : C21
//	D3DXVec3Cross(&Cross, &NAe2, &NBe1);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
//	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
//	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//	{
//		bCollision = false;
//	}
//
//	// ������ : C22
//	D3DXVec3Cross(&Cross, &NAe2, &NBe2);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
//	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
//	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//	{
//		bCollision = false;
//	}
//
//	// ������ : C23
//	D3DXVec3Cross(&Cross, &NAe2, &NBe3);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae3);
//	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
//	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//	{
//		bCollision = false;
//	}
//
//	// ������ : C31
//	D3DXVec3Cross(&Cross, &NAe3, &NBe1);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
//	rB = LenSegOnSeparateAxis(&Cross, &Be2, &Be3);
//	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//	{
//		bCollision = false;
//	}
//
//	// ������ : C32
//	D3DXVec3Cross(&Cross, &NAe3, &NBe2);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
//	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be3);
//	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//	{
//		bCollision = false;
//	}
//
//	// ������ : C33
//	D3DXVec3Cross(&Cross, &NAe3, &NBe3);
//	rA = LenSegOnSeparateAxis(&Cross, &Ae1, &Ae2);
//	rB = LenSegOnSeparateAxis(&Cross, &Be1, &Be2);
//	L = (float)fabs(D3DXVec3Dot(&Interval, &Cross));
//	if (L > rA + rB)
//	{
//		bCollision = false;
//	}
//
//	if (bCollision == true)
//	{
//		// �������Ă邩�킩��Ȃ�
//		// X���̖ʂɓ������Ă��邩
//		D3DXVECTOR3 IntervalX;
//		D3DXVECTOR3 norX;
//		FLOAT fDotX;
//		D3DXVECTOR3 IntervalX1;
//		D3DXVECTOR3 norX1;
//		D3DXVECTOR3 Math;
//		FLOAT fDotX1;
//
//		// X���̃}�C�i�X�����̃x�N�g��
//		Math = -g_StageModel[Indx].ObbModel.RotVec[0];
//		// X���̕����x�N�g��(�@��)
//		D3DXVec3Normalize(&norX, &g_StageModel[Indx].ObbModel.RotVec[0]);
//		// X���̃}�C�i�X�����̖@���x�N�g��
//		D3DXVec3Normalize(&norX1, &Math);
//
//		// �v���C���[�ۂ�����X���̖ʂ̒��S�|�X���������l
//		IntervalX = p - g_StageModel[Indx].ObbModel.CenterPos + 
//			(norX * g_StageModel[Indx].ObbModel.fLength[0]);
//		// �v���C���[�̃|�X����X���̃}�C�i�X�����̒��S�|�X���������l
//		IntervalX1 = p - g_StageModel[Indx].ObbModel.CenterPos +
//			(norX1 * g_StageModel[Indx].ObbModel.fLength[0]);
//
//		// ����(�}�C�i�X���Ɠ������Ă���)
//		fDotX = D3DXVec3Dot(&IntervalX, &norX);
//		// ����(�}�C�i�X���Ɠ������Ă���)
//		fDotX1 = D3DXVec3Dot(&IntervalX1, &norX1);
//
//		// Y���̖ʂɓ������Ă��邩
//		D3DXVECTOR3 IntervalY;
//		D3DXVECTOR3 norY;
//		FLOAT fDotY;
//		D3DXVECTOR3 IntervalY1;
//		D3DXVECTOR3 norY1;
//		D3DXVECTOR3 MathY;
//		FLOAT fDotY1;
//
//		// Y���̃}�C�i�X�����̃x�N�g��
//		MathY = g_StageModel[Indx].ObbModel.RotVec[1] * -1.0f;
//		// Y���̕����x�N�g��(�@��)
//		D3DXVec3Normalize(&norY, &g_StageModel[Indx].ObbModel.RotVec[1]);
//		// Y���̃}�C�i�X�����̃x�N�g��(�@��)
//		D3DXVec3Normalize(&norY1, &MathY);
//
//		// �v���C���[�ۂ�����Y���̖ʂ̒��S�|�X���������l
//		IntervalY = p - g_StageModel[Indx].ObbModel.CenterPos +
//			(norY * g_StageModel[Indx].ObbModel.fLength[1]);
//		// �v���C���[�ۂ�����Y���̃}�C�i�X�����̖ʂ̒��S�|�X���������l
//		IntervalY1 = p - g_StageModel[Indx].ObbModel.CenterPos +
//			(norY1 * g_StageModel[Indx].ObbModel.fLength[1]);
//
//		// ����(�}�C�i�X���Ɠ������Ă���)
//		fDotY = D3DXVec3Dot(&IntervalY, &norY);
//		// ����(�}�C�i�X���Ɠ������Ă���)
//		fDotY1 = D3DXVec3Dot(&IntervalY1, &norY1);
//
//		// Z���̖ʂɓ������Ă��邩
//		D3DXVECTOR3 IntervalZ;
//		D3DXVECTOR3 norZ;
//		FLOAT fDotZ;
//		D3DXVECTOR3 IntervalZ1;
//		D3DXVECTOR3 norZ1;
//		D3DXVECTOR3 MathZ;
//		FLOAT fDotZ1;
//
//		// Z���̃}�C�i�X�����̃x�N�g��
//		MathZ = -g_StageModel[Indx].ObbModel.RotVec[2];
//		// Z���̕����x�N�g��(�@��)
//		D3DXVec3Normalize(&norZ, &g_StageModel[Indx].ObbModel.RotVec[2]);
//		// Z���̃}�C�i�X�̕����x�N�g��(�@��)
//		D3DXVec3Normalize(&norZ1, &MathZ);
//
//		// �v���C���[�ۂ�����Z���̖ʂ̒��S�|�X���������l
//		IntervalZ = p - g_StageModel[Indx].ObbModel.CenterPos + 
//			(norZ * g_StageModel[Indx].ObbModel.fLength[2]);
//		// �v���C���[�ۂ�����Z���̃}�C�i�X�����̖ʂ̒��S�|�X���������l
//		IntervalZ1 = p - g_StageModel[Indx].ObbModel.CenterPos +
//			(norZ1 * g_StageModel[Indx].ObbModel.fLength[2]);
//
//		// ����(�}�C�i�X���Ɠ������Ă���)
//		fDotZ = D3DXVec3Dot(&IntervalZ, &norZ);
//		// ����(�}�C�i�X���Ɠ������Ă���)
//		fDotZ1 = D3DXVec3Dot(&IntervalZ1, &norZ1);
//
//		if (fDotY >= 0)
//		{
//			if (fDotX < fDotX1 && fDotX < fDotZ && fDotX < fDotZ1)
//			{
//				D3DXVECTOR3 pVec = pPlayer->posOld - pPlayer->pos;
//				D3DXVECTOR3 nor = norX;
//				D3DXVec3Normalize(&nor, &nor);
//				FLOAT osiete = D3DXVec3Dot(&pVec, &nor);
//				D3DXVECTOR3 test1 = nor * D3DXVec3Dot(&pVec, &nor);
//				if (osiete <= 0.0f)
//				{
//					pPlayer->pos += test1;
//				}
//			}
//			if (fDotX1 < fDotX && fDotX1 < fDotZ && fDotX1 < fDotZ1)
//			{
//				D3DXVECTOR3 pVec = pPlayer->posOld - pPlayer->pos;
//				D3DXVECTOR3 nor = norX1;
//				D3DXVec3Normalize(&nor, &nor);
//				FLOAT osiete = D3DXVec3Dot(&pVec, &nor);
//				D3DXVECTOR3 test1 = nor * D3DXVec3Dot(&pVec, &nor);
//				if (osiete <= 0.0f)
//				{
//					pPlayer->pos += test1;
//				}
//			}
//			if (fDotZ < fDotZ1 && fDotZ <= fDotX && fDotZ <= fDotX1)
//			{
//				D3DXVECTOR3 pVec = pPlayer->posOld - pPlayer->pos;
//				D3DXVECTOR3 nor = norZ;
//				D3DXVec3Normalize(&nor, &nor);
//				FLOAT osiete = D3DXVec3Dot(&pVec, &nor);
//				D3DXVECTOR3 test1 = nor * D3DXVec3Dot(&pVec, &nor);
//				if (osiete <= 0.0f)
//				{
//					pPlayer->pos += test1;
//				}
//			}
//			if (fDotZ1 < fDotZ && fDotZ1 < fDotX && fDotZ1 < fDotX1)
//			{
//				D3DXVECTOR3 pVec = pPlayer->posOld - pPlayer->pos;
//				D3DXVECTOR3 nor = norZ1;
//				D3DXVec3Normalize(&nor, &nor);
//				FLOAT osiete = D3DXVec3Dot(&pVec, &nor);
//				D3DXVECTOR3 test1 = nor * D3DXVec3Dot(&pVec, &nor);
//				if (osiete <= 0.0f)
//				{
//					pPlayer->pos += test1;
//				}
//			}
//		}
//		// �������
//		if (fDotY <= 1.0f && fDotX >= -2.0f && fDotX1 >= -2.0f && fDotZ >= -2.0f && fDotZ1 >= -2.0f)
//		{
//			D3DXVECTOR3 pVec = pPlayer->posOld - pPlayer->pos;
//			D3DXVECTOR3 nor = norY1;
//			D3DXVec3Normalize(&nor, &nor);
//			FLOAT osiete = D3DXVec3Dot(&pVec, &nor);
//			D3DXVECTOR3 test1 = nor * D3DXVec3Dot(&pVec, &nor);
//			if (osiete >= 0.0f)
//			{
//				pPlayer->pos += test1;
//			}
//		}
//		// �����火
//		if (fDotY1 <= 1.0f && fDotX >= -2.0f && fDotX1 >= -2.0f && fDotZ >= -2.0f && fDotZ1 >= -2.0f)
//		{
//			D3DXVECTOR3 pVec = pPlayer->posOld - pPlayer->pos;
//			D3DXVECTOR3 nor = norY;
//			D3DXVec3Normalize(&nor, &nor);
//			FLOAT osiete = D3DXVec3Dot(&pVec, &nor);
//			D3DXVECTOR3 test1 = nor * D3DXVec3Dot(&pVec, &nor);
//			if (osiete >= 0.0f)
//			{
//				pPlayer->pos = pPlayer->pos + test1;
//				pPlayer->bLanding = true;
//			}
//		}
//		else
//		{
//			pPlayer->bLanding = false;
//		}
//		//else
//		//{
//		//	if (fDotY >= 0)
//		//	{
//				//D3DXVECTOR3 pVec = pPlayer->posOld - pPlayer->pos;
//				//D3DXVECTOR3 nor = norY;
//				//D3DXVec3Normalize(&nor, &nor);
//				//D3DXVECTOR3 test1 = nor * D3DXVec3Dot(&pVec, &nor);
//				//pPlayer->pos += test1;
//		//	}
//		//	else if (fDotY1 >= 0)
//		//	{
//		//		D3DXVECTOR3 pVec = pPlayer->posOld - pPlayer->pos;
//		//		D3DXVECTOR3 nor = norY1;
//		//		D3DXVec3Normalize(&nor, &nor);
//		//		D3DXVECTOR3 test1 = nor * D3DXVec3Dot(&pVec, &nor);
//		//		pPlayer->pos += test1;
//		//	}
//		//}
//	}
//}
// �������ɓ��e���ꂽ���������瓊�e���������Z�o
FLOAT LenSegOnSeparateAxis(D3DXVECTOR3* Sep, D3DXVECTOR3* e1, D3DXVECTOR3* e2, D3DXVECTOR3* e3)
{
	// 3�̓��ς̐�Βl�̘a�œ��e���������v�Z
	// ������Sep�͕W��������Ă��邱��
	FLOAT r1 = (float)fabs(D3DXVec3Dot(Sep, e1));
	FLOAT r2 = (float)fabs(D3DXVec3Dot(Sep, e2));
	FLOAT r3 = e3 ? (float)fabs(D3DXVec3Dot(Sep, e3)) : 0;
	return r1 + r2 + r3;
}