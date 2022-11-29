//==========================================
//
//  テクスチャの制御(texture.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "texture.h"
#include <string.h>

//==========================================
//  マクロ定義
//==========================================
#define TEXTURE_PASS "data\\TEXTURE\\" //全テクスチャに共通するパス
#define FORMAT_PNG ".png" //テクスチャのファイル形式
#define FORMAT_JPG ".jpg" //テクスチャのファイル形式

//==========================================
//  初期化処理(png)
//==========================================
LPDIRECT3DTEXTURE9 InitTexturePng(char aStr[MAX_STRING])
{
	//ローカル変数宣言
	LPDIRECT3DTEXTURE9 pTexture;
	char aTexturePass[128] = TEXTURE_PASS;

	//テクスチャのパスを設定する
	strcat(&aTexturePass[0], &aStr[0]);
	strcat(&aTexturePass[0], FORMAT_PNG);

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		&aTexturePass[0],
		&pTexture
	);

	//変数を返す
	return pTexture;
}

//==========================================
//  初期化処理(jpg)
//==========================================
LPDIRECT3DTEXTURE9 InitTextureJpg(char aStr[MAX_STRING])
{
	//ローカル変数宣言
	LPDIRECT3DTEXTURE9 pTexture;
	char aTexturePass[128] = TEXTURE_PASS;

	//テクスチャのパスを設定する
	strcat(&aTexturePass[0], &aStr[0]);
	strcat(&aTexturePass[0], FORMAT_JPG);

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile
	(
		pDevice,
		&aTexturePass[0],
		&pTexture
	);

	//変数を返す
	return pTexture;
}

//==========================================
//  終了処理
//==========================================
void UninitTexture(LPDIRECT3DTEXTURE9 *pTexture, int nNum)
{
	for (int nCnt = 0; nCnt < nNum; nCnt++, pTexture++)
	{
		if ((*pTexture) != NULL)
		{
			(*pTexture)->Release();
			(*pTexture) = NULL;
		}
	}
}