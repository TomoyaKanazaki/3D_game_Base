//==========================================
//
//  �ǂ̐���(wall.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "wall.h"
#include "polygon.h"
#include "texture.h"
#include "input.h"

//==========================================
//  �}�N����`
//==========================================
#define MAX_WALL (4) //�ǂ̍ő吔

//==========================================
//  �\���̒�`
//==========================================
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld; //���[���h�}�g���b�N�X
	int nShadowNum;
	bool bUse;
}Wall;

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;
LPDIRECT3DTEXTURE9 g_pTextureWall = NULL;
Wall g_aWall[MAX_WALL]; //���̏��
float g = 0.0f;

//==========================================
//  ����������
//==========================================
void InitWall(void)
{
	//�e��ϐ��̏�����
	for (int nCnt = 0; nCnt < MAX_WALL; nCnt++)
	{
		switch (nCnt)
		{
		case 0:
			g_aWall[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 300.0f);
			g_aWall[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f * D3DX_PI, 0.0f); //����
			break;
		case 1:
			g_aWall[nCnt].pos = D3DXVECTOR3(300.0f, 0.0f, 0.0f);
			g_aWall[nCnt].rot = D3DXVECTOR3(0.0f, 0.5f * D3DX_PI, 0.0f); //�E
			break;
		case 2:
			g_aWall[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, -300.0f);
			g_aWall[nCnt].rot = D3DXVECTOR3(0.0f, 1.0f * D3DX_PI, 0.0f); //���
			break;
		case 3:
			g_aWall[nCnt].pos = D3DXVECTOR3(-300.0f, 0.0f, 0.0f);
			g_aWall[nCnt].rot = D3DXVECTOR3(0.0f, -0.5f * D3DX_PI, 0.0f); //��
			break;
		}
		g_aWall[nCnt].nShadowNum = -1;
		g_aWall[nCnt].bUse = true;
	}

	//�|���S���̏�����
	g_pVtxBuffWall = InitPolygon(MAX_WALL);

	//�e�N�X�`���̏�����
	g_pTextureWall = InitTextureJpg("holo");

	//���_�o�b�t�@�̌Ăяo��
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_WALL; nCnt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-300.0f, 200.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(300.0f, 200.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-300.0f, -0.01f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(300.0f, -0.01f, 0.0f);

		//�@���x�N�g���̐ݒ�
		for (int nCntNor = 0; nCntNor < 4; nCntNor++)
		{
			pVtx[nCntNor].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		}

		//���_�f�[�^��i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffWall->Unlock();
}

//==========================================
//  �I������
//==========================================
void UninitWall(void)
{
	//�|���S���̏I��
	UninitPolygon(&g_pVtxBuffWall);

	//�e�N�X�`���̏I��
	UninitTexture(&g_pTextureWall);
}

//==========================================
//  �X�V����
//==========================================
void UpdateWall(void)
{

}

//==========================================
//  �`�揈��
//==========================================
void DrawWall(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���[�J���ϐ��錾
	D3DXMATRIX mtxRot, mtxTrans; //�v�Z�p�}�g���b�N�X

	for (int nCnt = 0; nCnt < MAX_WALL; nCnt++)
	{
		if (g_aWall[nCnt].bUse)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aWall[nCnt].mtxWorld);

			//�����̔��f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aWall[nCnt].rot.y, g_aWall[nCnt].rot.x, g_aWall[nCnt].rot.z);
			D3DXMatrixMultiply(&g_aWall[nCnt].mtxWorld, &g_aWall[nCnt].mtxWorld, &mtxRot);

			//�ʒu�̔��f
			D3DXMatrixTranslation(&mtxTrans, g_aWall[nCnt].pos.x, g_aWall[nCnt].pos.y, g_aWall[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aWall[nCnt].mtxWorld, &g_aWall[nCnt].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aWall[nCnt].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureWall);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2);
		}
	}
}

//==========================================
//  �ǂ̃f�o�b�O�\��
//==========================================
void DrawDebuWall(void)
{
	//���[�J���ϐ��錾
	char aStr[256];
	int nCntPos = 0;

	//�t�H���g�̎擾
	LPD3DXFONT pFont = GetFont();

	//�͈͂̎w��
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		for (int nCnt = 0; nCnt < 2; nCnt++)
		{
			//�͈͂̎w��
			RECT rect = { 0, 20 * nCntPos + 60, SCREEN_WIDTH, SCREEN_HEIGHT };
			nCntPos++;

			//������ɑ��
			switch (nCnt)
			{
			case 0:
				sprintf(&aStr[0], "��%d�̈ʒu ( %.2f : %.2f : %.2f )", nCntWall, g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);
				break;
			case 1:
				sprintf(&aStr[0], "��%d�̌��� ( %.2f : %.2f : %.2f )", nCntWall, g_aWall[nCntWall].rot.x, g_aWall[nCntWall].rot.y, g_aWall[nCntWall].rot.z);
				break;
			}

			//�e�L�X�g�̕`��
			pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}
