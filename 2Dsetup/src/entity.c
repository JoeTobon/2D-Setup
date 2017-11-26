#include "simple_logger.h"
#include "entity.h"
#include "gf2d_types.h"
#include "gf2d_sprite.h"
#include "player.h"
#include "enemy.h"

typedef struct
{
	Uint32 maxEnt;
	Entity *entList;
} Entitymanager; 

static Entitymanager entity_manager = {0};

void entity_close()
{
	if (entity_manager.entList != NULL)
    {
		entity_clear_all();
        free(entity_manager.entList);
    }

	memset(&entity_manager, 0, sizeof(Entitymanager));

    slog("entity system closed");
}

void entity_system_init(Uint32 maxNum)
{
	if(maxNum <= 0)
	{
		slog("cannot intialize an entity manager for zero entities!");
        return;
	}

	memset(&entity_manager, 0, sizeof(Entitymanager));
	entity_manager.entList = (Entity *)malloc(sizeof(Entity)*maxNum);

	if(!entity_manager.entList)
	{
		slog("Failed to allocate entity list");
		entity_close();
		return;
	}

	memset(entity_manager.entList,0,sizeof(Entity)*maxNum);
    entity_manager.maxEnt = maxNum;

	atexit(entity_close);

	slog("entity system initialized");
}

void entity_delete(Entity *entity)
{
	if(!entity)
	{
		return;
	}

	memset(entity,0,sizeof(Entity));//clean up all other data
}

void entity_free(Entity *entity)
{
	if(!entity)
	{
		return;	
	}

	entity->inuse = 0;
}

void entity_clear_all()
{
	int i;

	for(i = 0; i < entity_manager.maxEnt; i++)
	{
		entity_delete(&entity_manager.entList[i]);// cleans up the data
	}
}

Entity *entity_new()
{
	//Search through the entity manager for an unused address
	int i;

	for(i = 0; i < entity_manager.maxEnt; i++)
	{
		if(entity_manager.entList[i].inuse == 0)
		{
			entity_delete(&entity_manager.entList[i]); //clean up old data
			entity_manager.entList[i].inuse = 1; //Set ref count to 1. Address is now in use

			//Initialize various default attributes of entity here
			entity_manager.entList[i].type = none;
			entity_manager.entList[i].spawnTime = -1;


			return &entity_manager.entList[i];		  //Return address of index in array
		}
	}
	
	slog("error: out of entity addresses");
	exit(0);
	return  NULL;
}

void entity_update_all()
{
	int i;

	for(i = 0; i < entity_manager.maxEnt; i++)
	{
		if(entity_manager.entList[i].inuse == 0)
		{
			continue;
		}

		//if entity has no update function go to next entity
		if(!entity_manager.entList[i].update)
		{
			continue;
		}

		entity_manager.entList[i].update(&entity_manager.entList[i]);
	}
}

void entity_draw(Entity *entity)
{
	if(!entity)
	{
		return;
	}

	gf2d_sprite_draw(entity->sprite, entity->position, entity->scale, entity->scaleCenter, 
					    entity->rotation, entity->flip, entity->colorShift, entity->frame);
}

void entity_draw_all()
{
	int i;

	for(i = 0; i < entity_manager.maxEnt; i++)
	{
		if(entity_manager.entList[i].inuse == 0)
		{
			continue;
		}

		entity_draw(&entity_manager.entList[i]);
	}
}

Bool entity_collsion(Entity *ent1, Entity *ent2)
{
	if(!ent1 || !ent2)
	{
		return;
	}

	if((ent1->boundingBox.x + ent1->boundingBox.w) < ent2->boundingBox.x)
	{
		return false;
	}
	else if((ent2->boundingBox.x + ent2->boundingBox.w) < ent1->boundingBox.x)
	{
		return false;
	}
	else if((ent1->boundingBox.y + ent1->boundingBox.h) < ent2->boundingBox.y)
	{
		return false;
	}
	else if((ent2->boundingBox.y + ent2->boundingBox.h) < ent1->boundingBox.y)
	{
		return false;
	}
	else
	{
		return true;
	}
}

