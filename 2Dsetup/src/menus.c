#include "menus.h"

void titleScreen()
{
	Sprite *titleScreen;
	int titleBool = 0;
	SDL_GameController *controller;
	SDL_Event e;

	titleScreen = gf2d_sprite_load_image("assets/images/UI/titleMenu.png");
	controller = SDL_GameControllerOpen(0);

	//title screen
	while(!titleBool)
	{
		while(SDL_PollEvent(&e) != 0)
		{
			gf2d_graphics_clear_screen();// clears drawing buffers

				gf2d_sprite_draw_image(titleScreen,vector2d(0, 0));

			gf2d_grahics_next_frame();// render current draw frame and skip to the next frame

			if(e.cbutton.button == SDL_CONTROLLER_BUTTON_A)
			{
				if(e.type == SDL_CONTROLLERBUTTONUP)
				{
					titleBool = 1;
				}
			}
		}

		slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
	}
}

void main_menu(Level *level)
{
	int menuBool = 0;
	float timer = 0;
	Bool stall, menuSwitch;
	SDL_GameController *controller;
	Button *play, *editor, *controls, *settings;
	SDL_Event e;
	Sprite *mainM = gf2d_sprite_load_image("assets/images/UI/MainMenu.png");
	Sprite *playText = gf2d_sprite_load_image("assets/images/UI/play.png");
	Sprite *controlText = gf2d_sprite_load_image("assets/images/UI/controlText.png");

	//dead zone for controller
	const int DEAD_ZONE = 8000;

	controller = SDL_GameControllerOpen(0);

	stall = false;
	menuSwitch = false;

	//initialize buttons
	play	 = button_new();
	editor	 = button_new();
	controls = button_new();
	settings = button_new();

	//Set button attributes
	play->position.x = 450;
	play->position.y = 0;
	play->bounds.x = play->position.x;
	play->bounds.y = play->position.y + 50;
	play->hover = true;

	editor->position.x = 450;
	editor->position.y = 150;
	editor->bounds.x = editor->position.x;
	editor->bounds.y = editor->position.y + 50;

	controls->position.x = 450;
	controls->position.y = 300;
	controls->bounds.x = controls->position.x;
	controls->bounds.y = controls->position.y + 50;

	settings->position.x = 450;
	settings->position.y = 450;
	settings->bounds.x = settings->position.x;
	settings->bounds.y = settings->position.y + 50;

	//Menu Screen
	while(!menuBool)
	{
		SDL_PumpEvents();   // update SDL's internal event structures

		//Delays time between controller input
		if(stall == true)
		{
			timer += 0.1;
			
		}

        if (timer >= 2.0) 
		{
			timer = 0;
			stall = false;
		}

		button_hover_all();

		gf2d_graphics_clear_screen();// clears drawing buffers

			gf2d_sprite_draw_image(mainM, vector2d(-700,-500));
			
			button_draw_all();

			gf2d_sprite_draw_image(playText, vector2d(450, 0));

			gf2d_sprite_draw_image(controlText, vector2d(450, 300));

		gf2d_grahics_next_frame();// render current draw frame and skip to the next frame

		//Deals with controller input when left stick is down
		if(play->hover == true && (SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) > DEAD_ZONE) && stall == false)
		{
			play->hover = false;
			editor->hover = true;
			stall = true;
		}
		if(editor->hover == true && (SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) > DEAD_ZONE) && stall == false)
		{
			editor->hover = false;
			controls->hover = true;
			stall = true;
		}
		if(controls->hover == true && (SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) > DEAD_ZONE) && stall == false)
		{
			controls->hover = false;
			settings->hover = true;
			stall = true;
		}
		if(settings->hover == true && (SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) > DEAD_ZONE) && stall == false)
		{
			settings->hover = false;
			play->hover = true;
			stall = true;
		}

		//Deals with controller input when left stick is up
		if(play->hover == true && (SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) < -DEAD_ZONE) && stall == false)
		{
			play->hover = false;
			settings->hover = true;
			stall = true;
		}
		if(editor->hover == true && (SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) < -DEAD_ZONE) && stall == false)
		{
			editor->hover = false;
			play->hover = true;
			stall = true;
		}
		if(controls->hover == true && (SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) < -DEAD_ZONE) && stall == false)
		{
			controls->hover = false;
			editor->hover = true;
			stall = true;
		}
		if(settings->hover == true && (SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) < -DEAD_ZONE) && stall == false)
		{
			settings->hover = false;
			controls->hover = true;
			stall = true;
		}

		//If A is clicked, exit while loop call appropriate function
		if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A))
		{
			menuBool = 1;
		}

		//If B is clicked, exit game entirely
		if(SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B))
		{
			exit(0);
		}

		slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
	}

	//Functions to depending on button hovered over
	if(play->hover == true)
	{
		button_delete_all();
		level_load(level, "assets/def/level/level1.level");
	}
	else if(editor->hover == true)
	{
		button_delete_all();
		editor_screen();
	}
	else if(controls->hover == true)
	{
		button_delete_all();
		control_screen();
	}
	else if(settings->hover == true)
	{
		button_delete_all();
		settings_screen();
	}
}

