/// #include "SDL2_stub.h"
#include "Music.h"
#include <SDL.h>
#include <stdio.h>
#include "BinaryBlob.h"
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

int stream_setup(SoundTrack* t) {
    if(Mix_PlayChannel(-1, t->sound, -1)) {
        printf("Mix_PlayChannel: %s\n",Mix_GetError());
        return 0;
    }
    return 1;
}

void musicclass::init()
{
    // load support for the OGG and MOD sample/music formats
    // int flags=MIX_INIT_OGG;
    // int initted=Mix_Init(flags);
    // if(initted&flags != flags) {
    //     printf("Mix_Init: Failed to init required ogg and mod support!\n");
    //     printf("Mix_Init: %s\n", Mix_GetError());
    // }

    // if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024)==-1) {
    //     printf("Mix_OpenAudio: %s\n", Mix_GetError());
    // }

    // Mix_AllocateChannels(32);

	soundTracks.push_back(SoundTrack( "sounds/jump.wav" ));
	soundTracks.push_back(SoundTrack( "sounds/jump2.wav" ));
	soundTracks.push_back(SoundTrack( "sounds/hurt.wav" ));
	soundTracks.push_back(SoundTrack( "sounds/souleyeminijingle.wav" ));
	soundTracks.push_back(SoundTrack( "sounds/coin.wav" ));
	soundTracks.push_back(SoundTrack( "sounds/save.wav" ));
	soundTracks.push_back(SoundTrack( "sounds/crumble.wav" ));
	soundTracks.push_back(SoundTrack( "sounds/vanish.wav" ));
	soundTracks.push_back(SoundTrack( "sounds/blip.wav" ));
	soundTracks.push_back(SoundTrack( "sounds/preteleport.wav" ));
	soundTracks.push_back(SoundTrack( "sounds/teleport.wav" ));
	soundTracks.push_back(SoundTrack( "sounds/crew1.wav" ));
	soundTracks.push_back(SoundTrack( "sounds/crew2.wav" ));
	soundTracks.push_back(SoundTrack( "sounds/crew3.wav" ));
	soundTracks.push_back(SoundTrack( "sounds/crew4.wav" ));
	soundTracks.push_back(SoundTrack( "sounds/crew5.wav" ));
	soundTracks.push_back(SoundTrack( "sounds/crew6.wav" ));
	soundTracks.push_back(SoundTrack( "sounds/terminal.wav" ));
	soundTracks.push_back(SoundTrack( "sounds/gamesaved.wav" ));
	soundTracks.push_back(SoundTrack( "sounds/crashing.wav" ));
	soundTracks.push_back(SoundTrack( "sounds/blip2.wav" ));
	soundTracks.push_back(SoundTrack( "sounds/countdown.wav" ));
	soundTracks.push_back(SoundTrack( "sounds/go.wav" ));
	soundTracks.push_back(SoundTrack( "sounds/crash.wav" ));
	soundTracks.push_back(SoundTrack( "sounds/combine.wav" ));
	soundTracks.push_back(SoundTrack( "sounds/newrecord.wav" ));
	soundTracks.push_back(SoundTrack( "sounds/trophy.wav" ));
	soundTracks.push_back(SoundTrack( "sounds/rescue.wav" ));


  // print_ram_stats();

#ifdef VVV_COMPILEMUSIC
	binaryBlob musicWriteBlob;
	musicWriteBlob.AddFileToBinaryBlob("data/music/0levelcomplete.ogg");
	musicWriteBlob.AddFileToBinaryBlob("data/music/1pushingonwards.ogg");
	musicWriteBlob.AddFileToBinaryBlob("data/music/2positiveforce.ogg");
	musicWriteBlob.AddFileToBinaryBlob("data/music/3potentialforanything.ogg");
	musicWriteBlob.AddFileToBinaryBlob("data/music/4passionforexploring.ogg");
	musicWriteBlob.AddFileToBinaryBlob("data/music/5intermission.ogg");
	musicWriteBlob.AddFileToBinaryBlob("data/music/6presentingvvvvvv.ogg");
	musicWriteBlob.AddFileToBinaryBlob("data/music/7gamecomplete.ogg");
	musicWriteBlob.AddFileToBinaryBlob("data/music/8predestinedfate.ogg");
	musicWriteBlob.AddFileToBinaryBlob("data/music/9positiveforcereversed.ogg");
	musicWriteBlob.AddFileToBinaryBlob("data/music/10popularpotpourri.ogg");
	musicWriteBlob.AddFileToBinaryBlob("data/music/11pipedream.ogg");
	musicWriteBlob.AddFileToBinaryBlob("data/music/12pressurecooker.ogg");
	musicWriteBlob.AddFileToBinaryBlob("data/music/13pacedenergy.ogg");
	musicWriteBlob.AddFileToBinaryBlob("data/music/14piercingthesky.ogg");
	musicWriteBlob.AddFileToBinaryBlob("data/music/predestinedfatefinallevel.ogg");

	musicWriteBlob.writeBinaryBlob("data/BinaryMusic.vvv");
#endif

	binaryBlob musicReadBlob;

  // print_ram_stats();

  // for (int i = 0; i < 32; ++i) {
  //   SFX[i].t = NULL;
  //   SFX[i].thread = NULL;
  //   SFX[i].pos = 0;
  // }

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
    printf("#####\nmusicclass::play %d\n#####\n", t);
    t = (t % 16);

    if(mmmmmm)
    {
        if(!usingmmmmmm)
        {
            t += 16;
        }
    }
    safeToProcessMusic = true;
    Mix_VolumeMusic(128);
    // mix_volume = 15;
    // spu_cdda_volume(15, 15);
    if (currentsong !=t)
    {
        if (t != -1)
        {
            currentsong = t;
            if (currentsong == 0 || currentsong == 7 || (!map.custommode && (currentsong == 16 || currentsong == 23)))
            {
                // Level Complete theme, no fade in or repeat
                mix_next = t;
                mix_loops = 0;
                mix_fading_ms = 0;

                mix_playing_music = 1;
                if(Mix_FadeInMusic(musicTracks[t].m_music, 0, 0)==-1)
                {
                    printf("Mix_PlayMusic: %s\n", Mix_GetError());
                    mix_playing_music = 0;
                }

            }
            else
            {
                if (mix_fading_out == 1) {
                    if (Mix_FadingMusic() == MIX_FADING_OUT) {
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
                    else if(Mix_FadeInMusic(musicTracks[t].m_music, -1, 3000)==-1)
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
        }
    } else {
        // TODO unsure
        currentsong = -1;
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
    //Mix_FadeOutMusic(2000);
    mix_playing_music = 0;
    Mix_PauseMusic();
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

    // Actual processing of mix variables
    if (mix_fading_out == 1) {  // FadingOut
        int fade_amount =  mix_volume / (mix_fading_ms / 33);
        mix_volume -= fade_amount;
        if (mix_volume < 0) mix_volume = 0;
        Mix_VolumeMusic(mix_volume);
            mix_fading_ms -= 33;
        if (mix_fading_ms <= 0) {
            mix_fading_ms = 0;
            mix_fading_out = 0;
        }
    } else if (mix_fading_out == -1) {  // FadingIn
        int fade_amount =  mix_volume / (mix_fading_ms / 33);
        mix_volume += fade_amount;
        if (mix_volume > 15) mix_volume = 15;
        Mix_VolumeMusic(mix_volume);
            mix_fading_ms -= 33;
        if (mix_fading_ms <= 0) {
            mix_fading_ms = 0;
            mix_fading_out = 0;
        }
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
    //thd_create(1, (void*)stream_thread, (void*)&soundTracks[t]);
    stream_setup(&soundTracks[t]);
}
