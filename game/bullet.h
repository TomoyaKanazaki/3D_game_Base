//==========================================
//
//  �e�̐���(bullet.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _BULLET_H_
#define _BULLET_H_
#include "main.h"

//==========================================
//  �}�N����`
//==========================================
#define BULLET_SIZE (18.75f) //�e�̃T�C�Y
#define MAX_BULLET (256) //�e�̍ő吔

//==========================================
//  �e�\���̂̒�`
//==========================================
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 move;
	D3DXVECTOR3 rot;
	bool bUse;
	int nLife;
	D3DXMATRIX mtxWorld;
}Bullet;

//==========================================
//  �v���g�^�C�v�錾
//==========================================
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move);

#endif
