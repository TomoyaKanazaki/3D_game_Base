//==========================================
//
//  ビルボードの制御(billboard.cpp)
//  Author :Tomoya Kanazaki
//
//==========================================
#include "billboard.h"
#include "polygon.h"
#include "texture.h"

//==========================================
//  構造体定義
//==========================================
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld; //ワールドマトリックス
	int nShadowNum;
}Billboard;

//==========================================
//  グローバル変数宣言
//==========================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBillboard = NULL;
LPDIRECT3DTEXTURE9 g_pTextureBillboard = NULL;
Billboard g_aBillboard; //床の情報

//==========================================
//  初期化処理
//==========================================
void InitBillboard(void)
{
	//各種変数の初期化
	g_aBillboard.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aBillboard.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aBillboard.nShadowNum = -1;

	//ポリゴンの初期化
	g_pVtxBuffBillboard = InitPolygon();

	//テクスチャの初期化
	g_pTextureBillboard = InitTexturePng("hosimati");

	//頂点バッファの呼び出し
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffBillboard->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-55.0f, 200.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(55.0f, 200.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-55.0f, 0.0f, -0.0f);
	pVtx[3].pos = D3DXVECTOR3(55.0f, 0.0f, -0.0f);

	//頂点バッファをアンロック
	g_pVtxBuffBillboard->Unlock();
}

//==========================================
//  終了処理
//==========================================
void UninitBillboard(void)
{
	//ポリゴンの終了
	UninitPolygon(&g_pVtxBuffBillboard);

	//テクスチャの終了
	UninitTexture(&g_pTextureBillboard);
}

//==========================================
//  更新処理
//==========================================
void UpdateBillboard(void)
{

}

//==========================================
//  描画処理
//==========================================
void DrawBillboard(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ローカル変数宣言
	D3DXMATRIX mtxTrans; //計算用マトリックス
	D3DXMATRIX mtxView; //ビューマトリックス取得用

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_aBillboard.mtxWorld);

	//ビューマトリックスの取得
	pDevice->GetTransform(D3DTS_VIEW, &mtxView);

	//ポリゴンをカメラに対して正面に向ける
	D3DXMatrixInverse(&g_aBillboard.mtxWorld, NULL, &mtxView);

	//値の補正
	g_aBillboard.mtxWorld._41 = 0.0f;
	g_aBillboard.mtxWorld._42 = 0.0f;
	g_aBillboard.mtxWorld._43 = 0.0f;

	//位置の反映
	D3DXMatrixTranslation(&mtxTrans, g_aBillboard.pos.x, g_aBillboard.pos.y, g_aBillboard.pos.z);
	D3DXMatrixMultiply(&g_aBillboard.mtxWorld, &g_aBillboard.mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_aBillboard.mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffBillboard, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureBillboard);

	//Zテストの無効化
	/*pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);*/

	//アルファテストの無効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//アルファテストの有効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//Zテストの有効化
	/*pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);*/

}
