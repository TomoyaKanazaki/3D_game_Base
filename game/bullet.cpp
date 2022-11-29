//==========================================
//
//  弾の制御(bullet.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "bullet.h"
#include "player.h"
#include "effect.h"
#include "polygon.h"
#include "texture.h"
#include "explosion.h"
#include "colllision.h"

//==========================================
//  マクロ定義
//==========================================
#define BULLET_SPEED (10.0f) //弾の速度
#define BULLET_COOL_TIME (4) //弾発射のクールタイム
#define BULLET_GROW (0.3f) //弾の拡大率
#define BULLET_LIFE (100) //弾の寿命

//==========================================
//  グローバル変数宣言
//==========================================
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL; //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL; //バッファへのポインタ
Bullet g_aBullet[MAX_BULLET]; //弾の情報
int g_nCoolTimeCounter; //クールタイムカウンター

//==========================================
//  初期化処理
//==========================================
void InitBullet(void)
{
	//初期化処理
	g_nCoolTimeCounter = 0;
	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		g_aBullet[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCnt].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCnt].rot= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCnt].bUse = false;
		g_aBullet[nCnt].nLife = BULLET_LIFE;
	}

	//テクスチャの読み込み
	g_pTextureBullet = InitTexturePng("comet");

	//頂点バッファの呼び出し
	g_pVtxBuffBullet = InitPolygon(MAX_BULLET);

	//頂点バッファの呼び出し
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPolygon = 0; nCntPolygon < MAX_BULLET; nCntPolygon++)
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
	g_pVtxBuffBullet->Unlock();
}

//==========================================
//  終了処理
//==========================================
void UninitBullet(void)
{
	//テクスチャの破棄
	UninitTexture(&g_pTextureBullet);

	//ポリゴンの破棄
	UninitPolygon(&g_pVtxBuffBullet);
}

//==========================================
//  更新処理
//==========================================
void UpdateBullet(void)
{
	//更新処理
	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		if (g_aBullet[nCnt].bUse == true)
		{
			//弾が使用されている場合

			//前回位置の保存
			g_aBullet[nCnt].posOld = g_aBullet[nCnt].pos;

			//位置の更新
			g_aBullet[nCnt].pos += g_aBullet[nCnt].move;

			//エフェクトの付与
			SetEffect(g_aBullet[nCnt].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f), 10.0f, 60);

			g_aBullet[nCnt].nLife--;

			//弾を不使用にする
			if (g_aBullet[nCnt].nLife <= 0)
			{
				//寿命が尽きた時
				g_aBullet[nCnt].bUse = false;
				SetExplosion(g_aBullet[nCnt].pos);
			}

			//外積を使った壁の判定

			//壁の端の座標を作成
			D3DXVECTOR3 wallPos[4] =
			{
				D3DXVECTOR3(-300.0f, 0.0f, -300.0f),
				D3DXVECTOR3(300.0f, 0.0f, -300.0f),
				D3DXVECTOR3(300.0f, 0.0f, 300.0f),
				D3DXVECTOR3(-300.0f, 0.0f, 300.0f)
			};

			for (int nCntWall = 0; nCntWall < 4; nCntWall++)
			{
				//ローカル変数宣言
				int posEnd = nCntWall;
				int posStart = nCntWall + 1;
				
				//数値の補正
				if (posStart == 4)
				{
					posStart = 0;
				}
#if 0 
				//D3DXVec3Cross  外積
				D3DXVECTOR3 vec;
				D3DXVec3Cross(&vec, &wallPos[posEnd], &wallPos[posStart]);

				vec; //

				//D3DXVec3Dot  内積
#endif

				//当たり判定
				bool b = CollisionCylinder(g_aBullet[nCnt].pos);

				//判定の処理
				if (b)
				{
					g_aBullet[nCnt].bUse = false;
					SetExplosion(g_aBullet[nCnt].pos);

					//壁を添わせる
				}
			}
		}
	}
}

//==========================================
//  描画処理
//==========================================
void DrawBullet(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ローカル変数宣言
	D3DXMATRIX mtxTrans; //計算用マトリックス
	D3DXMATRIX mtxView; //ビューマトリックス取得用

	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		if (g_aBullet[nCnt].bUse)
		{

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aBullet[nCnt].mtxWorld);

			//ビューマトリックスの取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aBullet[nCnt].mtxWorld, NULL, &mtxView);

			//値の補正
			g_aBullet[nCnt].mtxWorld._41 = 0.0f;
			g_aBullet[nCnt].mtxWorld._42 = 0.0f;
			g_aBullet[nCnt].mtxWorld._43 = 0.0f;

			//位置の反映
			D3DXMatrixTranslation(&mtxTrans, g_aBullet[nCnt].pos.x, g_aBullet[nCnt].pos.y, g_aBullet[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aBullet[nCnt].mtxWorld, &g_aBullet[nCnt].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBullet[nCnt].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureBullet);

			//Zテストの無効化
			/*pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);*/

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
			/*pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);*/
		}
	}
}

//==========================================
//  設定処理
//==========================================
void SetBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	//設定処理
	for (int nCnt = 0; nCnt < MAX_BULLET; nCnt++)
	{
		if (g_aBullet[nCnt].bUse == false)
		{
			//位置の設定
			g_aBullet[nCnt].pos = pos;
			g_aBullet[nCnt].pos.y += 5.0f;

			//移動量の設定
			g_aBullet[nCnt].move = move * BULLET_SPEED;

			g_aBullet[nCnt].nLife = BULLET_LIFE;

			//弾を使用する
			g_aBullet[nCnt].bUse = true;

			break;
		}
	}
}
