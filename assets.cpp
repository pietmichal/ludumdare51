#include "assets.h"
#include <SDL_image.h>
#include <SDL_ttf.h>

Assets assets;

SDL_Texture* loadTexture(SDL_Renderer* renderer, const char path[])
{
	SDL_Surface* loadedSurface = IMG_Load(path);

	if (loadedSurface == nullptr)
	{
		SDL_Log("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
		return nullptr;
	}

	SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
	if (newTexture == nullptr)
	{
		SDL_Log("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
		return nullptr;
	}

	SDL_FreeSurface(loadedSurface);

	return newTexture;
}


void loadAssets(SDL_Renderer* renderer)
{
	assets.ship = loadTexture(renderer, "ship.png");
	assets.bullet = loadTexture(renderer, "bullet.png");
	assets.bullet2 = loadTexture(renderer, "bullet2.png");
	assets.enemy = loadTexture(renderer, "enemy.png");
	assets.enemyHit = loadTexture(renderer, "enemyHit.png");
	assets.font8pt = TTF_OpenFont("PressStart2P-vaV7.ttf", 8);
	assets.mediumFont = TTF_OpenFont("PressStart2P-vaV7.ttf", 12);
	assets.largeFont = TTF_OpenFont("PressStart2P-vaV7.ttf", 24);
}