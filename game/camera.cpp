//==========================================
//
//  �J�����̐���(camera.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "main.h"
#include "camera.h"
#include "input.h"
#include "player.h"

//==========================================
//  �}�N����`
//==========================================
#define CAMERA_DISTANCE (256.0f) //���_�ƒ����_�̋���
#define CAMERA_SPEED (0.05f) //�J�����̃X�s�[�h
#define CAMERA_MAX_ROT (D3DX_PI * 0.99f) //���_�̌��E�p
#define CAMERA_MIN_ROT (D3DX_PI * 0.01f) //���_�̌��E�p
#define CAMERA_POS (10.0f) //��_�Ԃ̋���

//==========================================
//  �O���[�o���ϐ�
//==========================================
Camera g_camera; //�J�����̏��

//==========================================
//�J�����̏���������
//==========================================
void InitCamera(void)
{
	//���[�J���ϐ��錾
	D3DXVECTOR3 StartRot;

	//�e��ϐ��̏�����
	g_camera.posV = D3DXVECTOR3(100.0f, 150.0f, 100.0f);
	g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_camera.rot = D3DXVECTOR3(atan2f(g_camera.posR.x - g_camera.posV.x, g_camera.posR.z - g_camera.posV.z), 0.0f, tanf(g_camera.posR.y - g_camera.posV.y));

	//�����ʒu���v���C���[�̌���ɐݒ肷��
	StartRot = -GetPlayerRot();

	//�ڕW�ʒu�܂ňړ�����
	g_camera.rot.x = StartRot.y + (D3DX_PI * 0.5f);

	g_camera.posV.x = g_camera.posR.x + (sinf(g_camera.rot.z) * cosf(g_camera.rot.x)) * CAMERA_DISTANCE;
	g_camera.posV.y = g_camera.posR.y + cosf(g_camera.rot.z) * CAMERA_DISTANCE;
	g_camera.posV.z = g_camera.posR.z + (sinf(g_camera.rot.z) * sinf(g_camera.rot.x)) * CAMERA_DISTANCE;
}

//==========================================
//  �J�����̏I������
//==========================================
void UninitCamera(void)
{

}

//==========================================
//  �J�����̍X�V����
//==========================================
void UpdateCamera(void)
{
	//���[�J���ϐ��錾
	D3DXVECTOR3 TargetRot;
	if (GetMousePress(BUTTON_LEFT) && GetMousePress(BUTTON_RIGHT))
	{
		CameraMove();
	}
	else if (GetMousePress(BUTTON_LEFT))
	{
		TPSCamera();
	}
	else if (GetMousePress(BUTTON_RIGHT))
	{
		FPSCamera();
	}

	a();

	if (GetKeyboardTrigger(DIK_RETURN))
	{
		//�S�J�����������Z�b�g
		g_camera.posV = D3DXVECTOR3(100.0f, 150.0f, 100.0f);
		g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		g_camera.rot = D3DXVECTOR3(atan2f(g_camera.posR.x - g_camera.posV.x, g_camera.posR.z - g_camera.posV.z), 0.0f, tanf(g_camera.posR.y - g_camera.posV.y));

		//�ڕW�̈ʒu���Z�o
		TargetRot = -GetPlayerRot();

		//�ڕW�ʒu�܂ňړ�����
		g_camera.rot.x = TargetRot.y + (D3DX_PI * 0.5f);

		g_camera.posV.x = g_camera.posR.x + (sinf(g_camera.rot.z) * cosf(g_camera.rot.x)) * CAMERA_DISTANCE;
		g_camera.posV.y = g_camera.posR.y + cosf(g_camera.rot.z) * CAMERA_DISTANCE;
		g_camera.posV.z = g_camera.posR.z + (sinf(g_camera.rot.z) * sinf(g_camera.rot.x)) * CAMERA_DISTANCE;
	}
}

