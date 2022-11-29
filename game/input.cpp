//==========================================
//
//  入力の制御(input.cpp)
//  Author : Tomoya Kanazaki
//
//==========================================
#include "input.h"
#include "limits.h"
#include "option.h"

//==========================================
//  マクロ定義
//==========================================
#define NUM_KEY_MAX	(256) //キーの最大値
#define MAX_PLAYER (4) //プレイヤー数
#define MOUSE_SENS (0.01f) //マウス感度の補正

//==========================================
//  各デバイスの構造体
//==========================================
//キーボード
typedef struct
{
	LPDIRECTINPUTDEVICE8 pDevKeyboard = NULL; //入力デバイスへのポインタ
	BYTE aKeyState[NUM_KEY_MAX]; //全入力情報の保管
	BYTE aKeyStateTrigger[NUM_KEY_MAX]; //トリガー情報
	BYTE aKeyStateRerease[NUM_KEY_MAX]; //リリース情報
	BYTE aKeyStateRepeat[NUM_KEY_MAX]; //リピート情報
}KeyBoard;

//マウス
typedef struct
{
	LPDIRECTINPUTDEVICE8 pDevMouse = NULL; //入力デバイスへのポインタ
	DIMOUSESTATE2 MouseState; //全入力情報の保管
	DIMOUSESTATE2 MouseStateTrigger; //トリガー情報
	DIMOUSESTATE2 MouseStateRerease; //リリース情報
	DIMOUSESTATE2 MouseStateRepeat; //リピート情報
	BYTE aOldState[8]; //前回の入力情報を保存
}Mouse;

//ジョイパッド
typedef struct
{
	LPDIRECTINPUTDEVICE8 pDevJoypad = NULL; //入力デバイスへのポインタ
	DIJOYSTATE aKeyState; //全入力情報の保管
	DIJOYSTATE aKeyStateTrigger; //トリガー情報
	DIJOYSTATE aKeyStateRerease; //リリース情報
	DIJOYSTATE aKeyStateRepeat; //リピート情報
	BYTE aOldState[32]; //前回の入力情報を保存
}JoyPad;

//==========================================
//  グローバル変数宣言
//==========================================
LPDIRECTINPUT8 g_pInput = NULL; //DirectInputオブジェクトへのポインタ
KeyBoard g_KeyBoard; //キーボードの情報
JoyPad g_aJoyPad[MAX_PLAYER]; //ジョイパッドの情報
Mouse g_Mouse; //マウスの情報
int g_nJoyNumCnt; //使用するジョイパッドのカウント

//==========================================
//  デバイスの初期化
//==========================================
HRESULT InitDevice(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInputオブジェクトの生成
	if (g_pInput == nullptr)
	{
		if (FAILED(DirectInput8Create
		(
			hInstance,
			DIRECTINPUT_VERSION,
			IID_IDirectInput8,
			(void**)&g_pInput,
			NULL
		)))
		{
			return E_FAIL;
		}
	}

	//キーボード
	if (FAILED(InitKeyboard(hWnd)))
	{
		return E_FAIL;
	}

	//ジョイパッド
	InitJoyPad(hWnd);

	//マウス
	InitMouse(hWnd);

	return S_OK;
}

//==========================================
//  デバイスの終了処理
//==========================================
void UninitDevice(void)
{
	//キーボード
	UninitKeyboard();

	//ジョイパッド
	UninitJoyPad();

	//マウス
	UninitMouse();

	//DirectInputオブジェクトの破棄
	if (g_pInput != NULL)
	{
		g_pInput->Release();
		g_pInput = NULL;
	}
}

//==========================================
//  デバイスの更新処理
//==========================================
void UpdateDevice(void)
{
	//キーボードの更新処理
	UpdateKeyboard();

	//ジョイパッドの更新処理
	UpdateJoyPad();

	//マウスの更新処理
	UpdateMouse();
}

