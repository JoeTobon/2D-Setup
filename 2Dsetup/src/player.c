#include <SDL.h>
#include "stdio.h"
#include "entity.h"
#include "player.h"
#include "simple_logger.h"
#include "audio.h"

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
		//movement with keys and controller
		keys = SDL_GetKeyboardState(NULL);

		controller = SDL_GameControllerOpen(0);

		if(keys[SDL_SCANCODE_D] || SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) > DEAD_ZONE)
		{
			entity->position.x += 10;
		}
		if(keys[SDL_SCANCODE_A] || SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX) < -DEAD_ZONE)
		{
			entity->position.x -= 10;
		}
		if(keys[SDL_SCANCODE_W] || SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) < -DEAD_ZONE)
		{
			entity->position.y -= 10;
		}
		if(keys[SDL_SCANCODE_S] || SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) > DEAD_ZONE)
		{
			entity->position.y += 10;
		}
	}

	//updates bounding box
	entity->boundingBox.x = entity->position.x;
	entity->boundingBox.y = entity->position.y;
	entity->boundingBox.w = 50;
	entity->boundingBox.h = 100;

}

void player_attack(Entity *playerEnt, Entity *weaponEnt)
{
	Sprite *weaponS;
	const Uint8 *keys;
	
	//dead zone for controller
	const int DEAD_ZONE = 8000;

	//Game Controller 1 handler
	SDL_GameController *controller;
	weaponS = gf2d_sprite_load_image("images/Weapons/sword2.png");

	if(!playerEnt || !weaponEnt)
	{
		return;
	}

	//Delay between attacks
	if(weaponEnt->spawnTime >= 0)
	{
		weaponEnt->spawnTime += .1;
	}

	if(weaponEnt->spawnTime >= 3)
	{
		weaponEnt->sprite = NULL;
		weaponEnt->spawned = 0;
		weaponEnt->spawnTime = -1;
	}

	//Attacking
	if(playerEnt->type == player)
	{
		//attacking with controller
		controller = SDL_GameControllerOpen(0);

		if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX) < -DEAD_ZONE && weaponEnt->spawned != 1)
		{
			weaponEnt->sprite = weaponS;

			weaponEnt->position.x = playerEnt->position.x - 10;
			weaponEnt->position.y = playerEnt->position.y;

			
		
			sound_play(weaponEnt->entSound);
			weaponEnt->spawned = 1;
			weaponEnt->spawnTime = 0;

		}
		else if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX) > DEAD_ZONE && weaponEnt->spawned != 1)
		{
			weaponEnt->sprite = weaponS;

			weaponEnt->position.x = playerEnt->position.x + 80;
			weaponEnt->position.y = playerEnt->position.y;

			sound_play(weaponEnt->entSound);
			weaponEnt->spawned = 1;
			weaponEnt->spawnTime = 0;
		}
		else if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY) > DEAD_ZONE && weaponEnt->spawned != 1)
		{
			weaponEnt->sprite = weaponS;

			weaponEnt->position.x = playerEnt->position.x + 30;
			weaponEnt->position.y = playerEnt->position.y + 100;

			sound_play(weaponEnt->entSound);
			weaponEnt->spawned = 1;
			weaponEnt->spawnTime = 0;
		}
		else if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY) < -DEAD_ZONE && weaponEnt->spawned != 1)
		{
			weaponEnt->sprite = weaponS;

			weaponEnt->position.x = playerEnt->position.x + 30;
			weaponEnt->position.y = playerEnt->position.y - 100;

			sound_play(weaponEnt->entSound);
			weaponEnt->spawned = 1;
			weaponEnt->spawnTime = 0;
		}
	}

	//Detect collision if weapon is spawned
	if(weaponEnt->spawned == 1)
	{
		//update bounding box
		weaponEnt->boundingBox.x = weaponEnt->position.x;
		weaponEnt->boundingBox.y = weaponEnt->position.y;
		weaponEnt->boundingBox.w = 30;
		weaponEnt->boundingBox.h = 110;
	}
}

void bow_Attack(Entity *bow, Entity *arrow)
{
	const Uint8 *keys;
	Sprite *arrowS;
	
	//dead zone for controller
	const int DEAD_ZONE = 8000;

	//Game Controller 1 handler
	SDL_GameController *controller;

	arrowS = gf2d_sprite_load_image("images/Weapons/Arrow.png");

	if(!bow || !arrow)
	{
		slog("Bow or arrow does not exist");
		return;
	}

	//Delay between arrow spawn
	if(arrow->spawnTime >= 0)
	{
		arrow->spawnTime += .1;
	}

	if(arrow->spawnTime >= 4.5)
	{
		arrow->sprite = NULL;
		arrow->spawned = 0;
		arrow->spawnTime = -1;
	}


	//Spawn arrow in direction of right stick
	//if(playEnt->type == player)
	{
		//attacking with controller
		controller = SDL_GameControllerOpen(0);

		if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX) < -DEAD_ZONE && arrow->spawned != 1)
		{
			arrow->sprite = arrowS;

			arrow->position.x = bow->position.x - 60;
			arrow->position.y = bow->position.y;

			arrow->spawned = 1;
			arrow->spawnTime = 0;

			arrow->direct = 'l';

		}
		else if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTX) > DEAD_ZONE && arrow->spawned != 1)
		{
			arrow->sprite = arrowS;

			arrow->position.x = bow->position.x + 80;
			arrow->position.y = bow->position.y;

			arrow->spawned = 1;
			arrow->spawnTime = 0;

			arrow->direct = 'r';
		}
		else if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY) > DEAD_ZONE && arrow->spawned != 1)
		{
			arrow->sprite = arrowS;

			arrow->position.x = bow->position.x + 20;
			arrow->position.y = bow->position.y + 110;

			arrow->spawned = 1;
			arrow->spawnTime = 0;

			arrow->direct = 'u';
		}
		else if(SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_RIGHTY) < -DEAD_ZONE && arrow->spawned != 1)
		{
			arrow->sprite = arrowS;

			arrow->position.x = bow->position.x + 20;
			arrow->position.y = bow->position.y - 110;

			arrow->spawned = 1;
			arrow->spawnTime = 0;

			arrow->direct = 'd';
		}
	}

	//Make arrow move depending on right stick press
	if(arrow->direct == 'l')
	{
		arrow->position.x -= 10;
	}
	else if(arrow->direct == 'r')
	{
		arrow->position.x += 10;
	}
	else if(arrow->direct == 'd')
	{
		arrow->position.y -= 10;
	}
	else if(arrow->direct == 'u')
	{
		arrow->position.y += 10;
	}
}