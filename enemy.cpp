#include "enemy.h"

#include <malloc.h>
#include <assert.h>
#include <SDL.h>

#include "assets.h"
#include "camera.h"
#include "globals.h"
#include "bullet.h"
#include "ship.h"
#include <stdlib.h>

EnemyType getEnemyType()
{
	int decision = random(0, 100);

	if (decision <= 10)
	{
		return DUMB;
	}

	if (decision > 10 && decision < 90)
	{
		return SMART;
	}

	if (decision >= 90)
	{
		return CRAZY;
	}

}

Enemy* createEnemy(float x = 0, float y = 0)
{
	Enemy* enemy = (Enemy*)malloc(sizeof(Enemy));

	assert(enemy);

	enemy->position.x = x;
	enemy->position.y = y;

	enemy->velocity.x = 0;
	enemy->velocity.y = 0;

	enemy->direction.x = 0;
	enemy->direction.y = 0;

	enemy->rotation = 0;
	enemy->speed = 100.0f;

	enemy->life = 3;

	enemy->type = getEnemyType();

	enemy->state = THINK;
	enemy->secondaryState = NEUTRAL;

	enemy->hitTimer = 0.0f;

	enemy->lifeTime = 20.0f;

	return enemy;
}

void freeEnemy(Enemy* enemy)
{
	free(enemy);
}

void updateEnemy(Enemy* enemy, BulletManager* bulletManager, Ship* ship, float dt)
{
	enemy->lifeTime -= dt;

	switch (enemy->state)
	{

	case THINK: {
		enemy->thinkTimer -= dt;
		if (enemy->thinkTimer > 0)
		{
			break;
		}

		if (enemy->type == DUMB) 
		{

			int decision = random(0, 100);

			if (decision < 90)
			{
				enemy->state = FLY;
				enemy->speed = random(0, 20);
				enemy->direction.x = random(-1, 1);
				enemy->direction.y = random(-1, 1);
				enemy->flyTimer = 5.0f;
			}

			if (decision >= 90)
			{
				enemy->state = SHOOT;
			}
		}

		if (enemy->type == SMART)
		{

			int decision = random(0, 100);

			if (decision < 75)
			{
				enemy->state = FLY;
				enemy->speed = random(0, 150);
				enemy->direction.x = random(-1, 1);
				enemy->direction.y = random(-1, 1);
				enemy->flyTimer = random(1, 3) * 0.2f;
			}

			if (decision >= 75)
			{
				enemy->state = SHOOT;
			}
		}

		if (enemy->type == CRAZY)
		{

			int decision = random(0, 100);

			if (decision < 10)
			{
				enemy->state = FLY;
				enemy->speed = random(300, 600);
				enemy->direction.x = random(-1, 1);
				enemy->direction.y = random(-1, 1);
				enemy->flyTimer = 1.0f;
			}

			if (decision >= 10)
			{
				enemy->state = SHOOT;
			}
		}

		break;
	}

	case FLY: {
		enemy->flyTimer -= dt;
		if (enemy->flyTimer <= 0)
		{
			enemy->state = THINK;

			if (enemy->type == DUMB) {
				enemy->thinkTimer = 5.0f;
			}

			if (enemy->type == SMART) {
				enemy->thinkTimer = 1.0f;
			}

			if (enemy->type == CRAZY) {
				enemy->thinkTimer = random(0, 1000) / 1000;
			}

			break;
		}

		enemy->position += enemy->direction * enemy->speed * dt;
		break;
	}

	case SHOOT: {

		double bulletAngle = 90.0;

		if (enemy->type == DUMB)
		{
			bulletAngle = random(0, 360);
		}

		if (enemy->type == SMART)
		{
			bulletAngle = atan2(enemy->position.y - ship->position.y, enemy->position.x - ship->position.x) * 180 / PI - 90;
		}

		if (enemy->type == CRAZY)
		{
			bulletAngle = atan2(enemy->position.y - ship->position.y, enemy->position.x - ship->position.x) * 180 / PI - 90 + random(-90, 90);
		}

		addBullet(bulletManager, createBullet(ENEMY, enemy->position.x, enemy->position.y, 100.0f, bulletAngle, 20.0f));

		int decision = random(0, 100);

		if (decision <= 30)
		{
			enemy->state = SHOOT;
		}

		if (decision > 30)
		{
			enemy->state = THINK;
		}

		break;
	}
	
	}

	switch (enemy->secondaryState) 
	{

	case NEUTRAL: 
	{
		break;
	}

	case HIT:
	{
		enemy->hitTimer -= dt;
		if (enemy->hitTimer <= 0)
		{
			enemy->secondaryState = NEUTRAL;
		}
		break;
	}

	}
}

