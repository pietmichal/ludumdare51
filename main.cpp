#include <SDL.h>
#include <malloc.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "globals.h"
#include "game.h"
#include "assets.h"
#include "minimap.h"
#include "collision.h"

bool running = true;


int main(int argc, char* args[])
{
	srand(time(nullptr));

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}

	SDL_Window* window = SDL_CreateWindow("Chasing Horizon - Ludum Dare 51", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (!window)
	{
		SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}

	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		SDL_Log("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return -1;
	}

	if (TTF_Init() == -1) {
		SDL_Log("TTF_Init: %s\n", TTF_GetError());
		return -1;
	}

	SDL_Surface* surface = SDL_GetWindowSurface(window);

	if (!surface)
	{
		SDL_Log("Surface could not be created! SDL_Error: %s\n", SDL_GetError());
		return -1;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr)
	{
		SDL_Log("Could not create a renderer: %s", SDL_GetError());
		return -1;
	}

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	loadAssets(renderer);

	SDL_UpdateWindowSurface(window);

	Uint64 previousFrameStartTime = SDL_GetPerformanceCounter();

	Game* game = createGame();
	setGameIntroState(game);

	while (running)
	{
		Uint64 frameStartTime = SDL_GetPerformanceCounter();
		float timeElapsedSinceLastFrame = (frameStartTime - previousFrameStartTime) / (float)SDL_GetPerformanceFrequency();
		float timeElapsedSinceLastFrameInMs = timeElapsedSinceLastFrame * 1000.f;

		if (timeElapsedSinceLastFrameInMs < 16.666f)
		{
			continue;
		}

		previousFrameStartTime = frameStartTime;

		// SDL_Log("FPS: %f", SDL_round(1.0f / timeElapsedSinceLastFrame));

		float dt = timeElapsedSinceLastFrame;

		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT: {
				running = false;
				break;
			}
			}
		}

		if (!running)
		{
			break;
		}

		if (framesToFreeze > 0)
		{
			framesToFreeze--;
			continue;
		}

		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		SDL_RenderClear(renderer);

		switch (game->state)
		{

		case INTRO: {

			const Uint8* kbdState = SDL_GetKeyboardState(NULL);

			// Movement
			if (kbdState[SDL_SCANCODE_SPACE])
			{
				setGamePlayState(game);
			}

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);

			SDL_Color color = { 255, 255, 255 };
			SDL_Surface* surface = TTF_RenderUTF8_Solid(assets.largeFont, "CHASING HORIZON", color);
			SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

			SDL_Rect titleRect;
			SDL_QueryTexture(texture, nullptr, nullptr, &titleRect.w, &titleRect.h);
			titleRect.x = SCREEN_WIDTH / 2 - titleRect.w / 2;
			titleRect.y = SCREEN_HEIGHT / 2 - titleRect.h / 2;

			SDL_RenderCopy(renderer, texture, nullptr, &titleRect);
			SDL_DestroyTexture(texture);
			SDL_FreeSurface(surface);

			SDL_Color ctaColor = { 255, 255, 255 };
			SDL_Surface* ctaSurface = TTF_RenderUTF8_Solid(assets.mediumFont, "PRESS SPACE TO START", ctaColor);
			SDL_Texture* ctaTexture = SDL_CreateTextureFromSurface(renderer, ctaSurface);

			SDL_Rect ctaRect;
			SDL_QueryTexture(ctaTexture, nullptr, nullptr, &ctaRect.w, &ctaRect.h);
			ctaRect.x = SCREEN_WIDTH / 2 - ctaRect.w / 2;
			ctaRect.y = SCREEN_HEIGHT / 2 - ctaRect.h / 2 + 75;

			SDL_RenderCopy(renderer, ctaTexture, nullptr, &ctaRect);
			SDL_DestroyTexture(ctaTexture);
			SDL_FreeSurface(ctaSurface);

			SDL_Surface* tipSurface = TTF_RenderUTF8_Solid(assets.largeFont, "CHASING HORIZON", color);
			SDL_Texture* tipTexture = SDL_CreateTextureFromSurface(renderer, tipSurface);

			SDL_Rect tipRect;
			SDL_QueryTexture(texture, nullptr, nullptr, &tipRect.w, &tipRect.h);
			tipRect.x = SCREEN_WIDTH / 2 - tipRect.w / 2;
			tipRect.y = SCREEN_HEIGHT / 2 - tipRect.h / 2;

			SDL_RenderCopy(renderer, tipTexture, nullptr, &tipRect);
			SDL_DestroyTexture(tipTexture);
			SDL_FreeSurface(tipSurface);

			break;
		}

		case PLAY:
		{
			GamePlayState* state = (GamePlayState*)game->data;

			switch (state->status)
			{

			case BEGINNING: {

				drawUniverse(state->universe, renderer, state->camera);

				drawShip(state->ship, renderer, state->camera);

				state->camera->zoom += 0.3f * dt;
				if (state->camera->zoom >= 1.0f)
				{
					state->camera->zoom = 1.0f;
					state->status = INPROGRESS;
				}

				break;
			}

			case INPROGRESS: {

				state->timeSpent += dt;

				updateCamera(state->camera, dt);

				updateUniverse(state->universe, dt);

				updateShip(state->ship, state->bulletManager, dt);

				cameraLookAt(state->camera, state->ship->position);

				updateBulletManager(state->bulletManager, dt);

				updateEnemyManager(state->enemyManager, state->bulletManager, state->ship, dt);

				resolveCollisions(state->bulletManager, state->enemyManager, state->ship);

				drawUniverse(state->universe, renderer, state->camera);

				drawShip(state->ship, renderer, state->camera);

				drawAllBullets(state->bulletManager, renderer, state->camera);

				drawAllEnemies(state->enemyManager, renderer, state->camera);

				drawMinimap(renderer, state);

				if (!state->ship->alive)
				{
					state->status = LOSE;
					state->statusTimer = 2.0f;
				}		

				if (
					state->ship->position.x < state->universe->position.x ||
					state->ship->position.y < state->universe->position.y ||
					state->ship->position.x > state->universe->position.x + state->universe->size.x ||
					state->ship->position.y > state->universe->position.y + state->universe->size.y 
				) {
					state->status = WIN;
					state->statusTimer = 5.0f;
				}

				break;
			}

			case LOSE: {
				updateShip(state->ship, state->bulletManager, dt);

				cameraLookAt(state->camera, state->ship->position);

				drawUniverse(state->universe, renderer, state->camera);

				drawShip(state->ship, renderer, state->camera);

				drawAllEnemies(state->enemyManager, renderer, state->camera);

				drawAllBullets(state->bulletManager, renderer, state->camera);

				state->statusTimer -= dt;

				if (state->statusTimer <= 0)
				{
					setGameIntroState(game);
				}

				break;
			}

			case WIN: {

				state->universe->size.x -= 148 * 12 * dt;
				state->universe->size.y -= 148 * 12 * dt;

				state->ship->rotation -= 30 * dt;

				updateUniverse(state->universe, dt);

				cameraLookAt(state->camera, state->ship->position);

				drawUniverse(state->universe, renderer, state->camera);

				drawShip(state->ship, renderer, state->camera);

				char horizonDistanceBuffer[1024];
				SDL_snprintf(horizonDistanceBuffer, sizeof(horizonDistanceBuffer), "TIME: %f minutes", state->timeSpent / 60.0f);
				SDL_Color color = { 0, 0, 0 };
				SDL_Surface* surface = TTF_RenderUTF8_Solid(assets.largeFont, horizonDistanceBuffer, color);
				SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

				SDL_Rect horizonDistanceRect;
				SDL_QueryTexture(texture, nullptr, nullptr, &horizonDistanceRect.w, &horizonDistanceRect.h);
				horizonDistanceRect.x = SCREEN_WIDTH / 2 - horizonDistanceRect.w / 2;
				horizonDistanceRect.y = SCREEN_HEIGHT / 2 - horizonDistanceRect.h / 2 - 200;

				SDL_RenderCopy(renderer, texture, nullptr, &horizonDistanceRect);
				SDL_DestroyTexture(texture);
				SDL_FreeSurface(surface);

				state->statusTimer -= dt;

				if (state->statusTimer <= 0)
				{
					setGameIntroState(game);
				}

				break;
			}

			}


			break;
		}

		case END:
		{
			GameEndState* state = (GameEndState*)game->data;

			running = false;

			break;
		}

		}

		SDL_RenderPresent(renderer);
	}

	SDL_DestroyWindow(window);

	SDL_Quit();

	return 0;
}


// todo: polish
// // - sfx
// - friction based velocity
// - speed shake
// - ambient msc