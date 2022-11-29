//==========================================
//
//  ���f���̐���(player.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _PLAYER_H_
#define _PLAYER_H_
#include "main.h"

//==========================================
//  �v���g�^�C�v�錾
//==========================================
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
D3DXVECTOR3 GetPlayerPos(void);
D3DXVECTOR3 GetPlayerMove(void);
void DrawPlayerDebug(void);
D3DXVECTOR3 GetPlayerRot(void);

#endif
