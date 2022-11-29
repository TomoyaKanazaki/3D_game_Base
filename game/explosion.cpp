//==========================================
//
//  �e�̐���(bullet.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "explosion.h"
#include "player.h"
#include "effect.h"
#include "polygon.h"
#include "texture.h"

//==========================================
//  �}�N����`
//==========================================
#define EXPLOSION_SPEED (2.0f) //�e�̑��x
#define EXPLOSION_COOL_TIME (4) //�e���˂̃N�[���^�C��
#define EXPLOSION_GROW (0.3f) //�e�̊g�嗦
#define EXPLOSION_SIZE (18.75f) //�e�̃T�C�Y
#define MAX_EXPLOSION (256) //�e�̍ő吔

//==========================================
//  �e�\���̂̒�`
//==========================================
typedef struct
{
	D3DXVECTOR3 pos;
	bool bUse;
	int nLife;
	D3DXMATRIX mtxWorld;
	int nAnimTimer;
	int nAnimCounter;
}Explosion;

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL; //�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL; //�o�b�t�@�ւ̃|�C���^
Explosion g_aExplosion[MAX_EXPLOSION]; //�e�̏��

//==========================================
//  ����������
//==========================================
void InitExplosion(void)
{
	//����������
	for (int nCnt = 0; nCnt < MAX_EXPLOSION; nCnt++)
	{
		g_aExplosion[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCnt].bUse = false;
		g_aExplosion[nCnt].nLife = 32;
	}

	//�e�N�X�`���̓ǂݍ���
	g_pTextureExplosion = InitTexturePng("explosion000");

	//���_�o�b�t�@�̌Ăяo��
	g_pVtxBuffExplosion = InitPolygon(MAX_EXPLOSION);

	//���_�o�b�t�@�̌Ăяo��
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPolygon = 0; nCntPolygon < MAX_EXPLOSION; nCntPolygon++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-15.0f, 15.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(15.0f, 15.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-15.0f, -15.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(15.0f, -15.0f, 0.0f);
		//�@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		//���_�f�[�^�̃|�C���^��i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffExplosion->Unlock();
}

//==========================================
//  �I������
//==========================================
void UninitExplosion(void)
{
	//�e�N�X�`���̔j��
	UninitTexture(&g_pTextureExplosion);

	//�|���S���̔j��
	UninitPolygon(&g_pVtxBuffExplosion);
}

//==========================================
//  �X�V����
//==========================================
void UpdateExplosion(void)
{
	//���_�o�b�t�@�̌Ăяo��
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	//�X�V����
	for (int nCnt = 0; nCnt < MAX_EXPLOSION; nCnt++)
	{
		if (g_aExplosion[nCnt].bUse == true)
		{
			//�e���g�p����Ă���ꍇ

			g_aExplosion[nCnt].nLife--;

			//�e��s�g�p�ɂ���
			if (g_aExplosion[nCnt].nLife <= 0)
			{
				//��ʊO�ɏo���Ƃ�
				g_aExplosion[nCnt].bUse = false;
			}

			g_aExplosion[nCnt].nAnimTimer++;

			if (g_aExplosion[nCnt].nAnimTimer % 4 == 0)
			{
				g_aExplosion[nCnt].nAnimCounter++;
			}

			//���_���W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.125f * g_aExplosion[nCnt].nAnimCounter, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.125f * (g_aExplosion[nCnt].nAnimCounter + 1), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.125f * g_aExplosion[nCnt].nAnimCounter, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.125f * (g_aExplosion[nCnt].nAnimCounter + 1), 1.0f);
		}
		//���_�f�[�^��i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffExplosion->Unlock();
}

//==========================================
//  �`�揈��
//==========================================
void DrawExplosion(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���[�J���ϐ��錾
	D3DXMATRIX mtxTrans; //�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView; //�r���[�}�g���b�N�X�擾�p

	for (int nCnt = 0; nCnt < MAX_EXPLOSION; nCnt++)
	{
		if (g_aExplosion[nCnt].bUse)
		{

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aExplosion[nCnt].mtxWorld);

			//�r���[�}�g���b�N�X�̎擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aExplosion[nCnt].mtxWorld, NULL, &mtxView);

			//�l�̕␳
			g_aExplosion[nCnt].mtxWorld._41 = 0.0f;
			g_aExplosion[nCnt].mtxWorld._42 = 0.0f;
			g_aExplosion[nCnt].mtxWorld._43 = 0.0f;

			//�ʒu�̔��f
			D3DXMatrixTranslation(&mtxTrans, g_aExplosion[nCnt].pos.x, g_aExplosion[nCnt].pos.y, g_aExplosion[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aExplosion[nCnt].mtxWorld, &g_aExplosion[nCnt].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aExplosion[nCnt].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureExplosion);

			//Z�e�X�g�̖�����
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			//�A���t�@�e�X�g�̖�����
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);

			//�A���t�@�e�X�g�̖�����
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

			//Z�e�X�g�̗L����
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		}
	}
}

//==========================================
//  �ݒ菈��
//==========================================
void SetExplosion(D3DXVECTOR3 pos)
{
	//�ݒ菈��
	for (int nCnt = 0; nCnt < MAX_EXPLOSION; nCnt++)
	{
		if (g_aExplosion[nCnt].bUse == false)
		{
			//�ʒu�̐ݒ�
			g_aExplosion[nCnt].pos = pos;

			//������ݒ�
			g_aExplosion[nCnt].nLife = 32;

			//�e���g�p����
			g_aExplosion[nCnt].bUse = true;

			//���_�o�b�t�@�̌Ăяo��
			VERTEX_3D *pVtx;

			//���_�o�b�t�@�����b�N
			g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

			//���_���W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.125f, 1.0f);

			g_aExplosion[nCnt].nAnimTimer = 0;
			g_aExplosion[nCnt].nAnimCounter = 0;

			//���_�o�b�t�@���A�����b�N
			g_pVtxBuffExplosion->Unlock();
			break;
		}
	}
}