void entity_collide_approach_all()
{
	int i, j;
	Entity *playerEnt = NULL;

	//Finds player entity
	for(i = 0; i < entity_manager.maxEnt; i++)
	{
		if(entity_manager.entList[i].inuse == 0)
		{
			continue;
		}
		
		if(entity_manager.entList[i].type == player)
		{
			playerEnt = &entity_manager.entList[i];
			break;
		}
	}
	
	//checks collisions between player and enemies
	for(i = 0; i < entity_manager.maxEnt; i++)
	{
		if(entity_manager.entList[i].inuse == 0 || entity_manager.entList[i].type == player)
		{
			continue;
		}
		
		if(entity_manager.entList[i].type == enemy)
		{
			//approach handled here for now
			enemy_approach(playerEnt, &entity_manager.entList[i]);

			if(entity_collsion(playerEnt, &entity_manager.entList[i]) == true && playerEnt->invincible == false)
			{
				if(playerEnt->health == 1)
				{
					entity_delete(playerEnt);
				}
				else
				{
					playerEnt->health--;
				}

				//Enemy dies on impact for now
				entity_delete(&entity_manager.entList[i]);
			}
		}
	}

	//checks collision between weapon and enemy
	for(i = 0; i < entity_manager.maxEnt; i++)
	{
		if(entity_manager.entList[i].inuse == 0)
		{
			continue;
		}
		
		if(entity_manager.entList[i].type == sword)
		{
			player_attack(playerEnt, &entity_manager.entList[i]);

			for(j = 0; j < entity_manager.maxEnt; j++)
			{
				if(entity_manager.entList[j].type == enemy && entity_manager.entList[i].spawned == 1)
				{
					if(entity_collsion(&entity_manager.entList[i], &entity_manager.entList[j]) == true)
					{
						entity_delete(&entity_manager.entList[j]);
					}
				}
			}
		}
	}

	//checks collsion between player and health potion
	for(i = 0; i < entity_manager.maxEnt; i++)
	{
		if(entity_manager.entList[i].inuse == 0)
		{
			continue;
		}

		if(entity_manager.entList[i].type == hp)
		{
			if(entity_collsion(&entity_manager.entList[i], playerEnt) == true)
			{
				if(playerEnt->health < 3)
				{
					playerEnt->health++;
					entity_delete(&entity_manager.entList[i]);
				}
			}
		}
	}

	//checks collsion between player and invicibility potion
	for(i = 0; i < entity_manager.maxEnt; i++)
	{
		if(entity_manager.entList[i].inuse == 0)
		{
			continue;
		}

		if(entity_manager.entList[i].type == ip)
		{
			if(entity_collsion(&entity_manager.entList[i], playerEnt) == true && &entity_manager.entList[i].spawned != 0)
			{
				//set to false after certain amount of time
				//call seperate function
				playerEnt->invincible = true;
				entity_delete(&entity_manager.entList[i]);
			}
		}
	}

	//checks collsion between player and bomb
	for(i = 0; i < entity_manager.maxEnt; i++)
	{
		if(entity_manager.entList[i].type == bomb && entity_manager.entList[i].spawned == 0)
		{
			if(entity_collsion(playerEnt, &entity_manager.entList[i]) == true)
			{
				//set spawned to true and unassign sprite
				entity_manager.entList[i].spawned = 1;
				entity_manager.entList[i].sprite = NULL;
			}
		}
	}

	//checks collision between bomb and enemy
	for(i = 0; i < entity_manager.maxEnt; i++)
	{
		if(entity_manager.entList[i].inuse == 0)
		{
			continue;
		}
		
		if(entity_manager.entList[i].type == bomb)
		{
			for(j = 0; j < entity_manager.maxEnt; j++)
			{
				if(entity_manager.entList[j].type == enemy && entity_manager.entList[i].spawned == 1)
				{
					if(entity_collsion(&entity_manager.entList[i], &entity_manager.entList[j]) == true)
					{
						entity_delete(&entity_manager.entList[j]);
					}
				}
			}
		}
	}
}

