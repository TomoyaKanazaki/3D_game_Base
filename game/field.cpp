//==========================================
//
//  床の制御(test.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "field.h"
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
}Field;

//==========================================
//  グローバル変数宣言
//==========================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffField = NULL;
LPDIRECT3DTEXTURE9 g_pTextureField = NULL;
Field g_aField; //床の情報

//==========================================
//  初期化処理
//==========================================
void InitField(void)
{
	//各種変数の初期化
	g_aField.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aField.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aField.nShadowNum = -1;

	//ポリゴンの初期化
	g_pVtxBuffField = InitPolygon();

	//テクスチャの初期化
	g_pTextureField = InitTextureJpg("stellar_into_the_galaxy");

	//頂点バッファの呼び出し
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffField->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-300.0f, -0.01f, 300.0f);
	pVtx[1].pos = D3DXVECTOR3(300.0f, -0.01f, 300.0f);
	pVtx[2].pos = D3DXVECTOR3(-300.0f, -0.01f, -300.0f);
	pVtx[3].pos = D3DXVECTOR3(300.0f, -0.01f, -300.0f);

	//頂点バッファをアンロック
	g_pVtxBuffField->Unlock();
}

//==========================================
//  終了処理
//==========================================
void UninitField(void)
{
	//ポリゴンの終了
	UninitPolygon(&g_pVtxBuffField);

	//テクスチャの終了
	UninitTexture(&g_pTextureField);
}

//==========================================
//  更新処理
//==========================================
void UpdateField(void)
{

}

//==========================================
//  描画処理
//==========================================
void DrawField(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ローカル変数宣言
	D3DXMATRIX mtxRot, mtxTrans; //計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_aField.mtxWorld);

	//向きの反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aField.rot.y, g_aField.rot.x, g_aField.rot.z);
	D3DXMatrixMultiply(&g_aField.mtxWorld, &g_aField.mtxWorld, &mtxRot);

	//位置の反映
	D3DXMatrixTranslation(&mtxTrans, g_aField.pos.x, g_aField.pos.y, g_aField.pos.z);
	D3DXMatrixMultiply(&g_aField.mtxWorld, &g_aField.mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_aField.mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffField, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureField);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}
