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

void titleScreen();

void main_menu();

void level_Screen();

void pause_menu();

void game_over();

void editor_screen();

void control_screen();

void settings_screen();

void win_screen();

#endif