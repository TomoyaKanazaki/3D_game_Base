//==========================================
//
//  �e�̐���(shadow.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "shadow.h"
#include "polygon.h"
#include "texture.h"

//==========================================
//  �}�N����`
//==========================================
#define MAX_SHADOW (256) //�e�̍ő吔

//==========================================
//  �\���̒�`
//==========================================
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld; //���[���h�}�g���b�N�X
	bool bUse;
}Shadow;

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;
Shadow g_aShadow[MAX_SHADOW]; //���̏��

//==========================================
//  ����������
//==========================================
void InitShadow(void)
{
	//�e��ϐ��̏�����
	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++)
	{
		g_aShadow[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCnt].bUse = false;
	}

	//�|���S���̏�����
	g_pVtxBuffShadow = InitPolygon(MAX_SHADOW);

	//�e�N�X�`���̏�����
	g_pTextureShadow = InitTextureJpg("Shadow");

	//���_�o�b�t�@�̌Ăяo��
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-10.0f, 0.0f, 10.0f);
		pVtx[1].pos = D3DXVECTOR3(10.0f, 0.0f, 10.0f);
		pVtx[2].pos = D3DXVECTOR3(-10.0f, 0.0f, -10.0f);
		pVtx[3].pos = D3DXVECTOR3(10.0f, 0.0f, -10.0f);

		//���_�f�[�^��i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffShadow->Unlock();
}

//==========================================
//  �I������
//==========================================
void UninitShadow(void)
{
	//�|���S���̏I��
	UninitPolygon(&g_pVtxBuffShadow);

	//�e�N�X�`���̏I��
	UninitTexture(&g_pTextureShadow);
}

//==========================================
//  �X�V����
//==========================================
void UpdateShadow(void)
{

}

//==========================================
//  �`�揈��
//==========================================
void DrawShadow(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���[�J���ϐ��錾
	D3DXMATRIX mtxRot, mtxTrans; //�v�Z�p�}�g���b�N�X

	//�A���t�@�u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++)
	{
		if (g_aShadow[nCnt].bUse)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aShadow[nCnt].mtxWorld);

			//�����̔��f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCnt].rot.y, g_aShadow[nCnt].rot.x, g_aShadow[nCnt].rot.z);
			D3DXMatrixMultiply(&g_aShadow[nCnt].mtxWorld, &g_aShadow[nCnt].mtxWorld, &mtxRot);

			//�ʒu�̔��f
			D3DXMatrixTranslation(&mtxTrans, g_aShadow[nCnt].pos.x, g_aShadow[nCnt].pos.y, g_aShadow[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aShadow[nCnt].mtxWorld, &g_aShadow[nCnt].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCnt].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureShadow);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2);
		}
	}

	//�A���t�@�u�����f�B���O���̐ݒ�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//==========================================
//  �g�p����e��ݒ�
//==========================================
int SetShadow(void)
{
	//���[�J���ϐ��錾
	int nCntShadow = -1;

	//�g�p���Ă��Ȃ��e��I��
	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++)
	{
		if (g_aShadow[nCnt].bUse == false)
		{
			nCntShadow = nCnt;
			g_aShadow[nCnt].bUse = true;
			break;
		}
	}

	return nCntShadow;
}

//==========================================
//  �e�̈ʒu��ݒ�
//==========================================
void SetPositionShadow(int nldxShadow, D3DXVECTOR3 pos)
{
	g_aShadow[nldxShadow].pos = pos;
	g_aShadow[nldxShadow].pos.y = 0;
}

//==========================================
//  �e�̎g�p����\��
//==========================================
void DrowDebugShadow(void)
{
	//���[�J���ϐ��錾
	char aStr[256];
	int nCntShadow = 0;

	//�g�p���Ă���e�̃J�E���g
	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++)
	{
		if (g_aShadow[nCnt].bUse)
		{
			nCntShadow++;
		}
	}

	//�t�H���g�̎擾
	LPD3DXFONT pFont = GetFont();

	//�͈͂̎w��
	RECT rect = { 0, 40, SCREEN_WIDTH, SCREEN_HEIGHT };

	//������ɑ��
	sprintf(&aStr[0], "�e�̎g�p�� : %d", nCntShadow);

	//�e�L�X�g�̕`��
	pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}
