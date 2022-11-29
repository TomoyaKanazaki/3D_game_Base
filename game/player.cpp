//==========================================
//
//  モデルの制御(player.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "player.h"
#include "input.h"
#include "shadow.h"
#include "camera.h"
#include "bullet.h"

//==========================================
//  マクロ定義
//==========================================
#define PLAYER_SPEED (1.0f) //プレイヤーの移動速度

//==========================================
//  モデル構造体
//==========================================
typedef struct
{
	DWORD dwNumMat;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXVECTOR3 move;
	D3DXMATRIX mtxWorld;
	int nShadowNum; //使用している影の番号
	D3DXVECTOR3 vtxMax;
	D3DXVECTOR3 vtxMin;
}PLAYER;

//==========================================
//  グローバル変数宣言
//==========================================
LPD3DXMESH g_pMeshPlayer = NULL;
LPD3DXBUFFER g_pBuffMatPlayer = NULL;
LPDIRECT3DTEXTURE9 g_pTexturePlayer[16] = {};
PLAYER g_Player;

//==========================================
//  初期化処理
//==========================================
void InitPlayer(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//各種変数の初期化
	g_Player.dwNumMat = 0;
	g_Player.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Player.vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//ローカル変数宣言
	int nNumVtx;
	DWORD dwSizeFVF;
	BYTE *pVtxBuff;

	//Xファイルの読み込み
	D3DXLoadMeshFromX
	(
		"data\\MODEL\\head.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatPlayer,
		NULL,
		&g_Player.dwNumMat,
		&g_pMeshPlayer
	);

	//マテリアルデータへのポインタを取得
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)g_pBuffMatPlayer->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_Player.dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			D3DXCreateTextureFromFile
			(
				pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_pTexturePlayer[nCntMat]
			);
		}
	}

	//頂点数の取得
	nNumVtx = g_pMeshPlayer->GetNumVertices();

	//フォーマットのサイズを取得
	dwSizeFVF = D3DXGetFVFVertexSize(g_pMeshPlayer->GetFVF());

	//頂点バッファをロック
	g_pMeshPlayer->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	//頂点の比較
	for (int nCnt = 0; nCnt < nNumVtx; nCnt++)
	{
		//頂点座標の取得
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		//最大値の比較
		g_Player.vtxMax.x = (g_Player.vtxMax.x < vtx.x) ? vtx.x : g_Player.vtxMax.x; //X座標
		g_Player.vtxMax.y = (g_Player.vtxMax.y < vtx.y) ? vtx.y : g_Player.vtxMax.y; //Y座標
		g_Player.vtxMax.z = (g_Player.vtxMax.z < vtx.z) ? vtx.z : g_Player.vtxMax.z; //Z座標

		//最小値の比較
		g_Player.vtxMin.x = (g_Player.vtxMin.x > vtx.x) ? vtx.x : g_Player.vtxMin.x; //X座標
		g_Player.vtxMin.y = (g_Player.vtxMin.y > vtx.y) ? vtx.y : g_Player.vtxMin.y; //Y座標
		g_Player.vtxMin.z = (g_Player.vtxMin.z > vtx.z) ? vtx.z : g_Player.vtxMin.z; //Z座標

		//頂点データを進める
		pVtxBuff += dwSizeFVF;
	}

	//頂点バッファをアンロック
	g_pMeshPlayer->UnlockVertexBuffer();

	//影を設定
	g_Player.nShadowNum = SetShadow();
}

//==========================================
//  終了処理
//==========================================
void UninitPlayer(void)
{
	//メッシュの破棄
	if (g_pMeshPlayer != NULL)
	{
		g_pMeshPlayer->Release();
		g_pMeshPlayer = NULL;
	}

	//マテリアルの破棄
	if (g_pBuffMatPlayer != NULL)
	{
		g_pBuffMatPlayer->Release();
		g_pBuffMatPlayer = NULL;
	}
}

