#include <malloc.h>
#include <assert.h>
#include <SDL.h>

#include "ship.h"
#include "assets.h"
#include "camera.h"
#include "globals.h"
#include "bullet.h"

Ship* createShip(float x = 0, float y = 0)
{
	Ship* ship = (Ship*)malloc(sizeof(Ship));

	assert(ship);

	ship->position.x = x;
	ship->position.y = y;

	ship->gunPosition.x = x;
	ship->gunPosition.y = y - 40;

	ship->velocity.x = 0;
	ship->velocity.y = 0;

	ship->rotation = 0;
	ship->speed = 350.0f;

	ship->nextBulletTime = 0.0f;
	ship->fireRatio = 0.028f;

	ship->alive = true;

	return ship;
}

void freeShip(Ship* ship)
{
	free(ship);
}

void updateShip(Ship* ship, BulletManager* bulletManager, float dt)
{
	if (!ship->alive)
	{
		ship->rotation += 360.0f * dt;
		return;
	}

	const Uint8* kbdState = SDL_GetKeyboardState(NULL);

	// Movement
	if (kbdState[SDL_SCANCODE_Z])
	{
		ship->rotation -= 360 * dt;
		
		if (ship->rotation < 0)
		{
			ship->rotation = 360;
		}
	}

	if (kbdState[SDL_SCANCODE_X])
	{
		ship->rotation += 360 * dt;

		if (ship->rotation > 360)
		{
			ship->rotation = 0;
		}
	}

	Vector2f direction = { 0,0 };

	if (kbdState[SDL_SCANCODE_RIGHT])
	{
		direction.x += 1;
	}

	if (kbdState[SDL_SCANCODE_LEFT])
	{
		direction.x -= 1;
	}

	if (kbdState[SDL_SCANCODE_UP])
	{
		direction.y -= 1;
	}

	if (kbdState[SDL_SCANCODE_DOWN])
	{
		direction.y += 1;
	}

	ship->velocity = direction * ship->speed * dt;

	if (ship->velocity.x != 0 && ship->velocity.y != 0)
	{
		ship->velocity /= SDL_sqrtf(2);
	}

	ship->position += ship->velocity;

	ship->gunPosition.x = ship->position.x;
	ship->gunPosition.y = ship->position.y - 40;

	rotateVectorAroundVector(&ship->gunPosition, &ship->position, degToRad(ship->rotation));

	// Shooting
	if (kbdState[SDL_SCANCODE_SPACE])
	{
		if (ship->nextBulletTime <= 0) {
			addBullet(bulletManager, createBullet(PLAYER, ship->gunPosition.x, ship->gunPosition.y, 600.0f, ship->rotation + (ship->fireRatio > 0.03f ? random(-5, 5) : random(-10, 10)), 5.0f));
			ship->nextBulletTime = ship->fireRatio;
		}
	}

	ship->nextBulletTime -= dt;
}

void drawShip(Ship* ship, SDL_Renderer* renderer, Camera* camera)
{
	Vector2f relativePosition = getRelativePositionToCamera(camera, &ship->position);
	Vector2f shipSize = { 50, 50 };
	Vector2f relativeShipSize = getRelativeSizeToCamera(camera, &shipSize);

	SDL_Rect quad;
	quad.x = (int)(relativePosition.x - relativeShipSize.x/2);
	quad.y = (int)(relativePosition.y - relativeShipSize.y/2);
	quad.w = relativeShipSize.x;
	quad.h = relativeShipSize.y;

	SDL_Point p;
	p.x = relativeShipSize.x/2;
	p.y = relativeShipSize.y/2;

	SDL_RenderCopyEx(renderer, assets.ship, nullptr, &quad, ship->rotation, &p, SDL_FLIP_NONE);


	if (DEBUG) 
	{
		Vector2f relativeGunPosition = getRelativePositionToCamera(camera, &ship->gunPosition);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_Rect gunRect;
		gunRect.x = relativeGunPosition.x;
		gunRect.y = relativeGunPosition.y;
		gunRect.w = 3;
		gunRect.h = 3;
		SDL_RenderFillRect(renderer, &gunRect);
	}
}
