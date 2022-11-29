//==========================================
//
//  オプションの制御(option.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "option.h"
#include "input.h"

//==========================================
//  マクロ定義
//==========================================
#define MOUSE_INIT (0.1f) //マウス感度の初期値

//==========================================
//  グローバル変数宣言
//==========================================
float g_Mouse = MOUSE_INIT;

//==========================================
//  更新処理
//==========================================
void UpdateOption(void)
{
	//マウス感度の変更
	if (GetKeyboardTrigger(DIK_O))
	{
		g_Mouse += 0.01f;
	}
	if (GetKeyboardTrigger(DIK_L))
	{
		g_Mouse -= 0.01f;
	}
}

//==========================================
//  描画処理
//==========================================
void DrawOption(void)
{
	//ローカル変数宣言
	char aStr[256];

	//フォントの取得
	LPD3DXFONT pFont = GetFont();

	//範囲の指定
	RECT rect = { 0, 60, SCREEN_WIDTH, SCREEN_HEIGHT };

	//文字列に代入
	sprintf(&aStr[0], "マウス感度 : %.2f", g_Mouse);

	//テキストの描画
	pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_RIGHT, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
}

//==========================================
//  マウス感度の取得
//==========================================
float GetMouse(void)
{
	return g_Mouse;
}
