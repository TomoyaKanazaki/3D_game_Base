//==========================================
//
//  弾の制御(bullet.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "explosion.h"
#include "player.h"
#include "effect.h"
#include "polygon.h"
#include "texture.h"

//==========================================
//  マクロ定義
//==========================================
#define EXPLOSION_SPEED (2.0f) //弾の速度
#define EXPLOSION_COOL_TIME (4) //弾発射のクールタイム
#define EXPLOSION_GROW (0.3f) //弾の拡大率
#define EXPLOSION_SIZE (18.75f) //弾のサイズ
#define MAX_EXPLOSION (256) //弾の最大数

//==========================================
//  弾構造体の定義
//==========================================
typedef struct
{
	D3DXVECTOR3 pos;
	bool bUse;
	int nLife;
	D3DXMATRIX mtxWorld;
	int nAnimTimer;
	int nAnimCounter;
}Explosion;

//==========================================
//  グローバル変数宣言
//==========================================
LPDIRECT3DTEXTURE9 g_pTextureExplosion = NULL; //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffExplosion = NULL; //バッファへのポインタ
Explosion g_aExplosion[MAX_EXPLOSION]; //弾の情報

//==========================================
//  初期化処理
//==========================================
void InitExplosion(void)
{
	//初期化処理
	for (int nCnt = 0; nCnt < MAX_EXPLOSION; nCnt++)
	{
		g_aExplosion[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aExplosion[nCnt].bUse = false;
		g_aExplosion[nCnt].nLife = 32;
	}

	//テクスチャの読み込み
	g_pTextureExplosion = InitTexturePng("explosion000");

	//頂点バッファの呼び出し
	g_pVtxBuffExplosion = InitPolygon(MAX_EXPLOSION);

	//頂点バッファの呼び出し
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPolygon = 0; nCntPolygon < MAX_EXPLOSION; nCntPolygon++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-15.0f, 15.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(15.0f, 15.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-15.0f, -15.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(15.0f, -15.0f, 0.0f);
		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		//頂点データのポインタを進める
		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffExplosion->Unlock();
}

//==========================================
//  終了処理
//==========================================
void UninitExplosion(void)
{
	//テクスチャの破棄
	UninitTexture(&g_pTextureExplosion);

	//ポリゴンの破棄
	UninitPolygon(&g_pVtxBuffExplosion);
}

//==========================================
//  更新処理
//==========================================
void UpdateExplosion(void)
{
	//頂点バッファの呼び出し
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

	//更新処理
	for (int nCnt = 0; nCnt < MAX_EXPLOSION; nCnt++)
	{
		if (g_aExplosion[nCnt].bUse == true)
		{
			//弾が使用されている場合

			g_aExplosion[nCnt].nLife--;

			//弾を不使用にする
			if (g_aExplosion[nCnt].nLife <= 0)
			{
				//画面外に出たとき
				g_aExplosion[nCnt].bUse = false;
			}

			g_aExplosion[nCnt].nAnimTimer++;

			if (g_aExplosion[nCnt].nAnimTimer % 4 == 0)
			{
				g_aExplosion[nCnt].nAnimCounter++;
			}

			//頂点座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.125f * g_aExplosion[nCnt].nAnimCounter, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.125f * (g_aExplosion[nCnt].nAnimCounter + 1), 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.125f * g_aExplosion[nCnt].nAnimCounter, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.125f * (g_aExplosion[nCnt].nAnimCounter + 1), 1.0f);
		}
		//頂点データを進める
		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffExplosion->Unlock();
}

//==========================================
//  描画処理
//==========================================
void DrawExplosion(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ローカル変数宣言
	D3DXMATRIX mtxTrans; //計算用マトリックス
	D3DXMATRIX mtxView; //ビューマトリックス取得用

	for (int nCnt = 0; nCnt < MAX_EXPLOSION; nCnt++)
	{
		if (g_aExplosion[nCnt].bUse)
		{

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aExplosion[nCnt].mtxWorld);

			//ビューマトリックスの取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aExplosion[nCnt].mtxWorld, NULL, &mtxView);

			//値の補正
			g_aExplosion[nCnt].mtxWorld._41 = 0.0f;
			g_aExplosion[nCnt].mtxWorld._42 = 0.0f;
			g_aExplosion[nCnt].mtxWorld._43 = 0.0f;

			//位置の反映
			D3DXMatrixTranslation(&mtxTrans, g_aExplosion[nCnt].pos.x, g_aExplosion[nCnt].pos.y, g_aExplosion[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aExplosion[nCnt].mtxWorld, &g_aExplosion[nCnt].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aExplosion[nCnt].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffExplosion, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureExplosion);

			//Zテストの無効化
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

			//アルファテストの無効化
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);

			//アルファテストの無効化
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

			//Zテストの有効化
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		}
	}
}

//==========================================
//  設定処理
//==========================================
void SetExplosion(D3DXVECTOR3 pos)
{
	//設定処理
	for (int nCnt = 0; nCnt < MAX_EXPLOSION; nCnt++)
	{
		if (g_aExplosion[nCnt].bUse == false)
		{
			//位置の設定
			g_aExplosion[nCnt].pos = pos;

			//寿命を設定
			g_aExplosion[nCnt].nLife = 32;

			//弾を使用する
			g_aExplosion[nCnt].bUse = true;

			//頂点バッファの呼び出し
			VERTEX_3D *pVtx;

			//頂点バッファをロック
			g_pVtxBuffExplosion->Lock(0, 0, (void**)&pVtx, 0);

			//頂点座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.125f, 1.0f);

			g_aExplosion[nCnt].nAnimTimer = 0;
			g_aExplosion[nCnt].nAnimCounter = 0;

			//頂点バッファをアンロック
			g_pVtxBuffExplosion->Unlock();
			break;
		}
	}
}
