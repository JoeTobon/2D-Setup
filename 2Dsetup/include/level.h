#ifndef __level_h__
#define __level_h__

#include <stdio.h>
#include "gf2d_sprite.h"
#include "simple_logger.h"
#include "audio.h"
#include "window.h"
#include "widgets.h"

typedef struct level_S
{
	Sprite *background;
	Music  *levelMusic;
	Bool inuse;
}Level;

/**
 * @brief  initializes the level manager/system
 * @param  maxNum the maximum number of levels the system will handle
 */
void level_system_init(Uint32 maxNum);


/**
 * @brief will load a level based on the level's filename
 * @param level the level to load
 * @param filename the name of the level file to load
 */
void level_load(Level *level, char *filename);

/**
 * @brief will initialize and allocate address for a new level
 * @return a pointer to the memory address of the new level
 */
Level *level_new();

/**
 * @brief draws level to the screen
 * @param level the level to draw
 */
void level_draw(Level *level);

/**
 * @brief  deletes/removes a level from the level system
 * @param  level a pointer to the level that is to be deleted
 */
void level_delete(Level *level);

/**
 * @brief draws all levels in use to the screen
 */
void level_draw_all();

/**
 * @brief deletes all loaded levels from memory does not close the level system
 */
void level_delete_all();

/**
 * @brief loads next level
 */
void next_level();


#endif