//==========================================
//
//  カメラの制御(camera.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "main.h"
#include "camera.h"
#include "input.h"
#include "player.h"

//==========================================
//  マクロ定義
//==========================================
#define CAMERA_DISTANCE (256.0f) //視点と注視点の距離
#define CAMERA_SPEED (0.05f) //カメラのスピード
#define CAMERA_MAX_ROT (D3DX_PI * 0.99f) //視点の限界角
#define CAMERA_MIN_ROT (D3DX_PI * 0.01f) //視点の限界角
#define CAMERA_POS (10.0f) //二点間の距離

//==========================================
//  グローバル変数
//==========================================
Camera g_camera; //カメラの情報

//==========================================
//カメラの初期化処理
//==========================================
void InitCamera(void)
{
	//ローカル変数宣言
	D3DXVECTOR3 StartRot;

	//各種変数の初期化
	g_camera.posV = D3DXVECTOR3(100.0f, 150.0f, 100.0f);
	g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_camera.rot = D3DXVECTOR3(atan2f(g_camera.posR.x - g_camera.posV.x, g_camera.posR.z - g_camera.posV.z), 0.0f, tanf(g_camera.posR.y - g_camera.posV.y));

	//初期位置をプレイヤーの後方に設定する
	StartRot = -GetPlayerRot();

	//目標位置まで移動する
	g_camera.rot.x = StartRot.y + (D3DX_PI * 0.5f);

	g_camera.posV.x = g_camera.posR.x + (sinf(g_camera.rot.z) * cosf(g_camera.rot.x)) * CAMERA_DISTANCE;
	g_camera.posV.y = g_camera.posR.y + cosf(g_camera.rot.z) * CAMERA_DISTANCE;
	g_camera.posV.z = g_camera.posR.z + (sinf(g_camera.rot.z) * sinf(g_camera.rot.x)) * CAMERA_DISTANCE;
}

//==========================================
//  カメラの終了処理
//==========================================
void UninitCamera(void)
{

}

//==========================================
//  カメラの更新処理
//==========================================
void UpdateCamera(void)
{
	//ローカル変数宣言
	D3DXVECTOR3 TargetRot;
	if (GetMousePress(BUTTON_LEFT) && GetMousePress(BUTTON_RIGHT))
	{
		CameraMove();
	}
	else if (GetMousePress(BUTTON_LEFT))
	{
		TPSCamera();
	}
	else if (GetMousePress(BUTTON_RIGHT))
	{
		FPSCamera();
	}

	a();

	if (GetKeyboardTrigger(DIK_RETURN))
	{
		//全カメラ情報をリセット
		g_camera.posV = D3DXVECTOR3(100.0f, 150.0f, 100.0f);
		g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		g_camera.rot = D3DXVECTOR3(atan2f(g_camera.posR.x - g_camera.posV.x, g_camera.posR.z - g_camera.posV.z), 0.0f, tanf(g_camera.posR.y - g_camera.posV.y));

		//目標の位置を算出
		TargetRot = -GetPlayerRot();

		//目標位置まで移動する
		g_camera.rot.x = TargetRot.y + (D3DX_PI * 0.5f);

		g_camera.posV.x = g_camera.posR.x + (sinf(g_camera.rot.z) * cosf(g_camera.rot.x)) * CAMERA_DISTANCE;
		g_camera.posV.y = g_camera.posR.y + cosf(g_camera.rot.z) * CAMERA_DISTANCE;
		g_camera.posV.z = g_camera.posR.z + (sinf(g_camera.rot.z) * sinf(g_camera.rot.x)) * CAMERA_DISTANCE;
	}
}

