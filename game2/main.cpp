#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "Castlevania.h"
#include "TileMap.h"

#include "Cells.h"
#include "Timer.h"
#include "Monsters.h"


#define WINDOW_CLASS_NAME L"Game"
#define MAIN_WINDOW_TITLE L"Castlevania"

#define BACKGROUND_COLOR	D3DCOLOR_XRGB(0, 0, 0)

#define SCREEN_WIDTH		528.5f//512		// DELETE ME: NOTE: ACCEPTABLE
#define SCREEN_HEIGHT		487.5f//448

#define CELL_WIDTH			256
#define CELL_HEIGHT			224

#define VIEWPORT_WIDTH		512
#define VIEWPORT_HEIGHT		448

#define MAX_FRAME_RATE		120

CTileMap *tileMap;
CGame *game;
LPGAMEOBJECT gameObject;
CCells *cells;
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
	case DIK_N:
		CSimon::GetInstance()->SetAction(Action::JUMP);
		break;

	case DIK_M:
		if (game->IsKeyDown(DIK_UP))
			CSimon::GetInstance()->SetAction(Action::USE_ITEM);
		else
			CSimon::GetInstance()->SetAction(Action::ATTACK);
		break;

	case DIK_RIGHT:
		CSimon::GetInstance()->SetAction(Action::WALK_RIGHT);
		break;

	case DIK_LEFT:
		CSimon::GetInstance()->SetAction(Action::WALK_LEFT);
		break;

		// DEBUGGING
	case DIK_1:
		CSimon::GetInstance()->SetPosition(0.0f, 0.0f);
		CCamera::GetInstance()->ChangeLimitBound(1, 1);
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


void TestInit()
{
	//tileMap = new CTileMap(L"json\\maptest_jsonmap.json");
	tileMap = new CTileMap(L"json\\scene_inside_jsonmap.json");
	tileMap->Init(ID_TEX_TILESET);
	cells = new CCells();
	cells->Init(tileMap, CELL_WIDTH, CELL_HEIGHT);
}

