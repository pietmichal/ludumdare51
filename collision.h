#pragma once
#include "ship.h"
#include "enemy.h"
#include "bullet.h"

const int PLAYER_BULLET_RADIUS = 15;
const int ENEMY_BULLET_RADIUS = 15;
const int ENEMY_SHIP_RADIUS = 30;

void resolveCollisions(BulletManager* bulletManager, EnemyManager* enemyManager, Ship* ship);