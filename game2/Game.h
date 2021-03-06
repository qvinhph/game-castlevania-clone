#pragma once
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
#include <string>
#include "Camera.h"

#include "KeyEventHandler.h"

#define DIRECTINPUT_VERSION		0x0800
#define KEYBOARD_BUFFER_SIZE	1024

#define FONT_SIZE				16			// depend on the height of the font

#define ARGB_WHITE(alpha)		ARGB(alpha, 255, 255, 255)
#define ARGB_BLACK(alpha)		ARGB(alpha, 0, 0, 0)

#define ARGB_ALPHA_MAX_VALUE	255
#define ARGB_ALPHA_MIN_VALUE	0

/*
	Struct describes the ARGB color.
*/
struct ARGB
{
	ARGB() : alpha(255), red(255), green(255), blue(255) {}

	ARGB(int alpha, int red, int green, int blue)
		: alpha(alpha), red(red), green(green), blue(blue) {}

	int alpha;
	int red;
	int green;
	int blue;
};

class CGame
{	

	HWND hWnd;

	LPDIRECT3D9 d3d = NULL;						// Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL;				// Direct3D device object

	LPDIRECT3DSURFACE9 backBuffer = NULL;
	LPD3DXSPRITE spriteHandler = NULL;			// Sprite helper library to help us draw 2D image on the screen 
	LPD3DXFONT font = NULL;						// Helper library to help us draw text on the screen

	LPDIRECTINPUT8       di;					// The DirectInput object         
	LPDIRECTINPUTDEVICE8 didv;					// The keyboard device 

	BYTE  keyStates[256];						// DirectInput keyboard state buffer 
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data

	LPKEY_EVENT_HANDLER keyEventHandler;

	CCamera * cameraInstance = CCamera::GetInstance();

	static CGame * __instance;
	CGame() = default;

public:
	void InitKeyboard(LPKEY_EVENT_HANDLER handler);
	void Init(HWND hWnd);
	void Draw(float x, float y, LPDIRECT3DTEXTURE9 tex, int left, int top, int right, int bottom, 
		ARGB &argb = ARGB());

	/*
		Draw the given string with the default color (able to change).
	*/
	void DrawString(float x, float y, std::string text, ARGB argb = ARGB_WHITE(255));

	int IsKeyDown(int keyCode);
	void ProcessKeyboard();

	static void SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			// 
		float dy,			// 
		float sl,			// static left
		float st,
		float sr,
		float sb,
		float &t,
		float &nx,
		float &ny);

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }

	static CGame * GetInstance();

	~CGame();
};

