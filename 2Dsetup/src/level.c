#include "level.h"
#include "simple_logger.h"
#include "entity.h"

typedef struct
{
	Uint32 maxLevel;
	Level *levelList;
} Levelmanager; 

static Levelmanager level_manager = {0};
static int count = 0;

void level_close()
{
	if (level_manager.levelList != NULL)
    {
		level_delete_all();
        free(level_manager.levelList);
    }

	memset(&level_manager, 0, sizeof(Levelmanager));

    slog("level system closed");
}


void level_system_init(Uint32 maxNum)
{
	if(maxNum <= 0)
	{
		slog("cannot intialize an entity manager for zero entities!");
        return;
	}

	memset(&level_manager, 0, sizeof(Levelmanager));
	level_manager.levelList = (Level *)malloc(sizeof(Level)*maxNum);;

	if(!level_manager.levelList)
	{
		slog("Failed to allocate level list");
		level_close();
		return;
	}

	memset(level_manager.levelList,0,sizeof(Level)*maxNum);
    level_manager.maxLevel = maxNum;

	atexit(level_close);

	slog("level system initialized");
}

Level *level_new()
{
	//Search through the entity manager for an unused address
	int i;

	for(i = 0; i < level_manager.maxLevel; i++)
	{
		if(level_manager.levelList[i].inuse == 0)
		{
			level_delete(&level_manager.levelList[i]); //clean up old data
			level_manager.levelList[i].inuse = 1; //Set ref count to 1. Address is now in use

			return &level_manager.levelList[i];		  //Return address of index in array
		}
	}
	
	slog("error: out of level addresses");
	exit(0);
	return  NULL;
}

void level_load(Level *level, char *filename)
{
	FILE *file;
	char buffer[512];
	char hold[512];
	int loop = 0;

	if(!level)
	{
		slog("Level is null");
		return;
	}

	if(!filename)
	{
		slog("Filename is null");
		return;
	}

	file = fopen(filename, "r");

	if(!file)
	{
		slog("Failed to load file: %s", filename);
		return;
	}

	rewind(file);

	while(fscanf(file,"%s", buffer) != EOF)
	{
		if(strcmp(buffer, "background:") == 0)
		{
			fscanf(file, "%s", buffer);
			level->background = gf2d_sprite_load_image(buffer);

			slog("Level background: %s", buffer);
			continue;
		}
		if(strcmp(buffer, "levelMusic:") == 0)
		{
			fscanf(file, "%s", hold);
			slog("Level music: %s", hold);
			continue;
		}
		if(strcmp(buffer, "loop:") == 0)
		{
			fscanf(file, "%i", &loop);
			slog("Music loop: %i", loop);

			//initialize music
			level->levelMusic = music_new(hold, loop);

			continue;
		}
		if(strcmp(buffer, "entFile:") == 0)
		{
			fscanf(file, "%s", buffer);
			entity_load_all(buffer);
			slog("Entity File: %s", buffer);
			continue;
		}
	}

	fclose(file);
}

void level_draw(Level *level)
{
	if(!level)
	{
		return;
	}

	gf2d_sprite_draw_image(level->background,vector2d(0,0));
}

void level_delete(Level *level)
{
	if(!level)
	{
		return;
	}

	memset(level,0,sizeof(Level));//clean up all other data
}

void level_draw_all()
{
	int i;

	for(i = 0; i < level_manager.maxLevel; i++)
	{
		if(level_manager.levelList[i].inuse == 0)
		{
			continue;
		}

		level_draw(&level_manager.levelList[i]);
	}
}

void level_delete_all()
{
	int i;

	for(i = 0; i < level_manager.maxLevel; i++)
	{
		level_delete(&level_manager.levelList[i]);// cleans up the data
	}
}

void next_level()
{
	Level *level = NULL;

	if(enemies_dead() == true && count == 0)
	{
		music_clear_all();
		button_delete_all();
		window_delete_all();
		level_delete_all();

		level = level_new();

		slog("win!");
		count = 1;
		level_load(level, "def/level/level2.level");
	}
	/*if(enemies_dead() == true && count == 1)
	{
		music_clear_all();
		button_delete_all();
		window_delete_all();
		level_delete_all();

		level = level_new();

		slog("win!");
		count = 0;
		level_load(level, "def/level/level1.level");
	}*/

}

void level_over(Bool status)
{
	if(status == true)
	{
		count = 0;
	}
}