void pause_menu()
{
	Bool draw, stall, clicked;
	float timer = 0;
	SDL_Event e;
	Button *resume, *quit;
	SDL_GameController *controller;
	Level *level;
	Sprite *pauseM = gf2d_sprite_load_image("assets/images/UI/Pause.png");
	Sprite *playText = gf2d_sprite_load_image("assets/images/UI/play.png");
	Sprite *mainText = gf2d_sprite_load_image("assets/images/UI/mm.png");

	//dead zone for controller
	const int DEAD_ZONE = 8000;

	//Load level
	level = NULL;
	controller = SDL_GameControllerOpen(0);

	draw    = true;
	stall   = false;
	clicked = false;

	resume = button_new();
	quit   = button_new();

	resume->position.x = 450;
	resume->position.y = 150;
	resume->bounds.x = resume->position.x;
	resume->bounds.y = resume->position.y + 50;
	resume->hover = true;

	quit->position.x = 450;
	quit->position.y = 300;
	quit->bounds.x = quit->position.x;
	quit->bounds.y = quit->position.y + 50;

	while(draw)
	{
		SDL_PumpEvents();   // update SDL's internal event structures

		//Delays time between controller input
		if(stall == true)
		{
			timer += 0.1;
			
		}

        if(timer >= 2.0) 
		{
			timer = 0;
			stall = false;
		}

		button_hover_all();

		gf2d_graphics_clear_screen();// clears drawing buffers

			gf2d_sprite_draw_image(pauseM, vector2d(0,0));
			button_draw_all();
			gf2d_sprite_draw_image(playText, vector2d(450,150));
			gf2d_sprite_draw_image(mainText, vector2d(450,300));

		gf2d_grahics_next_frame();// render current draw frame and skip to the next frame

		//Deals with controller input when left stick is down
		if(resume->hover == true && (SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) > DEAD_ZONE) && stall == false)
		{
			resume->hover = false;
			quit->hover = true;
			stall = true;
		}
		if(quit->hover == true && (SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) > DEAD_ZONE) && stall == false)
		{
			quit->hover = false;
			resume->hover = true;
			stall = true;
		}

		//Deals with controller input when left stick is up
		if(resume->hover == true && (SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) < -DEAD_ZONE) && stall == false)
		{
			resume->hover = false;
			quit->hover = true;
			stall = true;
		}
		if(quit->hover == true && (SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY) < -DEAD_ZONE) && stall == false)
		{
			quit->hover = false;
			resume->hover = true;
			stall = true;
		}

		while(SDL_PollEvent(&e) != 0)
		{
			if(e.cbutton.button == SDL_CONTROLLER_BUTTON_A)
			{
				if(e.type == SDL_CONTROLLERBUTTONUP)
				{
					draw = false;
				}
			}
		}

		slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
	}
	
	if(quit->hover == true)
	{
		entity_clear_all();
		button_delete_all();
		window_delete_all();
		level_delete_all();

		level_over(true);
		level = level_new();
		main_menu(level);
	}
}

void game_over()
{
	Bool draw, stall, clicked;
	float timer = 0;
	SDL_Event e;
	Button *quit;
	SDL_GameController *controller;
	Level *level;
	Sprite *gameO = gf2d_sprite_load_image("assets/images/UI/GameOver.png");
	Sprite *mainText = gf2d_sprite_load_image("assets/images/UI/mm.png");

	//dead zone for controller
	const int DEAD_ZONE = 8000;

	//Load level
	level = NULL;
	controller = SDL_GameControllerOpen(0);

	draw    = true;
	stall   = false;
	clicked = false;

	quit   = button_new();

	quit->position.x = 450;
	quit->position.y = 300;
	quit->bounds.x = quit->position.x;
	quit->bounds.y = quit->position.y + 50;
	quit->hover = true;

	while(draw)
	{
		SDL_PumpEvents();   // update SDL's internal event structures

		//Delays time between controller input
		if(stall == true)
		{
			timer += 0.1;
			
		}

        if(timer >= 2.0) 
		{
			timer = 0;
			stall = false;
		}

		button_hover_all();

		gf2d_graphics_clear_screen();// clears drawing buffers

			gf2d_sprite_draw_image(gameO, vector2d(0, 0));
			button_draw_all();
			gf2d_sprite_draw_image(mainText, vector2d(450,300));

		gf2d_grahics_next_frame();// render current draw frame and skip to the next frame

		while(SDL_PollEvent(&e) != 0)
		{
			if(e.cbutton.button == SDL_CONTROLLER_BUTTON_A)
			{
				if(e.type == SDL_CONTROLLERBUTTONUP)
				{
					draw = false;
				}
			}
		}

		slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
	}
	
	if(quit->hover == true)
	{
		entity_clear_all();
		button_delete_all();
		window_delete_all();
		level_delete_all();
		
		level_over(true);
		level = level_new();

		main_menu(level);
	}
}

