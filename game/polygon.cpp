//==========================================
//
//  汎用的なポリゴンの制御(polygon.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "polygon.h"

//==========================================
//  ポリゴン情報の初期化を行う関数
//==========================================
LPDIRECT3DVERTEXBUFFER9 InitPolygon(int nNum)
{
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = NULL;

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer
	(
		sizeof(VERTEX_3D) * 4 * nNum,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&pVtxBuff,
		NULL
	);

	//頂点バッファの呼び出し
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPolygon = 0; nCntPolygon < nNum; nCntPolygon++)
	{
		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			//頂点座標の設定
			pVtx[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//法線ベクトルの設定
			pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラーの設定
			pVtx[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		if (nNum != 1)
		{
			//頂点データのポインタを進める
			pVtx += 4;
		}
	}

	//頂点バッファをアンロック
	pVtxBuff->Unlock();

	return pVtxBuff;
}

//==========================================
//  ポリゴン情報の終了を行う関数
//==========================================
void UninitPolygon(LPDIRECT3DVERTEXBUFFER9 *pVtxBuff)
{
	//頂点バッファの破棄
	if ((*pVtxBuff) != NULL)
	{
		(*pVtxBuff)->Release();
		(*pVtxBuff) = NULL;
	}
}

//==========================================
//  ポリゴンの描画を行う関数
//==========================================
void DrawPolygon(void)
{
	
}
