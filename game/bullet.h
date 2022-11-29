//==========================================
//
//  弾の制御(bullet.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _BULLET_H_
#define _BULLET_H_
#include "main.h"

//==========================================
//  マクロ定義
//==========================================
#define BULLET_SIZE (18.75f) //弾のサイズ
#define MAX_BULLET (256) //弾の最大数

//==========================================
//  弾構造体の定義
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
//  プロトタイプ宣言
//==========================================
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move);

#endif
