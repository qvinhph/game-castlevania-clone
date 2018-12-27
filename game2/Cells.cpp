#include "Cells.h"
#include <unordered_set>


void CCells::Init(CTileMap * tileMap, int cellWidth, int cellHeight)
{
	// Get some useful information
	int mapHeight;
	int mapWidth;
	tileMap->GetMapSize(mapWidth, mapHeight);

	this->cellHeight = cellHeight;
	this->cellWidth = cellWidth;


	// Get the total of columns, rows
	this->rows = (mapHeight % cellHeight == 0) ?
		mapHeight / cellHeight :
		mapHeight / cellHeight + 1;

	this->columns = (mapWidth % cellWidth == 0) ?
		mapWidth / cellWidth :
		mapWidth / cellWidth + 1;


	// Init the cells container
	cells.resize(columns);
	for (UINT i = 0; i < cells.size(); ++i)
		cells[i].resize(rows, NULL);


	// Get all the objects in the given map
	vector<LPGAMEOBJECT> *objects = NULL;
	tileMap->GetGameObjects(objects);


	// Put objects into suitable cell
	for (UINT i = 0; i < objects->size(); ++i)
		this->Classify(objects->at(i));

}

void CCells::GetObjectsInRectangle(float const & left, float const & top, 
								float const & right, float const & bottom, 
								vector<LPGAMEOBJECT>& objects)
{
	// Find the top-left and bottom-right cell that contains the viewport's area
	int firstCellColumn, firstCellRow;		// the left-top cell that containing the rectangle's area
	int lastCellColumn, lastCellRow;		// the right-bottom cell that containing the rectangle's area
	this->GetCellsContainRectangle(left, top, right, bottom,
		firstCellColumn, firstCellRow, lastCellColumn, lastCellRow);


	// get objects to from cells
	unordered_set<LPGAMEOBJECT> setOfObjects;
	for (UINT row = firstCellRow; row <= lastCellRow; row++)
		for (UINT column = firstCellColumn; column <= lastCellColumn; column++)
		{
			// Empty cell
			if (cells[column][row] == NULL)
				continue;

			// Get objects containing in cell
			vector<LPGAMEOBJECT> * cellObjects = NULL;
			cells[column][row]->GetObjects(cellObjects);

			// Get the unique pointer to the game object
			for (UINT i = 0; i < cellObjects->size(); i++)
				setOfObjects.insert(cellObjects->at(i));
		}

	for (auto it = setOfObjects.begin(); it != setOfObjects.end(); ++it)
		objects.push_back(*it);
}

void CCells::Classify(LPGAMEOBJECT obj)
{
	int beginCellColumn, beginCellRow, endCellColumn, endCellRow;
	float left, top, right, bottom;
	obj->GetBoundingBox(left, top, right, bottom);

	this->GetCellsContainRectangle(left, top, right, bottom,
		beginCellColumn, beginCellRow, endCellColumn, endCellRow);

	// add object to the suitable cells
	for (UINT row = beginCellRow; row <= endCellRow; row++)
		for (UINT column = beginCellColumn; column <= endCellColumn; column++)
		{
			if (cells[column][row] == NULL)
				cells[column][row] = new CCell();

			cells[column][row]->Add(obj);
		}
}

void CCells::GetCellsContainRectangle(float const & left, float const & top, 
									float const & right, float const & bottom, 
									int & firstCellColumn, int & firstCellRow, 
									int & lastCellColumn, int & lastCellRow)
{
	firstCellColumn = (left < 0) ? 0 : left / cellWidth;
	firstCellRow = (top < 0) ? 0 : top / cellHeight;

	lastCellColumn = (right / cellWidth < columns) ? (right / cellWidth) : columns - 1;
	lastCellRow = (bottom / cellHeight < rows) ? (bottom / cellHeight) : rows - 1;
}
