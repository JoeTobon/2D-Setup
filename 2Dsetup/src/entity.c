#include "simple_logger.h"
#include "entity.h"
#include "gf2d_types.h"
#include "gf2d_sprite.h"
#include "player.h"
#include "enemy.h"
#include <physfs.h>

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

			if(entity_collsion(playerEnt, &entity_manager.entList[i]) == true)
			{
				entity_delete(playerEnt);
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
		
		if(entity_manager.entList[i].type == weapon)
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
}

void entity_load(Entity *ent, char *filename)
{
	PHYSFS_File *file;
	char *pBuf;
	Sound *entSound;
	char buffer[512];
	int tempx, tempy;
	int incr = 0;

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

	if (!PHYSFS_exists(filename))
	{
		slog("Failed to open file (%s) for reading", filename);
		return;
	}

	file = PHYSFS_openRead(filename);

	pBuf = (char *)malloc(PHYSFS_fileLength(file));
	memset(pBuf, 0, PHYSFS_fileLength(file));

	PHYSFS_readBytes(file, pBuf, PHYSFS_fileLength(file));
	PHYSFS_close(file);

	slog("File contains: %s", pBuf);

	//scan 
	while(sscanf(pBuf, " %s\n%n", buffer, &incr) == 1)
	{
		pBuf += incr;

		if(pBuf[0] == '~')
		{
			return;
		}

		if(strcmp(buffer, "type:") == 0)
		{
			sscanf(pBuf, "%i%n", &ent->type, &incr);
			pBuf += incr;

			slog("Entity type: %i", ent->type);
			continue;
		}
		if(strcmp(buffer, "sprite:") == 0)
		{
			sscanf(pBuf, "%s\n%n", buffer, &incr);
			pBuf += incr;

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
			sscanf(pBuf, "%s\n%n", buffer, &incr);
			pBuf += incr;

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
			sscanf(pBuf, "%i%n", &ent->spawned, &incr);
			pBuf += incr;
			slog("Entity spawned: %i", ent->spawned);
			continue;
		}
		if(strcmp(buffer, "position:") == 0)
		{
			sscanf(pBuf, "%i%n", &tempx, &incr);
			pBuf += incr;

			sscanf(pBuf, "%i%n", &tempy, &incr);
			pBuf += incr;

			ent->position = vector2d(tempx, tempy);

			slog("Entity position.x: %f", ent->position.x);
			slog("Entity position.y: %f", ent->position.y);

			continue;
		}
		if(strcmp(buffer, "sound:") == 0)
		{
			sscanf(pBuf, "%s\n%n", buffer, &incr);
			pBuf += incr;
			ent->entSound = sound_new(buffer, 1, 1);
			slog("Sound:", buffer);

			continue;
		}
		if(strcmp(buffer, "frame:") == 0)
		{
			sscanf(pBuf, "%i%n", &ent->frame, &incr);
			pBuf += incr;
			slog("Entity frame: %i", ent->frame);
			continue;
		}
	}
}

void entity_load_all(char *filename)
{
	PHYSFS_File *file;
	char *pBuf;				//holds all file info
	char buffer[512];       //holds part of file to be inspected
	char *entFile;
	Entity *temp;			//used to initialize entity
	int incr = 0;

	if(!filename)
	{
		slog("file does not exist");
		return;
	}
	
	if (!PHYSFS_exists(filename))
	{
		slog("Failed to open file (%s) for reading", filename);
		return;
	}

	file = PHYSFS_openRead(filename);

	pBuf = (char *)malloc(PHYSFS_fileLength(file));
	memset(pBuf, 0, PHYSFS_fileLength(file));

	PHYSFS_readBytes(file, pBuf, PHYSFS_fileLength(file));
	PHYSFS_close(file);

	//Call file that has list of all entities
	while(sscanf(pBuf, " %s\n%n", buffer, &incr) == 1)
	{
		if(pBuf[0] == '~')
		{
			return;
		}

		pBuf += incr;
		entFile = buffer;
		temp = entity_new();

		entity_load(temp, entFile);
	}

}

