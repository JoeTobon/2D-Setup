#include "widgets.h"

typedef struct
{
	Uint32 maxButton;
	Button *buttonList;
}Buttonmanager;

static Buttonmanager button_manager = {0};

void button_close()
{
	if (button_manager.buttonList != NULL)
    {
		button_delete_all();
        free(button_manager.buttonList);
    }

	memset(&button_manager, 0, sizeof(Buttonmanager));

    slog("Window system closed");
}

void button_system_init(Uint32 maxNum)
{
	if(maxNum <= 0)
	{
		slog("cannot intialize a button manager for zero buttons!");
        return;
	}

	memset(&button_manager, 0, sizeof(Buttonmanager));
	button_manager.buttonList = (Button *)malloc(sizeof(Button)*maxNum);

	if(!button_manager.buttonList)
	{
		slog("Failed to allocate button list");
		button_close();
		return;
	}

	memset(button_manager.buttonList,0,sizeof(Button)*maxNum);
	button_manager.maxButton = maxNum;

	atexit(button_close);

	slog("window system initialized");
}

Button *button_new()
{
	//Search through the button manager for an unused address
	int i;

	for(i = 0; i < button_manager.maxButton; i++)
	{
		if(button_manager.buttonList[i].inuse == 0)
		{
			button_delete(&button_manager.buttonList[i]); //clean up old data
			button_manager.buttonList[i].inuse = 1; //Set ref count to 1. Address is now in use

			//Initialize various default attributes of entity here
			button_manager.buttonList[i].image = gf2d_sprite_load_image("images/UI/button.png");
			
			button_manager.buttonList[i].bounds.w = 300;
			button_manager.buttonList[i].bounds.h = 100;

			return &button_manager.buttonList[i];		  //Return address of index in array
		}
	}
	
	slog("error: out of button addresses");
	exit(0);
	return  NULL;
}

void button_draw(Button *button)
{
	if(!button)
	{
		return;
	}
	
	gf2d_sprite_draw(button->image, button->position, button->scale, button->scaleCenter, button->rotation, button->flip, button->colorShift, button->frame);
	//gf2d_draw_rect(button->bounds, vector4d(100, 200, 300, 100), true);
}

void button_delete(Button *button)
{
	if(!button)
	{
		return;
	}

	memset(button,0,sizeof(Button));//clean up all other data
}

void button_draw_all()
{
	int i;

	for(i = 0; i < button_manager.maxButton; i++)
	{
		if(button_manager.buttonList[i].inuse == 0)
		{
			continue;
		}

		button_draw(&button_manager.buttonList[i]);
	}
}

void button_update_all()
{
	int i; 

	for(i = 0; i < button_manager.maxButton; i++)
	{
		if(button_manager.buttonList[i].inuse == 0)
		{
			continue;
		}

		//if button has no update function go to next entity
		if(!button_manager.buttonList[i].update)
		{
			continue;
		}

		button_manager.buttonList[i].update(&button_manager.buttonList[i]);
	}
}

void button_delete_all()
{
	int i;

	for(i = 0; i < button_manager.maxButton; i++)
	{
		button_delete(&button_manager.buttonList[i]);
	}
}

void button_hover(Button *button)
{
	if(button->hover == true)
	{
		button->image = gf2d_sprite_load_image("images/UI/buttonHov.png");
	}
	else if(button->hover == false )//&& button->clicked == false)
	{
		button->image = gf2d_sprite_load_image("images/UI/button.png");
	}
}

void button_clicked(Button *button)
{
	if(button->clicked == true)
	{
		button->image = gf2d_sprite_load_image("images/UI/buttonClicked.png");
	}
	else if(button->hover == false && button->clicked == false)
	{
		button->image = gf2d_sprite_load_image("images/UI/button.png");
	}
}

void button_hover_all()
{
	int i;

	for(i = 0; i < button_manager.maxButton; i++)
	{
		if(button_manager.buttonList[i].inuse == 0)
		{
				return;
		}
		
		button_hover(&button_manager.buttonList[i]);
	}
}