//==========================================
//カメラの設定処理
//==========================================
void SetCamera(void)
{
	//デバイスの所得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxProjection);

	//プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
		D3DXToRadian(54.0f),
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
		10.0f,
		10000.0f);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxView);

	//ビューマトリックスを作成
	D3DXMatrixLookAtLH(&g_camera.mtxView,
		&g_camera.posV,
		&g_camera.posR,
		&g_camera.vecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

//==========================================
//  デバッグ表示
//==========================================
void DrawCameraDebug(void)
{
	//ローカル変数宣言
	char aStr[256];
	int nCntPos = 0;

	//フォントの取得
	LPD3DXFONT pFont = GetFont();

	//範囲の指定
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	//ボタン
	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		//範囲の指定
		RECT rect = { 0, 20 * nCntPos + 20, SCREEN_WIDTH, SCREEN_HEIGHT };
		nCntPos++;

		//文字列に代入
		switch (nCnt)
		{
		case 0:
			sprintf(&aStr[0], "視点位置 ( %.2f : %.2f : %.2f )", g_camera.posV.x, g_camera.posV.y, g_camera.posV.z);
			break; 
		case 1:
			sprintf(&aStr[0], "注視点位置 ( %.2f : %.2f : %.2f )", g_camera.posR.x, g_camera.posR.y, g_camera.posR.z);
			break;
		case 2:
			sprintf(&aStr[0], "角度 ( %.2f : %.2f : %.2f )", g_camera.rot.x, g_camera.rot.y, g_camera.rot.z);
			break;
		}

		//テキストの描画
		pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_RIGHT, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
}

//==========================================
//  カメラ角度の取得
//==========================================
D3DXVECTOR3 GetCameraRot(void)
{
	return g_camera.rot;
}

//==========================================
//  一人称視点の操作
//==========================================
void FPSCamera(void)
{
	//視点の値を更新
	g_camera.rot += GetMouseMove();

	//角度の補正
	if (g_camera.rot.z > CAMERA_MAX_ROT)
	{
		g_camera.rot.z = CAMERA_MAX_ROT;
	}
	if (g_camera.rot.z < CAMERA_MIN_ROT)
	{
		g_camera.rot.z = CAMERA_MIN_ROT;
	}
	if (g_camera.rot.x > D3DX_PI)
	{
		g_camera.rot.x = -D3DX_PI;
	}
	if (g_camera.rot.x < -D3DX_PI)
	{
		g_camera.rot.x = D3DX_PI;
	}

	//角度を更新
	g_camera.posR.x = g_camera.posV.x - (sinf(g_camera.rot.z) * cosf(g_camera.rot.x)) * CAMERA_DISTANCE;
	g_camera.posR.y = g_camera.posV.y - cosf(g_camera.rot.z) * CAMERA_DISTANCE;
	g_camera.posR.z = g_camera.posV.z - (sinf(g_camera.rot.z) * sinf(g_camera.rot.x)) * CAMERA_DISTANCE;
}

//==========================================
//  三人称視点の操作
//==========================================
void TPSCamera(void)
{
	//視点の値を更新
	g_camera.rot += GetMouseMove();

	//角度の補正
	if (g_camera.rot.z > CAMERA_MAX_ROT)
	{
		g_camera.rot.z = CAMERA_MAX_ROT;
	}
	if (g_camera.rot.z < CAMERA_MIN_ROT)
	{
		g_camera.rot.z = CAMERA_MIN_ROT;
	}
	if (g_camera.rot.x > D3DX_PI)
	{
		g_camera.rot.x = -D3DX_PI;
	}
	if (g_camera.rot.x < -D3DX_PI)
	{
		g_camera.rot.x = D3DX_PI;
	}

	//角度を更新
	g_camera.posV.x = g_camera.posR.x + (sinf(g_camera.rot.z) * cosf(g_camera.rot.x)) * CAMERA_DISTANCE;
	g_camera.posV.y = g_camera.posR.y + cosf(g_camera.rot.z) * CAMERA_DISTANCE;
	g_camera.posV.z = g_camera.posR.z + (sinf(g_camera.rot.z) * sinf(g_camera.rot.x)) * CAMERA_DISTANCE;
}

//==========================================
//  カメラの移動
//==========================================
void CameraMove(void)
{
	//マウス
	g_camera.posR += GetMouseMove();
	g_camera.posV += GetMouseMove();

	//キーボード
	if (GetKeyboardPress(DIK_Q))
	{
		g_camera.rot.x -= 0.1f;
	}
	else if (GetKeyboardPress(DIK_E))
	{
		g_camera.rot.x += 0.1f;
	}
	if (GetKeyboardPress(DIK_R))
	{
		g_camera.rot.y += 0.1f;
	}
	else if (GetKeyboardPress(DIK_V))
	{
		g_camera.rot.y -= 0.1f;
	}
}

//==========================================
//  追従
//==========================================
void a(void)
{
	//プレイヤーの向きと位置を取得
	D3DXVECTOR3 playerRot = GetPlayerRot();
	D3DXVECTOR3 playerPos = GetPlayerPos();
	D3DXVECTOR3 playerMove = GetPlayerMove();

	g_camera.posR += playerMove;
	g_camera.posV += playerMove;

	//注視点の目標位置を算出
	g_camera.TargetPosR = D3DXVECTOR3
	(
		playerPos.x - sinf(playerRot.y) * CAMERA_POS,
		playerPos.y,
		playerPos.z - cosf(playerRot.y) * CAMERA_POS
	);

	//注視点を補正
	D3DXVECTOR3 RDeff = D3DXVECTOR3
	(
		g_camera.TargetPosR.x - g_camera.posR.x,
		g_camera.TargetPosR.y - g_camera.posR.y,
		g_camera.TargetPosR.z - g_camera.posR.z
	);

	//慣性の付与
	g_camera.posR += RDeff * 0.1f;

	//視点-注視点間の距離を求める
	float fLength_X = sqrtf((g_camera.posR.x * g_camera.posR.x) + (g_camera.posV.x * g_camera.posV.x));
	float fLength_Y = sqrtf((g_camera.posR.y * g_camera.posR.y) + (g_camera.posV.y * g_camera.posV.y));
	float fLength_Z = sqrtf((g_camera.posR.z * g_camera.posR.z) + (g_camera.posV.z * g_camera.posV.z));

	//視点の目標位置を算出
	g_camera.TargetPosV = D3DXVECTOR3
	(
		playerMove.x - sinf(playerRot.y * fLength_X),
		playerMove.y - cosf(playerRot.z * fLength_Y),
		playerMove.z - cosf(playerRot.y * fLength_Z)
	);

	//視点を補正
	D3DXVECTOR3 VDeff = D3DXVECTOR3
	(
		g_camera.TargetPosV.x - g_camera.posV.x,
		g_camera.TargetPosV.y - g_camera.posV.y,
		g_camera.TargetPosV.z - g_camera.posV.z
	);

	//慣性の付与
	g_camera.posV += VDeff * 0.1f;

	//位置の修正
	g_camera.posV.x = g_camera.posR.x + (sinf(g_camera.rot.z) * cosf(g_camera.rot.x)) * CAMERA_DISTANCE;
	g_camera.posV.y = g_camera.posR.y + cosf(g_camera.rot.z) * CAMERA_DISTANCE;
	g_camera.posV.z = g_camera.posR.z + (sinf(g_camera.rot.z) * sinf(g_camera.rot.x)) * CAMERA_DISTANCE;
}