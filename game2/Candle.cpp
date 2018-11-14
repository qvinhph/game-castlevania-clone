#include "Candle.h"



void CCandle::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = left + CANDLE_BBOX_WIDTH;
	bottom = top + CANDLE_BBOX_HEIGHT;

}
