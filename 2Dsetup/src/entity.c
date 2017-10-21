#include "simple_logger.h"
#include "entity.h"
#include "gf2d_types.h"
#include "gf2d_sprite.h"
#include "player.h"

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
			entity_manager.entList[i].type = -1;
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

void entity_load(Entity *ent, char *filename)
{
	FILE *file;
	char buffer[512];
	int tempx, tempy;

	tempx = 0;
	tempy = 0;

	if(!ent)
	{
		slog("player does not exist");
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

			slog("Entity position.x: %i", tempx);
			slog("Entity position.y: %i", tempy);
			continue;
		}
		if(strcmp(buffer, "frame:") == 0)
		{
			fscanf(file, "%i", &ent->frame);
			slog("Entity frame: %i", ent->frame);
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

