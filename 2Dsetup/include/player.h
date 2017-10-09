#ifndef __player_h__
#define __player_h__


#include <SDL.h>
#include "entity.h"

/**
 * @brief  will load a player entity and its various attributes from a file
 * @param  Player entity to load to, name of file to load from
 */
void player_load(Entity *player, char *filename);

/**
 * @brief  will save a player entity and its various attributes to a file
 * @param  player entity to save, name of file to save to 
 */
void player_save(Entity *player, char *filename);

/**
 * @brief  will update various attributes of the player
 * @param  the entity to update (the player in this instance)
 */
void player_update(Entity *entity);

#endif