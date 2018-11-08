// INIT GIT

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "Castlevania.h"
#include "TileMap.h"

#define WINDOW_CLASS_NAME L"Game"
#define MAIN_WINDOW_TITLE L"Castlevania I"

#define BACKGROUND_COLOR	D3DCOLOR_XRGB(0, 0, 0)

#define SCREEN_WIDTH		528.5f//512		// DELETE ME: NOTE: ACCEPTABLE
#define SCREEN_HEIGHT		490.8f//448

#define MAX_FRAME_RATE		120

CTileMap *tileMap;
CGame *game;
CGameObject *gameObject;

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
	case DIK_N:
		CSimon::GetInstance()->SetAction(Action::JUMP);
		break;

	case DIK_M:
		CSimon::GetInstance()->SetAction(Action::ATTACK);
		break;

	case DIK_RIGHT:
		CSimon::GetInstance()->SetAction(Action::WALK_RIGHT);
		break;

	case DIK_LEFT:
		CSimon::GetInstance()->SetAction(Action::WALK_LEFT);
		break;

	case DIK_1:
		CSimon::GetInstance()->SetPosition(0.0f, 0.0f);
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
		CSimon::GetInstance()->SetAction(Action::CROUCH);
	}		
}

void CInputHandler::KeyState(BYTE *states)
{
	if (game->IsKeyDown(DIK_DOWN))
		CSimon::GetInstance()->SetAction(Action::CROUCH);

	else if (game->IsKeyDown(DIK_RIGHT))
		CSimon::GetInstance()->SetAction(Action::WALK_RIGHT);

	else if (game->IsKeyDown(DIK_LEFT))
		CSimon::GetInstance()->SetAction(Action::WALK_LEFT);

	else
		CSimon::GetInstance()->SetAction(Action::IDLE);
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

	gameObject = CSimon::GetInstance();
	objects.push_back(gameObject);
	gameObject->SetPosition(130.0f, -50.0f);

	gameObject = CRope::GetInstance();
	objects.push_back(gameObject);

	for (int i = 0; i < 30; i++)
	{
		gameObject = new CBrick();
		gameObject->SetPosition(0 + i * 32.0f, 300);
		objects.push_back(gameObject);
	}

	for (int i = 0; i < 3; i++)
	{
		gameObject = new CBrick();
		gameObject->SetPosition(100 + i * 32.0f, 50);
		objects.push_back(gameObject);
	}

	for (int i = 0; i < 3; i++)
	{
		gameObject = new CFlame();
		CFlames::GetInstance()->Add((CFlame *)gameObject);
		objects.push_back(gameObject);
	}

	gameObject = new CBrick();
	gameObject->SetPosition(132.0f, 240);
	objects.push_back(gameObject);

	gameObject = new CBigCandle();
	gameObject->SetPosition(200.0f, 165.0f);
	objects.push_back(gameObject);

	gameObject = new CItemRope();
	gameObject->SetPosition(000.0f, 0.0f);
	objects.push_back(gameObject);

	gameObject = new CItemRope();
	gameObject->SetPosition(400.0f, 60.0f);
	objects.push_back(gameObject);

	tileMap = new CTileMap(L"json\\scene_outside_jsonmap.json");
	tileMap->Init(ID_TEX_TILESET);
	tileMap->Draw();

}

void TestInit()
{
	tileMap = new CTileMap(L"json\\scene_outside_jsonmap.json");
	tileMap->Init(ID_TEX_TILESET);
	tileMap->Draw();
	objects = tileMap->GetGameObjects();
}

