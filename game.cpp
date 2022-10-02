#include "game.h"
#include "ship.h"
#include "globals.h"

#include <SDL.h>
#include <assert.h>
#include <malloc.h>

Game* createGame()
{
	Game* game = (Game*)malloc(sizeof(Game));
	assert(game);
	game->data = malloc(0);
	setGamePlayState(game);
	return game;
}

void setGameIntroState(Game* game)
{
	game->state = INTRO;
	GameIntroState* data = (GameIntroState*)realloc(game->data, sizeof(GameIntroState));
	assert(data);
	game->data = data;
}


void setGamePlayState(Game* game)
{
	game->state = PLAY;
	GamePlayState* data = (GamePlayState*)realloc(game->data, sizeof(GamePlayState));
	assert(data);
	game->data = data;
	data->ship = createShip(0, 0);
	data->camera = createCamera();
	data->universe = createUniverse();
	data->bulletManager = createBulletManager();
	data->enemyManager = createEnemyManager();
	data->status = BEGINNING;
	data->statusTimer = 2.0f;
	data->camera->zoom = 0.01f;
	data->timeSpent = 0.00f;
}

void setGameEndState(Game* game)
{
	game->state = END;
	void* ptr = realloc(game->data, sizeof(GameEndState));
	assert(ptr);
	game->data = ptr;
}