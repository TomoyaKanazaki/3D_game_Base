//==========================================
//
//  ���b�V���t�B�[���h(meshfield.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "meshfield.h"
#include "polygon.h"
#include "texture.h"

//==========================================
//  �}�N����`
//==========================================
#define MAX_INEX_FIELD (14) //�C���f�b�N�X��
#define MAX_VERTEX_FIELD (9) //���_��

//==========================================
//  �\���̒�`
//==========================================
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld; //���[���h�}�g���b�N�X
	int nShadowNum;
}MeshField;

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;
LPDIRECT3DTEXTURE9 g_pTextureMeshField = NULL;
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMEshField = NULL;
MeshField g_aMeshField; //���̏��

//==========================================
//  ����������
//==========================================
void InitMeshField(void)
{
	//�e��ϐ��̏�����
	g_aMeshField.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshField.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshField.nShadowNum = -1;

	//�|���S���̏�����
	g_pVtxBuffMeshField = InitPolygon(3);

	//�e�N�X�`���̏�����
	g_pTextureMeshField = InitTextureJpg("stellar_into_the_galaxy");

	//���_�o�b�t�@�̌Ăяo��
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-300.0f, -0.01f, 300.0f);
	pVtx[1].pos = D3DXVECTOR3(0, -0.01f, 300.0f);
	pVtx[2].pos = D3DXVECTOR3(300.0f, -0.01f, 300.0f);

	pVtx[3].pos = D3DXVECTOR3(-300.0f, -0.01f, 0.0f);
	pVtx[4].pos = D3DXVECTOR3(0.0f, -0.01f, 0.0f);
	pVtx[5].pos = D3DXVECTOR3(300.0f, -0.01f, 0.0f);

	pVtx[6].pos = D3DXVECTOR3(-300.0f, -0.01f, -300.0f);
	pVtx[7].pos = D3DXVECTOR3(0.0f, -0.01f, -300.0f);
	pVtx[8].pos = D3DXVECTOR3(300.0f, -0.01f, -300.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffMeshField->Unlock();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer
	(
		sizeof(WORD) * MAX_INEX_FIELD,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMEshField,
		NULL
	);

	//�C���f�b�N�X���ւ̃|�C���^
	WORD *pIdx;

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMEshField->Lock(0, 0, (void**)&pIdx, 0);

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
	g_pIdxBuffMEshField->Unlock();
}

//==========================================
//  �I������
//==========================================
void UninitMeshField(void)
{
	//�|���S���̏I��
	UninitPolygon(&g_pVtxBuffMeshField);

	//�e�N�X�`���̏I��
	UninitTexture(&g_pTextureMeshField);

	//�C���f�b�N�X�o�b�t�@�̔j��
	if (g_pIdxBuffMEshField != NULL)
	{
		g_pIdxBuffMEshField->Release();
		g_pIdxBuffMEshField = NULL;
	}
}

//==========================================
//  �X�V����
//==========================================
void UpdateMeshField(void)
{

}

//==========================================
//  �`�揈��
//==========================================
void DrawMeshField(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���[�J���ϐ��錾
	D3DXMATRIX mtxRot, mtxTrans; //�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_aMeshField.mtxWorld);

	//�����̔��f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshField.rot.y, g_aMeshField.rot.x, g_aMeshField.rot.z);
	D3DXMatrixMultiply(&g_aMeshField.mtxWorld, &g_aMeshField.mtxWorld, &mtxRot);

	//�ʒu�̔��f
	D3DXMatrixTranslation(&mtxTrans, g_aMeshField.pos.x, g_aMeshField.pos.y, g_aMeshField.pos.z);
	D3DXMatrixMultiply(&g_aMeshField.mtxWorld, &g_aMeshField.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_aMeshField.mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(g_pIdxBuffMEshField);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureMeshField);

	//�|���S���̕`��
	pDevice->DrawIndexedPrimitive
	(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		MAX_VERTEX_FIELD, //���_��
		0,
		MAX_INEX_FIELD - 2 //�v���~�e�B�u��
	);
}
