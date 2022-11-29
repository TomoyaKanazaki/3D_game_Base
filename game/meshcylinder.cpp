//==========================================
//
//  ���b�V���V�����_�[�̐���(meshcylinder.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "meshcylinder.h"
#include "polygon.h"
#include "texture.h"

//==========================================
//  �\���̒�`
//==========================================
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld; //���[���h�}�g���b�N�X
	int nShadowNum;
}MeshCylinder;

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshCylinder = NULL;
LPDIRECT3DTEXTURE9 g_pTextureMeshCylinder = NULL;
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshCylinder = NULL;
MeshCylinder g_aMeshCylinder; //���̏��
D3DXVECTOR3 g_aCollisionPos[((int)MAX_VERTEX_CYLINDER)] = {}; //���_���W

//==========================================
//  ����������
//==========================================
void InitMeshCylinder(void)
{
	//�e��ϐ��̏�����
	g_aMeshCylinder.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshCylinder.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshCylinder.nShadowNum = -1;

	//�|���S���̏�����
	g_pVtxBuffMeshCylinder = InitPolygon(20);

	//�e�N�X�`���̏�����
	g_pTextureMeshCylinder = InitTextureJpg("stellar_into_the_galaxy");

	//���_�o�b�t�@�̌Ăяo��
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	for (int nCntVtx = 0; nCntVtx < MAX_VERTEX_CYLINDER * 0.5f; nCntVtx++)
	{
		pVtx[nCntVtx].pos = D3DXVECTOR3
		(
			sinf(D3DX_PI * (1.0f - nCntVtx * (1.0f / VERTEX_ANGLE))) * CYLINDER_SIZE,
			CYLINDER_HEIGHT, 
			cosf(D3DX_PI * (1.0f - nCntVtx * (1.0f / VERTEX_ANGLE))) * CYLINDER_SIZE
		);

		pVtx[nCntVtx + (int)(MAX_VERTEX_CYLINDER * 0.5f)].pos = D3DXVECTOR3
		(
			sinf(D3DX_PI * (1.0f - nCntVtx * (1.0f / VERTEX_ANGLE))) * CYLINDER_SIZE,
			0.0f,
			cosf(D3DX_PI * (1.0f - nCntVtx * (1.0f / VERTEX_ANGLE))) * CYLINDER_SIZE
		);
	}

	//���_���̕ۑ�
	for (int nCntVtx = 0; nCntVtx < MAX_VERTEX_CYLINDER; nCntVtx++)
	{
		g_aCollisionPos[nCntVtx] = pVtx[nCntVtx].pos;
	}

	//�@���x�N�g���̐ݒ�
	for (int nCntNor = 0; nCntNor < MAX_VERTEX_CYLINDER; nCntNor++)
	{
		D3DXVECTOR3 nor = pVtx[nCntNor].pos;
		D3DXVec3Normalize(&pVtx[nCntNor].nor, &nor);
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffMeshCylinder->Unlock();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer
	(
		sizeof(WORD) * MAX_INDEX_CYLINDER,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshCylinder,
		NULL
	);

	//�C���f�b�N�X���ւ̃|�C���^
	WORD *pIdx;

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshCylinder->Lock(0, 0, (void**)&pIdx, 0);

	//���_�f�[�^�̐ݒ�
	for (int nCntIdx = 0; nCntIdx < MAX_VERTEX_CYLINDER * 0.5f; nCntIdx++)
	{
		//�ݒ肷��C���f�b�N�X���w��
		int nIdx = nCntIdx * 2;

		//�C���f�b�N�X��ݒ�
		pIdx[nIdx] = ((int)MAX_VERTEX_CYLINDER - 1) - nCntIdx;
		pIdx[nIdx + 1] = (int)((MAX_VERTEX_CYLINDER * 0.5f - 1) - nCntIdx);
	}

	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	g_pIdxBuffMeshCylinder->Unlock();
}

//==========================================
//  �I������
//==========================================
void UninitMeshCylinder(void)
{
	//�|���S���̏I��
	UninitPolygon(&g_pVtxBuffMeshCylinder);

	//�e�N�X�`���̏I��
	UninitTexture(&g_pTextureMeshCylinder);

	//�C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIdxBuffMeshCylinder != NULL)
	{
		g_pIdxBuffMeshCylinder->Release();
		g_pIdxBuffMeshCylinder = NULL;
	}
}

//==========================================
//  �X�V����
//==========================================
void UpdateMeshCylinder(void)
{

}

//==========================================
//  �`�揈��
//==========================================
void DrawMeshCylinder(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���[�J���ϐ��錾
	D3DXMATRIX mtxRot, mtxTrans; //�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_aMeshCylinder.mtxWorld);

	//�����̔��f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshCylinder.rot.y, g_aMeshCylinder.rot.x, g_aMeshCylinder.rot.z);
	D3DXMatrixMultiply(&g_aMeshCylinder.mtxWorld, &g_aMeshCylinder.mtxWorld, &mtxRot);

	//�ʒu�̔��f
	D3DXMatrixTranslation(&mtxTrans, g_aMeshCylinder.pos.x, g_aMeshCylinder.pos.y, g_aMeshCylinder.pos.z);
	D3DXMatrixMultiply(&g_aMeshCylinder.mtxWorld, &g_aMeshCylinder.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_aMeshCylinder.mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMeshCylinder, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffMeshCylinder);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMeshCylinder);

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive
	(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		(int)MAX_VERTEX_CYLINDER, //���_��
		0,
		MAX_INDEX_CYLINDER - 2 //�v���~�e�B�u��
	);
}

//==========================================
//  ���_���W�̎擾
//==========================================
D3DXVECTOR3* GetVtxPos(void)
{
	return &g_aCollisionPos[0];
}
