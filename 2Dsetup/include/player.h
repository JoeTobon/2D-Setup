#ifndef __player_h__
#define __player_h__


#include <SDL.h>
#include "entity.h"

/**
 * @brief  will update various attributes of the player
 * @param  the entity to update (the player in this instance)
 */
void player_update(Entity *entity);


/////FIX COMMENT
/**
 * @brief  will handle the player attacking
 * @param  the player entity attacking, the enemy entity to attack, and the weapon to do the attacking with
 */
void player_attack(Entity *player, Entity *weapon);

#endif