/*
	Load all game resources
	In this example: load textures, sprites, animations and mario object

	TO-DO: Improve this function by loading texture,sprite,animation,object from file
*/
void LoadResources()
{
	//
	//	GET VIEWPORT SIZE
	//
	CGame::GetInstance()->SetViewportWidth(SCREEN_WIDTH);
	CGame::GetInstance()->SetViewportHeight(SCREEN_HEIGHT);

	//
	// LOAD TEXTURES
	//
	CTextures * textures = CTextures::GetInstance();
	textures->Add(ID_TEX_SIMON, L"textures\\simon.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_GROUND, L"textures\\ground1.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_ROPE, L"textures\\rope.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_MISC, L"textures\\misc.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_MONSTERS, L"textures\\monsters.png", D3DCOLOR_XRGB(255, 0, 255));
	textures->Add(ID_TEX_TILESET, L"textures\\tileset.png", NULL);


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
	
	sprites->Add(99000, 436, 2, 468, 64, texSimon);		// idle right
	sprites->Add(99100, 12, 200, 44, 262, texSimon);	// idle left

	sprites->Add(99001, 374, 2, 406, 64, texSimon);		// walk right
	sprites->Add(99002, 313, 2, 345, 64, texSimon);		// walk right
	sprites->Add(99003, 256, 2, 288, 64, texSimon);		// walk right
	sprites->Add(99101, 74, 200, 106, 262, texSimon);	// walk left
	sprites->Add(99102, 135, 200, 167, 262, texSimon);	// walk left
	sprites->Add(99103, 192, 200, 224, 262, texSimon);	// walk left

	sprites->Add(99004, 196, 1, 228, 47, texSimon);		// crouch right
	sprites->Add(99104, 252, 199, 284, 245, texSimon);	// crouch left

	sprites->Add(99007, 136, 2, 168, 64, texSimon);		// attack right
	sprites->Add(99008, 75, 2, 107, 64, texSimon);		// attack right
	sprites->Add(99009, 16, 2, 48, 64, texSimon);		// attack right
	sprites->Add(99107, 312, 200, 344, 262, texSimon);	// attack left
	sprites->Add(99108, 373, 200, 405, 262, texSimon);	// attack left
	sprites->Add(99109, 432, 200, 464, 262, texSimon);	// attack left
	
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
	
	sprites->Add(99037, 16, 67, 48, 113, texSimon);		// crouch attack right
	sprites->Add(99038, 437, 133, 469, 179, texSimon);	// crouch attack right
	sprites->Add(99039, 375, 133, 407, 179, texSimon);	// crouch attack right
	sprites->Add(99137, 432, 265, 464, 311, texSimon);	// crouch attack left
	sprites->Add(99138, 11, 331, 43, 377, texSimon);	// crouch attack left
	sprites->Add(99139, 73, 331, 105, 377, texSimon);	// crouch attack left

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

	sprites->Add(99999, 60, 269, 64, 60, texSimon);	// die

	ani = new CAnimation(100);
	ani->AddFrame(99000);
	animations->Add((int)SimonAniID::IDLE_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99100);
	animations->Add((int)SimonAniID::IDLE_LEFT, ani);

	ani = new CAnimation(100);
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

	ani = new CAnimation(75);
	ani->AddFrame(99007);
	ani->AddFrame(99008);
	ani->AddFrame(99009, 200);
	animations->Add((int)SimonAniID::ATTACK_RIGHT, ani);

	ani = new CAnimation(75);
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

	sprites->Add(98011, 420, 0, 452, 64, texRope);		// rope lv1 right side
	sprites->Add(98012, 358, 3, 390, 52, texRope);		// rope lv1 right side
	sprites->Add(98013, 262, 18, 318, 36, texRope);		// rope lv1 right side
	sprites->Add(98111, 2, 0, 34, 64, texRope);			// rope lv1 left side
	sprites->Add(98112, 63, 5, 95, 54, texRope);		// rope lv1 left side
	sprites->Add(98113, 135, 20, 191, 38, texRope);		// rope lv1 left side

	sprites->Add(98021, 420, 64, 452, 128, texRope);	// rope lv2 right side
	sprites->Add(98022, 358, 69, 390, 118, texRope);	// rope lv2 right side
	sprites->Add(98023, 262, 84, 318, 102, texRope);	// rope lv2 right side
	sprites->Add(98121, 2, 66, 34, 130, texRope);		// rope lv2 left side
	sprites->Add(98122, 63, 69, 95, 120, texRope);		// rope lv2 left side
	sprites->Add(98123, 135, 86, 191, 104, texRope);	// rope lv2 left side

	sprites->Add(98031, 420, 132, 452, 196, texRope);	// rope lv3 right side
	sprites->Add(98032, 358, 135, 390, 186, texRope);	// rope lv3 right side
	sprites->Add(98033, 261, 152, 350, 170, texRope);	// rope lv3 right side
	sprites->Add(98131, 2, 134, 34, 198, texRope);		// rope lv3 left side
	sprites->Add(98132, 63, 137, 95, 188, texRope);		// rope lv3 left side
	sprites->Add(98133, 103, 154, 192, 172, texRope);	// rope lv3 left side

	ani = new CAnimation(75);
	ani->AddFrame(98011);
	ani->AddFrame(98012);
	ani->AddFrame(98013, 200);
	animations->Add((int)RopeAniID::LEVEL_ONE_RIGHT, ani);

	ani = new CAnimation(75);
	ani->AddFrame(98111);
	ani->AddFrame(98112);
	ani->AddFrame(98113, 200);
	animations->Add((int)RopeAniID::LEVEL_ONE_LEFT, ani);

	ani = new CAnimation(75);
	ani->AddFrame(98021);
	ani->AddFrame(98022);
	ani->AddFrame(98023, 200);
	animations->Add((int)RopeAniID::LEVEL_TWO_RIGHT, ani);

	ani = new CAnimation(75);
	ani->AddFrame(98121);
	ani->AddFrame(98122);
	ani->AddFrame(98123, 200);
	animations->Add((int)RopeAniID::LEVEL_TWO_LEFT, ani);

	ani = new CAnimation(75);
	ani->AddFrame(98031);
	ani->AddFrame(98032);
	ani->AddFrame(98033, 200);
	animations->Add((int)RopeAniID::LEVEL_THREE_RIGHT, ani);

	ani = new CAnimation(75);
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
	ani = new CAnimation(100);
	ani->AddFrame(11000);
	animations->Add(BrickAniID::IDLE, ani);

	// big candle
	sprites->Add(12000, 0, 0, 32, 64, texMisc);
	sprites->Add(12001, 32, 0, 64, 64, texMisc);
	ani = new CAnimation(100);
	ani->AddFrame(12000);
	ani->AddFrame(12001);
	animations->Add((int)BigCandleAniID::IDLE, ani);

	// small candle - spriteID = 13xxx

	// item Rope
	sprites->Add(14000, 146, 65, 178, 97, texMisc);
	ani = new CAnimation(100);
	ani->AddFrame(14000);
	animations->Add((int)ItemRopeAniID::IDLE, ani);

	// destroying flame
	sprites->Add(15000, 156, 7, 172, 37, texMisc);
	sprites->Add(15001, 198, 7, 214, 37, texMisc);
	sprites->Add(15002, 113, 7, 129, 37, texMisc);
	ani = new CAnimation(100);
	ani->AddFrame(15000);
	ani->AddFrame(15001);
	ani->AddFrame(15002);
	animations->Add((int)FlameAniID::IDLE, ani);

	// big heart
	sprites->Add(16000, 120, 65, 144, 85, texMisc);
	ani = new CAnimation(100);
	ani->AddFrame(16000);
	animations->Add((int)BigHeartAniID::IDLE, ani);


#pragma endregion


	//InitObjectsForTesting();
	TestInit();

}


/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	// TO-DO: Need an optimized way
	// only the objects in the camera zone
	vector<LPGAMEOBJECT> coObjects;
	for (UINT i = 0; i < objects.size(); i++)
	{
		if ( objects.at(i)->GetState() == STATE_VISIBLE)
			coObjects.push_back(objects[i]);
	}

	for (UINT i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<CMovableObject *>(objects[i])
			&& objects[i]->GetState() == STATE_VISIBLE)
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

		tileMap->Draw();

		for (UINT i = 0; i < objects.size(); i++)
		{
			objects[i]->Render();
			objects[i]->RenderBoundingBox();
		}

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

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	Run();

	return 0;
}