//==========================================
//  更新処理
//==========================================
void UpdatePlayer(void)
{
	//ローカル変数宣言
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float fRotMove = g_Player.rot.y; //現在の角度
	float fRotDest = g_Player.rot.y; //目標の角度
	float fRotDiff = g_Player.rot.y; //目標と現在の角度の差

									 //プレイヤーの更新
	if (GetKeyboardPress(DIK_A) == true)
	{
		if (GetKeyboardPress(DIK_W) == true)
		{
			move.x += cosf(GetCameraRot().x + (-D3DX_PI * 0.75f)) * PLAYER_SPEED;
			move.z += sinf(GetCameraRot().x + (-D3DX_PI * 0.75f)) * PLAYER_SPEED;
			fRotDest = (-GetCameraRot().x + D3DX_PI * 0.25f);
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{
			move.x += cosf(GetCameraRot().x + (-D3DX_PI * 0.25f)) * PLAYER_SPEED;
			move.z += sinf(GetCameraRot().x + (-D3DX_PI * 0.25f)) * PLAYER_SPEED;
			fRotDest = (-GetCameraRot().x + -D3DX_PI * 0.25f);
		}
		else
		{
			move.x += cosf(GetCameraRot().x + (-D3DX_PI * 0.5f)) * PLAYER_SPEED;
			move.z += sinf(GetCameraRot().x + (-D3DX_PI * 0.5f)) * PLAYER_SPEED;
			fRotDest = -GetCameraRot().x;
		}
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{

		if (GetKeyboardPress(DIK_W) == true)
		{
			move.x += cosf(GetCameraRot().x + (D3DX_PI * 0.75f)) * PLAYER_SPEED;
			move.z += sinf(GetCameraRot().x + (D3DX_PI * 0.75f)) * PLAYER_SPEED;

			fRotDest = (-GetCameraRot().x + D3DX_PI * 0.75f);
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{
			move.x += cosf(GetCameraRot().x + (D3DX_PI * 0.25f)) * PLAYER_SPEED;
			move.z += sinf(GetCameraRot().x + (D3DX_PI * 0.25f)) * PLAYER_SPEED;

			fRotDest = (-GetCameraRot().x + -D3DX_PI * 0.75f);
		}
		else
		{
			move.x += cosf(GetCameraRot().x + (D3DX_PI * 0.5f)) * PLAYER_SPEED;
			move.z += sinf(GetCameraRot().x + (D3DX_PI * 0.5f)) * PLAYER_SPEED;
			fRotDest = (-GetCameraRot().x + D3DX_PI * 1.0f);
		}
	}
	else if (GetKeyboardPress(DIK_W) == true)
	{
		move.x += -cosf(GetCameraRot().x) * PLAYER_SPEED;
		move.z += -sinf(GetCameraRot().x) * PLAYER_SPEED;
		fRotDest = (-GetCameraRot().x + D3DX_PI * 0.5f);

	}
	else if (GetKeyboardPress(DIK_S) == true)
	{
		move.x += cosf(GetCameraRot().x) * PLAYER_SPEED;
		move.z += sinf(GetCameraRot().x) * PLAYER_SPEED;
		fRotDest = (-GetCameraRot().x + -D3DX_PI * 0.5f);
	}

	//スティック入力
	D3DXVECTOR3 move2D = GetStickL(0);
	if (move2D.x >= 0.1f || move2D.z >= 0.1f)
	{
		move.x = move2D.x;
		move.z = move2D.y;
	}

	//Y移動量の保存
	move.y = g_Player.move.y;

	//移動量の代入
	g_Player.move = move;

	//重力
	g_Player.move.y -= 0.1f;

	//ジャンプ
	if (GetKeyboardTrigger(DIK_E))
	{
		g_Player.move.y = 5.0f;
	}

	//位置の更新
	g_Player.pos += g_Player.move;

	//床
	if (g_Player.pos.y < 0.0f)
	{
		g_Player.pos.y = 0.0f;
		g_Player.move.y = 0.0f;
	}

	//弾の発射
	if (GetKeyboardPress(DIK_SPACE))
	{
		SetBullet(g_Player.pos, D3DXVECTOR3(-sinf(g_Player.rot.y), 0.0f, -cosf(g_Player.rot.y)));
	}

	fRotDiff = fRotDest - fRotMove;	//目標までの移動方向の差分

	if (fRotDiff > D3DX_PI || fRotDiff < -D3DX_PI)
	{//-3.14～3.14の範囲外の場合
		if (fRotDiff > D3DX_PI)
		{
			fRotDiff += (-D3DX_PI * 2);
		}
		else if (fRotDiff <= -D3DX_PI)
		{
			fRotDiff += (D3DX_PI * 2);
		}
	}

	g_Player.rot.y += fRotDiff * 0.1f;

	if (g_Player.rot.y > D3DX_PI || g_Player.rot.y < -D3DX_PI)
	{//-3.14～3.14の範囲外の場合
		if (g_Player.rot.y > D3DX_PI)
		{
			g_Player.rot.y += (-D3DX_PI * 2);
		}
		else if (g_Player.rot.y < -D3DX_PI)
		{
			g_Player.rot.y += (D3DX_PI * 2);
		}
	}

	if (g_Player.pos.x < -300.0f + 10.0f)
	{
		g_Player.pos.x = -300.0f + 10.0f;
		g_Player.move.x = 0.0f;
	}
	if (g_Player.pos.x > 300.0f - 10.0f)
	{
		g_Player.pos.x = 300.0f - 10.0f;
		g_Player.move.x = 0.0f;
	}
	if (g_Player.pos.z < -300.0f + 10.0f)
	{
		g_Player.pos.z = -300.0f + 10.0f;
		g_Player.move.z = 0.0f;
	}
	if (g_Player.pos.z > 300.0f - 10.0f)
	{
		g_Player.pos.z = 300.0f - 10.0f;
		g_Player.move.z = 0.0f;
	}

	//影の位置の更新
	SetPositionShadow(g_Player.nShadowNum, g_Player.pos);
}

//==========================================
//  描画処理
//==========================================
void DrawPlayer(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ローカル変数宣言
	D3DXMATRIX mtxRot, mtxTrans; //計算用マトリックス
	D3DMATERIAL9 matDef; //現在のマテリアルの保存用
	D3DXMATERIAL *pMat; //マテリアルへのポインタ

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_Player.mtxWorld);

	//向きの反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_Player.rot.y, g_Player.rot.x, g_Player.rot.z);
	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxRot);

	//位置の反映
	D3DXMatrixTranslation(&mtxTrans, g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
	D3DXMatrixMultiply(&g_Player.mtxWorld, &g_Player.mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_Player.mtxWorld);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatPlayer->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_Player.dwNumMat; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePlayer[0]);

		//モデルの描画
		g_pMeshPlayer->DrawSubset(nCntMat);
	}

	//保存しているマテリアルを復元
	pDevice->SetMaterial(&matDef);
}

