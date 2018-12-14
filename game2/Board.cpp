#include "Board.h"
#include "Heart.h"
#include "Simon.h"
#include "debug.h"

CBoard * CBoard::__instance = NULL;
CCamera * CBoard::cameraInstance = CCamera::GetInstance();
CGame * CBoard::gameInstance = CGame::GetInstance();

void CBoard::Render()
{
	CGame * game = CGame::GetInstance();

	RenderScore();
	RenderHeart();
	RenderPoint();
	RenderTime();
	RenderEnemyLife();
	RenderPlayerHealth();
	RenderStage();
	RenderWeaponBox();

}

void CBoard::Update()
{
	if (playerHealthTemp != 0)
		ChangePlayerHealthGradually();
}

void CBoard::RenderScore()
{
	CGame * game = CGame::GetInstance();

	// Format the string to draw to the screen
	std::string number = std::to_string(score);
	number.insert(number.begin(), SCORE_DIGITS - number.size(), '0');


	std::string scoreString = SCORE_TEXT + number;
	game->DrawString(SCORE_POSITION_X, SCORE_POSITION_Y, scoreString);
}

void CBoard::RenderTime()
{
	CGame * game = CGame::GetInstance();

	// Format the string to draw to the screen
	std::string number = std::to_string(time);
	number.insert(number.begin(), TIME_DIGITS - number.size(), '0');


	std::string timeString = TIME_TEXT + number;
	game->DrawString(TIME_POSITION_X, TIME_POSITION_Y, timeString);
}

void CBoard::RenderStage()
{
	CGame * game = CGame::GetInstance();

	// Format the string to draw to the screen
	std::string number = std::to_string(stage);
	number.insert(number.begin(), STAGE_DIGITS - number.size(), '0');


	std::string stageString = STAGE_TEXT + number;
	game->DrawString(STAGE_POSITION_X, STAGE_POSITION_Y, stageString);
}

void CBoard::RenderPlayerHealth()
{
	CAnimations * animations = CAnimations::GetInstance();
	float xCam, yCam;
	cameraInstance->GetPosition(xCam, yCam);

	// Draw text
	gameInstance->DrawString(PLAYER_TEXT_POSITION_X, PLAYER_TEXT_POSITION_Y, PLAYER_LIFE_TEXT);


	// Draw the remaining health
	int i = 0;
	for (; i < playerHealth; i++)
	{
		animations->Get((int)BoardAniID::PLAYER_HEALTH_UNIT)
			->Render(xCam + PLAYER_HEALTH_BAR_POSITION_X + i * HEALTH_UNIT_WIDTH,
					yCam + PLAYER_HEALTH_BAR_POSITION_Y);
	}


	// Draw the lost health
	for (; i != PLAYER_HEALTH_DEFAULT; i++)
	{
		animations->Get((int)BoardAniID::EMPTY_UNIT_HEALTH)
			->Render(xCam + PLAYER_HEALTH_BAR_POSITION_X  + i * HEALTH_UNIT_WIDTH,
				yCam + PLAYER_HEALTH_BAR_POSITION_Y);
	}
}

void CBoard::RenderEnemyLife()
{
	CAnimations * animations = CAnimations::GetInstance();
	float xCam, yCam;
	cameraInstance->GetPosition(xCam, yCam);


	//Draw the text
	gameInstance->DrawString(ENEMY_TEXT_POSITION_X, ENEMY_TEXT_POSITION_Y, ENEMY_LIFE_TEXT);


	// Draw the remaining health
	int i = 0;
	for (; i < enemyHealth; i++)
	{
		animations->Get((int)BoardAniID::ENEMY_HEALTH_UNIT)
			->Render(xCam + i * HEALTH_UNIT_WIDTH + ENEMY_HEALTH_BAR_POSITION_X,
				yCam + ENEMY_HEALTH_BAR_POSITION_Y);
	}


	// Draw the lost health
	for (; i != ENEMY_HEALTH_DEFAULT; i++)
	{
		animations->Get((int)BoardAniID::EMPTY_UNIT_HEALTH)
			->Render(xCam + i * HEALTH_UNIT_WIDTH + ENEMY_HEALTH_BAR_POSITION_X,
				yCam + ENEMY_HEALTH_BAR_POSITION_Y);
	}
}

