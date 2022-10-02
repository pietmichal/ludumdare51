#pragma once

#include "gamemath.h"
#include "camera.h"
#include <SDL.h>

enum BulletType
{
	PLAYER,
	ENEMY
};

struct Bullet
{
	BulletType type;
	Vector2f position;
	float velocity;
	double rotation;
	float timeAliveLeft;
};


Bullet* createBullet(BulletType type, float x, float y, float velocity, double angle, float timeAlive);
void freeBullet(Bullet* bullet);

void updateBullet(Bullet* bullet, float dt);
void drawBullet(Bullet* bullet, SDL_Renderer* renderer, Camera* camera);


const int MAX_BULLETS = 10000;

struct BulletManager
{
	Bullet* bullets[MAX_BULLETS];
};

BulletManager* createBulletManager();
void freeBulletManager(BulletManager* manager);

void updateBulletManager(BulletManager* manager, float dt);
void drawAllBullets(BulletManager* manager, SDL_Renderer* renderer, Camera* camera);


void addBullet(BulletManager* manager, Bullet* bullet);
void removeBullet(BulletManager* manager, Bullet* bullet);