#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

struct Assets {
	SDL_Texture* ship;
	SDL_Texture* bullet;
	SDL_Texture* enemy;
	SDL_Texture* enemyHit;
	SDL_Texture* bullet2;
	TTF_Font* font8pt;
	TTF_Font* mediumFont;
	TTF_Font* largeFont;
};

extern Assets assets;

void loadAssets(SDL_Renderer* renderer);