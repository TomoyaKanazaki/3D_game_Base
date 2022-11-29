//==========================================
//
//  ���f���̐���(player.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "player.h"
#include "input.h"
#include "shadow.h"
#include "camera.h"
#include "bullet.h"

//==========================================
//  �}�N����`
//==========================================
#define PLAYER_SPEED (1.0f) //�v���C���[�̈ړ����x

//==========================================
//  ���f���\����
//==========================================
typedef struct
{
	DWORD dwNumMat;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 move;
	D3DXMATRIX mtxWorld;
	int nShadowNum; //�g�p���Ă���e�̔ԍ�
	D3DXVECTOR3 vtxMax;
	D3DXVECTOR3 vtxMin;
}PLAYER;

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
LPD3DXMESH g_pMeshPlayer = NULL;
LPD3DXBUFFER g_pBuffMatPlayer = NULL;
LPDIRECT3DTEXTURE9 g_pTexturePlayer[16] = {};
PLAYER g_Player;

//==========================================
//  ����������
//==========================================
void InitPlayer(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e��ϐ��̏�����
	g_Player.dwNumMat = 0;
	g_Player.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���[�J���ϐ��錾
	int nNumVtx;
	DWORD dwSizeFVF;
	BYTE *pVtxBuff;

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX
	(
		"data\\MODEL\\head.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatPlayer,
		NULL,
		&g_Player.dwNumMat,
		&g_pMeshPlayer
	);

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)g_pBuffMatPlayer->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_Player.dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			D3DXCreateTextureFromFile
			(
				pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_pTexturePlayer[nCntMat]
			);
		}
	}

	//���_���̎擾
	nNumVtx = g_pMeshPlayer->GetNumVertices();

	//�t�H�[�}�b�g�̃T�C�Y���擾
	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshPlayer->GetFVF());

	//���_�o�b�t�@�����b�N
	g_pMeshPlayer->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	//���_�̔�r
	for (int nCnt = 0; nCnt < nNumVtx; nCnt++)
	{
		//���_���W�̎擾
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		//�ő�l�̔�r
		g_Player.vtxMax.x = (g_Player.vtxMax.x < vtx.x) ? vtx.x : g_Player.vtxMax.x; //X���W
		g_Player.vtxMax.y = (g_Player.vtxMax.y < vtx.y) ? vtx.y : g_Player.vtxMax.y; //Y���W
		g_Player.vtxMax.z = (g_Player.vtxMax.z < vtx.z) ? vtx.z : g_Player.vtxMax.z; //Z���W

		//�ŏ��l�̔�r
		g_Player.vtxMin.x = (g_Player.vtxMin.x > vtx.x) ? vtx.x : g_Player.vtxMin.x; //X���W
		g_Player.vtxMin.y = (g_Player.vtxMin.y > vtx.y) ? vtx.y : g_Player.vtxMin.y; //Y���W
		g_Player.vtxMin.z = (g_Player.vtxMin.z > vtx.z) ? vtx.z : g_Player.vtxMin.z; //Z���W

		//���_�f�[�^��i�߂�
		pVtxBuff += dwSizeFVF;
	}

	//���_�o�b�t�@���A�����b�N
	g_pMeshPlayer->UnlockVertexBuffer();

	//�e��ݒ�
	g_Player.nShadowNum = SetShadow();
}

//==========================================
//  �I������
//==========================================
void UninitPlayer(void)
{
	//���b�V���̔j��
	if (g_pMeshPlayer != NULL)
	{
		g_pMeshPlayer->Release();
		g_pMeshPlayer = NULL;
	}

	//�}�e���A���̔j��
	if (g_pBuffMatPlayer != NULL)
	{
		g_pBuffMatPlayer->Release();
		g_pBuffMatPlayer = NULL;
	}
}

