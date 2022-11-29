//==========================================
//
//  ���b�V���E�H�[���̐���(meshwall.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "meshwall.h"
#include "polygon.h"
#include "texture.h"

//==========================================
//  �}�N����`
//==========================================
#define MAX_INDEX_WALL (14) //�C���f�b�N�X��
#define MAX_VERTEX_WALL (9) //���_��

//==========================================
//  �\���̒�`
//==========================================
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld; //���[���h�}�g���b�N�X
	int nShadowNum;
}MeshWall;

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshWall = NULL;
LPDIRECT3DTEXTURE9 g_pTextureMeshWall = NULL;
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMEshWall = NULL;
MeshWall g_aMeshWall; //���̏��

//==========================================
//  ����������
//==========================================
void InitMeshWall(void)
{
	//�e��ϐ��̏�����
	g_aMeshWall.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshWall.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshWall.nShadowNum = -1;

	//�|���S���̏�����
	g_pVtxBuffMeshWall = InitPolygon(3);

	//�e�N�X�`���̏�����
	g_pTextureMeshWall = InitTextureJpg("stellar_into_the_galaxy");

	//���_�o�b�t�@�̌Ăяo��
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffMeshWall->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-300.0f, 200.0f, 300.0f);
	pVtx[1].pos = D3DXVECTOR3(0, 200.0f, 300.0f);
	pVtx[2].pos = D3DXVECTOR3(300.0f, 200.0f, 300.0f);

	pVtx[3].pos = D3DXVECTOR3(-300.0f, 100.0f, 300.0f);
	pVtx[4].pos = D3DXVECTOR3(0.0f, 100.0f, 300.0f);
	pVtx[5].pos = D3DXVECTOR3(300.0f, 100.0f, 300.0f);

	pVtx[6].pos = D3DXVECTOR3(-300.0f, -0.01f, 300.0f);
	pVtx[7].pos = D3DXVECTOR3(0.0f, -0.01f, 300.0f);
	pVtx[8].pos = D3DXVECTOR3(300.0f, -0.01f, 300.0f);

	//�@���x�N�g���̐ݒ�
	for (int nCntNor = 0; nCntNor < 9; nCntNor++)
	{
		pVtx[nCntNor].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffMeshWall->Unlock();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer
	(
		sizeof(WORD) * MAX_INDEX_WALL,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMEshWall,
		NULL
	);

	//�C���f�b�N�X���ւ̃|�C���^
	WORD *pIdx;

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMEshWall->Lock(0, 0, (void**)&pIdx, 0);

	//���_�f�[�^�̐ݒ�
	pIdx[0] = 3; //n + 1
	pIdx[1] = 0; //n - n
	pIdx[2] = 4; //n + 2
	pIdx[3] = 1; //
	pIdx[4] = 5;
	pIdx[5] = 2;
	pIdx[6] = 2;
	pIdx[7] = 6;
	pIdx[8] = 6;
	pIdx[9] = 3;
	pIdx[10] = 7;
	pIdx[11] = 4;
	pIdx[12] = 8;
	pIdx[13] = 5;

	//�C���f�b�N�X�o�b�t�@�̃A�����b�N
	g_pIdxBuffMEshWall->Unlock();
}

//==========================================
//  �I������
//==========================================
void UninitMeshWall(void)
{
	//�|���S���̏I��
	UninitPolygon(&g_pVtxBuffMeshWall);

	//�e�N�X�`���̏I��
	UninitTexture(&g_pTextureMeshWall);

	//�C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIdxBuffMEshWall != NULL)
	{
		g_pIdxBuffMEshWall->Release();
		g_pIdxBuffMEshWall = NULL;
	}
}

//==========================================
//  �X�V����
//==========================================
void UpdateMeshWall(void)
{

}

//==========================================
//  �`�揈��
//==========================================
void DrawMeshWall(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���[�J���ϐ��錾
	D3DXMATRIX mtxRot, mtxTrans; //�v�Z�p�}�g���b�N�X

								 //���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_aMeshWall.mtxWorld);

	//�����̔��f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshWall.rot.y, g_aMeshWall.rot.x, g_aMeshWall.rot.z);
	D3DXMatrixMultiply(&g_aMeshWall.mtxWorld, &g_aMeshWall.mtxWorld, &mtxRot);

	//�ʒu�̔��f
	D3DXMatrixTranslation(&mtxTrans, g_aMeshWall.pos.x, g_aMeshWall.pos.y, g_aMeshWall.pos.z);
	D3DXMatrixMultiply(&g_aMeshWall.mtxWorld, &g_aMeshWall.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_aMeshWall.mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMeshWall, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffMEshWall);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMeshWall);

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive
	(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		MAX_VERTEX_WALL, //���_��
		0,
		MAX_INDEX_WALL - 2 //�v���~�e�B�u��
	);
}
