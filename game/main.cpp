//==========================================
//
//  �Q�[���S�ʂ̐���(main.h)
//  Author : Tomoya Kanazaki
//
//==========================================
#include <time.h>
#include "main.h"
#include "field.h"
#include "camera.h"
#include "light.h"
#include "player.h"
#include "input.h"
#include "option.h"
#include "shadow.h"
#include "wall.h"
#include "billboard.h"
#include "bullet.h"
#include "effect.h"
#include "explosion.h"
#include "meshfield.h"
#include "meshwall.h"
#include "meshcylinder.h"
#include "colllision.h"

//==========================================
//  �O���[�o���ϐ��錾
//==========================================
LPDIRECT3D9 g_pD3D = NULL; //Direct3D�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL; //Direct3D�f�o�C�X�ւ̃|�C���^
LPD3DXFONT g_pFont = NULL; //�t�H���g�ւ̃|�C���^
int g_nCountFPS = 0; //FPS�J�E���^
MODE g_mode = MODE_TITLE;
bool g_bOption = false;

//==========================================
//  ���C���֐�
//==========================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX), //WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC, //�E�B���h�E�̃X�^�C��
		WindowsProc, //�E�B���h�E�v���V�[�W��
		0, //0�ɂ���(�ʏ�͎g�p���Ȃ�)
		0, //0�ɂ���(�ʏ�͎g�p���Ȃ�)
		hInstance, //�C���X�^���X�n���h��
		LoadIcon(NULL,IDI_APPLICATION), //�^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL,IDC_ARROW), //�}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1), //�N���C�A���g�̔w�i�F
		NULL, //���j���[�o�[
		CLASS_NAME, //�E�B���h�E�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION), //�t�@�C���̃A�C�R��
	};
	HWND hWnd; //�E�B���h�E�n���h��
	MSG msg; //���b�Z�[�W���i�[����ϐ�
	DWORD dwCurrentTime; //���ݎ���
	DWORD dwExecLastTime; //�Ō�ɏ�����������
	DWORD dwFrameCount; //�t���[���J�E���g
	DWORD dwFPSLastTime; //�Ō��FPS���v����������

	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	//�E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	//�N���C�A���g�̈���w��̃T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//�E�B���h�E�𐶐�
	hWnd = CreateWindowEx
	(
		0, //�g���E�B���h�E�X�^�C��
		CLASS_NAME, //�E�B���h�E�N���X�̖��O
		WINDOW_NAME, //�E�B���h�E�̖��O
		WS_OVERLAPPEDWINDOW, //�E�B���h�E�X�^�C��
		CW_USEDEFAULT, //�E�B���h�E�̍���X���W
		CW_USEDEFAULT, //�E�B���h�E�̍���Y���W
		(rect.right - rect.left), //�E�B���h�E�̕�
		(rect.bottom - rect.top), //�E�B���h�E�̍���
		NULL, //�V�K�E�B���h�E�̃n���h��
		NULL, //���j���[�n���h���܂��͎q�E�B���h�EID
		hInstance, //�C���X�^���X�n���h��
		NULL
	);

	//����������
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{
		//�����������Ɏ��s�����ꍇ
		return -1;
	}

	//����\��ݒ�
	timeBeginPeriod(1);
	dwCurrentTime = 0; //������
	dwExecLastTime = timeGetTime(); //���ݎ������擾

	//�f�o�b�O�\���p�̐��l�̐ݒ�
	dwFrameCount = 0; //������
	dwFPSLastTime = timeGetTime(); //���ݎ������擾

	//�E�B���h�E�̕\��
	ShowWindow(hWnd, nCmdShow); //�E�B���h�E�̕\����Ԃ�ݒ�
	UpdateWindow(hWnd); //�N���C�A���g�̈�

	//���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			//windows�̏���
			if (msg.message == WM_QUIT)
			{
				//WM_QUIT���b�Z�[�W���󂯎�����ꍇ
				break;
			}
			else
			{
				//���b�Z�[�W�̐ݒ�
				TranslateMessage(&msg); //���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
				DispatchMessage(&msg); //�E�B���h�E�v���V�[�W���փ��b�Z�[�W�𑗏o
			}
		}
		else
		{
			//DirectX�̏���
			dwCurrentTime = timeGetTime(); //���ݎ������擾

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{
				//0.5�b�o��
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime; //FPS�𑪒肵������
				dwFrameCount = 0; //�t���[���J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{
				//60����1�b�o��
				dwExecLastTime = dwCurrentTime;
				dwFrameCount++; //�t���[���J�E���g�����Z

				//�X�V����
				Update();
				//�`�揈��
				Draw();
			}
		}
	}

	//�I������
	Uninit();

	//����\��߂�
	timeEndPeriod(1);

	//�E�B���h�E�̃N���X�̉���
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//==========================================
//  �E�B���h�E�v���[�W��
//==========================================
LRESULT CALLBACK WindowsProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY: //�E�B���h�E�j���̃��b�Z�[�W
		//WM_DUIT�̃��b�Z�[�W�𑗂�
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN: //�L�[�������b�Z�[�W
		switch (wParam)
		{
		case VK_ESCAPE:
			//�E�B���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�)
			DestroyWindow(hWnd);
			break;
		}
		break;

	case WM_CLOSE: //�L�[�������b�Z�[�W
		//�E�B���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�)
		DestroyWindow(hWnd);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam); //����̏�����Ԃ�
}

