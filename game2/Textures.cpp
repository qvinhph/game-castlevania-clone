#include <Windows.h>

#include <d3d9.h>
#include <d3dx9.h>
#include <fstream>

#include "debug.h"
#include "Game.h"
#include "textures.h"

CTextures * CTextures::__instance = NULL;

CTextures *CTextures::GetInstance()
{
	if (__instance == NULL) __instance = new CTextures();
	return __instance;
}

void CTextures::Add(int id, LPCWSTR filePath, D3DCOLOR transparentColor)
{
	D3DXIMAGE_INFO info;
	HRESULT result = D3DXGetImageInfoFromFile(filePath, &info);
	if (result != D3D_OK)
	{
		DebugOut(L"[ERROR] GetImageInfoFromFile failed: %s\n", filePath);
		return;
	}

	LPDIRECT3DDEVICE9 d3ddv = CGame::GetInstance()->GetDirect3DDevice();
	LPDIRECT3DTEXTURE9 texture;

	result = D3DXCreateTextureFromFileEx(
		d3ddv,								// Pointer to Direct3D device object
		filePath,							// Path to the image to load
		info.Width,							// Texture width
		info.Height,						// Texture height
		1,
		D3DUSAGE_DYNAMIC,
		D3DFMT_UNKNOWN,
		D3DPOOL_DEFAULT,
		D3DX_DEFAULT,
		D3DX_DEFAULT,
		transparentColor,
		&info,
		NULL,
		&texture);								// Created texture pointer

	if (result != D3D_OK)
	{
		OutputDebugString(L"[ERROR] CreateTextureFromFile failed\n");
		return;
	}

	textures[id] = texture;

	DebugOut(L"[INFO] Texture loaded Ok: id=%d, %s \n", id, filePath);
}

void CTextures::Add(int id, string filePath, D3DCOLOR transparentColor)
{
	std::wstring stemp = std::wstring(filePath.begin(), filePath.end());
	LPCWSTR convertedString = stemp.c_str();

	this->Add(id, convertedString, transparentColor);
}

void CTextures::AddTexturesFromFile(LPCWSTR filePath)
{
	ifstream inputFile(filePath);
	string data = "";
	inputFile >> data;

	// Find the begin of the data
	while (inputFile >> data)
	{
		if (data == "BEGIN")
		{
			int texID = 0;
			string path = "";
			int rgbRed = 0;
			int rgbBlue = 0;
			int rgbGreen = 0;

			// Read the infomation of texture
			while (inputFile >> data)
			{
				if (data == "END")
					return;

				// Get the texture ID
				texID = atoi(data.c_str());

				// Get the path
				inputFile >> data;
				path = data;

				// Get the rgb-red 
				inputFile >> data;
				rgbRed = atoi(data.c_str());

				// Get the rgb-green 
				inputFile >> data;
				rgbGreen = atoi(data.c_str());

				// Get the rgb-blue 
				inputFile >> data;
				rgbBlue = atoi(data.c_str());

				// Add texture
				this->Add(texID, path, D3DCOLOR_XRGB(rgbRed, rgbGreen, rgbBlue));
			}
		}
	}	
}

LPDIRECT3DTEXTURE9 CTextures::Get(unsigned int i)
{
	return textures[i];
}



