#pragma once

#include "gamemath.h"
#include "camera.h"
#include <SDL.h>
#include "bullet.h"

struct Ship
{
	Vector2f position;
	Vector2f gunPosition;
	Vector2f velocity;
	double rotation;
	float nextBulletTime;
	float fireRatio;
	float speed;
	bool alive;
};

Ship* createShip(float x, float y); 
void freeShip(Ship* ship);
void updateShip(Ship* ship, BulletManager* bulletManager, float dt);
void drawShip(Ship* ship, SDL_Renderer* renderer, Camera* camera);