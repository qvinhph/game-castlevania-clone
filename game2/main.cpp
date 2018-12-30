#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <fstream>

#include "Castlevania.h"
#include "TileMap.h"

#include "Cells.h"
#include "Timer.h"
#include "Monsters.h"
#include "Map.h"
#include "Maps.h"


#define WINDOW_CLASS_NAME L"Game"
#define MAIN_WINDOW_TITLE L"Castlevania"

#define BACKGROUND_COLOR	D3DCOLOR_XRGB(0, 0, 0)

#define SCREEN_WIDTH		528.5f//512		// DELETE ME: NOTE: ACCEPTABLE
#define SCREEN_HEIGHT		487.5f//448


#define VIEWPORT_WIDTH		512
#define VIEWPORT_HEIGHT		448

#define MAX_FRAME_RATE		120

CMaps * maps;
CGame *game;
LPGAMEOBJECT gameObject;
CBoard *board;

vector<LPGAMEOBJECT> updateObjects;			// The objects need be updated
vector<LPGAMEOBJECT> defaultObjects;		// Base objects

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
	case DIK_S:
		CSimon::GetInstance()->SetAction(Action::JUMP);
		break;

	case DIK_A:
		if (game->IsKeyDown(DIK_UP))
			CSimon::GetInstance()->SetAction(Action::USE_ITEM);
		else
			CSimon::GetInstance()->SetAction(Action::ATTACK);
		break;

	// For changing direction of simon while he is crouching
	case DIK_RIGHT:
		CSimon::GetInstance()->SetAction(Action::WALK_RIGHT);
		break;

	// For changing direction of simon while he is crouching
	case DIK_LEFT:
		CSimon::GetInstance()->SetAction(Action::WALK_LEFT);
		break;

	// DEBUGGING : Delete me
	case DIK_1:
		float x, y;
		CSimon::GetInstance()->GetPosition(x, y);
		CSimon::GetInstance()->SetPosition(x, 0.0f);
		CCamera::GetInstance()->ChangeLimitBound(x, 0.0f);
		break;
	case DIK_2:
		CSimon::GetInstance()->SetPosition(2823, 284);
		CCamera::GetInstance()->ChangeLimitBound(2823, 284);
		break;
	case DIK_3:
		CSimon::GetInstance()->SetSecondaryWeapon(Weapon::AXE);
		break;
	case DIK_4:
		CSimon::GetInstance()->SetSecondaryWeapon(Weapon::DAGGER);
		break;
	case DIK_5:
		CSimon::GetInstance()->SetSecondaryWeapon(Weapon::HOLYWATER);
		break;
	case DIK_6:
		CSimon::GetInstance()->SetSecondaryWeapon(Weapon::WATCH);
		break;
	case DIK_P:
		CSimon::GetInstance()->Revive();
		break;
	case DIK_O:
		CSimon::GetInstance()->RemoveOnStairs();
		break;

	default:
		break;
	}
}

void CInputHandler::OnKeyUp(int keyCode)
{
	//DebugOut(L"\n[INFO] KeyUp: %d", keyCode);
	switch (keyCode)
	{
	case DIK_DOWN:
		CSimon::GetInstance()->SetAction(Action::IDLE);
	}		
}

