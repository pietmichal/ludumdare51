#pragma once

#include <SDL.h>
#include "gamemath.h"
#include "bullet.h"
#include "camera.h"
#include "ship.h"

// OH THE MISERY

enum EnemyState
{
	THINK,
	FLY,
	SHOOT
};

enum EnemySecondaryState
{
	NEUTRAL,
	HIT
};

enum EnemyType
{
	DUMB,
	SMART,
	CRAZY
};

struct Enemy
{
	Vector2f position;
	Vector2f hitRadius;
	Vector2f direction;
	Vector2f velocity;
	EnemyState state;
	EnemySecondaryState secondaryState;
	EnemyType type;
	unsigned int life;
	float thinkTimer;
	float flyTimer;
	float hitTimer;
	double rotation;
	float speed;
	float lifeTime;
};

Enemy* createEnemy(float x, float y);
void freeEnemy(Enemy* enemy);
void updateEnemy(Enemy* enemy, BulletManager* bulletManager, Ship* ship, float dt);
void drawEnemy(Enemy* enemy, SDL_Renderer* renderer, Camera* camera);

const int MAX_ENEMIES = 999;

struct EnemyManager
{
	Enemy* enemies[MAX_ENEMIES];
	float timeUntilNextWave;
	unsigned int difficulty = 0;
};

EnemyManager* createEnemyManager();
void freeEnemyManager(EnemyManager* manager);

void updateEnemyManager(EnemyManager* manager, BulletManager* bulletManager, Ship* ship, float dt);
void drawAllEnemies(EnemyManager* manager, SDL_Renderer* renderer, Camera* camera);

void addEnemy(EnemyManager* manager, Enemy* enemy);
void removeEnemy(EnemyManager* manager, Enemy* enemy);
void enemyHit(Enemy* enemy);