//==========================================
//  モデルの位置の取得
//==========================================
D3DXVECTOR3 GetPlayerPos(void)
{
	return g_Player.pos;
}

//==========================================
//  モデルの移動量
//==========================================
D3DXVECTOR3 GetPlayerMove(void)
{
	return g_Player.move;
}

//==========================================
//  デバッグ表示
//==========================================
void DrawPlayerDebug(void)
{
	//ローカル変数宣言
	char aStr[256];
	int nCntPos = 0;

	//フォントの取得
	LPD3DXFONT pFont = GetFont();

	//範囲の指定
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	//ボタン
	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		//範囲の指定
		RECT rect = { 0, 20 * nCntPos, SCREEN_WIDTH, SCREEN_HEIGHT };
		nCntPos++;

		//文字列に代入
		switch (nCnt)
		{
		case 0:
			sprintf(&aStr[0], "モデルの座標 ( %.2f : %.2f : %.2f )", g_Player.pos.x, g_Player.pos.y, g_Player.pos.z);
			break;
		case 1:
			sprintf(&aStr[0], "モデルの移動量 ( %.2f : %.2f : %.2f )", g_Player.move.x, g_Player.move.y, g_Player.move.z);
			break;
		case 2:
			sprintf(&aStr[0], "頂点の最大値 ( %.2f : %.2f : %.2f )", g_Player.vtxMax.x, g_Player.vtxMax.y, g_Player.vtxMax.z);
			break;
		case 3:
			sprintf(&aStr[0], "頂点の最小値 ( %.2f : %.2f : %.2f )", g_Player.vtxMin.x, g_Player.vtxMin.y, g_Player.vtxMin.z);
			break;
		}

		//テキストの描画
		pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_CENTER, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
}

//==========================================
//  プレイヤーの角度の取得
//==========================================
D3DXVECTOR3 GetPlayerRot(void)
{
	return g_Player.rot;
}
