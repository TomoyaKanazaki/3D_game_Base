//==========================================
//
//  ���͂̐���ɓ��������t�@�C��(input.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _INPUT_H_
#define _INPUT_H_
#include "main.h"

//�{�^��
typedef enum
{
	KEY_X = 0,
	KEY_Y,
	KEY_A,
	KEY_B,
	KEY_LB,
	KEY_RB,
	KEY_LT,
	KEY_RT,
	KEY_L3,
	KEY_R3,
	KEY_BACK,
	KEY_START
}JOYKEY_BUTTON;

//�}�E�X�{�^��
typedef enum
{
	BUTTON_LEFT = 0,
	BUTTON_RIGHT,
	BUTTON_WHEEL,
	BUTTON_SIDE_1,
	BUTTON_SIDE_2
}MOUSE_BUTTON;

//==========================================
//  �v���g�^�C�v�錾
//==========================================
//�f�o�C�X����
HRESULT InitDevice(HINSTANCE hInstance, HWND hWnd);
void UninitDevice(void);
void UpdateDevice(void);
//==========================================

//�L�[�{�[�h================================
HRESULT InitKeyboard(HWND hWnd);
void UninitKeyboard(void);
void UpdateKeyboard(void);

//���̎擾
bool GetKeyboardPress(int nKey);
bool GetKeyboardTrigger(int nKey);
//==========================================

//�W���C�p�b�h==============================
HRESULT InitJoyPad(HWND hWnd);
void UninitJoyPad(void);
void UpdateJoyPad(void);

//���̎擾
bool GetJoyPadButtonPress(JOYKEY_BUTTON nKey,int nPlayer);
bool GetJoyPadButtonTrigger(JOYKEY_BUTTON nKey, int nPlayer);
D3DXVECTOR3 GetStickL(int nPlayer);
D3DXVECTOR3 GetStickR(int nPlayer);
//==========================================

//�}�E�X====================================
HRESULT InitMouse(HWND hWnd);
void UninitMouse(void);
void UpdateMouse(void);

//���̎擾
bool GetMousePress(MOUSE_BUTTON nKey);
D3DXVECTOR3 GetMouseMove(void);
//==========================================

#endif
