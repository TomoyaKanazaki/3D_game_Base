//==========================================
//
//  メッシュウォールの制御(meshwall.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "meshwall.h"
#include "polygon.h"
#include "texture.h"

//==========================================
//  マクロ定義
//==========================================
#define MAX_INDEX_WALL (14) //インデックス数
#define MAX_VERTEX_WALL (9) //頂点数

//==========================================
//  構造体定義
//==========================================
typedef struct
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	D3DXMATRIX mtxWorld; //ワールドマトリックス
	int nShadowNum;
}MeshWall;

//==========================================
//  グローバル変数宣言
//==========================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshWall = NULL;
LPDIRECT3DTEXTURE9 g_pTextureMeshWall = NULL;
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMEshWall = NULL;
MeshWall g_aMeshWall; //床の情報

//==========================================
//  初期化処理
//==========================================
void InitMeshWall(void)
{
	//各種変数の初期化
	g_aMeshWall.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshWall.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshWall.nShadowNum = -1;

	//ポリゴンの初期化
	g_pVtxBuffMeshWall = InitPolygon(3);

	//テクスチャの初期化
	g_pTextureMeshWall = InitTextureJpg("stellar_into_the_galaxy");

	//頂点バッファの呼び出し
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffMeshWall->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-300.0f, 200.0f, 300.0f);
	pVtx[1].pos = D3DXVECTOR3(0, 200.0f, 300.0f);
	pVtx[2].pos = D3DXVECTOR3(300.0f, 200.0f, 300.0f);

	pVtx[3].pos = D3DXVECTOR3(-300.0f, 100.0f, 300.0f);
	pVtx[4].pos = D3DXVECTOR3(0.0f, 100.0f, 300.0f);
	pVtx[5].pos = D3DXVECTOR3(300.0f, 100.0f, 300.0f);

	pVtx[6].pos = D3DXVECTOR3(-300.0f, -0.01f, 300.0f);
	pVtx[7].pos = D3DXVECTOR3(0.0f, -0.01f, 300.0f);
	pVtx[8].pos = D3DXVECTOR3(300.0f, -0.01f, 300.0f);

	//法線ベクトルの設定
	for (int nCntNor = 0; nCntNor < 9; nCntNor++)
	{
		pVtx[nCntNor].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	}

	//頂点バッファをアンロック
	g_pVtxBuffMeshWall->Unlock();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer
	(
		sizeof(WORD) * MAX_INDEX_WALL,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMEshWall,
		NULL
	);

	//インデックス情報へのポインタ
	WORD *pIdx;

	//インデックスバッファをロックし、頂点番号データへのポインタを取得
	g_pIdxBuffMEshWall->Lock(0, 0, (void**)&pIdx, 0);

	//頂点データの設定
	pIdx[0] = 3; //n + 1
	pIdx[1] = 0; //n - n
	pIdx[2] = 4; //n + 2
	pIdx[3] = 1; //
	pIdx[4] = 5;
	pIdx[5] = 2;
	pIdx[6] = 2;
	pIdx[7] = 6;
	pIdx[8] = 6;
	pIdx[9] = 3;
	pIdx[10] = 7;
	pIdx[11] = 4;
	pIdx[12] = 8;
	pIdx[13] = 5;

	//インデックスバッファのアンロック
	g_pIdxBuffMEshWall->Unlock();
}

//==========================================
//  終了処理
//==========================================
void UninitMeshWall(void)
{
	//ポリゴンの終了
	UninitPolygon(&g_pVtxBuffMeshWall);

	//テクスチャの終了
	UninitTexture(&g_pTextureMeshWall);

	//インデックスバッファの破棄
	if (g_pIdxBuffMEshWall != NULL)
	{
		g_pIdxBuffMEshWall->Release();
		g_pIdxBuffMEshWall = NULL;
	}
}

//==========================================
//  更新処理
//==========================================
void UpdateMeshWall(void)
{

}

//==========================================
//  描画処理
//==========================================
void DrawMeshWall(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ローカル変数宣言
	D3DXMATRIX mtxRot, mtxTrans; //計算用マトリックス

								 //ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_aMeshWall.mtxWorld);

	//向きの反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshWall.rot.y, g_aMeshWall.rot.x, g_aMeshWall.rot.z);
	D3DXMatrixMultiply(&g_aMeshWall.mtxWorld, &g_aMeshWall.mtxWorld, &mtxRot);

	//位置の反映
	D3DXMatrixTranslation(&mtxTrans, g_aMeshWall.pos.x, g_aMeshWall.pos.y, g_aMeshWall.pos.z);
	D3DXMatrixMultiply(&g_aMeshWall.mtxWorld, &g_aMeshWall.mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_aMeshWall.mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshWall, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMEshWall);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshWall);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive
	(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		MAX_VERTEX_WALL, //頂点数
		0,
		MAX_INDEX_WALL - 2 //プリミティブ数
	);
}
