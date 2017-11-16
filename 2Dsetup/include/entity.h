#ifndef __entity_h__
#define __entity_h__

#include <SDL.h>
#include "gf2d_sprite.h"
#include "gf2d_types.h"
#include "gf2d_vector.h"
#include "gf2d_text.h"
#include "audio.h"

//Used to identity what type of entity is being used
typedef enum
{
	player,
	enemy,									 //specify enemies, weapons, and projectiles as game is developed more
	projectile,
	sword,
	bow,
	none
}EntityType;

typedef struct Entity_S
{
	Bool inuse;							     //used to keep track of entity's use in system

	EntityType type;						 //Used to identity what type of entity is being used

	Sprite   *sprite;						 //sprite associated with entity 
	Vector2D position;						 //where the entity appears on the screen (using x and y coordinates)
	Vector2D *scale;
	Vector3D *rotation;
	Vector2D *scaleCenter;
	Vector2D *flip;
	Vector4D *colorShift;
	Uint32	 frame;
	
	SDL_Rect boundingBox;

	Vector2D velocity;						 //how fast the enity is moving in a given direction
	int health;								 //keeps track of entity's health

	float spawnTime;
	Bool spawned;
	char direct;							//Used for projectiles

	Sound *entSound;

	void (*update)(struct Entity_S *self);  //used to update the enity's current states
	void (*think)(struct Entity_S *self);

}Entity;

/**
 * @brief  initializes the enity manager/system
 * @param  maxNum the maximum number of enities the system can handle
 */
void entity_system_init(Uint32 maxNum);

/**
 * @brief   initialize a new entity
 * @param   none
 * @returns pointer to entity or null on error
 */
Entity *entity_new();

/**
 * @brief free an entity back to the entity manager. Stays in memory until the space is needed
 * @param entity the entity to free
 */
void entity_free(Entity *entity);

/**
 * @brief  deletes/removes an entity from the entity system
 * @param  entity a pointer to the entity that is to be deleted
 */
void entity_delete(Entity *entity);

/**
 * @brief deletes all loaded entities from memory does not close the entity system
 */
void entity_clear_all();

/**
 * @brief updates all the entities
 */
void entity_update_all();

/**
 * @brief draws entity to the screen
 * @param entity the entity to draw
 */
void entity_draw(Entity *entity);

/**
 * @brief draws all entities in use to the screen
 */
void entity_draw_all();

/**
 * @brief detects whether or not collsion occurs between two entities
 * @param ent1 first entity 
 * @param ent2 second entity
 */
Bool entity_collsion(Entity *ent1, Entity *ent2);

/**
 * @brief makes all entities specified approach the player
 */
void entity_collide_approach_all();

/**
 * @brief will load an entity and its various attributes from a file
 * @param ent entity to load from a file 
 * @param filname name of file to load from
 */
void entity_load(Entity *ent, char *filename);

/**
 * @brief will load all entities specified in a file
 * @param filename name of file to load from
 */
void entity_load_all(char *filename);

#endif