//==========================================
//  デバイスを列挙してデバイスを生成するコールバック関数
//==========================================
BOOL CALLBACK EnumJoySticksCallBack(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext)
{
	//ローカル変数宣言
	HRESULT hr;
	LPDIRECTINPUTDEVICE8 pDevJoypad = g_aJoyPad[g_nJoyNumCnt].pDevJoypad;

	//NULLチェック
	if (pDevJoypad != nullptr)
	{
		return E_FAIL;
	}

	//デバイスの生成
	hr = g_pInput->CreateDevice(pdidInstance->guidInstance, &pDevJoypad, NULL);
	if (FAILED(hr))
	{
		return E_FAIL;
	}

	//生成した変数を代入
	g_aJoyPad[g_nJoyNumCnt].pDevJoypad = pDevJoypad;

	//次のデバイスを調べるときはDIENUM_CONTINUE、最初の一回のみの場合はDIENUM_STOP
	return DIENUM_CONTINUE;
}

//==========================================
//  十字キーの範囲選択を行うコールバック関数
//==========================================
BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *pdidInstance, VOID *pContext)
{
	//ローカル変数宣言(引数をキャストして代入)
	LPDIRECTINPUTDEVICE8 pDevJoypad = (LPDIRECTINPUTDEVICE8)pContext;

	//入力範囲のセット
	DIPROPRANGE diprg;
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.lMax = 1000;
	diprg.lMin = -1000;

	//X軸
	diprg.diph.dwObj = DIJOFS_X;
	pDevJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);

	//Y軸
	diprg.diph.dwObj = DIJOFS_Y;
	pDevJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);

	//Z軸
	diprg.diph.dwObj = DIJOFS_Z;
	pDevJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);

	//こづなおりじなる

	//RX軸
	diprg.diph.dwObj = DIJOFS_RX;
	pDevJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);

	//RY軸
	diprg.diph.dwObj = DIJOFS_RY;
	pDevJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);

	//RZ軸
	diprg.diph.dwObj = DIJOFS_RZ;
	pDevJoypad->SetProperty(DIPROP_RANGE, &diprg.diph);

	//次のデバイスを調べるときはDIENUM_CONTINUE、最初の一回のみの場合はDIENUM_STOP
	return DIENUM_CONTINUE;
}

