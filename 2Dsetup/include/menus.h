#ifndef __menus_h__
#define __menus_h__

#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "entity.h"
#include "simple_logger.h"
#include "player.h"
#include "enemy.h"
#include "audio.h"
#include "level.h"
#include "window.h"
#include "widgets.h"

/**
 * @brief  Will handle all the processes and drawing of the titlescreen
 */
void titleScreen();

/**
 * @brief  Will handle all the processes and drawing of the main menu
 */
void main_menu();

/**
 * @brief  Will handle all the processes and drawing of the level screen (no longer implemented)
 */
void level_Screen();

/**
 * @brief  Will handle all the processes and drawing of the pause menu
 */
void pause_menu();

/**
 * @brief  Will handle all the processes and drawing of the game over screen
 */
void game_over();

/**
 * @brief  Will handle all the processes and drawing of the editor (not implemented)
 */
void editor_screen();

/**
 * @brief  Will handle all the processes and drawing of the control screen
 */
void control_screen();

/**
 * @brief  Will handle all the processes and drawing of the settings screen (not implemented)
 */
void settings_screen();

/**
 * @brief  Will handle all the processes and drawing of the win screen 
 */
void win_screen();

#endif