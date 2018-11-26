#pragma once
#include "Cell.h"
#include "GameObject.h"
#include "TileMap.h"

class CCells
{
	// cells[column][row]
	vector<vector<LPCELL>> cells;

	int rows;			// Number of cells in a column
	int columns;		// Number of cells in a row

	int cellHeight;		
	int cellWidth;

public:

	void Init(CTileMap *tileMap, int cellWidth, int cellHeight);
	void Classify(LPGAMEOBJECT obj);
	void GetCellsContainRectangle(float &left, float &top, float &right, float &bottom,
									int &beginCellColumn, int &beginCellRow,
									int &endCellColumn, int &endCellRow);

	// Get all the objects in cells that containing the given rectangle
	void GetObjectsByRec(float &left, float &top, float &right, float &bottom, vector<LPGAMEOBJECT> &objects);
};