//==========================================
//  ����������
//==========================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm; //�f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp; //�v���[���e�[�V�����p�����[�^
	
	//Direct3D�I�u�W�F�N�g
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp)); //�p�����[�^�̃[���N���A

	d3dpp.BackBufferWidth = SCREEN_WIDTH; //�Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT; //�Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format; //�o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1; //�o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; //�_�u���o�b�t�@�̐؂�ւ�
	d3dpp.EnableAutoDepthStencil = TRUE; //�f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16; //�f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow; //�E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; //���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT; //�C���^�[�o��

	//Direct3D�f�o�C�X�̐���
	if (FAILED(g_pD3D->CreateDevice
	(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice
	)))
	{
		if (FAILED(g_pD3D->CreateDevice
		(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice
		)))
		{
			if (FAILED(g_pD3D->CreateDevice
			(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_HARDWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice
			)))
			{
				return E_FAIL;
			}
		}
	}

	//�����_�[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//�e�N�X�`���X�e�[�g
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//�f�o�b�O�\���p�t�H���g�̐���
	D3DXCreateFont
	(
		g_pD3DDevice, 18, 0, 0, 0, FALSE,
		SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		"Terminal", &g_pFont
	);

	//�e��I�u�W�F�N�g�̏���������

	//�����V�[�h�̐ݒ�
	srand((unsigned int)time(0));

	//�f�o�C�X�̏�����
	if (FAILED(InitDevice(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	//�J�����̏�����
	InitCamera();

	//�r���{�[�h�̏�����
	InitBillboard();

	//���C�g�̏�����
	InitLight();

	//�e�̏�����
	InitShadow();

	//���f���̏�����
	InitPlayer();

	//�e�̏�����
	InitBullet();

	//�G�t�F�N�g�̏�����
	InitEffect();

	//�����̏�����
	InitExplosion();

	//���̏�����
	//InitMeshField();

	//���̏�����
	InitField();

	//�ǂ̏�����
	//InitMeshWall();

	//�ǂ̏�����
	//InitWall();

	//�V�����_�[�̏�����
	InitMeshCylinder();

	return S_OK;
}

//==========================================
//  �I������
//==========================================
void Uninit(void)
{
	//�e��I�u�W�F�N�g�̏I������

	//�f�o�C�X�̏I��
	UninitDevice();

	//�J�����̏I��
	UninitCamera();

	//�r���{�[�h�̏I��
	UninitBillboard();

	//���C�g�̏I��
	UninitLight();

	//���f���̏I��
	UninitPlayer();

	//�e�̏I��
	UninitBullet();

	//�G�t�F�N�g�̏I��
	UninitEffect();

	//�����̏I��
	UninitExplosion();

	//���̏I��
	//UninitMeshField();

	//���̏I������
	InitField();

	//�ǂ̏I������
	//InitMeshWall();

	//�ǂ̏I��
	//UninitWall();

	//�V�����_�[�̏I��
	UninitMeshCylinder();

	//�e�̏I������
	UninitShadow();

	//�f�o�b�O�p�t�H���g�̔j��
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}

	//Direct3D�f�o�C�X�̔j��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3D�I�u�W�F�N�g�̔j��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//==========================================
//  �X�V����
//==========================================
void Update(void)
{
#ifdef _DEBUG
	//�I�v�V�����̃t���O�Ǘ�
	if (GetKeyboardTrigger(DIK_F1))
	{
		g_bOption = !g_bOption;
	}

	//�I�v�V�����̍X�V
	if (g_bOption)
	{
		UpdateOption();
	}
#endif //_DEBUG

	//�f�o�C�X�̍X�V
	UpdateDevice();

	//�J�����̍X�V
	UpdateCamera();

	//�r���{�[�h�̍X�V
	UpdateBillboard();

	//���C�g�̍X�V
	UpdateLight();

	//���f���̍X�V
	UpdatePlayer();

	//�e�̍X�V
	UpdateBullet();

	//�G�t�F�N�g�̍X�V
	UpdateEffect();

	//�����̍X�V
	UpdateExplosion();

	//���̍X�V
	//UpdateMeshField();

	//���̍X�V����
	UpdateField();

	//�ǂ̍X�V����
	//UpdateMeshWall();

	//�ǂ̍X�V
	//UpdateWall();

	//�V�����_�[�̍X�V
	UpdateMeshCylinder();

	//�e�̏�����
	UpdateShadow();
}

//==========================================
//  �`�揈��
//==========================================
void Draw(void)
{
	//��ʃN���A
	g_pD3DDevice->Clear
	(
		0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0
	);

	//�`��J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		//�J�����̐ݒ�
		SetCamera();

		//���̕`��
		//DrawMeshField();

		//���̕`�揈��
		DrawField();

		//�r���{�[�h�̕`��
		DrawBillboard();

		//���f���̕`��
		DrawPlayer();

		//�e�̕`��
		DrawBullet();

		//�e�̕`��
		DrawShadow();

		//�ǂ̕`�揈��
		//DrawMeshWall();

		//�ǂ̕`��
		//DrawWall();

		//�V�����_�[�̕`��
		DrawMeshCylinder();

		//�����̕`��
		DrawExplosion();

		//�G�t�F�N�g�̕`��
		DrawEffect();

#ifdef _DEBUG

		DrawFPS();
		DrawPlayerDebug();
		DrawCameraDebug();
		DrawDebuWall();
		DrowDebugShadow();

		if (g_bOption)
		{
			DrawOption();
		}

#endif //_DEBUG

		//�`��I��
		g_pD3DDevice->EndScene();
	}

	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̐؂�ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//==========================================
//  ���[�h�ݒ�
//==========================================
void SetMode(MODE mode)
{
	
}

//==========================================
//  �f�o�C�X�̎擾
//==========================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//==========================================
//  �t�H���g�̎擾
//==========================================
LPD3DXFONT GetFont(void)
{
	return g_pFont;
}

//==========================================
//  FPS�̕\��
//==========================================
void DrawFPS(void)
{
	//���[�J���ϐ��錾
	char aStr[256];

	//�t�H���g�̎擾
	LPD3DXFONT pFont = GetFont();

	//�͈͂̎w��
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };

	//������ɑ��
	wsprintf(&aStr[0], "FPS:%d\n", g_nCountFPS);

	//�e�L�X�g�̕`��
	pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_RIGHT, D3DCOLOR_RGBA(255, 255, 255, 255));
}

//==========================================
//  �Q�[�����[�h�̎擾
//==========================================
MODE GetMode(void)
{
	return g_mode;
}