#include <SDL.h>
#include <string.h>
#include "stdio.h"
#include "simple_logger.h"
#include "audio.h"


//Used to manage all the sounds in the game
typedef struct 
{
	//sound list
	Uint32 maxSounds;
	Sound *soundList;

	//music list
	Uint32 maxMusic;
	Music *musicList;
}AudioManager;

static AudioManager audio_manager = {0};

void audio_system_close()
{
	if (audio_manager.soundList != NULL)
    {
		sound_clear_all();
        free(audio_manager.soundList);
    }

	memset(&audio_manager, 0, sizeof(AudioManager));
	
	slog("Audio system is closed.");
}

void audio_system_init(Uint32 maxSounds, Uint32 channels, Uint32 channelGroups, Uint32 maxMusic, Uint8  enableMP3, Uint8  enableOgg)
{
	int flags = 0;

    if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1)
    {
        slog("Failed to open audio: %s\n", SDL_GetError());
        return;
    }

	if (enableMP3)
    {
        flags |= MIX_INIT_MP3;
    }

    if (enableOgg)
    {
        flags |= MIX_INIT_OGG;
    }

    //if (!(Mix_Init(flags) & flags))
    {
     //   slog("failed to initialize some audio support: %s",SDL_GetError());
    }

	//Initializes list for sounds
	if(maxSounds <= 0 || maxMusic <= 0)
	{
		slog("Can't initialize a list of zero sounds or musics");
		return;
	}

	//initialize soundList
	memset(&audio_manager, 0, sizeof(AudioManager));
	audio_manager.soundList = (Sound *)malloc(sizeof(Sound)*maxSounds);

	if(!audio_manager.soundList)
	{
		slog("Failed to allocate sound list");
		audio_system_close();
		return;
	}

	memset(audio_manager.soundList, 0, sizeof(Sound)*maxSounds);
    audio_manager.maxSounds = maxSounds;

	atexit(audio_system_close); 
	atexit(Mix_CloseAudio);
	atexit(Mix_Quit); 

	slog("audio system initialized");
}

Sound *sound_new(char *filename, int loop, int channel)
{
	int i;

	if(!filename)
	{
		return;
	}

	//search through the sound list for an unused address
	for(i = 0; i < audio_manager.maxSounds; i++)
	{
		if(audio_manager.soundList[i].inuse == 0)
		{
			sound_free(&audio_manager.soundList[i]); //cleans up old data
			audio_manager.soundList[i].inuse = 1; //Set ref count to 1. Address is now in use

			//Initialize various attributes of audio here
			audio_manager.soundList[i].filename = filename;
			audio_manager.soundList[i].loop = loop;
			audio_manager.soundList[i].channel = channel;

			audio_manager.soundList[i].chunk = Mix_LoadWAV(audio_manager.soundList[i].filename);
			
			return &audio_manager.soundList[i];
		}
	}

	slog("error: out of addresses for sounds");
	exit(0);
	return NULL;
}

void sound_free(Sound *sound)
{
	if(!sound)
	{
		return;
	}

	sound->inuse = 0;

	Mix_FreeChunk(sound->chunk);
	memset(sound,0,sizeof(Sound));//clean up all other data
}

void sound_play(Sound *sound)
{
	if(!sound)
	{
		return;
	}

	Mix_PlayChannel(sound->channel, sound->chunk, sound->loop);
}

void sound_clear_all()
{
	int i;

	for(i = 0; i < audio_manager.maxSounds; i++)
	{
		sound_free(&audio_manager.soundList[i]);
	}
}