void entity_load(Entity *ent, char *filename)
{
	FILE *file;
	Sound *entSound;
	char buffer[512];
	int tempx, tempy;

	tempx = 0;
	tempy = 0;

	if(!ent)
	{
		slog("entity does not exist");
		return;
	}

	if(!filename)
	{
		slog("file does not exist");
		return;
	}

	file = fopen(filename, "r");

	if (!file)
	{
		slog("Failed to open file (%s) for reading", filename);
		return;
	}

	rewind(file);

	//scan 
	while(fscanf(file, "%s", buffer) != EOF)
	{
		if(strcmp(buffer, "type:") == 0)
		{
			fscanf(file, "%i", &ent->type);
			slog("Entity type: %i", ent->type);
			continue;
		}
		if(strcmp(buffer, "sprite:") == 0)
		{
			fscanf(file, "%s", buffer);

			if(ent->type == 0)
			{
				ent->sprite = gf2d_sprite_load_all(buffer, 128, 128, 16);
			}
			else
			{
				ent->sprite = gf2d_sprite_load_image(buffer);
			}
			
			slog("Entity sprite: %s", buffer);
			continue;
		}
		if(strcmp(buffer, "update:") == 0)
		{
			fscanf(file, "%s", buffer);

			//assign update
			if(strcmp(buffer, "player_update") == 0)
			{
				ent->update = &player_update;
				slog("Entity update is: %s", buffer);
				continue;
			}
			continue;
		}
		if(strcmp(buffer, "spawned:") == 0)
		{
			fscanf(file, "%i", &ent->spawned);
			slog("Entity spawned: %i", ent->spawned);
			continue;
		}
		if(strcmp(buffer, "position:") == 0)
		{
			fscanf(file, "%i", &tempx);
			fscanf(file, "%i", &tempy);

			ent->position = vector2d(tempx, tempy);

			slog("Entity position.x: %f", ent->position.x);
			slog("Entity position.y: %f", ent->position.y);

			continue;
		}
		if(strcmp(buffer, "sound:") == 0)
		{
			fscanf(file, "%s", buffer);
			ent->entSound = sound_new(buffer, 1, 1);
			slog("Sound:", buffer);

			continue;
		}
		if(strcmp(buffer, "frame:") == 0)
		{
			fscanf(file, "%i", &ent->frame);
			slog("Entity frame: %i", ent->frame);
			continue;
		}
		if(strcmp(buffer, "health:") == 0)
		{
			fscanf(file, "%i", &ent->health);
			slog("Entity health: %i", ent->health);
			continue;
		}
	}

	fclose(file);
}

void entity_load_all(char *filename)
{
	FILE *file;
	char buffer[512];
	char *entFile;
	Entity *temp;			//used to initialize entity

	if(!filename)
	{
		slog("file does not exist");
		return;
	}

	file = fopen(filename, "r");
	
	if (!file)
	{
		slog("Failed to open file (%s) for reading", filename);
		return;
	}

	rewind(file);

	//Call file that has list of all entities
	while(fscanf(file, "%s", buffer) != EOF)
	{
		entFile = buffer;
		temp = entity_new();

		entity_load(temp, entFile);
	}

	fclose(file);
}

void bomb_update(Entity *bomb)
{
	//start timer if bomb is picked up
	if(bomb->spawned == 1)
	{
		//expand bounding box	
		bomb->boundingBox.x = bomb->position.x - 200;
		bomb->boundingBox.y = bomb->position.y - 200;
		bomb->boundingBox.w = bomb->position.x + 400;
		bomb->boundingBox.h = bomb->position.y + 400;
		
		bomb->spawnTime += .1;
	}

	//Delete bomb entity when effect timer wears off
	if(bomb->spawnTime >= 8)
	{
		entity_delete(bomb);
	}

}

