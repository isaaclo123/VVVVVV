#include <SDL.h>
#include "SoundSystem.h"
#include "FileSystemUtils.h"

#include "SDL2_stub.h"

int MusicTrack::s_num_tracks = 0;

/*
 * * Struct that holds the RIFF data of the Wave file.
 * * The RIFF data is the meta data information that holds,
 * * the ID, size and format of the wave file
 * */
struct RIFF_Header {
  char chunkID[4];
  long chunkSize;  //size not including chunkSize or chunkID
  char format[4];
};

/*
 * * Struct to hold fmt subchunk data for WAVE files.
 * */
struct WAVE_Format {
  char subChunkID[4];
  long subChunkSize;
  short audioFormat;
  short numChannels;
  long sampleRate;
  long byteRate;
  short blockAlign;
  short bitsPerSample;
};

/*
 * * Struct to hold the data of the wave file
 * */
struct WAVE_Data {
  char subChunkID[4];  //should contain the word data
  long subChunk2Size;  //Stores the size of the data block
};

MusicTrack::MusicTrack(const char* fileName)
{
  m_music = s_num_tracks;
  ++s_num_tracks;
  m_isValid = true;
}

SoundTrack::SoundTrack(const char* fileName)
{
    /*
	sound = 0;
  char tmp[256];
  memset(tmp, '\0', 256);
  // Is it an absolute path (e.g: /rd/rescueA.wav)?
  if (fileName[0] == '/') {
    strncpy(tmp, fileName, 256);
  } else {
    strcpy(tmp, FS_PREFIX);
    strcat(tmp, "/");
    strcat(tmp, fileName);
  }

  // bool ret = LoadWavFileAL(tmp, &sound, &size, &frequency, &format);
  // if (ret == false) {
  //       	fprintf(stderr, "Unable to load WAV file: %s\n", fileName);
  //       } else {
  //   printf("Loaded WAV file: %s %d bytes %d Hz %d format\n", fileName, size,
  //          frequency, format);
  // }

  sound = snd_sfx_load(tmp);
  if (sound == SFXHND_INVALID) {
		printf("Unable to load WAV file: %s\n", fileName);
  }
  */
}

SoundSystem::SoundSystem()
{
  return;
}

void SoundSystem::playMusic(MusicTrack* music)
{
    return;
    /*
	if(!music->m_isValid)
	{
		fprintf(stderr, "Invalid music track specified: %d\n", music->m_music);
	}
	//if(Mix_PlayMusic(music->m_music, 0) == -1)
	if(cdrom_cdda_play(music->m_music, music->m_music, 15, CDDA_TRACKS) != ERR_OK)
	{
		//fprintf(stderr, "Unable to play Ogg file: %s\n", Mix_GetError());
		fprintf(stderr, "Unable to play track: %d\n", music->m_music);
	}
        */
}
