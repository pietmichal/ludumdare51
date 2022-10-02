#pragma once

#include "ship.h"
#include "camera.h"
#include "universe.h"
#include "bullet.h"
#include "enemy.h"

enum GameState
{
	INTRO,
	PLAY,
	END
};

struct GameIntroState
{
};

enum GamePlayStateStatus
{
	BEGINNING,
	INPROGRESS,
	LOSE,
	WIN
};

struct GamePlayState
{
	Camera* camera;
	Universe* universe;
	Ship* ship;
	BulletManager* bulletManager;
	EnemyManager* enemyManager;

	GamePlayStateStatus status;
	float statusTimer;

	float timeSpent;
};

struct GameEndState
{
	
};

struct Game
{
	GameState state;
	void* data;
};

Game* createGame();
void setGameIntroState(Game* game);
void setGamePlayState(Game* game);
void setGameEndState(Game* game);