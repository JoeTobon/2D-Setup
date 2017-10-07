#include <SDL.h>
#include "entity.h"






void player_update(Entity *entity)
{
	const Uint8 *keys;
	
	//dead zone for controller
	const int DEAD_ZONE = 8000;

	//Game Controller 1 handler
	SDL_GameController *controller;

	if(!entity)
	{
		return;
	}

	if(!entity->inuse)
	{
		return;
	}

	//used to make player move
	if(entity->type == player)
	{
		//movement with keys
		keys = SDL_GetKeyboardState(NULL);

		if(keys[SDL_SCANCODE_D])
		{
			entity->position.x += 10;
		}
		if(keys[SDL_SCANCODE_A])
		{
			entity->position.x -= 10;
		}
		if(keys[SDL_SCANCODE_W])
		{
			entity->position.y -= 10;
		}
		if(keys[SDL_SCANCODE_S])
		{
			entity->position.y += 10;
		}

		//movement for controller
		controller = SDL_GameControllerOpen(0);

		if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) < -DEAD_ZONE)
		{
			entity->position.x -= 10;
		}
		if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) > DEAD_ZONE)
		{
			entity->position.x += 10;
		}
		if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) > DEAD_ZONE)
		{
			entity->position.y += 10;
		}
		if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) < -DEAD_ZONE)
		{
			entity->position.y -= 10;
		}
	}
}