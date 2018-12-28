#pragma once
#include <unordered_map>
#include <d3dx9.h>
#include <string>

using namespace std;

/*
	Manage texture database
*/
class CTextures
{
	static CTextures * __instance;
	CTextures() = default;
	unordered_map<int, LPDIRECT3DTEXTURE9> textures;

public:

	void Add(int id, LPCWSTR filePath, D3DCOLOR transparentColor);
	void Add(int id, string filePath, D3DCOLOR transparentColor);
	void AddTexturesFromFile(LPCWSTR filePath);
	LPDIRECT3DTEXTURE9 Get(unsigned int i);

	static CTextures * GetInstance();
};