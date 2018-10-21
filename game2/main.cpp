#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "Textures.h"
#include "Sprites.h"
#include "Animations.h"
#include "MovableObject.h"

#include "Simon.h"
#include "Brick.h"

#define WINDOW_CLASS_NAME L"Game"
#define MAIN_WINDOW_TITLE L"Castlevania"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(255, 255, 200)
#define SCREEN_WIDTH		500
#define SCREEN_HEIGHT		400

#define MAX_FRAME_RATE		120

#define ID_TEX_SIMON		0
#define ID_TEX_GROUND		1

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
	case DIK_RIGHT:
		simon->SetState(SIMON_STATE_WALK_RIGHT);
		break;

	case DIK_LEFT:
		simon->SetState(SIMON_STATE_WALK_LEFT);
		break;

	case DIK_Z:
		simon->SetState(SIMON_STATE_JUMP);
		break;

	case DIK_X:
		simon->SetState(SIMON_STATE_ATTACK);
		break;

	default:
		break;
	}
}

void CInputHandler::OnKeyUp(int keyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", keyCode);
	switch (keyCode)
	{
	case DIK_DOWN:
		simon->SetState(SIMON_STATE_IDLE);
	}
}

void CInputHandler::KeyState(BYTE *states)
{
	if (game->IsKeyDown(DIK_DOWN))
		simon->SetState(SIMON_STATE_CROUCH);

	else if (game->IsKeyDown(DIK_RIGHT))
		simon->SetState(SIMON_STATE_WALK_RIGHT);

	else if (game->IsKeyDown(DIK_LEFT))
		simon->SetState(SIMON_STATE_WALK_LEFT);

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
	textures->Add(ID_TEX_GROUND, L"Textures\\ground1.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_BBOX, L"Textures\\bbox.png", D3DCOLOR_XRGB(255, 0, 255));

	//
	// LOAD SPRITES AND ANIMATIONS
	//
	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();
	LPANIMATION ani;


#pragma region Loading Simon resources

	// 99xxx: Simon's sprite ID
	// xx0xx: sprite face to right side
	// xx1xx: sprite face to left side
	//                left, top, right, bottom

	LPDIRECT3DTEXTURE9 texSimon = textures->Get(ID_TEX_SIMON);
	
	sprites->Add(99000, 436, 4, 468, 64, texSimon);		// idle right
	sprites->Add(99100, 12, 202, 44, 262, texSimon);	// idle left

	sprites->Add(99001, 378, 2, 402, 64, texSimon);		// walk right
	sprites->Add(99002, 314, 4, 344, 64, texSimon);		// walk right
	sprites->Add(99003, 260, 3, 284, 64, texSimon);		// walk right
	sprites->Add(99101, 78, 200, 102, 262, texSimon);	// walk left
	sprites->Add(99102, 136, 202, 166, 262, texSimon);	// walk left
	sprites->Add(99103, 196, 201, 220, 262, texSimon);	// walk left

	sprites->Add(99004, 196, 1, 229, 47, texSimon);		// crouch right
	sprites->Add(99104, 252, 199, 284, 245, texSimon);	// crouch left

	sprites->Add(99007, 120, 4, 168, 64, texSimon);		// attack right
	sprites->Add(99008, 75, 4, 107, 64, texSimon);		// attack right
	sprites->Add(99009, 16, 6, 59, 64, texSimon);		// attack right
	sprites->Add(99107, 312, 202, 360, 262, texSimon);	// attack left
	sprites->Add(99108, 373, 202, 405, 262, texSimon);	// attack left
	sprites->Add(99109, 421, 204, 464, 262, texSimon);	// attack left
	
	sprites->Add(99020, 438, 72, 470, 130, texSimon);	// go in

	sprites->Add(99021, 10, 270, 42, 328, texSimon);	// pushed back right
	sprites->Add(99121, 438, 72, 470, 130, texSimon);	// pushed back left

	sprites->Add(99022, 318, 68, 350, 130, texSimon);	// down stair right
	sprites->Add(99023, 266, 68, 290, 130, texSimon);	// down stair right
	sprites->Add(99122, 130, 266, 162, 328, texSimon);	// down stair left
	sprites->Add(99123, 190, 266, 214, 328, texSimon);	// down stair left

	sprites->Add(99024, 191, 68, 222, 130, texSimon);	// up stair right
	sprites->Add(99025, 134, 68, 158, 130, texSimon);	// up stair right
	sprites->Add(99124, 259, 266, 289, 328, texSimon);	// up stair left
	sprites->Add(99125, 322, 266, 346, 328, texSimon);	// up stair left
	
	sprites->Add(99037, 0, 67, 48, 113, texSimon);		// crouch attack right
	sprites->Add(99038, 437, 133, 469, 179, texSimon);	// crouch attack right
	sprites->Add(99039, 375, 135, 420, 179, texSimon);	// crouch attack right
	sprites->Add(99137, 432, 265, 483, 311, texSimon);	// crouch attack left
	sprites->Add(99138, 11, 311, 43, 377, texSimon);	// crouch attack left
	sprites->Add(99139, 61, 333, 105, 377, texSimon);	// crouch attack left

	sprites->Add(99047, 302, 134, 350, 196, texSimon);	// down stair attack right
	sprites->Add(99048, 258, 134, 290, 196, texSimon);	// down stair attack right
	sprites->Add(99049, 198, 136, 239, 196, texSimon);	// down stair attack right
	sprites->Add(99147, 130, 332, 178, 394, texSimon);	// down stair attack left
	sprites->Add(99148, 190, 332, 222, 394, texSimon);	// down stair attack left
	sprites->Add(99149, 241, 334, 282, 394, texSimon);	// down stair attack left

	sprites->Add(99057, 120, 134, 161, 196, texSimon);	// up stair attack right
	sprites->Add(99058, 69, 134, 101, 196, texSimon);	// up stair attack right
	sprites->Add(99059, 9, 136, 53, 196, texSimon);		// up stair attack right
	sprites->Add(99157, 319, 332, 360, 394, texSimon);	// up stair attack left
	sprites->Add(99158, 379, 332, 411, 394, texSimon);	// up stair attack left
	sprites->Add(99159, 427, 334, 471, 394, texSimon);	// up stair attack left

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
	ani->AddFrame(99004);
	animations->Add(SimonAniID::CROUCH_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99104);
	animations->Add(SimonAniID::CROUCH_LEFT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99020);
	animations->Add(SimonAniID::GO_IN, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99021);
	animations->Add(SimonAniID::PUSHED_BACK_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99121);
	animations->Add(SimonAniID::PUSHED_BACK_LEFT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99022);
	ani->AddFrame(99023);
	animations->Add(SimonAniID::DOWN_STAIR_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99122);
	ani->AddFrame(99123);
	animations->Add(SimonAniID::DOWN_STAIR_LEFT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99024);
	ani->AddFrame(99025);
	animations->Add(SimonAniID::UP_STAIR_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99124);
	ani->AddFrame(99125);
	animations->Add(SimonAniID::UP_STAIR_LEFT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99037);
	ani->AddFrame(99038);
	ani->AddFrame(99039);
	animations->Add(SimonAniID::CROUCH_ATTACK_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99137);
	ani->AddFrame(99138);
	ani->AddFrame(99139);
	animations->Add(SimonAniID::CROUCH_ATTACK_LEFT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99047);
	ani->AddFrame(99048);
	ani->AddFrame(99049);
	animations->Add(SimonAniID::DOWN_STAIR_ATTACK_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99147);
	ani->AddFrame(99148);
	ani->AddFrame(99149);
	animations->Add(SimonAniID::DOWN_STAIR_ATTACK_LEFT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99057);
	ani->AddFrame(99058);
	ani->AddFrame(99059);
	animations->Add(SimonAniID::UP_STAIR_ATTACK_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99157);
	ani->AddFrame(99158);
	ani->AddFrame(99159);
	animations->Add(SimonAniID::UP_STAIR_ATTACK_LEFT, ani);

	simon = new CSimon();
	simon->AddAnimation(SimonAniID::IDLE_RIGHT);
	simon->AddAnimation(SimonAniID::IDLE_LEFT);
	simon->AddAnimation(SimonAniID::WALK_RIGHT);
	simon->AddAnimation(SimonAniID::WALK_LEFT);
	simon->AddAnimation(SimonAniID::CROUCH_RIGHT);
	simon->AddAnimation(SimonAniID::CROUCH_LEFT);
	simon->AddAnimation(SimonAniID::ATTACK_RIGHT);
	simon->AddAnimation(SimonAniID::ATTACK_LEFT);
	simon->AddAnimation(SimonAniID::PUSHED_BACK_RIGHT);
	simon->AddAnimation(SimonAniID::PUSHED_BACK_LEFT);
	simon->AddAnimation(SimonAniID::DOWN_STAIR_RIGHT);
	simon->AddAnimation(SimonAniID::DOWN_STAIR_LEFT);
	simon->AddAnimation(SimonAniID::UP_STAIR_RIGHT);
	simon->AddAnimation(SimonAniID::UP_STAIR_LEFT);
	simon->AddAnimation(SimonAniID::CROUCH_ATTACK_RIGHT);
	simon->AddAnimation(SimonAniID::CROUCH_ATTACK_LEFT);
	simon->AddAnimation(SimonAniID::DOWN_STAIR_ATTACK_RIGHT);
	simon->AddAnimation(SimonAniID::DOWN_STAIR_ATTACK_LEFT);
	simon->AddAnimation(SimonAniID::UP_STAIR_ATTACK_RIGHT);
	simon->AddAnimation(SimonAniID::UP_STAIR_ATTACK_LEFT);
	simon->AddAnimation(SimonAniID::GO_IN);

	simon->SetPosition(0.0f, 0.0f);
	objects.push_back(simon);

#pragma endregion

#pragma region Loading ground resources

	LPDIRECT3DTEXTURE9 texGround = CTextures::GetInstance()->Get(ID_TEX_GROUND);
	sprites->Add(11000, 0, 0, 32, 32, texGround);
	
	CBrick* brick = new CBrick();
	ani = new CAnimation(100);
	ani->AddFrame(11000);
	animations->Add(BrickAniID::IDLE, ani);
	brick->AddAnimation(BrickAniID::IDLE);
	for (int i = 0; i < 30; i++)
	{
		brick = new CBrick();
		brick->SetPosition(0 + i * 32.0f, 300);
		objects.push_back(brick);
	}

	for (int i = 0; i < 3; i++)
	{
		brick = new CBrick();
		brick->SetPosition(100 + i * 32.0f, 50);
		objects.push_back(brick);
	}


	/*ani = new CAnimation(100);
	ani->AddFrame(11000);
	animations->Add(BrickAniID::IDLE, ani);

	for (int i = 0; i < 30; i++)
	{
		CBrick *brick = new CBrick();
		brick->AddAnimation(BrickAniID::IDLE);
		brick->SetPosition(0 + i * 32.0f, 200);
		objects.push_back(brick);
	}*/


#pragma endregion
		
}


/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	vector<LPGAMEOBJECT> coObjects;
	for (int i = 0; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (int i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<CMovableObject *>(objects[i]))
		{
			LPMOVABLEOBJECT obj = dynamic_cast<CMovableObject *>(objects[i]);
			obj->Update(dt, &coObjects);
		}
	}
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

		for (int i = 0; i < objects.size(); i++)
			objects[i]->Render();

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