//==========================================
//  キーボード初期化処理
//==========================================
HRESULT InitKeyboard(HWND hWnd)
{
	//入力デバイスの設定
	if (FAILED(g_pInput->CreateDevice(GUID_SysKeyboard, &g_KeyBoard.pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(g_KeyBoard.pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(g_KeyBoard.pDevKeyboard->SetCooperativeLevel
	(
		hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)
	)))
	{
		return E_FAIL;
	}

	//キーボードへのアクセス権を獲得
	g_KeyBoard.pDevKeyboard->Acquire();

	return S_OK;
}

//==========================================
//  キーボード終了処理
//==========================================
void UninitKeyboard(void)
{
	//入力デバイス(キーボード)の破棄
	if (g_KeyBoard.pDevKeyboard != NULL)
	{
		g_KeyBoard.pDevKeyboard->Unacquire();
		g_KeyBoard.pDevKeyboard->Release();
		g_KeyBoard.pDevKeyboard = NULL;
	}
}

//==========================================
//  キーボード更新処理
//==========================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX]; //キーボードの入力情報
	int nCntKey;

	//入力デバイスからデータを取得
	if (SUCCEEDED(g_KeyBoard.pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			g_KeyBoard.aKeyStateTrigger[nCntKey] = (g_KeyBoard.aKeyState[nCntKey] ^ aKeyState[nCntKey]) & aKeyState[nCntKey]; //キーボードのトリガー情報の保存
			g_KeyBoard.aKeyState[nCntKey] = aKeyState[nCntKey]; //キーボードのプレス情報の保存
		}
	}
	else
	{
		g_KeyBoard.pDevKeyboard->Acquire(); //キーボードのアクセス権
	}
}

//==========================================
//  キーボードのプレス情報の取得
//==========================================
bool GetKeyboardPress(int nKey)
{
	return (g_KeyBoard.aKeyState[nKey] & 0x80) ? true : false;
}

//==========================================
//  キーボードのトリガー情報の取得
//==========================================
bool GetKeyboardTrigger(int nKey)
{
	return (g_KeyBoard.aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//==========================================
//  初期化処理
//==========================================
HRESULT InitJoyPad(HWND hWnd)
{
	//数値の初期化
	g_nJoyNumCnt = 0;

	//デバイスの列挙
	if (FAILED(g_pInput->EnumDevices(
		DI8DEVCLASS_GAMECTRL,
		EnumJoySticksCallBack,
		nullptr,
		DIEDFL_ATTACHEDONLY
	)))
	{
		return E_FAIL;
	}

	//入力デバイスの設定
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (g_aJoyPad[nCnt].pDevJoypad == nullptr)
		{
			continue;
		}

		//デバイスのフォーマットの設定
		HRESULT hr = g_aJoyPad[nCnt].pDevJoypad->SetDataFormat(&c_dfDIJoystick);

		if (FAILED(hr))
		{
			return E_FAIL;
		}

		//協調モードの設定
		hr = g_aJoyPad[nCnt].pDevJoypad->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);

		if (FAILED(hr))
		{
			return E_FAIL;
		}

		//デバイスに対しての十字キーの範囲等を指定
		if (FAILED(g_aJoyPad[nCnt].pDevJoypad->EnumObjects(EnumAxesCallback,
			g_aJoyPad[nCnt].pDevJoypad,
			DIDFT_AXIS)))
		{
			return E_FAIL;
		}
	}

	//変数を返す
	return S_OK;
}

//==========================================
//  終了処理
//==========================================
void UninitJoyPad(void)
{
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		//入力デバイスの破棄
		if (g_aJoyPad[nCnt].pDevJoypad != NULL)
		{
			g_aJoyPad[nCnt].pDevJoypad->Unacquire();
			g_aJoyPad[nCnt].pDevJoypad->Release();
			g_aJoyPad[nCnt].pDevJoypad = NULL;
		}
	}
}

//==========================================
//  更新処理
//==========================================
void UpdateJoyPad(void)
{
	for (int nCntPlayer = 0; nCntPlayer < MAX_PLAYER; nCntPlayer++)
	{
		//NULLチェック
		if (g_aJoyPad[nCntPlayer].pDevJoypad == nullptr)
		{
			continue;
		}

		//デバイスからデータを取得できることを確認し、できなかった場合はアクセス権が取得
		HRESULT hr = g_aJoyPad[nCntPlayer].pDevJoypad->Poll();

		if (FAILED(hr))
		{
			hr = g_aJoyPad[nCntPlayer].pDevJoypad->Acquire();
			while (hr == DIERR_INPUTLOST)
			{
				hr = g_aJoyPad[nCntPlayer].pDevJoypad->Acquire();
			}
		}

		//一時保管場所
		DIJOYSTATE JoyKey;

		//コントローラの状態を取得
		if (SUCCEEDED(g_aJoyPad[nCntPlayer].pDevJoypad->GetDeviceState(sizeof(JoyKey), &JoyKey)))
		{
			for (int nCntKey = 0; nCntKey < 32; nCntKey++)
			{
				g_aJoyPad[nCntPlayer].aKeyStateTrigger.rgbButtons[nCntKey] = 
					(
						(g_aJoyPad[nCntPlayer].aKeyStateTrigger.rgbButtons[nCntKey] ^
						JoyKey.rgbButtons[nCntKey]) &
						JoyKey.rgbButtons[nCntKey]
					);
			}

			//入力情報の保存
			g_aJoyPad[nCntPlayer].aKeyState = JoyKey;
		}
	}
}

//==========================================
//  ボタン情報の取得
//==========================================
bool GetJoyPadButtonPress(JOYKEY_BUTTON nKey, int nPlayer) //プレス
{
	return (g_aJoyPad[nPlayer].aKeyState.rgbButtons[nKey] & 0x80) ? true : false;
}