//==========================================
//�J�����̐ݒ菈��
//==========================================
void SetCamera(void)
{
	//�f�o�C�X�̏���
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X���쐬
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
		D3DXToRadian(54.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		10000.0f);

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_camera.mtxView);

	//�r���[�}�g���b�N�X���쐬
	D3DXMatrixLookAtLH(&g_camera.mtxView,
		&g_camera.posV,
		&g_camera.posR,
		&g_camera.vecU);

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

//==========================================
//  �f�o�b�O�\��
//==========================================
void DrawCameraDebug(void)
{
	//���[�J���ϐ��錾
	char aStr[256];
	int nCntPos = 0;

	//�t�H���g�̎擾
	LPD3DXFONT pFont = GetFont();

	//�͈͂̎w��
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	//�{�^��
	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		//�͈͂̎w��
		RECT rect = { 0, 20 * nCntPos + 20, SCREEN_WIDTH, SCREEN_HEIGHT };
		nCntPos++;

		//������ɑ��
		switch (nCnt)
		{
		case 0:
			sprintf(&aStr[0], "���_�ʒu ( %.2f : %.2f : %.2f )", g_camera.posV.x, g_camera.posV.y, g_camera.posV.z);
			break; 
		case 1:
			sprintf(&aStr[0], "�����_�ʒu ( %.2f : %.2f : %.2f )", g_camera.posR.x, g_camera.posR.y, g_camera.posR.z);
			break;
		case 2:
			sprintf(&aStr[0], "�p�x ( %.2f : %.2f : %.2f )", g_camera.rot.x, g_camera.rot.y, g_camera.rot.z);
			break;
		}

		//�e�L�X�g�̕`��
		pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_RIGHT, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
}

//==========================================
//  �J�����p�x�̎擾
//==========================================
D3DXVECTOR3 GetCameraRot(void)
{
	return g_camera.rot;
}

//==========================================
//  ��l�̎��_�̑���
//==========================================
void FPSCamera(void)
{
	//���_�̒l���X�V
	g_camera.rot += GetMouseMove();

	//�p�x�̕␳
	if (g_camera.rot.z > CAMERA_MAX_ROT)
	{
		g_camera.rot.z = CAMERA_MAX_ROT;
	}
	if (g_camera.rot.z < CAMERA_MIN_ROT)
	{
		g_camera.rot.z = CAMERA_MIN_ROT;
	}
	if (g_camera.rot.x > D3DX_PI)
	{
		g_camera.rot.x = -D3DX_PI;
	}
	if (g_camera.rot.x < -D3DX_PI)
	{
		g_camera.rot.x = D3DX_PI;
	}

	//�p�x���X�V
	g_camera.posR.x = g_camera.posV.x - (sinf(g_camera.rot.z) * cosf(g_camera.rot.x)) * CAMERA_DISTANCE;
	g_camera.posR.y = g_camera.posV.y - cosf(g_camera.rot.z) * CAMERA_DISTANCE;
	g_camera.posR.z = g_camera.posV.z - (sinf(g_camera.rot.z) * sinf(g_camera.rot.x)) * CAMERA_DISTANCE;
}

//==========================================
//  �O�l�̎��_�̑���
//==========================================
void TPSCamera(void)
{
	//���_�̒l���X�V
	g_camera.rot += GetMouseMove();

	//�p�x�̕␳
	if (g_camera.rot.z > CAMERA_MAX_ROT)
	{
		g_camera.rot.z = CAMERA_MAX_ROT;
	}
	if (g_camera.rot.z < CAMERA_MIN_ROT)
	{
		g_camera.rot.z = CAMERA_MIN_ROT;
	}
	if (g_camera.rot.x > D3DX_PI)
	{
		g_camera.rot.x = -D3DX_PI;
	}
	if (g_camera.rot.x < -D3DX_PI)
	{
		g_camera.rot.x = D3DX_PI;
	}

	//�p�x���X�V
	g_camera.posV.x = g_camera.posR.x + (sinf(g_camera.rot.z) * cosf(g_camera.rot.x)) * CAMERA_DISTANCE;
	g_camera.posV.y = g_camera.posR.y + cosf(g_camera.rot.z) * CAMERA_DISTANCE;
	g_camera.posV.z = g_camera.posR.z + (sinf(g_camera.rot.z) * sinf(g_camera.rot.x)) * CAMERA_DISTANCE;
}

