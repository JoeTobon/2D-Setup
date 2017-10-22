#include "level.h"
#include "simple_logger.h"
#include "entity.h"

Level *level_new()
{
	Level *defaultL;

	defaultL = (Level *)malloc(sizeof(Level));

	if(!defaultL)
	{
		slog("Failed to allocate entity list");
		return;
	}
	memset(defaultL,0,sizeof(Level));

	return defaultL;
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