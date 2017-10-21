#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "entity.h"
#include "simple_logger.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "audio.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;
	int i, in;
	Sound *testS;
	Music *testM;
	
	//Used for Entity assignment
	Entity *entPlayer;
	Entity *enemyEnt;
	Entity *weaponEnt;

    int mx,my;
    float mf = 0;
    Sprite *mouse;
	
    Vector4D mouseColor = {255,100,255,200};

	//Game Controller 1 handler
	SDL_GameController *controller = NULL;

    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        vector4d(0,0,0,255),
        0);
    gf2d_graphics_set_frame_delay(16);
    gf2d_sprite_init(1024);

	//entity system initialized
	entity_system_init(1024);

	//audio system initialized
	audio_system_init(256,16,4,1,1,1);

    SDL_ShowCursor(SDL_DISABLE);

	//Controller support
	SDL_Init(SDL_INIT_GAMECONTROLLER);

	//Check for joysticks
    if( SDL_NumJoysticks() < 1 )
    {
		slog( "No controllers connected!" );
    }
    else
    {
		//Load joystick
		controller = SDL_GameControllerOpen(0);
            
		if(controller == NULL)
        {
			slog( "Unable to open controller! SDL Error: %s\n", SDL_GetError() );
        }
		else
		{
			slog("Controller is attached.");
		}
	}


    /*demo setup*/
	

	//Load all sprites in system
    sprite = gf2d_sprite_load_image("images/backgrounds/newBackground.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
	
	//player
	entPlayer = entity_new();
	entity_load(entPlayer, "def/entity/player1.player");

	//enemy
	enemyEnt = entity_new();
	entity_load(enemyEnt, "def/entity/skeleton.enemy");

	//Weapon
	weaponEnt = entity_new();
	weaponEnt->type = weapon;
	weaponEnt->spawned = 0;

	//test sound
	testS = sound_new("audio/swish_2.wav", 1, 1);

	weaponEnt->entSound = testS; 

	//test music
	testM = music_new("audio/little town.mid", 1);
	music_play(testM);

	//titleScreen
	titleScreen();

    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
		
        mf+=0.1;

        if (mf >= 16.0)mf = 0;

		enemy_approach(entPlayer, enemyEnt);
		enemy_attack(entPlayer, enemyEnt);
		player_attack(entPlayer, enemyEnt, weaponEnt);
		entity_update_all();
		        
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first

            gf2d_sprite_draw_image(sprite,vector2d(0,0));

            //UI elements last
            gf2d_sprite_draw(
                mouse,
                vector2d(mx,my),
                NULL,
                NULL,
                NULL,
                NULL,
                &mouseColor,
                (int)mf);

			entity_draw_all();
			
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame

		if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }

	//Close game controller
	SDL_GameControllerClose(controller);
    controller = NULL;


    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/

void titleScreen()
{
	Sprite *titleScreen;
	int titleBool = 0;
	SDL_GameController *controller;

	titleScreen = gf2d_sprite_load_image("images/UI/titleMenu.png");
	controller = SDL_GameControllerOpen(0);


	//title screen
	while(!titleBool)
	{
		SDL_PumpEvents();   // update SDL's internal event structures

		gf2d_graphics_clear_screen();// clears drawing buffers

			gf2d_sprite_draw_image(titleScreen,vector2d(0, 0));

		gf2d_grahics_next_frame();// render current draw frame and skip to the next frame

		if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A))
		{
			titleBool = 1;
		}

		slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
	}
}