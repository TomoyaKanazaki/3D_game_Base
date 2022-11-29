//==========================================
//
//  ゲーム全般の制御(main.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#ifndef _MAIN_H_
#define _MAIN_H_
#include <Windows.h>
#include "d3dx9.h"
#define DIRECTINPUT_VERSION (0x0800) //ビルド時の警告対処用マクロ
#include "dinput.h"
#include <stdio.h>
#include "xaudio2.h"

//==========================================
//  ライブラリのリンク
//==========================================
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dinput8.lib")

//==========================================
//  マクロ定義
//==========================================
#define SCREEN_WIDTH (1280) //ウィンドウの幅
#define SCREEN_HEIGHT (720) //ウィンドウの高さ
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define CLASS_NAME "WindowClass"
#define WINDOW_NAME "3D雛形"

//==========================================
//  頂点座標[2D]の構造体を定義
//==========================================
typedef struct
{
	D3DXVECTOR3 pos;
	float rhw;
	D3DCOLOR col;
	D3DXVECTOR2 tex;
}VERTEX_2D;

//==========================================
//  頂点座標[3D]の構造体を定義
//==========================================
typedef struct
{
	D3DXVECTOR3 pos; //頂点座標
	D3DXVECTOR3 nor; //法線ベクトル
	D3DCOLOR col; //頂点カラー
	D3DXVECTOR2 tex; //テクスチャ座標
}VERTEX_3D;

//==========================================
//  列挙型定義
//==========================================
typedef enum
{
	MODE_TITLE = 0,
	MODE_TUTORIAL,
	MODE_GAME,
	MODE_RESULT,
	MODE_MAX
}MODE;

//==========================================
//  プロトタイプ
//==========================================
LRESULT CALLBACK WindowsProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
LPDIRECT3DDEVICE9 GetDevice(void);
LPD3DXFONT GetFont(void);
void Uninit(void);
void Update(void);
void Draw(void);
void SetMode(MODE mode);
void DrawFPS(void);
MODE GetMode(void);

#endif
