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
 * @brief will define the player HUD
 * @param entity the player HUD
 */
void player_hud(Entity *entity);

/**
 * @brief will handle the player attacking
 * @param player the player entity attacking
 * @param weapon the weapon entity to do the attacking with
 */
void sword_Attack(Entity *player, Entity *weapon); 

/**
 * @brief will handle the player attacking with a bow
 * @param bow the bow entity to attack from
 * @param arrow the arrow entity to do the attacking with
 */
void knife_Attack(Entity *playerEnt, Entity *knife); //WILL PROBABLY SWITCH TO JUST A THROWING KNIFE

/**
 * @brief will handle the player attacking with a shield
 * @param shieldEnt the shield entity to do the attacking with
 */
void shield_Attack(Entity *playerEnt, Entity *shieldEnt);

#endif