void CInputHandler::KeyState(BYTE *states)
{
	if (game->IsKeyDown(DIK_DOWN))
	{
		CSimon::GetInstance()->SetAction(Action::DOWNSTAIRS);
		CSimon::GetInstance()->SetAction(Action::CROUCH);
	}

	else if (game->IsKeyDown(DIK_RIGHT))
		CSimon::GetInstance()->SetAction(Action::WALK_RIGHT);

	else if (game->IsKeyDown(DIK_LEFT))
		CSimon::GetInstance()->SetAction(Action::WALK_LEFT);

	else if (game->IsKeyDown(DIK_UP))
		CSimon::GetInstance()->SetAction(Action::UPSTAIRS);

	else if (game->IsKeyDown(DIK_DOWN))
		CSimon::GetInstance()->SetAction(Action::DOWNSTAIRS);

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


/*
	Init tileMap and classify the map's objects to cell for space partitioning.
*/
void InitMap()
{
	maps = CMaps::GetInstance();
	CMap * map;

	// Map level one
	map = new CMap(L"json\\map_level_one.json");
	maps->AddMap(map);

	// Map level two
	// ....
	// ....

	// Set the map for the first showing time.
	// NOTE: By default the currentMap will be at first index of the collection
	// maps->SetCurrentMap(0)
}


void LoadListDefaultObjects(LPCWSTR filePath, vector<LPGAMEOBJECT> &defaultObjects)
{
	ifstream inputFile(filePath);
	string data = "";
	inputFile >> data;

	// Instance of the collection of item, flame, weapon,...
	CItems * items = CItems::GetInstance();
	CFlames * flames = CFlames::GetInstance();
	CWeapons * weapons = CWeapons::GetInstance();
	CMonsters * monsters = CMonsters::GetInstance();
	CBubbles * bubbles = CBubbles::GetInstance();
	CRocks * rocks = CRocks::GetInstance();

	// Find the begin of the data
	while (inputFile >> data)
	{
		if (data == "BEGIN")
		{
			string objName;
			int quantity;

			// Read the infomation of texture
			while (inputFile >> data)
			{
				if (data == "END")
					break;

				// Get name of the object
				objName = data;

				// Get the quantity by default
				inputFile >> data;
				quantity = atoi(data.c_str());

				LPGAMEOBJECT obj = NULL;

				for (UINT i = 0; i < quantity; i++)
				{
					// Item things
					if (objName == "itemrope")
					{
						obj = new CItemRope();
						items->Add(Item::ITEM_ROPE, obj);
					}
					else if (objName == "itemdagger")
					{
						obj = new CItemDagger();
						items->Add(Item::ITEM_DAGGER, obj);

					}
					else if (objName == "bigheart")
					{
						obj = new CBigHeart();
						items->Add(Item::BIG_HEART, obj);
					}
					else if (objName == "itemmeat")
					{
						obj = new CItemMeat();
						items->Add(Item::ITEM_MEAT, obj);
					}
					else if (objName == "cross")
					{
						obj = new CCross();
						items->Add(Item::CROSS, obj);
					}
					else if (objName == "itemaxe")
					{
						obj = new CItemAxe();
						items->Add(Item::ITEM_AXE, obj);
					}
					else if (objName == "itemholywater")
					{
						obj = new CItemHolyWater();
						items->Add(Item::ITEM_HOLY_WATER, obj);
					}
					else if (objName == "itemsuperpotion")
					{
						obj = new CItemSuperPotion();
						items->Add(Item::ITEM_SUPER_POTION, obj);
					}
					else if (objName == "watch")
					{
						obj = new CWatch();
						items->Add(Item::WATCH, obj);
					}
					else if (objName == "doubleshot")
					{
						obj = new CDoubleShot();
						items->Add(Item::DOUBLE_SHOT, obj);
					}
					else if (objName == "magicball")
					{
						obj = new CMagicBall();
						items->Add(Item::MAGIC_BALL, obj);
					}
					else if (objName == "heart")
					{
						obj = new CHeart();
						items->Add(Item::HEART, obj);
					}
					else if (objName == "moneybagblue")
					{
						obj = new CMoneyBag((int)MoneyBagAniID::BLUE_BAG);
						items->Add(Item::MONEY_BAG_BLUE, obj);
					}
					else if (objName == "moneybagred")
					{
						obj = new CMoneyBag((int)MoneyBagAniID::RED_BAG);
						items->Add(Item::MONEY_BAG_RED, obj);
					}
					else if (objName == "moneybagwhite")
					{
						obj = new CMoneyBag((int)MoneyBagAniID::WHITE_BAG);
						items->Add(Item::MONEY_BAG_WHITE, obj);
					}


					// Weapon things
					else if (objName == "dagger")
					{
						obj = new CDagger();
						weapons->Add(Weapon::DAGGER, obj);
					}
					else if (objName == "axe")
					{
						obj = new CAxe();
						weapons->Add(Weapon::AXE, obj);
					}
					else if (objName == "holywater")
					{
						obj = new CHolyWater();
						weapons->Add(Weapon::HOLYWATER, obj);
					}
					else if (objName == "fireball")
					{
						obj = new CFireBall();
						weapons->Add(Weapon::FIREBALL, obj);
					}


					// Monsters
					else if (objName == "zombie")
					{
						obj = new CZombie();
						monsters->Add(Monster::ZOMBIE, obj);
					}
					else if (objName == "panther")
					{
						obj = new CPanther();
						monsters->Add(Monster::PANTHER, obj);
					}
					else if (objName == "pinkbat")
					{
						obj = new CPinkBat();
						monsters->Add(Monster::PINKBAT, obj);
					}
					else if (objName == "fish")
					{
						obj = new CFish();
						monsters->Add(Monster::FISH, obj);
					}


					// Effect objects
					else if (objName == "flame")
					{
						obj = new CFlame();
						flames->Add((CFlame *)obj);
					}
					else if (objName == "bubble")
					{
						obj = new CBubble();
						bubbles->Add((CBubble *)obj);
					}
					else if (objName == "rock")
					{
						obj = new CRock();
						rocks->Add((CRock *)obj);
					}


					// Add to default object vector
					if (obj != NULL)
						defaultObjects.push_back(obj);
					else
						DebugOut(L"\n[ERROR] Something cannot be loaded to default object vector.");
				}
			}
		}
	}
}


void InitDefaultObjects()
{
	// Init game board
	board = CBoard::GetInstance();

	// Init Simon - player
	gameObject = CSimon::GetInstance();
	defaultObjects.push_back(gameObject);

	// Init Rope of Simon
	gameObject = CRope::GetInstance();
	defaultObjects.push_back(gameObject);

	// Init list of default objects
	LoadListDefaultObjects(L"textures\\DefaultObjects.txt", defaultObjects);
}


/*
	Load all game resources
	In this example: load textures, sprites, animations and mario object
*/
void LoadResources()
{
	//
	//	GET VIEWPORT SIZE
	//
	CCamera * camera = CCamera::GetInstance();
	camera->SetViewportSize(VIEWPORT_WIDTH, VIEWPORT_HEIGHT);


	//
	// LOAD TEXTURES
	//
	CTextures * textures = CTextures::GetInstance();
	textures->AddTexturesFromFile(L"textures\\TexturesInfo.txt");


	//
	// LOAD SPRITES AND ANIMATIONS
	//
	CSprites * sprites = CSprites::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();

	// Simon's resources
	LPDIRECT3DTEXTURE9 texSimon = textures->Get(ID_TEX_SIMON);
	sprites->AddSpritesFromFile(L"textures\\SimonSpritesInfo.txt", texSimon);
	animations->AddAnimationsFromFile(L"textures\\SimonAnimationInfo.txt");

	// Rope's resources
	LPDIRECT3DTEXTURE9 texRope = textures->Get(ID_TEX_ROPE);
	sprites->AddSpritesFromFile(L"textures\\RopeSpritesInfo.txt", texRope);
	animations->AddAnimationsFromFile(L"textures\\RopeAnimationInfo.txt");

	// Monsters' resources
	LPDIRECT3DTEXTURE9 texMonsters = textures->Get(ID_TEX_MONSTERS);
	sprites->AddSpritesFromFile(L"textures\\MonsterSpritesInfo.txt", texMonsters);
	animations->AddAnimationsFromFile(L"textures\\MonsterAnimationInfo.txt");

	// Other objects' resources
	LPDIRECT3DTEXTURE9 texMisc = textures->Get(ID_TEX_MISC);
	sprites->AddSpritesFromFile(L"textures\\MiscSpritesInfo.txt", texMisc);
	animations->AddAnimationsFromFile(L"textures\\MiscAnimationInfo.txt");


	//
	// Load Default Objects and Map
	//
	InitDefaultObjects();
	InitMap();
}


/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	vector<LPGAMEOBJECT> coObjects;
	coObjects.clear();
	LPMAP currentMap = maps->GetCurrentMap();


	// Get the bounding box of viewport
	float left, top, right, bottom;
	CCamera::GetInstance()->GetBoundingBox(left, top, right, bottom);


	// Get objects in the cells
	updateObjects.clear();
	currentMap->GetCells()->GetObjectsInRectangle(left, top, right, bottom, updateObjects);


	// Add the default objects
	for (UINT i = 0; i < defaultObjects.size(); ++i)
	{
		if (defaultObjects[i]->GetState() == STATE_VISIBLE)
		{
			updateObjects.push_back(defaultObjects[i]);
		}
	}


	// Get collide-able objects
	for (UINT i = 0; i < updateObjects.size(); i++)
	{
		if (updateObjects[i]->GetState() == STATE_VISIBLE
			&& updateObjects[i]->IsInViewport() == true)
			coObjects.push_back(updateObjects[i]);
	}


	// Call Update function of each object
	for (UINT i = 0; i < updateObjects.size(); i++)
	{
		if (updateObjects[i]->GetState() == STATE_VISIBLE
			&& updateObjects[i]->GetFreezing() == false)
		{
			updateObjects[i]->Update(dt, &coObjects);
		}
	}	


	// Help freezing time in game
	CTimer::GetInstance()->Update(dt, &updateObjects);
	CBoard::GetInstance()->Update();
}


/*
	Render a frame
*/
void Render()
{
	LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
	LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
	LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();
	LPMAP currentMap = maps->GetCurrentMap();

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		// Only draw the tile in the camera 
		float left, top, right, bottom;
		CCamera::GetInstance()->GetBoundingBox(left, top, right, bottom);
		currentMap->GetTileMap()->Draw(left, top, right, bottom);

		for (UINT i = 0; i < updateObjects.size(); i++)
		{
			updateObjects[i]->Render();
			//updateObjects[i]->RenderBoundingBox();
		}

		board->Render();

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
	DebugOut(L"\nStart             asdasdasdsadsad");

	HWND hWnd = CreateGameWindow(hInstance, nCmdShow, SCREEN_WIDTH, SCREEN_HEIGHT);

	game = CGame::GetInstance();
	game->Init(hWnd);

	inputHandler = new CInputHandler();
	game->InitKeyboard(inputHandler);
	DebugOut(L"\nStart");
	DebugOut(L"\nStart ---------- 2");


	LoadResources();

	SetWindowPos(hWnd, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
	DebugOut(L"\nEnd------------------444444");

	Run();

	return 0;
}
