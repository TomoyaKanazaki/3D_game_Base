//==========================================
//
//  メッシュシリンダーの制御(meshcylinder.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "meshcylinder.h"
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
}MeshCylinder;

//==========================================
//  グローバル変数宣言
//==========================================
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshCylinder = NULL;
LPDIRECT3DTEXTURE9 g_pTextureMeshCylinder = NULL;
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshCylinder = NULL;
MeshCylinder g_aMeshCylinder; //床の情報
D3DXVECTOR3 g_aCollisionPos[((int)MAX_VERTEX_CYLINDER)] = {}; //頂点座標

//==========================================
//  初期化処理
//==========================================
void InitMeshCylinder(void)
{
	//各種変数の初期化
	g_aMeshCylinder.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshCylinder.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aMeshCylinder.nShadowNum = -1;

	//ポリゴンの初期化
	g_pVtxBuffMeshCylinder = InitPolygon(20);

	//テクスチャの初期化
	g_pTextureMeshCylinder = InitTextureJpg("stellar_into_the_galaxy");

	//頂点バッファの呼び出し
	VERTEX_3D *pVtx;

	//頂点バッファをロック
	g_pVtxBuffMeshCylinder->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	for (int nCntVtx = 0; nCntVtx < MAX_VERTEX_CYLINDER * 0.5f; nCntVtx++)
	{
		pVtx[nCntVtx].pos = D3DXVECTOR3
		(
			sinf(D3DX_PI * (1.0f - nCntVtx * (1.0f / VERTEX_ANGLE))) * CYLINDER_SIZE,
			CYLINDER_HEIGHT, 
			cosf(D3DX_PI * (1.0f - nCntVtx * (1.0f / VERTEX_ANGLE))) * CYLINDER_SIZE
		);

		pVtx[nCntVtx + (int)(MAX_VERTEX_CYLINDER * 0.5f)].pos = D3DXVECTOR3
		(
			sinf(D3DX_PI * (1.0f - nCntVtx * (1.0f / VERTEX_ANGLE))) * CYLINDER_SIZE,
			0.0f,
			cosf(D3DX_PI * (1.0f - nCntVtx * (1.0f / VERTEX_ANGLE))) * CYLINDER_SIZE
		);
	}

	//頂点情報の保存
	for (int nCntVtx = 0; nCntVtx < MAX_VERTEX_CYLINDER; nCntVtx++)
	{
		g_aCollisionPos[nCntVtx] = pVtx[nCntVtx].pos;
	}

	//法線ベクトルの設定
	for (int nCntNor = 0; nCntNor < MAX_VERTEX_CYLINDER; nCntNor++)
	{
		D3DXVECTOR3 nor = pVtx[nCntNor].pos;
		D3DXVec3Normalize(&pVtx[nCntNor].nor, &nor);
	}

	//頂点バッファをアンロック
	g_pVtxBuffMeshCylinder->Unlock();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer
	(
		sizeof(WORD) * MAX_INDEX_CYLINDER,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshCylinder,
		NULL
	);

	//インデックス情報へのポインタ
	WORD *pIdx;

	//インデックスバッファをロックし、頂点番号データへのポインタを取得
	g_pIdxBuffMeshCylinder->Lock(0, 0, (void**)&pIdx, 0);

	//頂点データの設定
	for (int nCntIdx = 0; nCntIdx < MAX_VERTEX_CYLINDER * 0.5f; nCntIdx++)
	{
		//設定するインデックスを指定
		int nIdx = nCntIdx * 2;

		//インデックスを設定
		pIdx[nIdx] = ((int)MAX_VERTEX_CYLINDER - 1) - nCntIdx;
		pIdx[nIdx + 1] = (int)((MAX_VERTEX_CYLINDER * 0.5f - 1) - nCntIdx);
	}

	//インデックスバッファのアンロック
	g_pIdxBuffMeshCylinder->Unlock();
}

//==========================================
//  終了処理
//==========================================
void UninitMeshCylinder(void)
{
	//ポリゴンの終了
	UninitPolygon(&g_pVtxBuffMeshCylinder);

	//テクスチャの終了
	UninitTexture(&g_pTextureMeshCylinder);

	//インデックスバッファの破棄
	if (g_pIdxBuffMeshCylinder != NULL)
	{
		g_pIdxBuffMeshCylinder->Release();
		g_pIdxBuffMeshCylinder = NULL;
	}
}

//==========================================
//  更新処理
//==========================================
void UpdateMeshCylinder(void)
{

}

//==========================================
//  描画処理
//==========================================
void DrawMeshCylinder(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ローカル変数宣言
	D3DXMATRIX mtxRot, mtxTrans; //計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_aMeshCylinder.mtxWorld);

	//向きの反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshCylinder.rot.y, g_aMeshCylinder.rot.x, g_aMeshCylinder.rot.z);
	D3DXMatrixMultiply(&g_aMeshCylinder.mtxWorld, &g_aMeshCylinder.mtxWorld, &mtxRot);

	//位置の反映
	D3DXMatrixTranslation(&mtxTrans, g_aMeshCylinder.pos.x, g_aMeshCylinder.pos.y, g_aMeshCylinder.pos.z);
	D3DXMatrixMultiply(&g_aMeshCylinder.mtxWorld, &g_aMeshCylinder.mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_aMeshCylinder.mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMeshCylinder, 0, sizeof(VERTEX_3D));

	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(g_pIdxBuffMeshCylinder);

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureMeshCylinder);

	//ポリゴンの描画
	pDevice->DrawIndexedPrimitive
	(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		(int)MAX_VERTEX_CYLINDER, //頂点数
		0,
		MAX_INDEX_CYLINDER - 2 //プリミティブ数
	);
}

//==========================================
//  頂点座標の取得
//==========================================
D3DXVECTOR3* GetVtxPos(void)
{
	return &g_aCollisionPos[0];
}