void drawEnemy(Enemy* enemy, SDL_Renderer* renderer, Camera* camera)
{
	Vector2f relativePosition = getRelativePositionToCamera(camera, &enemy->position);
	Vector2f enemySize = { 50, 50 };
	Vector2f relativeEnemySize = getRelativeSizeToCamera(camera, &enemySize);

	SDL_Rect quad;
	quad.x = (int)(relativePosition.x - relativeEnemySize.x / 2);
	quad.y = (int)(relativePosition.y - relativeEnemySize.y / 2);
	quad.w = relativeEnemySize.x;
	quad.h = relativeEnemySize.y;

	SDL_Point p;
	p.x = relativeEnemySize.x / 2;
	p.y = relativeEnemySize.y / 2;

	if (enemy->secondaryState == HIT)
	{
		SDL_RenderCopyEx(renderer, assets.enemyHit, nullptr, &quad, enemy->rotation, &p, SDL_FLIP_NONE);
	}
	else {
		SDL_RenderCopyEx(renderer, assets.enemy, nullptr, &quad, enemy->rotation, &p, SDL_FLIP_NONE);
	}
}

EnemyManager* createEnemyManager()
{
	EnemyManager* enemyManager = (EnemyManager*)malloc(sizeof(EnemyManager));
	assert(enemyManager);

	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		enemyManager->enemies[i] = nullptr;
	}

	enemyManager->timeUntilNextWave = 3.0f;

	return enemyManager;
}

void freeEnemyManager(EnemyManager* manager)
{
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		if (manager->enemies[i] != nullptr) {
			freeEnemy(manager->enemies[i]);
		}
	}
	free(manager);
}

void addEnemy(EnemyManager* manager, Enemy* enemy)
{
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		if (manager->enemies[i] == nullptr)
		{
			manager->enemies[i] = enemy;
			return;
		}
	}
}

void removeEnemy(EnemyManager* manager, Enemy* enemy)
{
	freeEnemy(enemy);

	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		if (manager->enemies[i] == enemy)
		{
			manager->enemies[i] = nullptr;
			break;
		}
	}
}

Vector2f getRandomEnemyPosition(Ship* ship)
{
	Vector2f v;

	int chance = random(0, 100);

	if (chance < 25)
	{
		v.y = ship->position.y - 700;
		v.x = random(ship->position.x - 700, ship->position.x + 700);
	}

	if (chance >= 25 && chance < 50)
	{
		v.y = ship->position.y + 700;
		v.x = random(ship->position.x - 700, ship->position.x + 700);
	}

	if (chance >= 50 && chance < 75)
	{
		v.x = ship->position.x - 700;
		v.y = random(ship->position.y - 700, ship->position.y + 700);
	}

	if (chance >= 75)
	{
		v.x = ship->position.x + 700;
		v.y = random(ship->position.y - 700, ship->position.y + 700);
	}

	return v;
}

void updateEnemyManager(EnemyManager* manager, BulletManager* bulletManager, Ship* ship, float dt)
{

	manager->timeUntilNextWave -= dt;

	if (manager->timeUntilNextWave <= 0)
	{
		manager->timeUntilNextWave += 10.0f;

		for (int i = 0; i < 15; i++)
		{
			Vector2f v = getRandomEnemyPosition(ship);
			addEnemy(manager, createEnemy(v.x, v.y));
		}
	}

	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		if (manager->enemies[i] != nullptr)
		{
			if (manager->enemies[i]->life <= 0 || manager->enemies[i]->lifeTime <= 0)
			{
				removeEnemy(manager, manager->enemies[i]);
			}
			else {
				updateEnemy(manager->enemies[i], bulletManager, ship, dt);
			}
		}
	}
}

void drawAllEnemies(EnemyManager* manager, SDL_Renderer* renderer, Camera* camera)
{
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		if (manager->enemies[i] != nullptr)
		{
			drawEnemy(manager->enemies[i], renderer, camera);
		}
	}
}

void enemyHit(Enemy* enemy)
{
	enemy->life -= 1;
	enemy->secondaryState = HIT;
	enemy->hitTimer = 1.f / 30;
}
