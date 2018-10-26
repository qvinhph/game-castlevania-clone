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
#include "BigCandle.h"
#include "ItemRope.h"

#define WINDOW_CLASS_NAME L"Game"
#define MAIN_WINDOW_TITLE L"Castlevania"

#define BACKGROUND_COLOR D3DCOLOR_XRGB(255, 255, 200)
#define SCREEN_WIDTH		1000//256
#define SCREEN_HEIGHT		240

#define MAX_FRAME_RATE		100

#define ID_TEX_SIMON		0
#define ID_TEX_ROPE			1
#define ID_TEX_GROUND		2
#define ID_TEX_MISC			3
#define ID_TEX_MONSTERS		4

CGame *game;

CSimon * simon;
CRope * rope;
CBigCandle * bigCandle;
CBrick * brick;
CItemRope * itemRope;

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
	DebugOut(L"\n[INFO] KeyDown: %d", keyCode);
	switch (keyCode)
	{
	case DIK_RIGHT:
		simon->SetState(SIMON_STATE_WALK_RIGHT);
		break;

	case DIK_LEFT:
		simon->SetState(SIMON_STATE_WALK_LEFT);
		break;

	case DIK_N:
		simon->SetState(SIMON_STATE_JUMP);
		break;

	case DIK_M:
		simon->SetState(SIMON_STATE_ATTACK);
		break;

	default:
		break;
	}
}