//==========================================
//  �X�V����
//==========================================
void UpdatePlayer(void)
{
	//���[�J���ϐ��錾
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float fRotMove = g_Player.rot.y; //���݂̊p�x
	float fRotDest = g_Player.rot.y; //�ڕW�̊p�x
	float fRotDiff = g_Player.rot.y; //�ڕW�ƌ��݂̊p�x�̍�

									 //�v���C���[�̍X�V
	if (GetKeyboardPress(DIK_A) == true)
	{
		if (GetKeyboardPress(DIK_W) == true)
		{
			move.x += cosf(GetCameraRot().x + (-D3DX_PI * 0.75f)) * PLAYER_SPEED;
			move.z += sinf(GetCameraRot().x + (-D3DX_PI * 0.75f)) * PLAYER_SPEED;
			fRotDest = (-GetCameraRot().x + D3DX_PI * 0.25f);
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{
			move.x += cosf(GetCameraRot().x + (-D3DX_PI * 0.25f)) * PLAYER_SPEED;
			move.z += sinf(GetCameraRot().x + (-D3DX_PI * 0.25f)) * PLAYER_SPEED;
			fRotDest = (-GetCameraRot().x + -D3DX_PI * 0.25f);
		}
		else
		{
			move.x += cosf(GetCameraRot().x + (-D3DX_PI * 0.5f)) * PLAYER_SPEED;
			move.z += sinf(GetCameraRot().x + (-D3DX_PI * 0.5f)) * PLAYER_SPEED;
			fRotDest = -GetCameraRot().x;
		}
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{

		if (GetKeyboardPress(DIK_W) == true)
		{
			move.x += cosf(GetCameraRot().x + (D3DX_PI * 0.75f)) * PLAYER_SPEED;
			move.z += sinf(GetCameraRot().x + (D3DX_PI * 0.75f)) * PLAYER_SPEED;

			fRotDest = (-GetCameraRot().x + D3DX_PI * 0.75f);
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{
			move.x += cosf(GetCameraRot().x + (D3DX_PI * 0.25f)) * PLAYER_SPEED;
			move.z += sinf(GetCameraRot().x + (D3DX_PI * 0.25f)) * PLAYER_SPEED;

			fRotDest = (-GetCameraRot().x + -D3DX_PI * 0.75f);
		}
		else
		{
			move.x += cosf(GetCameraRot().x + (D3DX_PI * 0.5f)) * PLAYER_SPEED;
			move.z += sinf(GetCameraRot().x + (D3DX_PI * 0.5f)) * PLAYER_SPEED;
			fRotDest = (-GetCameraRot().x + D3DX_PI * 1.0f);
		}
	}
	else if (GetKeyboardPress(DIK_W) == true)
	{
		move.x += -cosf(GetCameraRot().x) * PLAYER_SPEED;
		move.z += -sinf(GetCameraRot().x) * PLAYER_SPEED;
		fRotDest = (-GetCameraRot().x + D3DX_PI * 0.5f);

	}
	else if (GetKeyboardPress(DIK_S) == true)
	{
		move.x += cosf(GetCameraRot().x) * PLAYER_SPEED;
		move.z += sinf(GetCameraRot().x) * PLAYER_SPEED;
		fRotDest = (-GetCameraRot().x + -D3DX_PI * 0.5f);
	}

	//�X�e�B�b�N����
	D3DXVECTOR3 move2D = GetStickL(0);
	if (move2D.x >= 0.1f || move2D.z >= 0.1f)
	{
		move.x = move2D.x;
		move.z = move2D.y;
	}

	//Y�ړ��ʂ̕ۑ�
	move.y = g_Player.move.y;

	//�ړ��ʂ̑��
	g_Player.move = move;

	//�d��
	g_Player.move.y -= 0.1f;

	//�W�����v
	if (GetKeyboardTrigger(DIK_E))
	{
		g_Player.move.y = 5.0f;
	}

	//�ʒu�̍X�V
	g_Player.pos += g_Player.move;

	//��
	if (g_Player.pos.y < 0.0f)
	{
		g_Player.pos.y = 0.0f;
		g_Player.move.y = 0.0f;
	}

	//�e�̔���
	if (GetKeyboardPress(DIK_SPACE))
	{
		SetBullet(g_Player.pos, D3DXVECTOR3(-sinf(g_Player.rot.y), 0.0f, -cosf(g_Player.rot.y)));
	}

	fRotDiff = fRotDest - fRotMove;	//�ڕW�܂ł̈ړ������̍���

	if (fRotDiff > D3DX_PI || fRotDiff < -D3DX_PI)
	{//-3.14�`3.14�͈̔͊O�̏ꍇ
		if (fRotDiff > D3DX_PI)
		{
			fRotDiff += (-D3DX_PI * 2);
		}
		else if (fRotDiff <= -D3DX_PI)
		{
			fRotDiff += (D3DX_PI * 2);
		}
	}

	g_Player.rot.y += fRotDiff * 0.1f;

	if (g_Player.rot.y > D3DX_PI || g_Player.rot.y < -D3DX_PI)
	{//-3.14�`3.14�͈̔͊O�̏ꍇ
		if (g_Player.rot.y > D3DX_PI)
		{
			g_Player.rot.y += (-D3DX_PI * 2);
		}
		else if (g_Player.rot.y < -D3DX_PI)
		{
			g_Player.rot.y += (D3DX_PI * 2);
		}
	}

	if (g_Player.pos.x < -300.0f + 10.0f)
	{
		g_Player.pos.x = -300.0f + 10.0f;
		g_Player.move.x = 0.0f;
	}
	if (g_Player.pos.x > 300.0f - 10.0f)
	{
		g_Player.pos.x = 300.0f - 10.0f;
		g_Player.move.x = 0.0f;
	}
	if (g_Player.pos.z < -300.0f + 10.0f)
	{
		g_Player.pos.z = -300.0f + 10.0f;
		g_Player.move.z = 0.0f;
	}
	if (g_Player.pos.z > 300.0f - 10.0f)
	{
		g_Player.pos.z = 300.0f - 10.0f;
		g_Player.move.z = 0.0f;
	}

	//�e�̈ʒu�̍X�V
	SetPositionShadow(g_Player.nShadowNum, g_Player.pos);
}

//==========================================
//  �`�揈��
//==========================================
void DrawPlayer(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���[�J���ϐ��錾
	D3DXMATRIX mtxRot, mtxTrans; //�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef; //���݂̃}�e���A���̕ۑ��p
	D3DXMATERIAL *pMat; //�}�e���A���ւ̃|�C���^

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_Player.mtxWorld);

	//�����̔��f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Player.rot.y, g_Player.rot.x, g_Player.rot.z);
	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxRot);

	//�ʒu�̔��f
	D3DXMatrixTranslation(&mtxTrans, g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_Player.mtxWorld);

	//���݂̃}�e���A�����擾
	pDevice->GetMaterial(&matDef);

	//�}�e���A���f�[�^�ւ̃|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatPlayer->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_Player.dwNumMat; nCntMat++)
	{
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePlayer[0]);

		//���f���̕`��
		g_pMeshPlayer->DrawSubset(nCntMat);
	}

	//�ۑ����Ă���}�e���A���𕜌�
	pDevice->SetMaterial(&matDef);
}

//==========================================
//  ���f���̈ʒu�̎擾
//==========================================
D3DXVECTOR3 GetPlayerPos(void)
{
	return g_Player.pos;
}

//==========================================
//  ���f���̈ړ���
//==========================================
D3DXVECTOR3 GetPlayerMove(void)
{
	return g_Player.move;
}

//==========================================
//  �f�o�b�O�\��
//==========================================
void DrawPlayerDebug(void)
{
	//���[�J���ϐ��錾
	char aStr[256];
	int nCntPos = 0;

	//�t�H���g�̎擾
	LPD3DXFONT pFont = GetFont();

	//�͈͂̎w��
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	//�{�^��
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		//�͈͂̎w��
		RECT rect = { 0, 20 * nCntPos, SCREEN_WIDTH, SCREEN_HEIGHT };
		nCntPos++;

		//������ɑ��
		switch (nCnt)
		{
		case 0:
			sprintf(&aStr[0], "���f���̍��W ( %.2f : %.2f : %.2f )", g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
			break;
		case 1:
			sprintf(&aStr[0], "���f���̈ړ��� ( %.2f : %.2f : %.2f )", g_Player.move.x, g_Player.move.y, g_Player.move.z);
			break;
		case 2:
			sprintf(&aStr[0], "���_�̍ő�l ( %.2f : %.2f : %.2f )", g_Player.vtxMax.x, g_Player.vtxMax.y, g_Player.vtxMax.z);
			break;
		case 3:
			sprintf(&aStr[0], "���_�̍ŏ��l ( %.2f : %.2f : %.2f )", g_Player.vtxMin.x, g_Player.vtxMin.y, g_Player.vtxMin.z);
			break;
		}

		//�e�L�X�g�̕`��
		pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_CENTER, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

//==========================================
//  �v���C���[�̊p�x�̎擾
//==========================================
D3DXVECTOR3 GetPlayerRot(void)
{
	return g_Player.rot;
}
