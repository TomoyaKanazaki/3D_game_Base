//==========================================
//
//  影の制御(shadow.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "shadow.h"
#include "polygon.h"
#include "texture.h"

//==========================================
//  マクロ定義
//==========================================
#define MAX_SHADOW (256) //影の最大数

//==========================================
//  構造体定義
//==========================================
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld; //ワールドマトリックス
	bool bUse;
}Shadow;

//==========================================
//  グローバル変数宣言
//==========================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL;
LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL;
Shadow g_aShadow[MAX_SHADOW]; //床の情報

//==========================================
//  初期化処理
//==========================================
void InitShadow(void)
{
	//各種変数の初期化
	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++)
	{
		g_aShadow[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aShadow[nCnt].bUse = false;
	}

	//ポリゴンの初期化
	g_pVtxBuffShadow = InitPolygon(MAX_SHADOW);

	//テクスチャの初期化
	g_pTextureShadow = InitTextureJpg("Shadow");

	//頂点バッファの呼び出し
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffShadow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-10.0f, 0.0f, 10.0f);
		pVtx[1].pos = D3DXVECTOR3(10.0f, 0.0f, 10.0f);
		pVtx[2].pos = D3DXVECTOR3(-10.0f, 0.0f, -10.0f);
		pVtx[3].pos = D3DXVECTOR3(10.0f, 0.0f, -10.0f);

		//頂点データを進める
		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffShadow->Unlock();
}

//==========================================
//  終了処理
//==========================================
void UninitShadow(void)
{
	//ポリゴンの終了
	UninitPolygon(&g_pVtxBuffShadow);

	//テクスチャの終了
	UninitTexture(&g_pTextureShadow);
}

//==========================================
//  更新処理
//==========================================
void UpdateShadow(void)
{

}

//==========================================
//  描画処理
//==========================================
void DrawShadow(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ローカル変数宣言
	D3DXMATRIX mtxRot, mtxTrans; //計算用マトリックス

	//アルファブレンディングを減算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++)
	{
		if (g_aShadow[nCnt].bUse)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aShadow[nCnt].mtxWorld);

			//向きの反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aShadow[nCnt].rot.y, g_aShadow[nCnt].rot.x, g_aShadow[nCnt].rot.z);
			D3DXMatrixMultiply(&g_aShadow[nCnt].mtxWorld, &g_aShadow[nCnt].mtxWorld, &mtxRot);

			//位置の反映
			D3DXMatrixTranslation(&mtxTrans, g_aShadow[nCnt].pos.x, g_aShadow[nCnt].pos.y, g_aShadow[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aShadow[nCnt].mtxWorld, &g_aShadow[nCnt].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aShadow[nCnt].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureShadow);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 4 * nCnt, 2);
		}
	}

	//アルファブレンディングをの設定を元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//==========================================
//  使用する影を設定
//==========================================
int SetShadow(void)
{
	//ローカル変数宣言
	int nCntShadow = -1;

	//使用していない影を選択
	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++)
	{
		if (g_aShadow[nCnt].bUse == false)
		{
			nCntShadow = nCnt;
			g_aShadow[nCnt].bUse = true;
			break;
		}
	}

	return nCntShadow;
}

//==========================================
//  影の位置を設定
//==========================================
void SetPositionShadow(int nldxShadow, D3DXVECTOR3 pos)
{
	g_aShadow[nldxShadow].pos = pos;
	g_aShadow[nldxShadow].pos.y = 0;
}

//==========================================
//  影の使用数を表示
//==========================================
void DrowDebugShadow(void)
{
	//ローカル変数宣言
	char aStr[256];
	int nCntShadow = 0;

	//使用している影のカウント
	for (int nCnt = 0; nCnt < MAX_SHADOW; nCnt++)
	{
		if (g_aShadow[nCnt].bUse)
		{
			nCntShadow++;
		}
	}

	//フォントの取得
	LPD3DXFONT pFont = GetFont();

	//範囲の指定
	RECT rect = { 0, 40, SCREEN_WIDTH, SCREEN_HEIGHT };

	//文字列に代入
	sprintf(&aStr[0], "影の使用数 : %d", nCntShadow);

	//テキストの描画
	pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}
