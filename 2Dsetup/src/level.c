#include "level.h"

void level_load(Level *level, char *filename)
{
	FILE *file;
	char buffer[512];

	if(!level)
	{
		return;
	}

	if(!filename)
	{
		return;
	}

	file = fopen(filename, "r");

	if(!file)
	{

	}
}