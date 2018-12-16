#pragma once
#include "GameObject.h"
#include "Camera.h"

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
#define HEART_POSITION_X	338
#define HEART_POSITION_Y	32

#define LIFE_POINT_DEFAULT		3
#define LIFE_POINT_DIGITS		2
#define LIFE_POINT_POSITION_X	338
#define LIFE_POINT_POSITION_Y	48

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
#define LIFE_POINT_TEXT					"P-"
#define HEART_TEXT						" -"
#define STAGE_TEXT						"STAGE "
#define PLAYER_LIFE_TEXT				"PLAYER "
#define ENEMY_LIFE_TEXT					"ENEMY  "

#define ITEM_BOX_POSITION_X		256
#define ITEM_BOX_POSITION_Y		32
#define ITEM_BOX_WIDTH			64
#define ITEM_BOX_HEIGHT			44

#define HEALTH_UNIT_WIDTH		8
#define HEALTH_UNIT_HEIGHT		14

#define HEALTH_PAUSE_TIME		100			// The time health bar take to pause after changing


enum class BoardAniID
{
	PLAYER_HEALTH_UNIT = 9701,
	ENEMY_HEALTH_UNIT,
	EMPTY_UNIT_HEALTH,
	ITEM_BOX,
	HEART
};



class CBoard
{
	int score;
	int time;
	int stage;
	int playerHealth;
	int enemyHealth;
	int heart;
	int lifePoint;			// number of Simon's life left

	int playerHealthTemp = 0;
	int enemyHealthTemp = 0;

	// Timers, also be used as FLAG for the action when assigned to TIMER_IDLE(false) or non-TIMER_IDLE(true)
	// Assigned to TIMER_IDLE			: the timer is not working,
	// Assigned to TIMER_ETERNAL		: the timer is working and the action is not going to stop
	// Assigned to unsigned number		: the timer is working
	DWORD player_health_pause_start = TIMER_IDLE;

	static CBoard * __instance;
	static CGame * gameInstance;
	static CCamera * cameraInstance;
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

	int GetLifePoint() { return this->lifePoint; }
	void SetLifePoint(int point) { this->lifePoint = point; }

	int GetPlayerLife() { return this->playerHealth; }
	void SetPlayerLife(int playerLife) { this->playerHealth = playerLife; }

	int GetEnemyLife() { return this->enemyHealth; }
	void SetEnemyLife(int enemyLife) { this->enemyHealth = enemyLife; }

#pragma endregion

	void Render();
	void Update();

	void RenderScore();
	void RenderTime();
	void RenderStage();
	void RenderPlayerHealth();
	void RenderEnemyLife();
	void RenderHeart();
	void RenderPoint();
	void RenderWeaponBox();

	// Also use these functions for subtraction
	void AddScore(int number) { this->score += number; }
	void AddTime(int number) { this->time += number; }
	void AddHeart(int number) { this->heart += number; }
	void AddLifePoint(int number) { this->lifePoint += number; }
	void AddStage(int number) { this->stage += number; }
	void AddEnemyLife(int number) { this->enemyHealth += number; }
	void AddPlayerLife(int number) { this->playerHealthTemp += number; }

	void ChangePlayerHealthGradually();
	void CalcWeaponInsidePosition(float weaponWidth, float weaponHeight, float &x, float &y);

	static CBoard * GetInstance();
};

