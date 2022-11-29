//==========================================
//
//  �e�̐���(bullet.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "bullet.h"
#include "player.h"
#include "effect.h"
#include "polygon.h"
#include "texture.h"
#include "explosion.h"
#include "colllision.h"

//==========================================
//  �}�N����`
//==========================================
#define BULLET_SPEED (10.0f) //�e�̑��x
#define BULLET_COOL_TIME (4) //�e���˂̃N�[���^�C��
#define BULLET_GROW (0.3f) //�e�̊g�嗦
#define BULLET_LIFE (100) //�e�̎���

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL; //�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL; //�o�b�t�@�ւ̃|�C���^
Bullet g_aBullet[MAX_BULLET]; //�e�̏��
int g_nCoolTimeCounter; //�N�[���^�C���J�E���^�[

//==========================================
//  ����������
//==========================================
void InitBullet(void)
{
	//����������
	g_nCoolTimeCounter = 0;
	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		g_aBullet[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCnt].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCnt].rot= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCnt].bUse = false;
		g_aBullet[nCnt].nLife = BULLET_LIFE;
	}

	//�e�N�X�`���̓ǂݍ���
	g_pTextureBullet = InitTexturePng("comet");

	//���_�o�b�t�@�̌Ăяo��
	g_pVtxBuffBullet = InitPolygon(MAX_BULLET);

	//���_�o�b�t�@�̌Ăяo��
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPolygon = 0; nCntPolygon < MAX_BULLET; nCntPolygon++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-5.0f, 5.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(5.0f, 5.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-5.0f, -5.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(5.0f, -5.0f, 0.0f);
		//�@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		//���_�f�[�^�̃|�C���^��i�߂�
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffBullet->Unlock();
}

//==========================================
//  �I������
//==========================================
void UninitBullet(void)
{
	//�e�N�X�`���̔j��
	UninitTexture(&g_pTextureBullet);

	//�|���S���̔j��
	UninitPolygon(&g_pVtxBuffBullet);
}

//==========================================
//  �X�V����
//==========================================
void UpdateBullet(void)
{
	//�X�V����
	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		if (g_aBullet[nCnt].bUse == true)
		{
			//�e���g�p����Ă���ꍇ

			//�O��ʒu�̕ۑ�
			g_aBullet[nCnt].posOld = g_aBullet[nCnt].pos;

			//�ʒu�̍X�V
			g_aBullet[nCnt].pos += g_aBullet[nCnt].move;

			//�G�t�F�N�g�̕t�^
			SetEffect(g_aBullet[nCnt].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 10.0f, 60);

			g_aBullet[nCnt].nLife--;

			//�e��s�g�p�ɂ���
			if (g_aBullet[nCnt].nLife <= 0)
			{
				//�������s������
				g_aBullet[nCnt].bUse = false;
				SetExplosion(g_aBullet[nCnt].pos);
			}

			//�O�ς��g�����ǂ̔���

			//�ǂ̒[�̍��W���쐬
			D3DXVECTOR3 wallPos[4] =
			{
				D3DXVECTOR3(-300.0f, 0.0f, -300.0f),
				D3DXVECTOR3(300.0f, 0.0f, -300.0f),
				D3DXVECTOR3(300.0f, 0.0f, 300.0f),
				D3DXVECTOR3(-300.0f, 0.0f, 300.0f)
			};

			for (int nCntWall = 0; nCntWall < 4; nCntWall++)
			{
				//���[�J���ϐ��錾
				int posEnd = nCntWall;
				int posStart = nCntWall + 1;
				
				//���l�̕␳
				if (posStart == 4)
				{
					posStart = 0;
				}
#if 0 
				//D3DXVec3Cross  �O��
				D3DXVECTOR3 vec;
				D3DXVec3Cross(&vec, &wallPos[posEnd], &wallPos[posStart]);

				vec; //

				//D3DXVec3Dot  ����
#endif

				//�����蔻��
				bool b = CollisionCylinder(g_aBullet[nCnt].pos);

				//����̏���
				if (b)
				{
					g_aBullet[nCnt].bUse = false;
					SetExplosion(g_aBullet[nCnt].pos);

					//�ǂ�Y�킹��
				}
			}
		}
	}
}

//==========================================
//  �`�揈��
//==========================================
void DrawBullet(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���[�J���ϐ��錾
	D3DXMATRIX mtxTrans; //�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView; //�r���[�}�g���b�N�X�擾�p

	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		if (g_aBullet[nCnt].bUse)
		{

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aBullet[nCnt].mtxWorld);

			//�r���[�}�g���b�N�X�̎擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aBullet[nCnt].mtxWorld, NULL, &mtxView);

			//�l�̕␳
			g_aBullet[nCnt].mtxWorld._41 = 0.0f;
			g_aBullet[nCnt].mtxWorld._42 = 0.0f;
			g_aBullet[nCnt].mtxWorld._43 = 0.0f;

			//�ʒu�̔��f
			D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCnt].pos.x, g_aBullet[nCnt].pos.y, g_aBullet[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aBullet[nCnt].mtxWorld, &g_aBullet[nCnt].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCnt].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureBullet);

			//Z�e�X�g�̖�����
			/*pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);*/

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
			/*pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);*/
		}
	}
}

//==========================================
//  �ݒ菈��
//==========================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	//�ݒ菈��
	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		if (g_aBullet[nCnt].bUse == false)
		{
			//�ʒu�̐ݒ�
			g_aBullet[nCnt].pos = pos;
			g_aBullet[nCnt].pos.y += 5.0f;

			//�ړ��ʂ̐ݒ�
			g_aBullet[nCnt].move = move * BULLET_SPEED;

			g_aBullet[nCnt].nLife = BULLET_LIFE;

			//�e���g�p����
			g_aBullet[nCnt].bUse = true;

			break;
		}
	}
}
