#include "CameraEvent.h"


void CCameraEvent::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	right = x + width;
	top = y;
	bottom = y + height;
}

CCameraEvent::CCameraEvent(float width, float height, CameraEvent cameraEvent)
{
	this->width = width;
	this->height = height;
	this->cameraEvent = cameraEvent;
}