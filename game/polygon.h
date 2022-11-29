//==========================================
//
//  汎用的なポリゴンの制御(polygon.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _POLYGON_H_
#define _POLYGON_H_
#include "main.h"

//==========================================
//  プロトタイプ宣言
//==========================================
LPDIRECT3DVERTEXBUFFER9 InitPolygon(int nNum = 1); //ポリゴン情報の初期化を行う関数
void UninitPolygon(LPDIRECT3DVERTEXBUFFER9 *pVtxBuff); //ポリゴン情報の終了を行う関数
void DrawPolygon(void); //ポリゴンの描画を行う関数

#endif
