//==========================================
//
//  �e�̏���(shadow.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _SHADOW_H_
#define _SHADOW_H_
#include "main.h"

//==========================================
//  �v���g�^�C�v�錾
//==========================================
void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int SetShadow(void);
void SetPositionShadow(int nldxShadow, D3DXVECTOR3 pos);
void DrowDebugShadow(void);

#endif