void CBoard::RenderHeart()
{
	CAnimations * animations = CAnimations::GetInstance();
	float xCam, yCam;
	cameraInstance->GetPosition(xCam, yCam);

	// Format the string to draw to the screen
	std::string number = std::to_string(heart);
	number.insert(number.begin(), HEART_DIGITS - number.size(), '0');


	std::string heartString = HEART_TEXT + number;
	gameInstance->DrawString(HEART_POSITION_X, HEART_POSITION_Y, heartString);

	// Draw the heart image
	animations->Get((int)BoardAniID::HEART)->Render(xCam + HEART_POSITION_X,
													yCam + HEART_POSITION_Y);

}

void CBoard::RenderPoint()
{
	CGame * game = CGame::GetInstance();

	// Format the string to draw to the screen
	std::string number = std::to_string(lifePoint);
	number.insert(number.begin(), LIFE_POINT_DIGITS - number.size(), '0');


	std::string pointString = LIFE_POINT_TEXT + number;
	game->DrawString(LIFE_POINT_POSITION_X, LIFE_POINT_POSITION_Y, pointString);
}

void CBoard::RenderWeaponBox()
{
	CAnimations * animations = CAnimations::GetInstance();
	float xCam, yCam;
	cameraInstance->GetPosition(xCam, yCam);

	// Render the box
	animations->Get((int)BoardAniID::ITEM_BOX)->Render(xCam + ITEM_BOX_POSITION_X,
														yCam + ITEM_BOX_POSITION_Y);

	// Render the weapon holding inside
	Weapon weapon = CSimon::GetInstance()->GetSecondaryWeapon();
	float xWeapon, yWeapon;
	switch (weapon)
	{
	case Weapon::DAGGER:
		CalcWeaponInsidePosition(DAGGER_BBOX_WIDTH, DAGGER_BBOX_HEIGHT, xWeapon, yWeapon);
		animations->Get((int)DaggerAniID::IDLE_RIGHT)->Render(xWeapon, yWeapon);
		break;
	case Weapon::NONE:
		break;
	}
	
}

void CBoard::ChangePlayerHealthGradually()
{
	if (player_health_pause_start == TIMER_IDLE)
	{
		if (playerHealthTemp > 0)
		{
			playerHealthTemp--;
			playerHealth++;
		}

		if (playerHealthTemp < 0)
		{
			playerHealthTemp++;
			playerHealth--;
		}

		player_health_pause_start = GetTickCount();
	}
	else
	{
		if (GetTickCount() - player_health_pause_start > HEALTH_PAUSE_TIME)
			player_health_pause_start = TIMER_IDLE;
	}
	
}

void CBoard::CalcWeaponInsidePosition(float weaponWidth, float weaponHeight, float & x, float & y)
{
	float xCam, yCam;
	cameraInstance->GetPosition(xCam, yCam);

	// Central position of item box
	float xCenter = ITEM_BOX_POSITION_X + ITEM_BOX_WIDTH / 2;
	float yCenter = ITEM_BOX_POSITION_Y + ITEM_BOX_HEIGHT / 2;

	// Calculate the weapon image position
	x = xCam + xCenter - weaponWidth / 2;
	y = yCam + yCenter - weaponHeight / 2;
}

CBoard * CBoard::GetInstance()
{
	if (__instance == NULL)
		__instance = new CBoard();

	return __instance;
}

CBoard::CBoard()
{
	score = SCORE_DEFAULT;
	time = TIME_DEFAULT;
	playerHealth = PLAYER_HEALTH_DEFAULT;
	enemyHealth = ENEMY_HEALTH_DEFAULT;
	stage = STAGE_DEFAULT;
	heart = HEART_DEFAULT;
	lifePoint = LIFE_POINT_DEFAULT;

}

