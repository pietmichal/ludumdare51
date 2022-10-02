#pragma once

#include <SDL.h>
#include "gamemath.h"


struct Camera {
	SDL_Renderer* renderer;
	Vector2f position;
	float zoom;
};

Camera* createCamera();

void freeCamera(Camera* camera);

void updateCamera(Camera* camera, float dt);

void cameraLookAt(Camera* camera, Vector2f lookAt);

Vector2f getRelativePositionToCamera(Camera* camera, Vector2f* position);
Vector2f getRelativeSizeToCamera(Camera* camera, Vector2f* size);
