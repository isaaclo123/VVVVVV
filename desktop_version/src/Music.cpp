/// #include "SDL2_stub.h"
#include "Music.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include "Map.h"

// Variables for streaming music
// static file_t soundFile = NULL;
//
int mix_fading_out = -1;  // -1 = FadingIn , +1 = Fading Out , 0 = Nothing
int mix_fading_ms = -1;
uint mix_playing_music = 0;
int mix_volume = 15;
int mix_loops = -1;
int mix_next = -1;

inline int printf(const char *format, ...) {}

void musicclass::init()
{
    for (int i = 0; i < 28; i++) {
        soundTracks[i] = SoundTrack(SOUND_TRACK_PATHS[i]);
    }

  // print_ram_stats();
  // print_ram_stats();

	safeToProcessMusic= false;
	m_doFadeInVol = false;
	musicVolume = 128;
	FadeVolAmountPerFrame = 0;

	custompd = false;

	currentsong = 0;
	musicfade = 0;
	musicfadein = 0;
	nicechange = 0;
	nicefade = 0;
	resumesong = 0;
	volume = 0.0f;
	fadeoutqueuesong = -1;
	dontquickfade = false;
}

void musicclass::play(int t)
{
    printf("#####\nmusicclass::play %d\n#####currentsong %d\n", t, currentsong);
    t = (t % 16);

    if(mmmmmm)
    {
        if(!usingmmmmmm)
        {
            t += 16;
        }
    }
    safeToProcessMusic = true;
    musicVolume = MIX_MAX_VOLUME;

    if (currentsong == t && Mix_PlayingMusic() != MIX_FADING_OUT)
    {
            return;
    }

    // currentsong = t;

    if (t == -1)
    {
            return;
    }

    if (t < 0 || t >= 16) {
        // invalid t
        // currentsong = -1;
        return;
    }

    Mix_VolumeMusic(musicVolume);


    // if (currentsong == 0 || currentsong == 7 || (!map.custommode && (currentsong == 0+num_mmmmmm_tracks || currentsong == 7+num_mmmmmm_tracks)))
    if (t == 0 || t == 7 || (!map.custommode && (t == 16 || t == 23)))
    {
        // Level Complete theme, no fade in or repeat
        if (playMusic(t, 1, 0)==-1) {
            printf("Mix_FadeInMusic: %s\n", Mix_GetError());
            mix_playing_music = 0;
        }
    }
    else
    {
        if (Mix_FadingMusic() == MIX_FADING_OUT)
        {
            // We're already fading out
            fadeoutqueuesong = t;
            currentsong = -1;
            if (!dontquickfade) {
                Mix_FadeOutMusic(500); // fade out quicker
                mix_fading_out = 1;
                mix_fading_ms = 500;
            } else
                dontquickfade = false;
        }
        else if(playMusic(t, -1, 3000)==-1)
        {
            printf("Mix_FadeInMusic: %s\n", Mix_GetError());
            mix_fading_out = -1;
            mix_next = t;
            mix_loops = -1;
            mix_fading_ms = 3000;
            mix_playing_music = 1;
        }
    }
}

void musicclass::haltdasmusik()
{
    printf("musicclass::haltdasmusik\n");
    resumesong = currentsong;
    Mix_HaltMusic();
    currentsong = -1;
}

void musicclass::silencedasmusik()
{
    printf("musicclass::silencedasmusik\n");
    Mix_VolumeMusic(0);
    mix_volume = 0;
    musicVolume = 0;
}

void musicclass::fadeMusicVolumeIn(int ms)
{
    m_doFadeInVol = true;
    FadeVolAmountPerFrame =  MIX_MAX_VOLUME / (ms / 33);
    // FadeVolAmountPerFrame =  15 / (ms / 33);
    mix_volume = 15;
    Mix_VolumeMusic(128);
}

void musicclass::fadeout()
{
    Mix_FadeOutMusic(2000);
    mix_playing_music = 0;
    // Mix_PauseMusic();
    mix_fading_out = 1;
    mix_fading_ms = 2000;
    currentsong = -1;
}

