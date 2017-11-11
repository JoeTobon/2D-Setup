#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "entity.h"
#include "simple_logger.h"
#include "player.h"
#include "enemy.h"
#include "audio.h"
#include "level.h"

/**
 * @brief will handle the titlescreen and its prompts
 */

void titleScreen();

/**
 * @brief will handle the level select screen and initiate the level based on selection
 * @param level the level to be initialized based on selection 
 */
void level_Screen(Level *level);