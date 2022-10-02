#pragma once

#include "gamemath.h"
#include "camera.h"

#include <SDL.h>

const int MAX_STARS = 2000;
const int MAX_UNIVERSE_LAYERS = 10;

struct Universe {
	Vector2f stars[MAX_UNIVERSE_LAYERS][MAX_STARS];
	Vector2f size;
	Vector2f position;
};

Universe* createUniverse();

void updateUniverse(Universe* universe, float dt);

void drawUniverse(Universe* universe, SDL_Renderer* renderer, Camera* camera);

