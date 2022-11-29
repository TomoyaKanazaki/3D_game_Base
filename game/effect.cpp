//==========================================
//
//  エフェクトの制御(effect.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "effect.h"
#include "polygon.h"
#include "texture.h"

//==========================================
//  マクロ定義
//==========================================
#define MAX_EFFECT (8192) //エフェクトの最大数

//==========================================
//  エフェクト構造体定義
//==========================================
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 move;
	D3DXCOLOR col;
	float fRadius;
	int nLife;
	bool bUse;
	D3DXMATRIX mtxWorld;
}EFECT;

//==========================================
//  グローバル変数宣言
//==========================================
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL; //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL; //バッファへのポインタ
EFECT g_aEffect[MAX_EFFECT]; //エフェクトの情報
float g_aRadius[MAX_EFFECT]; //半径の初期値
int g_aLife[MAX_EFFECT]; //寿命の初期値
D3DXCOLOR g_aCol[MAX_EFFECT]; //カラーの初期値

//==========================================
//  初期化処理
//==========================================
void InitEffect(void)
{
	//初期化処理
	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{
		g_aEffect[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCnt].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aEffect[nCnt].nLife = 0;
		g_aEffect[nCnt].fRadius = 0.0f;
		g_aEffect[nCnt].bUse = false;
		g_aRadius[nCnt] = 0.0f;
		g_aLife[nCnt] = 0;
		g_aCol[nCnt] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	//テクスチャの読み込み
	g_pTextureEffect = InitTextureJpg("\\effect");

	//頂点バッファの生成
	g_pVtxBuffEffect = InitPolygon(MAX_EFFECT);

	//頂点バッファの呼び出し
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPolygon = 0; nCntPolygon < MAX_EFFECT; nCntPolygon++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-5.0f, 5.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(5.0f, 5.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-5.0f, -5.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(5.0f, -5.0f, 0.0f);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//頂点データのポインタを進める
		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffEffect->Unlock();
}

//==========================================
//  終了処理
//==========================================
void UninitEffect(void)
{
	//テクスチャの破棄
	UninitTexture(&g_pTextureEffect);

	//ポリゴンの破棄
	UninitPolygon(&g_pVtxBuffEffect);
}

//==========================================
//  更新処理
//==========================================
void UpdateEffect(void)
{
	//頂点バッファの呼び出し
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	//更新処理
	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{
		if (g_aEffect[nCnt].bUse == true)
		{
			//エフェクトが使用されている場合

			//エフェクトを縮小する
			g_aEffect[nCnt].fRadius -= (g_aRadius[nCnt] / (float)g_aLife[nCnt]);

			//エフェクトを薄くする
			g_aEffect[nCnt].col.a -= (g_aCol[nCnt].a / (float)g_aLife[nCnt]);

			for (int nCntVtx = 0; nCntVtx < 4; nCntVtx++)
			{
				//透明度の更新
				pVtx[nCntVtx].col = g_aEffect[nCnt].col;
			}

			//寿命を減らす
			g_aEffect[nCnt].nLife -= 1;

			//寿命0の場合弾を不使用にする
			g_aEffect[nCnt].bUse = (g_aEffect[nCnt].nLife == 0) ? false : true;
		}

		//頂点データのポインタを進める
		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffEffect->Unlock();
}

//==========================================
//  描画処理
//==========================================
void DrawEffect(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ローカル変数宣言
	D3DXMATRIX mtxTrans; //計算用マトリックス
	D3DXMATRIX mtxView; //ビューマトリックス取得用

	//アルファブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//Zテストの無効化
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//アルファテストの無効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{
		if (g_aEffect[nCnt].bUse)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEffect[nCnt].mtxWorld);

			//ビューマトリックスの取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aEffect[nCnt].mtxWorld, NULL, &mtxView);

			//値の補正
			g_aEffect[nCnt].mtxWorld._41 = 0.0f;
			g_aEffect[nCnt].mtxWorld._42 = 0.0f;
			g_aEffect[nCnt].mtxWorld._43 = 0.0f;

			//位置の反映
			D3DXMatrixTranslation(&mtxTrans, g_aEffect[nCnt].pos.x, g_aEffect[nCnt].pos.y, g_aEffect[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aEffect[nCnt].mtxWorld, &g_aEffect[nCnt].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEffect[nCnt].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEffect);

			

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);

			
		}
	}
	//アルファテストの無効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//Zテストの有効化
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//アルファブレンディングをの設定を元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//==========================================
//  設定処理
//==========================================
void SetEffect(D3DXVECTOR3 pos,D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife)
{
	//設定処理
	for (int nCnt = 0; nCnt < MAX_EFFECT; nCnt++)
	{
		if (g_aEffect[nCnt].bUse == false)
		{
			//位置の設定
			g_aEffect[nCnt].pos = pos;

			//移動量の設定
			g_aEffect[nCnt].move = move;

			//大きさの設定
			g_aEffect[nCnt].fRadius = fRadius;
			g_aRadius[nCnt] = fRadius;

			//透明度の設定
			g_aEffect[nCnt].col = col;
			g_aCol[nCnt] = col;

			//寿命の設定
			g_aEffect[nCnt].nLife = nLife;
			g_aLife[nCnt] = nLife;

			//使用/不使用の設定
			g_aEffect[nCnt].bUse = true;

			break;
		}
	}
}
