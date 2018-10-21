#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"
#include "Sprites.h"
#include "Animations.h"

#include "Simon.h"

#define WINDOW_CLASS_NAME L"Game"
#define MAIN_WINDOW_TITLE L"Castlevania"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(255, 255, 200)
#define SCREEN_WIDTH		800
#define SCREEN_HEIGHT		800

#define MAX_FRAME_RATE		90

#define ID_TEX_SIMON		0

CGame *game;
CSimon *simon;

vector<LPGAMEOBJECT> objects;

#pragma region Player input

class CInputHandler : public CKeyEventHandler
{
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int keyCode);
	virtual void OnKeyUp(int keyCode);
};

CInputHandler * inputHandler;

void CInputHandler::OnKeyDown(int keyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", keyCode);
	switch (keyCode)
	{
	case DIK_Z:
		simon->SetState(SIMON_STATE_JUMPING);
		break;
	case DIK_X:
		simon->SetState(SIMON_STATE_ATTACKING);
		break;
	default:
		break;
	}
}

void CInputHandler::OnKeyUp(int keyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", keyCode);
}

void CInputHandler::KeyState(BYTE *states)
{
	if (game->IsKeyDown(DIK_DOWN))
		simon->SetState(SIMON_STATE_CROUCHING);

	else if (game->IsKeyDown(DIK_RIGHT))
		simon->SetState(SIMON_STATE_WALKING_RIGHT);

	else if (game->IsKeyDown(DIK_LEFT))
		simon->SetState(SIMON_STATE_WALKING_LEFT);

	else 
		simon->SetState(SIMON_STATE_IDLE);
}

#pragma endregion

#pragma region Window functions

LRESULT CALLBACK WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

HWND CreateGameWindow(HINSTANCE hInstance, int nCmdShow, int ScreenWidth, int ScreenHeight)
{
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.hInstance = hInstance;

	wc.lpfnWndProc = (WNDPROC)WinProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;
	wc.hIconSm = NULL;

	RegisterClassEx(&wc);

	HWND hWnd =
		CreateWindow(
			WINDOW_CLASS_NAME,
			MAIN_WINDOW_TITLE,
			WS_OVERLAPPEDWINDOW, // WS_EX_TOPMOST | WS_VISIBLE | WS_POPUP,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			ScreenWidth,
			ScreenHeight,
			NULL,
			NULL,
			hInstance,
			NULL);

	if (!hWnd)
	{
		OutputDebugString(L"[ERROR] CreateWindow failed");
		DWORD ErrCode = GetLastError();
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return hWnd;
}

#pragma endregion


/*
	Load all game resources
	In this example: load textures, sprites, animations and mario object

	TO-DO: Improve this function by loading texture,sprite,animation,object from file
*/
void LoadResources()
{
	//
	// LOAD TEXTURES
	//
	CTextures * textures = CTextures::GetInstance();
	textures->Add(ID_TEX_SIMON, L"Textures\\simon.png", D3DCOLOR_XRGB(255, 0, 255));

	//
	// LOAD SPRITES AND ANIMATIONS
	//
	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();

	LPDIRECT3DTEXTURE9 texSimon = textures->Get(ID_TEX_SIMON);
	//                 left, top, right, bottom
	sprites->Add(99000, 436, 4, 468, 64, texSimon);		// idle right
	sprites->Add(99100, 12, 202, 44, 262, texSimon);	// idle left
				 
	sprites->Add(99001, 378, 2, 402, 64, texSimon);		// walk right
	sprites->Add(99002, 314, 4, 344, 64, texSimon);		// walk right
	sprites->Add(99003, 260, 3, 284, 64, texSimon);		// walk right
	sprites->Add(99101, 78, 200, 102, 262, texSimon);	// walk left
	sprites->Add(99102, 136, 202, 166, 262, texSimon);	// walk left
	sprites->Add(99103, 196, 201, 220, 262, texSimon);	// walk left
				 
	sprites->Add(99007, 120, 4, 168, 64, texSimon);		// attack right
	sprites->Add(99008, 75, 4, 107, 64, texSimon);		// attack right
	sprites->Add(99009, 16, 6, 59, 64, texSimon);		// attack right
	sprites->Add(99107, 312, 202, 360, 262, texSimon);	// attack left
	sprites->Add(99108, 373, 202, 405, 262, texSimon);	// attack left
	sprites->Add(99109, 421, 204, 464, 262, texSimon);	// attack left
				 
	sprites->Add(99020, 196, 1, 229, 47, texSimon);		// crouch right
	sprites->Add(99120, 252, 199, 284, 245, texSimon);	// crouch left

	LPANIMATION ani;

	ani = new CAnimation(100);
	ani->AddFrame(99000);
	animations->Add(SimonAniID::IDLE_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99100);
	animations->Add(SimonAniID::IDLE_LEFT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99000);
	ani->AddFrame(99001);
	ani->AddFrame(99002);
	ani->AddFrame(99003);
	animations->Add(SimonAniID::WALK_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99100);
	ani->AddFrame(99101);
	ani->AddFrame(99102);
	ani->AddFrame(99103);
	animations->Add(SimonAniID::WALK_LEFT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99007);
	ani->AddFrame(99008);
	ani->AddFrame(99009);
	animations->Add(SimonAniID::ATTACK_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99107);
	ani->AddFrame(99108);
	ani->AddFrame(99109);
	animations->Add(SimonAniID::ATTACK_LEFT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99020);
	animations->Add(SimonAniID::CROUCH_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99120);
	animations->Add(SimonAniID::CROUCH_LEFT, ani);

	simon = new CSimon();
	simon->AddAnimation(SimonAniID::IDLE_RIGHT);
	simon->AddAnimation(SimonAniID::IDLE_LEFT);
	simon->AddAnimation(SimonAniID::WALK_RIGHT);
	simon->AddAnimation(SimonAniID::WALK_LEFT);
	simon->AddAnimation(SimonAniID::CROUCH_RIGHT);
	simon->AddAnimation(SimonAniID::CROUCH_LEFT);
	simon->AddAnimation(SimonAniID::ATTACK_RIGHT);
	simon->AddAnimation(SimonAniID::ATTACK_LEFT);

	simon->SetPosition(50.0f, 50.0f);
}


/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	simon->Update(dt);
}


/*
	Render a frame
*/
void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		simon->Render();

		spriteHandler->End();
		d3ddv->EndScene();
	}

	// Display back buffer content to the screen
	d3ddv->Present(NULL, NULL, NULL, NULL);
}


int Run()
{
	MSG msg;
	int done = 0;
	DWORD frameStart = GetTickCount();
	DWORD tickPerFrame = 1000 / MAX_FRAME_RATE;

	while (!done)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				done = 1;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		DWORD now = GetTickCount();

		// dt: the time between (beginning of last frame) and now
		// this frame: the frame we are about to render
		DWORD dt = now - frameStart;

		if (dt >= tickPerFrame)
		{
			frameStart = now;

			game->ProcessKeyboard();

			Update(dt);
			Render();
		}
		else
			Sleep(tickPerFrame - dt);
	}

	return 1;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = CGame::GetInstance();
	game->Init(hWnd);

	inputHandler = new CInputHandler();
	game->InitKeyboard(inputHandler);

	LoadResources();
	Run();

	return 0;
}