
#include "minimap.h"
#include "game.h"
#include "globals.h"
#include "assets.h"

#include <SDL.h>
#include <SDL_ttf.h>

float getHorizonDistance(GamePlayState* state)
{
	return fminf(
		fminf(
			fabsf(state->universe->position.x - state->ship->position.x),
			fabsf(state->universe->position.y - state->ship->position.y)
		),
		fminf(
			fabsf(state->universe->position.x + state->universe->size.x - state->ship->position.x),
			fabsf(state->universe->position.y + state->universe->size.x - state->ship->position.y)
		)
	);
}

void drawMinimap(SDL_Renderer* renderer, GamePlayState* state)
{
	SDL_Rect minimapOutline;
	minimapOutline.w = 100;
	minimapOutline.h = 100;
	minimapOutline.x = SCREEN_WIDTH - 100 - 10;
	minimapOutline.y = SCREEN_HEIGHT - 100 - 30;
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, &minimapOutline);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer, &minimapOutline);

	SDL_Rect minimapShip;
	minimapShip.w = 2;
	minimapShip.h = 2;
	minimapShip.x = SCREEN_WIDTH - 100 - 10 + 50 + fmin(state->ship->position.x * 0.002, 50);
	minimapShip.y = SCREEN_HEIGHT - 100 - 30 + 50 + fmin(state->ship->position.y * 0.002, 50);
	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
	SDL_RenderFillRect(renderer, &minimapShip);

	SDL_Rect minimapHorizon;
	minimapHorizon.w = fmin(state->universe->size.x * 0.002, 100);
	minimapHorizon.h = fmin(state->universe->size.y * 0.002, 100);
	minimapHorizon.x = SCREEN_WIDTH - 100 - 10 + 50 - minimapHorizon.w / 2;
	minimapHorizon.y = SCREEN_HEIGHT - 100 - 30 + 50 - minimapHorizon.h / 2;
	SDL_SetRenderDrawColor(renderer, 255, 0, 128, 255);
	SDL_RenderDrawRect(renderer, &minimapHorizon);

	char horizonDistanceBuffer[1024];
	SDL_snprintf(horizonDistanceBuffer, sizeof(horizonDistanceBuffer), "HORIZON: %i", (int)getHorizonDistance(state));
	SDL_Color color = { 255, 0, 255 };
	SDL_Surface* surface = TTF_RenderUTF8_Solid(assets.font8pt, horizonDistanceBuffer, color);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_Rect horizonDistanceRect;
	SDL_QueryTexture(texture, nullptr, nullptr, &horizonDistanceRect.w, &horizonDistanceRect.h);
	horizonDistanceRect.x = minimapOutline.x + minimapOutline.w / 2 - horizonDistanceRect.w / 2;
	horizonDistanceRect.y = minimapOutline.y + minimapOutline.h - horizonDistanceRect.h / 2 + 30 / 2;

	SDL_RenderCopy(renderer, texture, nullptr, &horizonDistanceRect);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);


	char velocityBuffer[1024];
	SDL_snprintf(velocityBuffer, sizeof(velocityBuffer), "SPEED:%iu/s", (int)state->ship->speed);
	SDL_Color velocityColor = { 255, 255, 255 };
	SDL_Surface* velocitySurface = TTF_RenderUTF8_Solid(assets.font8pt, velocityBuffer, velocityColor);
	SDL_Texture* velocityTexture = SDL_CreateTextureFromSurface(renderer, velocitySurface);

	SDL_Rect velocityRect;
	SDL_QueryTexture(velocityTexture, nullptr, nullptr, &velocityRect.w, &velocityRect.h);
	velocityRect.x = minimapOutline.x + minimapOutline.w / 2 - velocityRect.w / 2;
	velocityRect.y = minimapOutline.y - velocityRect.h - 30 / 2;

	SDL_RenderCopy(renderer, velocityTexture, nullptr, &velocityRect);
	SDL_DestroyTexture(velocityTexture);
	SDL_FreeSurface(velocitySurface);
}