bool GetJoyPadButtonTrigger(JOYKEY_BUTTON nKey, int nPlayer) //トリガー
{
	return (g_aJoyPad[nPlayer].aKeyStateTrigger.rgbButtons[nKey] & 0x80) ? true : false;
}

//==========================================
//  スティックの判定
//==========================================
D3DXVECTOR3 GetStickL(int nPlayer) //左
{
	//ローカル変数宣言
	D3DXVECTOR3 stick = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//返り値の設定
	stick.x = (float)g_aJoyPad[nPlayer].aKeyState.lX / 1000.0f;
	stick.y = -(float)g_aJoyPad[nPlayer].aKeyState.lY / 1000.0f;

	//変数を返す
	return stick;
}

D3DXVECTOR3 GetStickR(int nPlayer) //右
{
	//ローカル変数宣言
	D3DXVECTOR3 stick = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//返り値の設定
	stick.x = (float)g_aJoyPad[nPlayer].aKeyState.lZ / 1000.0f;
	stick.y = (float)g_aJoyPad[nPlayer].aKeyState.lRz / 1000.0f;

	//変数を返す
	return stick;
}

//==========================================
//  マウスの初期化
//==========================================
HRESULT InitMouse(HWND hWnd)
{
	//入力デバイスの設定
	if (FAILED(g_pInput->CreateDevice(GUID_SysMouse, &g_Mouse.pDevMouse, NULL)))
	{
		return E_FAIL;
	}

	//データフォーマットを設定
	if (FAILED(g_Mouse.pDevMouse->SetDataFormat(&c_dfDIMouse2)))
	{
		return E_FAIL;
	}

	//協調モードを設定
	if (FAILED(g_Mouse.pDevMouse->SetCooperativeLevel
	(
		hWnd,
		(DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)
	)))
	{
		return E_FAIL;
	}

	// デバイスの設定
	DIPROPDWORD diprop;
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwObj = 0;
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.dwData = DIPROPAXISMODE_REL; //相対値モードで設定（絶対値はDIPROPAXISMODE_ABS）

	if (FAILED(g_Mouse.pDevMouse->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		// デバイスの設定に失敗
		return E_FAIL;
	}

	//マウスへのアクセス権を取得
	g_Mouse.pDevMouse->Acquire();

#ifndef _DEBUG

	//マウスカーソルの消去
	ShowCursor(false);

#endif //_DEBUG

	return S_OK;
}

//==========================================
//  マウスの終了処理
//==========================================
void UninitMouse(void)
{
	//入力デバイス(マウス)の破棄
	if (g_Mouse.pDevMouse != NULL)
	{
		g_Mouse.pDevMouse->Unacquire();
		g_Mouse.pDevMouse->Release();
		g_Mouse.pDevMouse = NULL;
	}
}

//==========================================
//  マウスの更新処理
//==========================================
void UpdateMouse(void)
{
	DIMOUSESTATE2 mouse; //マウスの入力情報

	//入力デバイスからデータを取得
	if (SUCCEEDED(g_Mouse.pDevMouse->GetDeviceState(sizeof(mouse), &mouse)))
	{
		//入力情報の保存
		g_Mouse.MouseState = mouse;
	}
	else
	{
		g_Mouse.pDevMouse->Acquire(); //マウスへのアクセス権
	}
}

//==========================================
//  マウスのプレス情報
//==========================================
bool GetMousePress(MOUSE_BUTTON nKey)
{
	return (g_Mouse.MouseState.rgbButtons[nKey] & 0x80) ? true : false;
}

//==========================================
//  マウスの移動量
//==========================================
D3DXVECTOR3 GetMouseMove(void)
{
	return D3DXVECTOR3(((float)g_Mouse.MouseState.lX) * MOUSE_SENS, 0.0f, ((float)g_Mouse.MouseState.lY) * MOUSE_SENS);
}
