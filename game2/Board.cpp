#include "Board.h"
#include "Animations.h"
#include "debug.h"

CBoard * CBoard::__instance = NULL;

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
	CGame * game = CGame::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();
	float xCam, yCam;
	game->GetCameraPosition(xCam, yCam);

	// Draw text
	game->DrawString(PLAYER_TEXT_POSITION_X, PLAYER_TEXT_POSITION_Y, PLAYER_LIFE_TEXT);


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
	CGame * game = CGame::GetInstance();
	CAnimations * animations = CAnimations::GetInstance();
	float xCam, yCam;
	game->GetCameraPosition(xCam, yCam);


	//Draw the text
	game->DrawString(ENEMY_TEXT_POSITION_X, ENEMY_TEXT_POSITION_Y, ENEMY_LIFE_TEXT);


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
}

void CBoard::RenderPoint()
{
	CGame * game = CGame::GetInstance();

	// Format the string to draw to the screen
	std::string number = std::to_string(point);
	number.insert(number.begin(), POINT_DIGITS - number.size(), '0');


	std::string pointString = POINT_TEXT + number;
	game->DrawString(POINT_POSITION_X, POINT_POSITION_Y, pointString);
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
	point = POINT_DEFAULT;
}

