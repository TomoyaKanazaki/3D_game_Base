//==========================================
//
//  ���̐���(test.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "field.h"
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
}Field;

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffField = NULL;
LPDIRECT3DTEXTURE9 g_pTextureField = NULL;
Field g_aField; //���̏��

//==========================================
//  ����������
//==========================================
void InitField(void)
{
	//�e��ϐ��̏�����
	g_aField.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aField.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aField.nShadowNum = -1;

	//�|���S���̏�����
	g_pVtxBuffField = InitPolygon();

	//�e�N�X�`���̏�����
	g_pTextureField = InitTextureJpg("stellar_into_the_galaxy");

	//���_�o�b�t�@�̌Ăяo��
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-300.0f, -0.01f, 300.0f);
	pVtx[1].pos = D3DXVECTOR3(300.0f, -0.01f, 300.0f);
	pVtx[2].pos = D3DXVECTOR3(-300.0f, -0.01f, -300.0f);
	pVtx[3].pos = D3DXVECTOR3(300.0f, -0.01f, -300.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffField->Unlock();
}

//==========================================
//  �I������
//==========================================
void UninitField(void)
{
	//�|���S���̏I��
	UninitPolygon(&g_pVtxBuffField);

	//�e�N�X�`���̏I��
	UninitTexture(&g_pTextureField);
}

//==========================================
//  �X�V����
//==========================================
void UpdateField(void)
{

}

//==========================================
//  �`�揈��
//==========================================
void DrawField(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���[�J���ϐ��錾
	D3DXMATRIX mtxRot, mtxTrans; //�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_aField.mtxWorld);

	//�����̔��f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aField.rot.y, g_aField.rot.x, g_aField.rot.z);
	D3DXMatrixMultiply(&g_aField.mtxWorld, &g_aField.mtxWorld, &mtxRot);

	//�ʒu�̔��f
	D3DXMatrixTranslation(&mtxTrans, g_aField.pos.x, g_aField.pos.y, g_aField.pos.z);
	D3DXMatrixMultiply(&g_aField.mtxWorld, &g_aField.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_aField.mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffField, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureField);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
