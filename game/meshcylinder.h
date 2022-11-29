//==========================================
//
//  メッシュシリンダーの制御(meshcylinder.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_
#include "main.h"

//==========================================
//  マクロ定義
//==========================================
#define MAX_INDEX_CYLINDER (66) //インデックス数
#define NUM_VERTEX (32.0f) //分割数
#define VERTEX_ANGLE (NUM_VERTEX * 0.5f) //角度
#define CYLINDER_SIZE (300.0f) //シリンダーの半径
#define CYLINDER_HEIGHT (100.0f) //シリンダーの高さ
#define MAX_VERTEX_CYLINDER (NUM_VERTEX * 2 + 2) //頂点数

//==========================================
//  プロトタイプ宣言
//==========================================
void InitMeshCylinder(void);
void UninitMeshCylinder(void);
void UpdateMeshCylinder(void);
void DrawMeshCylinder(void);
D3DXVECTOR3* GetVtxPos(void);

#endif
