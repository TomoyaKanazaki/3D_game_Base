//==========================================
//
//  壁の制御(wall.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "wall.h"
#include "polygon.h"
#include "texture.h"
#include "input.h"

//==========================================
//  マクロ定義
//==========================================
#define MAX_WALL (4) //壁の最大数

//==========================================
//  構造体定義
//==========================================
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld; //ワールドマトリックス
	int nShadowNum;
	bool bUse;
}Wall;

//==========================================
//  グローバル変数宣言
//==========================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWall = NULL;
LPDIRECT3DTEXTURE9 g_pTextureWall = NULL;
Wall g_aWall[MAX_WALL]; //床の情報
float g = 0.0f;

//==========================================
//  初期化処理
//==========================================
void InitWall(void)
{
	//各種変数の初期化
	for (int nCnt = 0; nCnt < MAX_WALL; nCnt++)
	{
		switch (nCnt)
		{
		case 0:
			g_aWall[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 300.0f);
			g_aWall[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f * D3DX_PI, 0.0f); //正面
			break;
		case 1:
			g_aWall[nCnt].pos = D3DXVECTOR3(300.0f, 0.0f, 0.0f);
			g_aWall[nCnt].rot = D3DXVECTOR3(0.0f, 0.5f * D3DX_PI, 0.0f); //右
			break;
		case 2:
			g_aWall[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, -300.0f);
			g_aWall[nCnt].rot = D3DXVECTOR3(0.0f, 1.0f * D3DX_PI, 0.0f); //後ろ
			break;
		case 3:
			g_aWall[nCnt].pos = D3DXVECTOR3(-300.0f, 0.0f, 0.0f);
			g_aWall[nCnt].rot = D3DXVECTOR3(0.0f, -0.5f * D3DX_PI, 0.0f); //左
			break;
		}
		g_aWall[nCnt].nShadowNum = -1;
		g_aWall[nCnt].bUse = true;
	}

	//ポリゴンの初期化
	g_pVtxBuffWall = InitPolygon(MAX_WALL);

	//テクスチャの初期化
	g_pTextureWall = InitTextureJpg("holo");

	//頂点バッファの呼び出し
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffWall->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_WALL; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-300.0f, 200.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(300.0f, 200.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-300.0f, -0.01f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(300.0f, -0.01f, 0.0f);

		//法線ベクトルの設定
		for (int nCntNor = 0; nCntNor < 4; nCntNor++)
		{
			pVtx[nCntNor].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		}

		//頂点データを進める
		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffWall->Unlock();
}

//==========================================
//  終了処理
//==========================================
void UninitWall(void)
{
	//ポリゴンの終了
	UninitPolygon(&g_pVtxBuffWall);

	//テクスチャの終了
	UninitTexture(&g_pTextureWall);
}

//==========================================
//  更新処理
//==========================================
void UpdateWall(void)
{

}

//==========================================
//  描画処理
//==========================================
void DrawWall(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ローカル変数宣言
	D3DXMATRIX mtxRot, mtxTrans; //計算用マトリックス

	for (int nCnt = 0; nCnt < MAX_WALL; nCnt++)
	{
		if (g_aWall[nCnt].bUse)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aWall[nCnt].mtxWorld);

			//向きの反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aWall[nCnt].rot.y, g_aWall[nCnt].rot.x, g_aWall[nCnt].rot.z);
			D3DXMatrixMultiply(&g_aWall[nCnt].mtxWorld, &g_aWall[nCnt].mtxWorld, &mtxRot);

			//位置の反映
			D3DXMatrixTranslation(&mtxTrans, g_aWall[nCnt].pos.x, g_aWall[nCnt].pos.y, g_aWall[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aWall[nCnt].mtxWorld, &g_aWall[nCnt].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aWall[nCnt].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffWall, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureWall);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2);
		}
	}
}

//==========================================
//  壁のデバッグ表示
//==========================================
void DrawDebuWall(void)
{
	//ローカル変数宣言
	char aStr[256];
	int nCntPos = 0;

	//フォントの取得
	LPD3DXFONT pFont = GetFont();

	//範囲の指定
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++)
	{
		for (int nCnt = 0; nCnt < 2; nCnt++)
		{
			//範囲の指定
			RECT rect = { 0, 20 * nCntPos + 60, SCREEN_WIDTH, SCREEN_HEIGHT };
			nCntPos++;

			//文字列に代入
			switch (nCnt)
			{
			case 0:
				sprintf(&aStr[0], "壁%dの位置 ( %.2f : %.2f : %.2f )", nCntWall, g_aWall[nCntWall].pos.x, g_aWall[nCntWall].pos.y, g_aWall[nCntWall].pos.z);
				break;
			case 1:
				sprintf(&aStr[0], "壁%dの向き ( %.2f : %.2f : %.2f )", nCntWall, g_aWall[nCntWall].rot.x, g_aWall[nCntWall].rot.y, g_aWall[nCntWall].rot.z);
				break;
			}

			//テキストの描画
			pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}
