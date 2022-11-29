//==========================================
//
//  カメラの制御(camera.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _CAMERA_H_
#define _CAMERA_H_
#include "main.h"

//==========================================
//  カメラ構造体定義
//==========================================
typedef struct
{
	D3DXVECTOR3 posV; //視点
	D3DXVECTOR3 posR; //注視点
	D3DXVECTOR3 vecU; //上方向ベクトル
	D3DXMATRIX mtxProjection; //プロジェクションマトリックス
	D3DXMATRIX mtxView; //ビューマトリックス
	D3DXVECTOR3 rot; //Xの計算に使用する角度
	D3DXVECTOR3 TargetPosV; //目標の視点
	D3DXVECTOR3 TargetPosR; //目標の注視点
}Camera;

//==========================================
//  プロトタイプ宣言
//==========================================
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(void);
void DrawCameraDebug(void);
D3DXVECTOR3 GetCameraRot(void);
void FPSCamera(void);
void TPSCamera(void);
void CameraMove(void);
void a(void);

#endif
