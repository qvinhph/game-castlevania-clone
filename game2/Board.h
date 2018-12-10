#pragma once
#include "Game.h"

#define SCORE_DEFAULT		0
#define SCORE_DIGITS		6	
#define SCORE_POSITION_X	2
#define SCORE_POSITION_Y	16

#define TIME_DEFAULT		300
#define TIME_DIGITS			3
#define TIME_POSITION_X		212
#define TIME_POSITION_Y		16

#define STAGE_DEFAULT		1
#define STAGE_DIGITS		2
#define STAGE_POSITION_X	370
#define STAGE_POSITION_Y	16

#define HEART_DEFAULT		5
#define HEART_DIGITS		2
#define HEART_POSITION_X	336
#define HEART_POSITION_Y	32

#define POINT_DEFAULT		3
#define POINT_DIGITS		2
#define POINT_POSITION_X	338
#define POINT_POSITION_Y	48

#define PLAYER_HEALTH_DEFAULT			16
#define PLAYER_TEXT_POSITION_X			2
#define PLAYER_TEXT_POSITION_Y			32
#define PLAYER_HEALTH_BAR_POSITION_X	111
#define PLAYER_HEALTH_BAR_POSITION_Y	33

#define ENEMY_HEALTH_DEFAULT			16
#define ENEMY_TEXT_POSITION_X			4
#define ENEMY_TEXT_POSITION_Y			48
#define ENEMY_HEALTH_BAR_POSITION_X		111
#define ENEMY_HEALTH_BAR_POSITION_Y		49

#define SCORE_TEXT						"SCORE-"
#define TIME_TEXT						"TIME "
#define POINT_TEXT						"P- "
#define STAGE_TEXT						"STAGE "
#define PLAYER_LIFE_TEXT				"PLAYER "
#define ENEMY_LIFE_TEXT					"ENEMY  "

#define ITEM_BOX_POSITION_X		256
#define ITEM_BOX_POSITION_Y		32

#define HEALTH_UNIT_WIDTH		8
#define HEALTH_UNIT_HEIGHT		14

enum class BoardAniID
{
	PLAYER_HEALTH_UNIT = 9701,
	ENEMY_HEALTH_UNIT,
	EMPTY_UNIT_HEALTH,
	ITEM_BOX,
};



class CBoard
{
	int score;
	int time;
	int stage;
	int playerHealth;
	int enemyHealth;
	int heart;
	int point;			// number of Simon's life left

	static CBoard * __instance;

	CBoard();

public:

#pragma region Get/Set functions

	int GetScore() { return this->score; }
	void SetScore(int score) { this->score = score; }

	int GetTime() { return this->time; }
	void SetTime(int time) { this->time = time; }

	int GetStage() { return this->stage; }
	void SetStage(int stage) { this->stage = stage; }

	int GetHeart() { return this->heart; }
	void SetHeart(int heart) { this->heart = heart; }

	int GetPoint() { return this->point; }
	void SetPoint(int point) { this->point = point; }

	int GetPlayerLife() { return this->playerHealth; }
	void SetPlayerLife(int playerLife) { this->playerHealth = playerLife; }

	int GetEnemyLife() { return this->enemyHealth; }
	void SetEnemyLife(int enemyLife) { this->enemyHealth = enemyLife; }

#pragma endregion

	void Render();
	void RenderScore();
	void RenderTime();
	void RenderStage();
	void RenderPlayerHealth();
	void RenderEnemyLife();
	void RenderHeart();
	void RenderPoint();

	// Also use these functions for subtraction
	void AddScore(int number) { this->score += number; }
	void AddTime(int number) { this->time += number; }
	void AddPlayerLife(int number) { this->playerHealth += number; }
	void AddEnemyLife(int number) { this->enemyHealth += number; }
	void AddHeart(int number) { this->heart += number; }
	void AddPoint(int number) { this->point += point; }
	void AddStage(int number) { this->stage += number; }

	static CBoard * GetInstance();
};

