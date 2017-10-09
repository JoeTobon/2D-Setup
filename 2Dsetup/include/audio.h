#ifndef __GF2D_AUDIO_H__
#define __GF2D_AUDIO_H__

#include <SDL.h>
#include <SDL_mixer.h>
#include "stdio.h"
#include "simple_logger.h"
#include "gf2d_types.h"

typedef struct Sound_S
{
	Bool inuse;				//Lets the resource manager know if the sound is still needed
	char filename[100];		//The filename the sound will be taken from
	int channel;			//The channel this sound is played on
	int loop;				//Number of times the sound will be played
	float volume;			//How loud the sound will play
}Sound;

/**
 * @brief initializes the audio system based on the passed in parameters
 * @param maxSounds the maximum number of sounds that can be loaded into memory at once
 * @param channels the nrumber of allocated audio channels (excluding music channel)
 * @param channelGroups the number of channels to be reserved for groups to be set up
 * @param maxMusic the number of simultaneous music files that will be supported
 * @param enableMP3 if true, initializes audio system with mp3 support, if available
 * @param enableOgg if true, initializes audio system with ogg vorbis support, if available
 */

void gf2d_audio_init(Uint32 maxSounds, Uint32 channels, Uint32 channelGroups, Uint32 maxMusic, Uint8  enableMP3, Uint8  enableOgg);

#endif

