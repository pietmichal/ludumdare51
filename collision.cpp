#include "collision.h"
#include "gamemath.h"
#include "enemy.h"
#include "globals.h"

void resolveCollisions(BulletManager* bulletManager, EnemyManager* enemyManager, Ship* ship)
{
	for (int i = 0; i < MAX_BULLETS; i++)
	{
		if (bulletManager->bullets[i] == nullptr)
		{
			continue;
		}

		// player bullet hitting enemies
		if (bulletManager->bullets[i]->type == PLAYER)
		{
			for (int j = 0; j < MAX_ENEMIES; j++)
			{
				if (enemyManager->enemies[j] == nullptr)
				{
					continue;
				}

				if (isPointInCircle(enemyManager->enemies[j]->position, bulletManager->bullets[i]->position, PLAYER_BULLET_RADIUS))
				{
					removeBullet(bulletManager, bulletManager->bullets[i]);
					enemyHit(enemyManager->enemies[j]);

					if (enemyManager->enemies[j]->life <= 0)
					{
						ship->speed += 45.0f;


					}

					framesToFreeze = 0;
					break;
				}

			}
		}
		// enemy bullet hitting player
		else if (bulletManager->bullets[i]->type == ENEMY)
		{
			if (isPointInCircle(ship->position, bulletManager->bullets[i]->position, ENEMY_BULLET_RADIUS))
			{
				ship->alive = false;
				break;
			}
		}
	}


	// player colliding with enemy
	for (int i = 0; i < MAX_ENEMIES; i++)
	{
		if (enemyManager->enemies[i] == nullptr)
		{
			continue;
		}

		if (isPointInCircle(ship->position, enemyManager->enemies[i]->position, ENEMY_SHIP_RADIUS))
		{
			ship->alive = false;
			break;
		}

	}
	
}
