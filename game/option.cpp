//==========================================
//
//  �I�v�V�����̐���(option.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "option.h"
#include "input.h"

//==========================================
//  �}�N����`
//==========================================
#define MOUSE_INIT (0.1f) //�}�E�X���x�̏����l

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
float g_Mouse = MOUSE_INIT;

//==========================================
//  �X�V����
//==========================================
void UpdateOption(void)
{
	//�}�E�X���x�̕ύX
	if (GetKeyboardTrigger(DIK_O))
	{
		g_Mouse += 0.01f;
	}
	if (GetKeyboardTrigger(DIK_L))
	{
		g_Mouse -= 0.01f;
	}
}

//==========================================
//  �`�揈��
//==========================================
void DrawOption(void)
{
	//���[�J���ϐ��錾
	char aStr[256];

	//�t�H���g�̎擾
	LPD3DXFONT pFont = GetFont();

	//�͈͂̎w��
	RECT rect = { 0, 60, SCREEN_WIDTH, SCREEN_HEIGHT };

	//������ɑ��
	sprintf(&aStr[0], "�}�E�X���x : %.2f", g_Mouse);

	//�e�L�X�g�̕`��
	pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_RIGHT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

//==========================================
//  �}�E�X���x�̎擾
//==========================================
float GetMouse(void)
{
	return g_Mouse;
}
