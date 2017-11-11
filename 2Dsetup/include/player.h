#ifndef __player_h__
#define __player_h__


#include <SDL.h>
#include "entity.h"

/**
 * @brief will update various attributes of the player
 * @param entity the entity to update (the player in this instance)
 */
void player_update(Entity *entity);

/**
 * @brief will handle the player attacking
 * @param player the player entity attacking
 * @param weapon the weapon entity to do the attacking with
 */
void player_attack(Entity *player, Entity *weapon);

#endif