void editor_screen()
{
	SDL_Event e;
	Bool editorD = true;
	Level *level = NULL;

	while(editorD)
	{
		gf2d_graphics_clear_screen();// clears drawing buffers
			

		gf2d_grahics_next_frame();// render current draw frame and skip to the next frame

		while(SDL_PollEvent(&e) != 0)
		{
			if(e.cbutton.button == SDL_CONTROLLER_BUTTON_B)
			{
				if(e.type == SDL_CONTROLLERBUTTONUP)
				{
					editorD = false;
				}
			}
		}

		slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
	}

	level_delete_all();
	level = level_new();
	main_menu(level);
}

void control_screen()
{
	SDL_Event e;
	Bool controlD = true;
	Level *level = NULL;
	Sprite *controls = gf2d_sprite_load_image("assets/images/UI/controller.png");

	while(controlD)
	{
		gf2d_graphics_clear_screen();// clears drawing buffers
			
			gf2d_sprite_draw_image(controls, vector2d(0, 0));

		gf2d_grahics_next_frame();// render current draw frame and skip to the next frame

		while(SDL_PollEvent(&e) != 0)
		{
			if(e.cbutton.button == SDL_CONTROLLER_BUTTON_B)
			{
				if(e.type == SDL_CONTROLLERBUTTONUP)
				{
					controlD = false;
				}
			}
		}

		slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
	}

	level_delete_all();
	level = level_new();
	main_menu(level);
}

void settings_screen()
{
	SDL_Event e;
	Bool screenD = true;
	Level *level = NULL;

	while(screenD)
	{
		gf2d_graphics_clear_screen();// clears drawing buffers
			

		gf2d_grahics_next_frame();// render current draw frame and skip to the next frame

		while(SDL_PollEvent(&e) != 0)
		{
			if(e.cbutton.button == SDL_CONTROLLER_BUTTON_B)
			{
				if(e.type == SDL_CONTROLLERBUTTONUP)
				{
					screenD = false;
				}
			}
		}

		slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
	}

	level_delete_all();
	level = level_new();
	main_menu(level);
}

void win_screen()
{
	Bool draw, stall, clicked;
	float timer = 0;
	SDL_Event e;
	Button *quit;
	SDL_GameController *controller;
	Level *level;
	Sprite *win = gf2d_sprite_load_image("assets/images/UI/winScreen.png");
	Sprite *mainText = gf2d_sprite_load_image("assets/images/UI/mm.png");

	//dead zone for controller
	const int DEAD_ZONE = 8000;

	//Load level
	level = NULL;
	controller = SDL_GameControllerOpen(0);

	draw    = true;
	stall   = false;
	clicked = false;

	quit   = button_new();

	quit->position.x = 450;
	quit->position.y = 300;
	quit->bounds.x = quit->position.x;
	quit->bounds.y = quit->position.y + 50;
	quit->hover = true;

	while(draw)
	{
		SDL_PumpEvents();   // update SDL's internal event structures

		//Delays time between controller input
		if(stall == true)
		{
			timer += 0.1;
			
		}

        if(timer >= 2.0) 
		{
			timer = 0;
			stall = false;
		}

		button_hover_all();

		gf2d_graphics_clear_screen();// clears drawing buffers

			gf2d_sprite_draw_image(win, vector2d(0, 0));
			button_draw_all();
			gf2d_sprite_draw_image(mainText, vector2d(450,300));

		gf2d_grahics_next_frame();// render current draw frame and skip to the next frame

		while(SDL_PollEvent(&e) != 0)
		{
			if(e.cbutton.button == SDL_CONTROLLER_BUTTON_A)
			{
				if(e.type == SDL_CONTROLLERBUTTONUP)
				{
					draw = false;
				}
			}
		}

		slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
	}
	
	if(quit->hover == true)
	{
		entity_clear_all();
		button_delete_all();
		window_delete_all();
		level_delete_all();
		
		level_over(true);
		level = level_new();

		main_menu(level);
	}
}
