#pragma once
#include "GameObject.h"

enum class CameraEvent
{
	SLOWLY_MOVE,
	LOCK,
};

class CCameraEvent : public CGameObject
{

	CameraEvent cameraEvent;
	float width;
	float height;

public:
	
	void GetBoundingBox(float &left, float &top, float &right, float &bottom) override;
	void Render() override { /*	Nothing here. Because this is an invisible object */ }

	CameraEvent GetCameraEvent() { return this->cameraEvent; }
	void SetCameraEvent(CameraEvent cameraEvent) { this->cameraEvent = cameraEvent; }

	CCameraEvent(float width, float height, CameraEvent cameraEvent);
};