void musicclass::processmusicfadein()
{
    musicVolume += FadeVolAmountPerFrame;
    Mix_VolumeMusic(musicVolume);

    mix_volume = musicVolume;

    if (musicVolume >= MIX_MAX_VOLUME)
    {
        m_doFadeInVol = false;
    }
}

void musicclass::processmusic()
{
    if(!safeToProcessMusic)
    {
        return;
    }

    //if (fadeoutqueuesong != -1 && Mix_PlayingMusic() == 0) {
    if (fadeoutqueuesong != -1 && mix_playing_music == 0) {
        play(fadeoutqueuesong);
        fadeoutqueuesong = -1;
    }

    //if (nicefade == 1 && Mix_PlayingMusic() == 0)
    if (nicefade == 1 && mix_playing_music == 0)
    {
        play(nicechange);
        nicechange = -1; nicefade = 0;
    }

    if(m_doFadeInVol)
    {
        processmusicfadein();
    }
    // stream_loop();
}


void musicclass::niceplay(int t)
{
    // important: do nothing if the correct song is playing!
    //if((!mmmmmm && currentsong!=t) || (mmmmmm && usingmmmmmm && currentsong!=t) || (mmmmmm && !usingmmmmmm && currentsong!=t+16))
    if(currentsong!=t)
    {
        if(currentsong!=-1)
        {
            dontquickfade = true;
            fadeout();
        }
        nicefade = 1;
        nicechange = t;
    }
}

void musicclass::changemusicarea(int x, int y)
{
    switch(musicroom(x, y))
    {
        case musicroom(11, 4):
            niceplay(2);
            break;

        case musicroom(2, 4):
        case musicroom(7, 15):
            niceplay(3);
            break;

        case musicroom(18, 1):
        case musicroom(15, 0):
            niceplay(12);
            break;

        case musicroom(0, 0):
        case musicroom(0, 16):
        case musicroom(2, 11):
        case musicroom(7, 9):
        case musicroom(8, 11):
        case musicroom(13, 2):
        case musicroom(17, 12):
        case musicroom(14, 19):
        case musicroom(17, 17):
            niceplay(4);
            break;

        default:
            niceplay(1);
            break;
    }
}

void musicclass::playef(int t)
{
    if(Mix_PlayChannel(-1, soundTracks[t].sound, 0) == -1) {
        printf("Mix_PlayChannel: %s\n",Mix_GetError());
    }
}

#ifdef VVV_COMPILEMUSIC

/*
int musicclass::playMusic(int t, int loops, int ms) {
    // printf("Before setPrevMusicTrack:\n");
    MusicTrack tmpMusicTrack = musicTrack;
    // printf("After setPrevMusicTrack:\n");
    // printf("Set new musictrack:\n");
    musicTrack = MusicTrack(MUSIC_TRACK_PATHS[t]);
    // printf("After Set new musictrack:\n");

    if (musicTrack.m_isValid) {
        currentsong = t;
        // printf("before fadein %d\n", musicTrack.m_music);
        int result = Mix_FadeInMusic(musicTrack.m_music, loops, ms);
        // printf("after fadein %d result\n", result);
        // tmpMusicTrack.free();
        return result;
    }

    // free track TODO
    // musicTrack = tmpMusicTrack;

    return -1;
}
*/

int musicclass::playMusic(int t, int loops, int ms) {
    // load music file if not in yet
    if (musicTracks[t].m_isValid) {
        currentsong = t;
        return Mix_FadeInMusic(musicTracks[t].m_music, loops, ms);
    }

    musicTracks[t] = MusicTrack(MUSIC_TRACK_PATHS[t]);

    if (musicTracks[t].m_isValid) {
        currentsong = t;
        return Mix_FadeInMusic(musicTracks[t].m_music, loops, ms);
    }

    return -1;
}

// void musicclass::freeMusic() {
//     for (int i = 0; i < 16; i++) {
//         if (i == currentsong) {
//             // dont free currently playing song
//             continue;
//         }
//         Mix_FreeMusic(musicTracks[i].m_music);
//         musicTracks[i].m_isValid = false;
//     }
// }
#else
int musicclass::playMusic(int t, int loop, int ms) { return -1; }
// void musicclass::freeMusic() {}
#endif
