#include "camera.h"
#include "globals.h"

#include <assert.h>

Camera* createCamera()
{
	Camera* camera = (Camera*)malloc(sizeof(Camera));
	assert(camera);

	camera->position.x = 0;
	camera->position.y = 0;
	camera->zoom = 1.0f;

	return camera;
}

void freeCamera(Camera* camera)
{
	free(camera);
}

void updateCamera(Camera* camera, float dt)
{
	const Uint8* kbdState = SDL_GetKeyboardState(NULL);

	if (kbdState[SDL_SCANCODE_O])
	{
		camera->zoom -= 1.0f * dt;

	}

	if (kbdState[SDL_SCANCODE_P])
	{
		camera->zoom += 1.0f * dt;

	}
}

void cameraLookAt(Camera* camera, Vector2f lookAt)
{
	camera->position = lookAt;
}

Vector2f getRelativePositionToCamera(Camera* camera, Vector2f* position)
{
	Vector2f relativePosition{};
	relativePosition.x = (position->x - camera->position.x) * camera->zoom + SCREEN_WIDTH / 2;
	relativePosition.y = (position->y - camera->position.y) * camera->zoom + SCREEN_HEIGHT / 2;
	return relativePosition;
}

Vector2f getRelativeSizeToCamera(Camera* camera, Vector2f* size)
{
	Vector2f relativeSize{};
	relativeSize.x = size->x * camera->zoom;
	relativeSize.y = size->y * camera->zoom;
	return relativeSize;
}