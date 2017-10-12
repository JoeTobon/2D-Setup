#ifndef __GF2D_AUDIO_H__
#define __GF2D_AUDIO_H__

#include <SDL.h>
#include <SDL_mixer.h>
#include "stdio.h"
#include "simple_logger.h"
#include "gf2d_types.h"

/**
 * @struct Sound
 * @brief  struct used to define sound effect (Mix_chunk)
 */
typedef struct Sound_S
{
	Mix_Chunk *chunk;

	Bool inuse;				//Lets the resource manager know if the sound is still needed
	char *filename;		//The filename the sound will be taken from
	int channel;			//The channel this sound is played on
	int loop;				//Number of times the sound will be played
	float volume;			//How loud the sound will play
}Sound;

/**
 * @struct Sound
 * @brief  struct used to define sound effect (Mix_chunk)
 */
typedef struct Music_S
{

	Bool inuse;
	char *filename;


}Music;

/**
 * @brief initializes the audio system based on the passed in parameters
 * @param maxSounds the maximum number of sounds that can be loaded into memory at once
 * @param channels the nrumber of allocated audio channels (excluding music channel)
 * @param channelGroups the number of channels to be reserved for groups to be set up
 * @param maxMusic the number of simultaneous music files that will be supported
 * @param enableMP3 if true, initializes audio system with mp3 support, if available
 * @param enableOgg if true, initializes audio system with ogg vorbis support, if available
 */

void audio_system_init(Uint32 maxSounds, Uint32 channels, Uint32 channelGroups, Uint32 maxMusic, Uint8  enableMP3, Uint8  enableOgg);

/**
 * @brief  intializes a sound and puts it into the sound list
 * @param  filename- the name of the file for the sound
 * @param  loop- the number of times the sound will play
 * @param  channel- the channel the sound will play on
 * @return pointer to a sound of null on error
 */
Sound *sound_new(char *fileName, int loop, int channel);

/**
 * @brief Frees a sound from the audio system
 * @param A pointer to the sound that one wants to free.
 */
void sound_free(Sound *sound);

/**
 * @brief Plays a sound from the audio system
 * @param A pointer to the sound that one wants to free.
 */
void sound_play(Sound *sound);

/**
 * @brief deletes all loaded sounds from memory
 * does not close the entity system
 */
void sound_clear_all();

/**
 * @brief  intializes a music and puts it into the music list
 * @param  
 * @return address to music or null on error
 */
Music *music_new();

/**
 * @brief  deletes music from the music list
 * @param  the music to be deleted
 */
void music_free(Music *music);

/**
 * @brief  plays music from the music list
 * @param  the music to be played
 */
void music_play(Music *music);

/**
 * @brief deletes all loaded music from memory
 * does not close the entity system
 */
void music_clear_all();

#endif

