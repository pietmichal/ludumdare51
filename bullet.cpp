#include "bullet.h"
#include <stdlib.h>
#include <assert.h>
#include <SDL.h>
#include "camera.h"
#include "assets.h"

Bullet* createBullet(BulletType type, float x, float y, float velocity, double angle, float timeAlive)
{
	Bullet* bullet = (Bullet*)malloc(sizeof(Bullet));
	assert(bullet);

	bullet->type = type;

	bullet->position.x = x;
	bullet->position.y = y;

	bullet->velocity = velocity;

	bullet->rotation = angle;

	bullet->timeAliveLeft = timeAlive;

	return bullet;
}

void freeBullet(Bullet* bullet)
{
	free(bullet);
}

void updateBullet(Bullet* bullet, float dt)
{
	bullet->position.x += sinf(degToRad(bullet->rotation)) * bullet->velocity * dt;
	bullet->position.y -= cosf(degToRad(bullet->rotation)) * bullet->velocity * dt;
}

void drawBullet(Bullet* bullet, SDL_Renderer* renderer, Camera* camera)
{
	if (bullet->type == PLAYER)
	{

		Vector2f relativePosition = getRelativePositionToCamera(camera, &bullet->position);
		Vector2f size = { 15, 30 };
		Vector2f relativeSize = getRelativeSizeToCamera(camera, &size);

		SDL_Rect quad;
		quad.x = (int)(relativePosition.x - relativeSize.x / 2);
		quad.y = (int)(relativePosition.y - relativeSize.y / 2);
		quad.w = relativeSize.x;
		quad.h = relativeSize.y;

		SDL_Point p;
		p.x = relativeSize.x / 2;
		p.y = relativeSize.y / 2;

		SDL_RenderCopyEx(renderer, assets.bullet, nullptr, &quad, bullet->rotation, &p, SDL_FLIP_NONE);
	}

	if (bullet->type == ENEMY)
	{

		Vector2f relativePosition = getRelativePositionToCamera(camera, &bullet->position);
		Vector2f size = { 15, 15 };
		Vector2f relativeSize = getRelativeSizeToCamera(camera, &size);

		SDL_Rect quad;
		quad.x = (int)(relativePosition.x - relativeSize.x / 2);
		quad.y = (int)(relativePosition.y - relativeSize.y / 2);
		quad.w = relativeSize.x;
		quad.h = relativeSize.y;

		SDL_Point p;
		p.x = relativeSize.x / 2;
		p.y = relativeSize.y / 2;

		SDL_RenderCopyEx(renderer, assets.bullet2, nullptr, &quad, bullet->rotation, &p, SDL_FLIP_NONE);
	}
}

BulletManager* createBulletManager()
{
	BulletManager* bulletManager = (BulletManager*)malloc(sizeof(BulletManager));
	assert(bulletManager);

	for (int i = 0; i < MAX_BULLETS; i++)
	{
		bulletManager->bullets[i] = nullptr;
	}

	return bulletManager;
}

void freeBulletManager(BulletManager* manager)
{
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (manager->bullets[i] != nullptr) {
			freeBullet(manager->bullets[i]);
		}
	}
	free(manager);
}

void addBullet(BulletManager* manager, Bullet* bullet)
{
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (manager->bullets[i] == nullptr)
		{
			manager->bullets[i] = bullet;
			return;
		}
	}
}

void removeBullet(BulletManager* manager, Bullet* bullet)
{
	freeBullet(bullet);

	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (manager->bullets[i] == bullet)
		{
			manager->bullets[i] = nullptr;
			break;
		}
	}
}

void updateBulletManager(BulletManager* manager, float dt)
{
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (manager->bullets[i] != nullptr)
		{
			manager->bullets[i]->timeAliveLeft -= dt;
			if (manager->bullets[i]->timeAliveLeft <= 0)
			{
				removeBullet(manager, manager->bullets[i]);
			}
			else {
				updateBullet(manager->bullets[i], dt);
			}
		}
	}
}

void drawAllBullets(BulletManager* manager, SDL_Renderer* renderer, Camera* camera)
{
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (manager->bullets[i] != nullptr)
		{
			drawBullet(manager->bullets[i], renderer, camera);
		}
	}
}
