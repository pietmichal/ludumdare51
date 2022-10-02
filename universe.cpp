#include "universe.h"
#include <assert.h>
#include <stdlib.h>

Universe* createUniverse()
{
	Universe* universe = (Universe*)malloc(sizeof(Universe));
	assert(universe);

	for (int i = 0; i < MAX_UNIVERSE_LAYERS; i++)
	{
		for (int j = 0; j < MAX_STARS; j++)
		{
			universe->stars[i][j] = { (float)(rand() % 20000 - 10000), (float)(rand() % 20000 - 10000) };
		}
	}

	universe->size.x = 5000;
	universe->size.y = 5000;
	universe->position.x = -universe->size.x / 2;
	universe->position.y = -universe->size.y / 2;

	return universe;
}

void updateUniverse(Universe* universe, float dt)
{
	universe->size.x += 148.0f * 10 * dt;
	universe->size.y += 148.0f * 10 * dt;

	Vector2f universeShake = { rand() % 5, rand() % 5 };
	universe->position = { universeShake.x - universe->size.x / 2, universeShake.y - universe->size.y / 2 };
}

void drawUniverse(Universe* universe, SDL_Renderer* renderer, Camera* camera)
{

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	Vector2f universePositionRelativeToCamera = getRelativePositionToCamera(camera, &universe->position);
	Vector2f relativeUniverseSize = getRelativeSizeToCamera(camera, &universe->size);

	SDL_Rect r;
	r.w = relativeUniverseSize.x;
	r.h = relativeUniverseSize.y;
	r.x = (int) universePositionRelativeToCamera.x;
	r.y = (int) universePositionRelativeToCamera.y;

	SDL_RenderFillRect(renderer, &r);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);

	for (int i = 0; i < MAX_UNIVERSE_LAYERS; i++)
	{

		Vector2f starSize = { 6 - i, 6 - i };
		Vector2f relativeStarSize = getRelativeSizeToCamera(camera, &starSize);

		SDL_Rect r;
		r.w = (int)relativeStarSize.x;
		r.h = (int)relativeStarSize.y;

		for (int j = 0; j < MAX_STARS; j++)
		{
			Vector2f relativePosition = getRelativePositionToCamera(camera, &universe->stars[i][j]);
			Vector2f relativeDifference = universe->stars[i][j] - relativePosition;

			r.x = universe->stars[i][j].x - (relativeDifference.x * (0.2f - 0.015f * i));
			r.y = universe->stars[i][j].y - (relativeDifference.y * (0.2f - 0.015f * i));

			SDL_RenderFillRect(renderer, &r);
		}
	}
}