//==========================================
//  �J�����̈ړ�
//==========================================
void CameraMove(void)
{
	//�}�E�X
	g_camera.posR += GetMouseMove();
	g_camera.posV += GetMouseMove();

	//�L�[�{�[�h
	if (GetKeyboardPress(DIK_Q))
	{
		g_camera.rot.x -= 0.1f;
	}
	else if (GetKeyboardPress(DIK_E))
	{
		g_camera.rot.x += 0.1f;
	}
	if (GetKeyboardPress(DIK_R))
	{
		g_camera.rot.y += 0.1f;
	}
	else if (GetKeyboardPress(DIK_V))
	{
		g_camera.rot.y -= 0.1f;
	}
}

//==========================================
//  �Ǐ]
//==========================================
void a(void)
{
	//�v���C���[�̌����ƈʒu���擾
	D3DXVECTOR3 playerRot = GetPlayerRot();
	D3DXVECTOR3 playerPos = GetPlayerPos();
	D3DXVECTOR3 playerMove = GetPlayerMove();

	g_camera.posR += playerMove;
	g_camera.posV += playerMove;

	//�����_�̖ڕW�ʒu���Z�o
	g_camera.TargetPosR = D3DXVECTOR3
	(
		playerPos.x - sinf(playerRot.y) * CAMERA_POS,
		playerPos.y,
		playerPos.z - cosf(playerRot.y) * CAMERA_POS
	);

	//�����_��␳
	D3DXVECTOR3 RDeff = D3DXVECTOR3
	(
		g_camera.TargetPosR.x - g_camera.posR.x,
		g_camera.TargetPosR.y - g_camera.posR.y,
		g_camera.TargetPosR.z - g_camera.posR.z
	);

	//�����̕t�^
	g_camera.posR += RDeff * 0.1f;

	//���_-�����_�Ԃ̋��������߂�
	float fLength_X = sqrtf((g_camera.posR.x * g_camera.posR.x) + (g_camera.posV.x * g_camera.posV.x));
	float fLength_Y = sqrtf((g_camera.posR.y * g_camera.posR.y) + (g_camera.posV.y * g_camera.posV.y));
	float fLength_Z = sqrtf((g_camera.posR.z * g_camera.posR.z) + (g_camera.posV.z * g_camera.posV.z));

	//���_�̖ڕW�ʒu���Z�o
	g_camera.TargetPosV = D3DXVECTOR3
	(
		playerMove.x - sinf(playerRot.y * fLength_X),
		playerMove.y - cosf(playerRot.z * fLength_Y),
		playerMove.z - cosf(playerRot.y * fLength_Z)
	);

	//���_��␳
	D3DXVECTOR3 VDeff = D3DXVECTOR3
	(
		g_camera.TargetPosV.x - g_camera.posV.x,
		g_camera.TargetPosV.y - g_camera.posV.y,
		g_camera.TargetPosV.z - g_camera.posV.z
	);

	//�����̕t�^
	g_camera.posV += VDeff * 0.1f;

	//�ʒu�̏C��
	g_camera.posV.x = g_camera.posR.x + (sinf(g_camera.rot.z) * cosf(g_camera.rot.x)) * CAMERA_DISTANCE;
	g_camera.posV.y = g_camera.posR.y + cosf(g_camera.rot.z) * CAMERA_DISTANCE;
	g_camera.posV.z = g_camera.posR.z + (sinf(g_camera.rot.z) * sinf(g_camera.rot.x)) * CAMERA_DISTANCE;
}