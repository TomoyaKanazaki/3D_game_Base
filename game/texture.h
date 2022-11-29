//==========================================
//
//  テクスチャの制御(texture.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _TEXTURE_H_
#define _TEXTURE_H_
#include "main.h"

//==========================================
//  マクロ定義
//==========================================
#define MAX_STRING (64) //引数になる文字列の最大値

//==========================================
//  プロトタイプ宣言
//==========================================
LPDIRECT3DTEXTURE9 InitTexturePng(char aStr[MAX_STRING]); //テクスチャの初期化を行う関数
LPDIRECT3DTEXTURE9 InitTextureJpg(char aStr[MAX_STRING]); //テクスチャの初期化を行う関数
void UninitTexture(LPDIRECT3DTEXTURE9 *pTexture,int nNum = 1); //テクスチャの破棄を行う関数

#endif
