#include <SDL.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "entity.h"
#include "simple_logger.h"
#include "game.h"

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int done = 0;
    const Uint8 * keys;
    Sprite *sprite;
	int i, in;

	//Used for Entity assignment
	Entity *ent;
	Sprite *bug1;
	Sprite *bug2;
	Sprite *ship1;
	Sprite *ship2;

    int mx,my;
    float mf = 0;
    Sprite *mouse;
    Vector4D mouseColor = {255,100,255,200};
    
	
	in = 0;

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

    SDL_ShowCursor(SDL_DISABLE);
    
    /*demo setup*/
	

	//Load all sprites in system
    sprite = gf2d_sprite_load_image("images/backgrounds/bg_flat.png");
    mouse = gf2d_sprite_load_all("images/pointer.png",32,32,16);
	
	//Load sprites
	bug1 = gf2d_sprite_load_all("images/space_bug.png", 128, 128, 16);
	bug2 = gf2d_sprite_load_all("images/space_bug_top.png", 128, 128, 16);
	ship1 = gf2d_sprite_load_all("images/ed210.png", 128, 128, 16);
	ship2 = gf2d_sprite_load_all("images/ed210_top.png", 128, 128, 16);

    /*main game loop*/
    while(!done)
    {
        SDL_PumpEvents();   // update SDL's internal event structures
        keys = SDL_GetKeyboardState(NULL); // get the keyboard state for this frame
        /*update things here*/
        SDL_GetMouseState(&mx,&my);
		
		in += 1;
        mf+=0.1;

        if (mf >= 16.0)mf = 0;
		if(in == 4) in = 0;

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

			//Create new ent
	        ent = entity_new();

			//choose sprite for entity
			if(in == 0) ent->sprite = bug1;
			else if(in == 1) ent->sprite = bug2;
			else if(in == 2) ent->sprite = ship1;
			else if (in == 3)ent->sprite = ship2;
			
			//initialize attributes of entity
			ent->position = vector2d(rand() % 1000, rand() % 1000);
			ent->velocity = vector2d(rand() % 20, rand() % 2);
			ent->scale = NULL;
			ent->scaleCenter = NULL;
			ent->rotation = NULL;
			ent->flip = NULL;
			ent->colorShift = NULL;
			ent->frame = (int)mf;
			
		
			entity_draw_all();

			//Deletes all entities 
			if(mf >= 15.0)
			{
				entity_clear_all();
			}
			
        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame

		if (keys[SDL_SCANCODE_ESCAPE])done = 1; // exit condition
        slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