void InitDefaultObjects()
{
	CItems * items = CItems::GetInstance();			// For adding and managing the item-type objects
	CFlames * flames = CFlames::GetInstance();
	CWeapons * weapons = CWeapons::GetInstance();
	CMonsters * monsters = CMonsters::GetInstance();
	CBubbles * bubbles = CBubbles::GetInstance();
	CRocks * rocks = CRocks::GetInstance();
	board = CBoard::GetInstance();

	gameObject = CSimon::GetInstance();
	defaultObjects.push_back(gameObject);

	gameObject = CRope::GetInstance();
	defaultObjects.push_back(gameObject);

	// Dropable things
	gameObject = new CItemRope();
	defaultObjects.push_back(gameObject);
	items->Add(Item::ITEMROPE, gameObject);

	gameObject = new CItemDagger();
	defaultObjects.push_back(gameObject);
	items->Add(Item::ITEMDAGGER, gameObject);

	gameObject = new CBigHeart();
	defaultObjects.push_back(gameObject);
	items->Add(Item::BIGHEART, gameObject);

	gameObject = new CItemMeat();
	defaultObjects.push_back(gameObject);
	items->Add(Item::ITEMMEAT, gameObject);

	for (int i = 0; i < NUMBER_OF_HEART; ++i)
	{
		gameObject = new CHeart();
		defaultObjects.push_back(gameObject);
		items->Add(Item::HEART, gameObject);
	}

	for (int i = 0; i < NUMBER_OF_FLAME; ++i)
	{
		gameObject = new CFlame();
		defaultObjects.push_back(gameObject);
		flames->Add((CFlame *)gameObject);
	}

	for (int i = 0; i < NUMBER_OF_BUBBLE; ++i)
	{
		gameObject = new CBubble();
		defaultObjects.push_back(gameObject);
		bubbles->Add((CBubble *)gameObject);
	}

	for (int i = 0; i < NUMBER_OF_ROCK; ++i)
	{
		gameObject = new CRock();
		defaultObjects.push_back(gameObject);
		rocks->Add((CRock *)gameObject);
	}


	// Weapons
	for (int i = 0; i < NUMBER_OF_DAGGER; ++i)
	{
		gameObject = new CDagger();
		defaultObjects.push_back(gameObject);
		weapons->Add(Weapon::DAGGER, gameObject);
	}

	for (int i = 0; i < NUMBER_OF_FIREBALL; ++i)
	{
		gameObject = new CFireBall();
		defaultObjects.push_back(gameObject);
		weapons->Add(Weapon::FIREBALL, gameObject);
	}


	// Monsters
	for (int i = 0; i < NUMBER_OF_ZOMBIE; ++i)
	{
		gameObject = new CZombie();
		defaultObjects.push_back(gameObject);
		monsters->Add(Monster::ZOMBIE, gameObject);
	}

	for (int i = 0; i < NUMBER_OF_PANTHER; ++i)
	{
		gameObject = new CPanther();
		defaultObjects.push_back(gameObject);
		monsters->Add(Monster::PANTHER, gameObject);
	}

	for (int i = 0; i < NUMBER_OF_PINKBAT; ++i)
	{
		gameObject = new CPinkBat();
		defaultObjects.push_back(gameObject);
		monsters->Add(Monster::PINKBAT, gameObject);
	}

	for (int i = 0; i < NUMBER_OF_FISH; ++i)
	{
		gameObject = new CFish();
		defaultObjects.push_back(gameObject);
		monsters->Add(Monster::FISH, gameObject);
	}
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
	CCamera * camera = CCamera::GetInstance();
	camera->SetViewportSize(VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

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

	LPDIRECT3DTEXTURE9 texSimon = textures->Get(ID_TEX_SIMON);
	
	sprites->Add(99000, 436, 2, 468, 64, texSimon);		// idle right
	sprites->Add(99100, 12, 200, 44, 262, texSimon);	// idle left

	sprites->Add(99001, 374, 2, 406, 64, texSimon);		// walk right
	sprites->Add(99002, 313, 2, 345, 64, texSimon);		// walk right
	sprites->Add(99003, 256, 2, 288, 64, texSimon);		// walk right
	sprites->Add(99101, 74, 200, 106, 262, texSimon);	// walk left
	sprites->Add(99102, 135, 200, 167, 262, texSimon);	// walk left
	sprites->Add(99103, 192, 200, 224, 262, texSimon);	// walk left

	sprites->Add(99004, 196, 18, 228, 64, texSimon);	// crouch right
	sprites->Add(99104, 252, 216, 284, 262, texSimon);	// crouch left

	sprites->Add(99007, 136, 2, 168, 64, texSimon);		// attack right
	sprites->Add(99008, 75, 2, 107, 64, texSimon);		// attack right
	sprites->Add(99009, 16, 2, 48, 64, texSimon);		// attack right
	sprites->Add(99107, 312, 200, 360, 262, texSimon);	// attack left
	sprites->Add(99108, 373, 200, 405, 262, texSimon);	// attack left
	sprites->Add(99109, 432, 200, 464, 262, texSimon);	// attack left
	
	sprites->Add(99020, 438, 72, 470, 130, texSimon);	// go in

	sprites->Add(99022, 262, 68, 294, 130, texSimon);	// climbing right
	sprites->Add(99023, 191, 68, 221, 130, texSimon);	// upstairs right
	sprites->Add(99024, 319, 68, 349, 130, texSimon);	// downstairs right

	sprites->Add(99122, 186, 266, 218, 328, texSimon);	// climbing left
	sprites->Add(99123, 259, 266, 289, 328, texSimon);	// upstairs left
	sprites->Add(99124, 131, 266, 161, 328, texSimon);	// downstairs left
	
	sprites->Add(99037, 16, 67, 48, 113, texSimon);		// crouch attack right
	sprites->Add(99038, 437, 133, 469, 179, texSimon);	// crouch attack right
	sprites->Add(99039, 375, 133, 407, 179, texSimon);	// crouch attack right
	sprites->Add(99137, 432, 265, 464, 311, texSimon);	// crouch attack left
	sprites->Add(99138, 11, 331, 43, 377, texSimon);	// crouch attack left
	sprites->Add(99139, 73, 331, 105, 377, texSimon);	// crouch attack left

	sprites->Add(99047, 318, 134, 350, 196, texSimon);	// down stair attack right
	sprites->Add(99048, 258, 134, 290, 196, texSimon);	// down stair attack right
	sprites->Add(99049, 198, 134, 230, 196, texSimon);	// down stair attack right
	sprites->Add(99147, 130, 332, 162, 394, texSimon);	// down stair attack left
	sprites->Add(99148, 190, 332, 222, 394, texSimon);	// down stair attack left
	sprites->Add(99149, 250, 332, 282, 394, texSimon);	// down stair attack left

	sprites->Add(99057, 129, 134, 161, 196, texSimon);	// up stair attack right
	sprites->Add(99058, 69, 134, 101, 196, texSimon);	// up stair attack right
	sprites->Add(99059, 9, 134, 41, 196, texSimon);		// up stair attack right
	sprites->Add(99157, 319, 332, 351, 394, texSimon);	// up stair attack left
	sprites->Add(99158, 379, 332, 411, 394, texSimon);	// up stair attack left
	sprites->Add(99159, 439, 332, 471, 394, texSimon);	// up stair attack left

	sprites->Add(99090, 10, 270, 42, 328, texSimon);	// damaging right
	sprites->Add(99190, 438, 72, 470, 130, texSimon);	// damaging left

	sprites->Add(99198, 252, 200, 284, 262, texSimon);	// die_left
	sprites->Add(99199, 60, 267, 124, 329, texSimon);	// die_left
	sprites->Add(99098, 196, 2, 228, 64, texSimon);		// die_right
	sprites->Add(99099, 115, 68, 179, 130, texSimon);	// die_right

	ani = new CAnimation(100);
	ani->AddFrame(99000);
	animations->Add((int)SimonAniID::IDLE_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99100);
	animations->Add((int)SimonAniID::IDLE_LEFT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99001);
	ani->AddFrame(99002);
	ani->AddFrame(99003);
	ani->AddFrame(99000);
	animations->Add((int)SimonAniID::WALK_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99101);
	ani->AddFrame(99102);
	ani->AddFrame(99103);
	ani->AddFrame(99100);
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

	ani = new CAnimation(150);
	ani->AddFrame(99022);
	ani->AddFrame(99023);
	animations->Add((int)SimonAniID::WALK_UPSTAIRS_RIGHT, ani);

	ani = new CAnimation(150);
	ani->AddFrame(99022);
	ani->AddFrame(99024);
	animations->Add((int)SimonAniID::WALK_DOWNSTAIRS_RIGHT, ani);
	
	ani = new CAnimation(150);
	ani->AddFrame(99122);
	ani->AddFrame(99123);
	animations->Add((int)SimonAniID::WALK_UPSTAIRS_LEFT, ani);	

	ani = new CAnimation(150);
	ani->AddFrame(99122);
	ani->AddFrame(99124);
	animations->Add((int)SimonAniID::WALK_DOWNSTAIRS_LEFT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99023);
	animations->Add((int)SimonAniID::IDLE_UPSTAIRS_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99024);
	animations->Add((int)SimonAniID::IDLE_DOWNSTAIRS_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99123);
	animations->Add((int)SimonAniID::IDLE_UPSTAIRS_LEFT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99124);
	animations->Add((int)SimonAniID::IDLE_DOWNSTAIRS_LEFT, ani);

	ani = new CAnimation(75);
	ani->AddFrame(99037);
	ani->AddFrame(99038);
	ani->AddFrame(99039, 200);
	animations->Add((int)SimonAniID::CROUCH_ATTACK_RIGHT, ani);

	ani = new CAnimation(75);
	ani->AddFrame(99137);
	ani->AddFrame(99138);
	ani->AddFrame(99139, 200);
	animations->Add((int)SimonAniID::CROUCH_ATTACK_LEFT, ani);

	ani = new CAnimation(75);
	ani->AddFrame(99047);
	ani->AddFrame(99048);
	ani->AddFrame(99049, 200);
	animations->Add((int)SimonAniID::DOWN_STAIR_ATTACK_RIGHT, ani);

	ani = new CAnimation(75);
	ani->AddFrame(99147);
	ani->AddFrame(99148);
	ani->AddFrame(99149, 200);
	animations->Add((int)SimonAniID::DOWN_STAIR_ATTACK_LEFT, ani);

	ani = new CAnimation(75);
	ani->AddFrame(99057);
	ani->AddFrame(99058);
	ani->AddFrame(99059, 200);
	animations->Add((int)SimonAniID::UP_STAIR_ATTACK_RIGHT, ani);

	ani = new CAnimation(75);
	ani->AddFrame(99157);
	ani->AddFrame(99158);
	ani->AddFrame(99159, 200);
	animations->Add((int)SimonAniID::UP_STAIR_ATTACK_LEFT, ani);

	ani = new CAnimation(500);
	ani->AddFrame(99198);
	ani->AddFrame(99199, 999999999);
	animations->Add((int)SimonAniID::DIE_LEFT, ani);

	ani = new CAnimation(500);
	ani->AddFrame(99098);
	ani->AddFrame(99099, 999999999);
	animations->Add((int)SimonAniID::DIE_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99090);
	animations->Add((int)SimonAniID::DAMAGING_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(99190);
	animations->Add((int)SimonAniID::DAMAGING_LEFT, ani);

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
	
	// big candle
	sprites->Add(12000, 0, 0, 32, 64, texMisc);
	sprites->Add(12001, 32, 0, 64, 64, texMisc);
	ani = new CAnimation(100);
	ani->AddFrame(12000);
	ani->AddFrame(12001);
	animations->Add((int)BigCandleAniID::IDLE, ani);

	// small candle - spriteID = 13xxx
	sprites->Add(13000, 67, 0, 83, 32, texMisc);
	sprites->Add(13001, 83, 0, 99, 32, texMisc);
	ani = new CAnimation(100);
	ani->AddFrame(13000);
	ani->AddFrame(13001);
	animations->Add((int)CandleAniID::IDLE, ani);

	// item Rope
	sprites->Add(14000, 61, 64, 93, 96, texMisc);
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
	sprites->Add(16000, 95, 105, 119, 125, texMisc);
	ani = new CAnimation(100);
	ani->AddFrame(16000);
	animations->Add((int)BigHeartAniID::IDLE, ani);

	// heart
	sprites->Add(17000, 126, 110, 142, 126, texMisc);
	ani = new CAnimation(100);
	ani->AddFrame(17000);
	animations->Add((int)HeartAniID::IDLE, ani);

	// item dagger
	sprites->Add(18000, 153, 46, 185, 64, texMisc);
	ani = new CAnimation(100);
	ani->AddFrame(18000);
	animations->Add((int)ItemDaggerAniID::IDLE, ani);

	// dagger
	sprites->Add(18100, 192, 46, 224, 64, texMisc);

	ani = new CAnimation(100);
	ani->AddFrame(18000);		// use the same sprite as itemdagger
	animations->Add((int)DaggerAniID::IDLE_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(18100);		
	animations->Add((int)DaggerAniID::IDLE_LEFT, ani);

	// fire ball
	sprites->Add(19000, 163, 112, 177, 124, texMisc);		// flying right
	sprites->Add(19100, 147, 112, 161, 124, texMisc);		// flying left

	ani = new CAnimation(100);
	ani->AddFrame(19000);
	animations->Add((int)FireBallAniID::FLYING_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(19100);
	animations->Add((int)FireBallAniID::FLYING_LEFT, ani);

	// bubble
	sprites->Add(20000, 85, 34, 99, 62, texMisc);

	ani = new CAnimation(100);
	ani->AddFrame(20000);
	animations->Add((int)BubbleAniID::IDLE, ani);

	// secret bricks
	sprites->Add(21000, 129, 67, 161, 99, texMisc);
	sprites->Add(21001, 173, 67, 205, 99, texMisc);

	ani = new CAnimation(100);
	ani->AddFrame(21000);
	animations->Add((int)SecretBrickAniID::IDLE_TYPE_1, ani); 
	
	ani = new CAnimation(100);
	ani->AddFrame(21001);
	animations->Add((int)SecretBrickAniID::IDLE_TYPE_2, ani);

	// rock 
	sprites->Add(22000, 229, 45, 243, 61, texMisc);

	ani = new CAnimation(100);
	ani->AddFrame(22000);
	animations->Add((int)RockAniID::IDLE, ani);

	// item meat
	sprites->Add(23000, 132, 228, 164, 254, texMisc);

	ani = new CAnimation(100);
	ani->AddFrame(23000);
	animations->Add((int)ItemMeatAniID::IDLE, ani);

	// gate
	sprites->Add(24000, 4, 131, 20, 227, texMisc);		// closing
	sprites->Add(24001, 49, 131, 81, 227, texMisc);		// opening
	sprites->Add(24002, 97, 131, 177, 227, texMisc);	// opening

	ani = new CAnimation(100);
	ani->AddFrame(24000, 1000);
	ani->AddFrame(24001, 500);
	ani->AddFrame(24002, 500);
	ani->AddFrame(24000);		// close again
	animations->Add((int)GateAniID::OPENING, ani);

	ani = new CAnimation(100);
	ani->AddFrame(24000);
	animations->Add((int)GateAniID::CLOSING, ani);


#pragma endregion

#pragma region Loading monsters resources

	LPDIRECT3DTEXTURE9 texMonsters = textures->Get(ID_TEX_MONSTERS);

	// Zombie
	sprites->Add(51000, 0, 0, 32, 64, texMonsters);				// walk_left
	sprites->Add(51001, 36, 0, 68, 64, texMonsters);			// walk_left
	sprites->Add(51100, 294, 180, 326, 244, texMonsters);		// walk_right
	sprites->Add(51101, 258, 180, 290, 244, texMonsters);		// walk_right

	ani = new CAnimation(100);
	ani->AddFrame(51000);
	ani->AddFrame(51001);
	animations->Add((int)ZombieAniID::WALK_LEFT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(51100);
	ani->AddFrame(51101);
	animations->Add((int)ZombieAniID::WALK_RIGHT, ani);

	// Panther
	sprites->Add(52000, 201, 212, 249, 244, texMonsters);		// idle right
	sprites->Add(52001, 134, 212, 198, 244, texMonsters);		// run right
	sprites->Add(52002, 68, 212, 132, 244, texMonsters);		// run right
	sprites->Add(52003, 2, 212, 66, 244, texMonsters);			// run right / jumping right

	sprites->Add(52100, 77, 32, 125, 64, texMonsters);			// idle left
	sprites->Add(52101, 128, 32, 192, 64, texMonsters);			// run left
	sprites->Add(52102, 194, 32, 258, 64, texMonsters);			// run left
	sprites->Add(52103, 260, 32, 324, 64, texMonsters);			// run left / jumping left

	ani = new CAnimation(100);
	ani->AddFrame(52000);
	animations->Add((int)PantherAniID::IDLE_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(52100);
	animations->Add((int)PantherAniID::IDLE_LEFT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(52001);
	ani->AddFrame(52002);
	ani->AddFrame(52003);
	animations->Add((int)PantherAniID::RUN_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(52101);
	ani->AddFrame(52102);
	ani->AddFrame(52103);
	animations->Add((int)PantherAniID::RUN_LEFT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(52003);
	animations->Add((int)PantherAniID::JUMP_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(52103);
	animations->Add((int)PantherAniID::JUMP_LEFT, ani);

	// Bat
	sprites->Add(53101, 100, 0, 132, 32, texMonsters);		// flying left
	sprites->Add(53102, 132, 0, 164, 32, texMonsters);		// flying left
	sprites->Add(53103, 164, 0, 196, 32, texMonsters);		// flying left
				  
	sprites->Add(53001, 194, 180, 226, 212, texMonsters);	// flying right
	sprites->Add(53002, 162, 180, 194, 212, texMonsters);	// flying right
	sprites->Add(53003, 130, 180, 162, 212, texMonsters);	// flying right

	ani = new CAnimation(100);
	ani->AddFrame(53001);
	ani->AddFrame(53002);
	ani->AddFrame(53003);
	animations->Add((int)PinkBatAniID::FLYING_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(53101);
	ani->AddFrame(53102);
	ani->AddFrame(53103);
	animations->Add((int)PinkBatAniID::FLYING_LEFT, ani);

	// Fish
	sprites->Add(54100, 64, 66, 96, 130, texMonsters);		// IDLE LEFT
	sprites->Add(54101, 32, 66, 64, 130, texMonsters);		// WALK LEFT
	sprites->Add(54102, 0, 66, 32, 130, texMonsters);		// ATTACK

	sprites->Add(54000, 230, 246, 262, 310, texMonsters);	// IDLE RIGHT
	sprites->Add(54001, 262, 246, 294, 310, texMonsters);	// WALK RIGHT
	sprites->Add(54002, 294, 246, 326, 310, texMonsters);	// ATTACK RIGHT

	ani = new CAnimation(100);
	ani->AddFrame(54000);
	animations->Add((int)FishAniID::IDLE_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(54100);
	animations->Add((int)FishAniID::IDLE_LEFT, ani);

	ani = new CAnimation(200);
	ani->AddFrame(54000);
	ani->AddFrame(54001);
	animations->Add((int)FishAniID::MOVE_RIGHT, ani);

	ani = new CAnimation(200);
	ani->AddFrame(54100);
	ani->AddFrame(54101);
	animations->Add((int)FishAniID::MOVE_LEFT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(54002);
	animations->Add((int)FishAniID::ATTACK_RIGHT, ani);

	ani = new CAnimation(100);
	ani->AddFrame(54102);
	animations->Add((int)FishAniID::ATTACK_LEFT, ani);

	// BOSS BAT
	sprites->Add(55000, 182, 132, 278, 176, texMonsters);
	sprites->Add(55001, 99, 132, 195, 176, texMonsters);
	sprites->Add(55002, 1, 133, 97, 177, texMonsters);

	ani = new CAnimation(100);
	ani->AddFrame(55000);
	animations->Add((int)BossBatAniID::SLEEP, ani);

	ani = new CAnimation(100);
	ani->AddFrame(55001);
	ani->AddFrame(55002);
	animations->Add((int)BossBatAniID::FLY, ani);

#pragma endregion

#pragma region Loading board resources

	// player health unit
	sprites->Add(97001, 112, 45, 120, 59, texMisc);
	ani = new CAnimation(100);
	ani->AddFrame(97001);
	animations->Add((int)BoardAniID::PLAYER_HEALTH_UNIT, ani);

	// enemy health unit
	sprites->Add(97002, 123, 45, 131, 59, texMisc);
	ani = new CAnimation(100);
	ani->AddFrame(97002);
	animations->Add((int)BoardAniID::ENEMY_HEALTH_UNIT, ani);

	// empty health unit
	sprites->Add(97003, 134, 45, 142, 59, texMisc);
	ani = new CAnimation(100);
	ani->AddFrame(97003);
	animations->Add((int)BoardAniID::EMPTY_UNIT_HEALTH, ani);

	// item box
	sprites->Add(97004, 179, 116, 243, 160, texMisc);
	ani = new CAnimation(100);
	ani->AddFrame(97004);
	animations->Add((int)BoardAniID::ITEM_BOX, ani);

	// heart
	sprites->Add(97004, 228, 162, 244, 188, texMisc);
	ani = new CAnimation(100);
	ani->AddFrame(97004);
	animations->Add((int)BoardAniID::HEART, ani);

#pragma endregion


	InitDefaultObjects();
	TestInit();

}


/*
	Update world status for this frame
	dt: time period between beginning of last frame and beginning of this frame
*/
void Update(DWORD dt)
{
	vector<LPGAMEOBJECT> coObjects;
	coObjects.clear();


	// Get the bounding box of viewport
	float left, top, right, bottom;
	CCamera::GetInstance()->GetBoundingBox(left, top, right, bottom);


	// Get objects in the cells
	updateObjects.clear();
	cells->GetObjectsInRectangle(left, top, right, bottom, updateObjects);


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


	// Call Update function of each objects
	for (UINT i = 0; i < updateObjects.size(); i++)
	{
		if (dynamic_cast<CActiveObject *>(updateObjects[i])
			&& updateObjects[i]->GetState() == STATE_VISIBLE
			&& updateObjects[i]->GetFreezing() == false)
		{
			dynamic_cast<CActiveObject *>(updateObjects[i])->Update(dt, &coObjects);
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

	if (d3ddv->BeginScene())
	{
		// Clear back buffer with a color
		d3ddv->ColorFill(bb, NULL, BACKGROUND_COLOR);

		spriteHandler->Begin(D3DXSPRITE_ALPHABLEND);

		// Only draw the tile in the camera 
		float left, top, right, bottom;
		CCamera::GetInstance()->GetBoundingBox(left, top, right, bottom);
		tileMap->Draw(left, top, right, bottom);

		for (UINT i = 0; i < updateObjects.size(); i++)
		{
			updateObjects[i]->Render();
			updateObjects[i]->RenderBoundingBox();
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