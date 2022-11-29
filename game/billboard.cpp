//==========================================
//
//  �r���{�[�h�̐���(billboard.cpp)
//  Author :Tomoya Kanazaki
//
//==========================================
#include "billboard.h"
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
}Billboard;

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;
LPDIRECT3DTEXTURE9 g_pTextureBillboard = NULL;
Billboard g_aBillboard; //���̏��

//==========================================
//  ����������
//==========================================
void InitBillboard(void)
{
	//�e��ϐ��̏�����
	g_aBillboard.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aBillboard.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aBillboard.nShadowNum = -1;

	//�|���S���̏�����
	g_pVtxBuffBillboard = InitPolygon();

	//�e�N�X�`���̏�����
	g_pTextureBillboard = InitTexturePng("hosimati");

	//���_�o�b�t�@�̌Ăяo��
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-55.0f, 200.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(55.0f, 200.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-55.0f, 0.0f, -0.0f);
	pVtx[3].pos = D3DXVECTOR3(55.0f, 0.0f, -0.0f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffBillboard->Unlock();
}

//==========================================
//  �I������
//==========================================
void UninitBillboard(void)
{
	//�|���S���̏I��
	UninitPolygon(&g_pVtxBuffBillboard);

	//�e�N�X�`���̏I��
	UninitTexture(&g_pTextureBillboard);
}

//==========================================
//  �X�V����
//==========================================
void UpdateBillboard(void)
{

}

//==========================================
//  �`�揈��
//==========================================
void DrawBillboard(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���[�J���ϐ��錾
	D3DXMATRIX mtxTrans; //�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView; //�r���[�}�g���b�N�X�擾�p

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_aBillboard.mtxWorld);

	//�r���[�}�g���b�N�X�̎擾
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
	D3DXMatrixInverse(&g_aBillboard.mtxWorld, NULL, &mtxView);

	//�l�̕␳
	g_aBillboard.mtxWorld._41 = 0.0f;
	g_aBillboard.mtxWorld._42 = 0.0f;
	g_aBillboard.mtxWorld._43 = 0.0f;

	//�ʒu�̔��f
	D3DXMatrixTranslation(&mtxTrans, g_aBillboard.pos.x, g_aBillboard.pos.y, g_aBillboard.pos.z);
	D3DXMatrixMultiply(&g_aBillboard.mtxWorld, &g_aBillboard.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_aBillboard.mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBillboard);

	//Z�e�X�g�̖�����
	/*pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);*/

	//�A���t�@�e�X�g�̖�����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//�A���t�@�e�X�g�̗L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//Z�e�X�g�̗L����
	/*pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);*/

}