void CInputHandler::OnKeyUp(int keyCode)
{
	DebugOut(L"\n[INFO] KeyUp: %d", keyCode);
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


void InitObjectsForTesting()
{

	simon = CSimon::GetInstance();
	objects.push_back(simon);
	simon->SetPosition(0.0f, 0.0f);

	rope = CRope::GetInstance();
	objects.push_back(rope);

	for (int i = 0; i < 30; i++)
	{
		brick = new CBrick();
		brick->SetPosition(0 + i * 32.0f, 200);
		objects.push_back(brick);
	}

	for (int i = 0; i < 3; i++)
	{
		brick = new CBrick();
		brick->SetPosition(100 + i * 32.0f, 50);
		objects.push_back(brick);
	}


	bigCandle = new CBigCandle();
	bigCandle->SetPosition(100.0f, 165.0f);
	//objects.push_back(bigCandle);

	itemRope = new CItemRope();
	itemRope->SetPosition(000.0f, 0.0f);
	//objects.push_back(itemRope);

}

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
	textures->Add(ID_TEX_ROPE, L"Textures\\rope.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_MISC, L"Textures\\misc.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_MONSTERS, L"Textures\\monsters.png", D3DCOLOR_XRGB(255, 0, 255));


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
	
	sprites->Add(99000, 218, 2, 234, 32, texSimon);		// idle right
	sprites->Add(99100, 6, 101, 22, 131, texSimon);		// idle left

	sprites->Add(99001, 189, 1, 201, 32, texSimon);		// walk right
	sprites->Add(99002, 157, 2, 172, 32, texSimon);		// walk right 
	sprites->Add(99003, 130, 1, 142, 32, texSimon);		// walk right
	sprites->Add(99101, 39, 100, 51, 131, texSimon);	// walk left
	sprites->Add(99102, 68, 101, 83, 131, texSimon);	// walk left
	sprites->Add(99103, 98, 100, 110, 131, texSimon);	// walk left

	sprites->Add(99004, 98, 0, 114, 23, texSimon);		// crouch right
	sprites->Add(99104, 126, 99, 142, 122, texSimon);	// crouch left

	sprites->Add(99007, 68, 2, 84, 32, texSimon);		// attack right
	sprites->Add(99008, 37, 2, 53, 32, texSimon);		// attack right
	sprites->Add(99009, 8, 2, 24, 32, texSimon);		// attack right
	sprites->Add(99107, 156, 101, 172, 131, texSimon);	// attack left
	sprites->Add(99108, 186, 101, 202, 131, texSimon);	// attack left
	sprites->Add(99109, 216, 101, 232, 131, texSimon);	// attack left
	
	sprites->Add(99020, 184, 33, 200, 65, texSimon);	// go in

	sprites->Add(99021, 5, 135, 21, 164, texSimon);		// pushed back right
	sprites->Add(99121, 219, 36, 235, 65, texSimon);	// pushed back left

	sprites->Add(99022, 159, 34, 175, 65, texSimon);	// down stair right
	sprites->Add(99023, 133, 34, 145, 65, texSimon);	// down stair right
	sprites->Add(99122, 65, 133, 81, 164, texSimon);	// down stair left
	sprites->Add(99123, 85, 133, 107, 164, texSimon);	// down stair left

	sprites->Add(99024, 95, 34, 110, 65, texSimon);		// up stair right
	sprites->Add(99025, 67, 34, 79, 65, texSimon);		// up stair right
	sprites->Add(99124, 129, 133, 144, 164, texSimon);	// up stair left
	sprites->Add(99125, 161, 133, 173, 164, texSimon);	// up stair left
	
	sprites->Add(99037, 8, 33, 24, 56, texSimon);		// crouch attack right
	sprites->Add(99038, 218, 66, 234, 89, texSimon);	// crouch attack right
	sprites->Add(99039, 187, 66, 203, 89, texSimon);	// crouch attack right
	sprites->Add(99137, 216, 132, 232, 155, texSimon);	// crouch attack left
	sprites->Add(99138, 5, 165, 21, 188, texSimon);		// crouch attack left
	sprites->Add(99139, 36, 165, 52, 188, texSimon);	// crouch attack left

	sprites->Add(99047, 159, 67, 175, 98, texSimon);	// down stair attack right
	sprites->Add(99048, 129, 67, 145, 98, texSimon);	// down stair attack right
	sprites->Add(99049, 99, 67, 115, 98, texSimon);		// down stair attack right
	sprites->Add(99147, 65, 166, 81, 197, texSimon);	// down stair attack left
	sprites->Add(99148, 95, 166, 111, 197, texSimon);	// down stair attack left
	sprites->Add(99149, 125, 166, 141, 197, texSimon);	// down stair attack left

	sprites->Add(99057, 34, 67, 50, 98, texSimon);		// up stair attack right
	sprites->Add(99058, 64, 64, 80, 98, texSimon);		// up stair attack right
	sprites->Add(99059, 4, 68, 20, 98, texSimon);		// up stair attack right
	sprites->Add(99157, 159, 166, 175, 197, texSimon);	// up stair attack left
	sprites->Add(99158, 189, 166, 205, 197, texSimon);	// up stair attack left
	sprites->Add(99159, 219, 166, 235, 197, texSimon);	// up stair attack left

	sprites->Add(99999, 28, 149, 60, 164, texSimon);	// die

	ani = new CAnimation(3000);
	ani->AddFrame(99000);
	animations->Add((int)SimonAniID::IDLE_RIGHT, ani);

	ani = new CAnimation(3000);
	ani->AddFrame(99100);
	animations->Add((int)SimonAniID::IDLE_LEFT, ani);

	ani = new CAnimation(1000);
	ani->AddFrame(99000);
	ani->AddFrame(99001);
	ani->AddFrame(99002);
	ani->AddFrame(99003);
	animations->Add((int)SimonAniID::WALK_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99100);
	ani->AddFrame(99101);
	ani->AddFrame(99102);
	ani->AddFrame(99103);
	animations->Add((int)SimonAniID::WALK_LEFT, ani);

	ani = new CAnimation(1000);
	ani->AddFrame(99007);
	ani->AddFrame(99008);
	ani->AddFrame(99009, 1000);
	animations->Add((int)SimonAniID::ATTACK_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99107);
	ani->AddFrame(99108);
	ani->AddFrame(99109, 200);
	animations->Add((int)SimonAniID::ATTACK_LEFT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99004);
	animations->Add((int)SimonAniID::CROUCH_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99104);
	animations->Add((int)SimonAniID::CROUCH_LEFT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99020);
	animations->Add((int)SimonAniID::GO_IN, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99021);
	animations->Add((int)SimonAniID::PUSHED_BACK_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99121);
	animations->Add((int)SimonAniID::PUSHED_BACK_LEFT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99022);
	ani->AddFrame(99023);
	animations->Add((int)SimonAniID::DOWN_STAIR_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99122);
	ani->AddFrame(99123);
	animations->Add((int)SimonAniID::DOWN_STAIR_LEFT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99024);
	ani->AddFrame(99025);
	animations->Add((int)SimonAniID::UP_STAIR_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99124);
	ani->AddFrame(99125);
	animations->Add((int)SimonAniID::UP_STAIR_LEFT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99037);
	ani->AddFrame(99038);
	ani->AddFrame(99039, 200);
	animations->Add((int)SimonAniID::CROUCH_ATTACK_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99137);
	ani->AddFrame(99138);
	ani->AddFrame(99139, 200);
	animations->Add((int)SimonAniID::CROUCH_ATTACK_LEFT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99047);
	ani->AddFrame(99048);
	ani->AddFrame(99049, 200);
	animations->Add((int)SimonAniID::DOWN_STAIR_ATTACK_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99147);
	ani->AddFrame(99148);
	ani->AddFrame(99149, 200);
	animations->Add((int)SimonAniID::DOWN_STAIR_ATTACK_LEFT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99057);
	ani->AddFrame(99058);
	ani->AddFrame(99059, 200);
	animations->Add((int)SimonAniID::UP_STAIR_ATTACK_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99157);
	ani->AddFrame(99158);
	ani->AddFrame(99159, 200);
	animations->Add((int)SimonAniID::UP_STAIR_ATTACK_LEFT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99999);
	animations->Add((int)SimonAniID::DIE, ani);


#pragma endregion


#pragma region Loading rope resources

	LPDIRECT3DTEXTURE9 texRope = textures->Get(ID_TEX_ROPE);

	sprites->Add(98011, 185, 1, 201, 30, texRope);		// rope lv1 right side
	sprites->Add(98012, 154, 6, 170, 25, texRope);		// rope lv1 right side
	sprites->Add(98013, 106	, 8, 134, 17, texRope);		// rope lv1 right side
	sprites->Add(98111, 1, 2, 16, 31, texRope);			// rope lv1 left side
	sprites->Add(98112, 31, 7, 47, 26, texRope);		// rope lv1 left side
	sprites->Add(98113, 67, 9, 95, 18, texRope);		// rope lv1 left side

	sprites->Add(98021, 185, 35, 201, 64, texRope);		// rope lv2 right side
	sprites->Add(98022, 154, 40, 170, 59, texRope);		// rope lv2 right side
	sprites->Add(98023, 106, 45, 134, 51, texRope);		// rope lv2 right side
	sprites->Add(98121, 0, 36, 16, 65, texRope);		// rope lv2 left side
	sprites->Add(98122, 31, 41, 47, 60, texRope);		// rope lv2 left side
	sprites->Add(98123, 67, 46, 95, 52, texRope);		// rope lv2 left side

	sprites->Add(98031, 185, 69, 201, 98, texRope);		// rope lv3 right side
	sprites->Add(98032, 154, 74, 170, 93, texRope);		// rope lv3 right side
	sprites->Add(98033, 106, 79, 150, 85, texRope);		// rope lv3 right side
	sprites->Add(98131, 1, 70, 16, 99, texRope);		// rope lv3 left side
	sprites->Add(98132, 31, 75, 47, 94, texRope);		// rope lv3 left side
	sprites->Add(98133, 51, 80, 96, 86, texRope);		// rope lv3 left side

	ani = new CAnimation(1000);
	ani->AddFrame(98011);
	ani->AddFrame(98012);
	ani->AddFrame(98013); 
	animations->Add((int)RopeAniID::LEVEL_ONE_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(98111);
	ani->AddFrame(98112);
	ani->AddFrame(98113, 200);
	animations->Add((int)RopeAniID::LEVEL_ONE_LEFT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(98021);
	ani->AddFrame(98022);
	ani->AddFrame(98023, 200);
	animations->Add((int)RopeAniID::LEVEL_TWO_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(98121);
	ani->AddFrame(98122);
	ani->AddFrame(98123, 200);
	animations->Add((int)RopeAniID::LEVEL_TWO_LEFT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(98031);
	ani->AddFrame(98032);
	ani->AddFrame(98033, 200);
	animations->Add((int)RopeAniID::LEVEL_THREE_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(98131);
	ani->AddFrame(98132);
	ani->AddFrame(98133, 200);
	animations->Add((int)RopeAniID::LEVEL_THREE_LEFT, ani);
		 
#pragma endregion


#pragma region Loading other objects

	LPDIRECT3DTEXTURE9 texMisc = textures->Get(ID_TEX_MISC);
	LPDIRECT3DTEXTURE9 texGround = CTextures::GetInstance()->Get(ID_TEX_GROUND);

	// brick
	sprites->Add(11000, 0, 0, 32, 32, texGround);

	ani = new CAnimation(999999);
	ani->AddFrame(11000);
	animations->Add(BrickAniID::IDLE, ani);

	// big candle
	sprites->Add(12000, 0, 0, 16, 32, texMisc);
	sprites->Add(12001, 16, 0, 32, 32, texMisc);

	ani = new CAnimation(100);
	ani->AddFrame(12000);
	ani->AddFrame(12001);
	animations->Add((int)BigCandleAniID::IDLE, ani);

	// small candle - spriteID = 13xxx

	// Item Rope
	sprites->Add(14000, 73, 32, 89, 48, texMisc);

	ani = new CAnimation(100);
	ani->AddFrame(14000);
	animations->Add((int)ItemRopeAniID::IDLE, ani);

#pragma endregion


	InitObjectsForTesting();
}




/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	// TODO: Need an optimized way

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