#include <SDL.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include "SoundSystem.h"
#include "FileSystemUtils.h"

#include "SDL2_stub.h"

MusicTrack::MusicTrack(const char* fileName)
{
	// unsigned char *mem;
	// size_t length = 0;
	// FILESYSTEM_loadFileToMemory(fileName, &mem, &length);
	// SDL_RWops *fileIn = SDL_RWFromMem(mem, length);

	// m_music = Mix_LoadMUS_RW(fileIn);
	// m_isValid = true;

        // printf("LoadOgg: %s loaded\n", fileName);;

	// if (length)
	// {
	// 	FILESYSTEM_freeMemory(&mem);
	// }

	// if(m_music == NULL)
	// {
	// 	printf("Unable to load Ogg Music file: %s\n", Mix_GetError());;
	// 	m_isValid = false;
	// }
        char* basename = "ms0:/PSP/GAME/VVVVVV/";
        char path[100];
        strcpy(path, basename);
        strcat(path, fileName);

        printf("LoadOgg: %s\n", path);

	m_music = Mix_LoadMUS(path);
	m_isValid = true;
	if(m_music == NULL)
	{
		printf("Unable to load Ogg Music file: %s\n", Mix_GetError());;
		m_isValid = false;
	}
}

MusicTrack::MusicTrack(SDL_RWops *rw)
{
	m_music = Mix_LoadMUS_RW(rw);
	m_isValid = true;
	if(m_music == NULL)
	{
		printf("Unable to load Magic Binary Music file: %s\n", Mix_GetError());
		m_isValid = false;
	}
}

MusicTrack::MusicTrack() {
    m_isValid = false;
    m_music = NULL;
}

void MusicTrack::free() {
    if (m_isValid && m_music != NULL) {
        Mix_FreeMusic(m_music);
    }
    m_isValid = false;
    m_music = NULL;
}

SoundTrack::SoundTrack(const char* fileName)
{
	sound = NULL;

	unsigned char *mem;
	size_t length = 0;
	FILESYSTEM_loadFileToMemory(fileName, &mem, &length);
	SDL_RWops *fileIn = SDL_RWFromMem(mem, length);
	sound = Mix_LoadWAV_RW(fileIn, 1);
	if (length)
	{
		FILESYSTEM_freeMemory(&mem);
	}

	if (sound == NULL)
	{
		printf("Unable to load WAV file: %s\n", Mix_GetError());
	}
}

SoundTrack::SoundTrack() {
    sound = NULL;
}

SoundSystem::SoundSystem() {}

int SoundSystem::playMusic(MusicTrack* music)
{
	if(!music->m_isValid)
	{
		printf("Invalid mix specified: %s\n", Mix_GetError());
                return -1;
	}
	if(Mix_PlayMusic(music->m_music, 0) == -1)
	{
		printf("Unable to play Ogg file: %s\n", Mix_GetError());
                return -1;
	}
        return 0;
}
