#include <melib.h>

#include <SDL.h>
#include <SDL_mixer.h>
#include "SoundSystem.h"

#include <pspkernel.h>
#include <psppower.h>
#include <pspdebug.h>
#include <pspdisplay.h>
#include <pspsdk.h>
// #include <pspthreadman.h>
#include <stdlib.h>
#include <stdio.h>

#include <sys/unistd.h>

#include "UtilityClass.h"
#include "Game.h"
#include "Graphics.h"
#include "KeyPoll.h"
#include "Render.h"

#include "Tower.h"
#include "WarpClass.h"
#include "Labclass.h"
#include "Finalclass.h"
#include "Map.h"

#include "Screen.h"

#include "Script.h"

#include "Logic.h"

#include "Input.h"
#include "editor.h"
#include "preloader.h"

#include "FileSystemUtils.h"
// #include "Network.h"

#include <malloc.h>
#include <stdio.h>
#include <string.h>

scriptclass script;

#if !defined(NO_CUSTOM_LEVELS)
std::vector<edentities> edentity;
editorclass ed;
#endif

UtilityClass help;
Graphics graphics;
musicclass music;
Game game;
KeyPoll key;
mapclass map;
entityclass obj;

int sdl_psp_exit_callback(int arg1, int arg2, void *common)
{
	exit(0);
	return 0;
}

int sdl_psp_callback_thread(SceSize args, void *argp)
{
	int cbid;
	cbid = sceKernelCreateCallback("Exit Callback",
				       sdl_psp_exit_callback, NULL);
	sceKernelRegisterExitCallback(cbid);
	sceKernelSleepThreadCB();
	return 0;
}

int sdl_psp_setup_callbacks(void)
{
	int thid = 0;
	thid = sceKernelCreateThread("update_thread",
				     sdl_psp_callback_thread, 0x11, 0xFA0, 0, 0);
	if(thid >= 0)
		sceKernelStartThread(thid, 0, 0);
	return thid;
}

char baseDir[256];

int towermapwrapper(JobData d) {
    (void) d;
    graphics.drawtowermap_nobackground();
    // graphics.drawtowermap();
    return 0;
}

int inputwrapper(JobData d) {
    (void) d;
    gameinput();
    return 0;
}

int entitywrapper(JobData d) {
    (void) d;
    entityrender();
    return 0;
}

int clearwrapper(JobData d) {
    (void) d;
    FillRect(graphics.backBuffer, 0x000000);
    return 0;
}

int renderwrapper(JobData d) {
    (void) d;
    towerrender();
    return 0;
}

int barwrapper(JobData d) {
    (void) d;
    graphics.cutscenebars();
    return 0;
}

int spikewrapper(JobData d) {
    (void) d;
    graphics.drawtowerspikes();
    return 0;
}

int towerentitydrawwrapper(JobData d) {
    (void) d;
    graphics.drawtowerentities();
return 0;
}

int guiwrapper(JobData d) {
    (void) d;
    graphics.drawgui();
return 0;
}

int blitwrapper(JobData d) {
(void) d;
    BlitSurfaceStandard(graphics.backBuffer, NULL, graphics.tempBuffer, NULL);
return 0;
}

int logicwrapper(JobData d) {
    (void) d;
    towerlogic();
    return 0;
}




int backgroundwrapper(JobData d) {
    (void) d;
    if(!game.colourblindmode)
    {
        graphics.drawbackground(map.background);
    }
    else
    {
        FillRect(graphics.backBuffer,0x00000);
    }
    return 0;
}

int mapwrapper(JobData d) {
    (void) d;
    if (map.final_colormode)
    {
        graphics.drawfinalmap();
    }
    else
    {
        graphics.drawmap();
    }
    return 0;
}

int entitydrawwrapper(JobData d) {
    (void) d;
    graphics.drawentities();
    return 0;
}

inline struct Job* job(JobFunction jobfunc, int execMode) {

    struct Job* j1 = (struct Job*)malloc(sizeof(struct Job));
    j1->jobInfo.id = 1;
    j1->jobInfo.execMode = execMode;
    j1->function = jobfunc;
    j1->data = 0;
    return j1;
}

PSP_MODULE_INFO("VVVVVV", 0, 1, 1);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_VFPU | PSP_THREAD_ATTR_USER);
PSP_HEAP_SIZE_KB(-1024);


int myInt = 77;
int addFunc(JobData data) {
	myInt++;

	return 0; //Result doesn't matter
}

int test(int argc, char* argv[])
{
	pspDebugScreenInit();
        printf("new main\n");

	J_Init(true);


	while (1)
	{
		pspDebugScreenSetXY(0, 0);
		pspDebugScreenPrintf("MY COUNTER: %x\n", myInt);

		struct Job* j = (struct Job*)malloc(sizeof(struct Job));
		j->jobInfo.id = 1;
		j->jobInfo.execMode = MELIB_EXEC_ME;

		j->function = &addFunc;
		j->data = (int)&myInt;

		J_AddJob(j);

		J_DispatchJobs(0.0f); //No dynamic rebalancing so this doesn't matter.
		sceDisplayWaitVblankStart();
	}

	J_Cleanup();

	return 0;
}

int main(int argc, char *argv[])
{
    // SDL main
    // sceDisplaySetMode(PSP_DISPLAY_PIXEL_FORMAT_8888, 480, 272);
    scePowerSetClockFrequency(333,333,166);

    pspDebugScreenInit();
    J_Init(false);

    printf("After J_Init\n");

    sdl_psp_setup_callbacks();
    atexit(sceKernelExitGame);

    // char* baseDir = "ms0:/PSP/GAME/VVVVVV/";
    getcwd(baseDir, 256);
    strcat(baseDir, "/");


    char* assetsPath = NULL;

    // for (int i = 1; i < argc; ++i) {
    //     if (strcmp(argv[i], "-renderer") == 0) {
    //         ++i;
    //         SDL_SetHintWithPriority(SDL_HINT_RENDER_DRIVER, argv[i], SDL_HINT_OVERRIDE);
    //     } else if (strcmp(argv[i], "-basedir") == 0) {
    //         ++i;
    //         baseDir = argv[i];
    //     } else if (strcmp(argv[i], "-assets") == 0) {
    //         ++i;
    //         assetsPath = argv[i];
    //     }
    // }

    if(!FILESYSTEM_init(argv[0], baseDir, assetsPath))
    {
        return 1;
    }

    SDL_Init(
        // SDL_VIDEO_OPENGL |
        SDL_INIT_VIDEO |
        SDL_INIT_AUDIO
        // | SDL_INIT_JOYSTICK
        // | SDL_INIT_GAMECONTROLLER
    );

    int audio_rate = 44100;
    uint16_t audio_format = AUDIO_S16LSB;
    int audio_channels = 2;
    int audio_buffers = 2048;

    int flags=MIX_INIT_OGG|MIX_INIT_MOD;
    int initted;

    printf("SoundSystem Setup\n");

    if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
    {
            printf("Unable to initialize audio subsystem: %s\n", Mix_GetError());
            SDL_assert(0 && "Unable to initialize audio!");
    }

    initted=Mix_Init(flags);
    if(initted&flags != flags) {
        printf("Mix_Init: Failed to init required ogg support!\n");
        printf("Mix_Init: %s\n", Mix_GetError());
    }

    if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0)
    {
            printf("Unable to open audio: %s\n", Mix_GetError());
            SDL_assert(0 && "Unable to initialize audio!");
    }

    Mix_AllocateChannels(16);

    // NETWORK_init();

    Screen gameScreen;

    // printf("\t\t\n");
    // printf("\t\t\n");
    // printf("\t\t       VVVVVV\n");
    // printf("\t\t\n");
    // printf("\t\t\n");
    // printf("\t\t  8888888888888888  \n");
    // printf("\t\t88888888888888888888\n");
    // printf("\t\t888888    8888    88\n");
    // printf("\t\t888888    8888    88\n");
    // printf("\t\t88888888888888888888\n");
    // printf("\t\t88888888888888888888\n");
    // printf("\t\t888888            88\n");
    // printf("\t\t88888888        8888\n");
    // printf("\t\t  8888888888888888  \n");
    // printf("\t\t      88888888      \n");
    // printf("\t\t  8888888888888888  \n");
    // printf("\t\t88888888888888888888\n");
    // printf("\t\t88888888888888888888\n");
    // printf("\t\t88888888888888888888\n");
    // printf("\t\t8888  88888888  8888\n");
    // printf("\t\t8888  88888888  8888\n");
    // printf("\t\t    888888888888    \n");
    // printf("\t\t    8888    8888    \n");
    // printf("\t\t  888888    888888  \n");
    // printf("\t\t  888888    888888  \n");
    // printf("\t\t  888888    888888  \n");
    // printf("\t\t\n");
    // printf("\t\t\n");

    //Set up screen


    // Load Ini


    graphics.init();



    music.init();
    game.init();
    game.infocus = true;

    graphics.MakeTileArray();
    graphics.MakeSpriteArray();
    graphics.maketelearray();


    graphics.images.push_back(graphics.grphx.im_image0);
    graphics.images.push_back(graphics.grphx.im_image1);
    graphics.images.push_back(graphics.grphx.im_image2);
    graphics.images.push_back(graphics.grphx.im_image3);
    graphics.images.push_back(graphics.grphx.im_image4);
    graphics.images.push_back(graphics.grphx.im_image5);
    graphics.images.push_back(graphics.grphx.im_image6);

    graphics.images.push_back(graphics.grphx.im_image7);
    graphics.images.push_back(graphics.grphx.im_image8);
    graphics.images.push_back(graphics.grphx.im_image9);
    graphics.images.push_back(graphics.grphx.im_image10);
    graphics.images.push_back(graphics.grphx.im_image11);
    graphics.images.push_back(graphics.grphx.im_image12);

    const SDL_PixelFormat* fmt = gameScreen.GetFormat();
    graphics.backBuffer = SDL_CreateRGBSurface(SDL_SWSURFACE|SDL_ASYNCBLIT, 320, 240, fmt->BitsPerPixel, fmt->Rmask, fmt->Gmask, fmt->Bmask, fmt->Amask);
    SDL_SetSurfaceBlendMode(graphics.backBuffer, SDL_BLENDMODE_NONE);
    graphics.footerbuffer = SDL_CreateRGBSurface(SDL_SWSURFACE, 320, 10, fmt->BitsPerPixel, fmt->Rmask, fmt->Gmask, fmt->Bmask, fmt->Amask);
    SDL_SetSurfaceBlendMode(graphics.footerbuffer, SDL_BLENDMODE_BLEND);
    // SDL_SetSurfaceAlphaMode(graphics.footerbuffer, 127);

    SDL_SetColorKey(graphics.footerbuffer, SDL_SRCCOLORKEY | SDL_RLEACCEL, SDL_MapRGB(graphics.footerbuffer->format, 0, 0, 0));
    FillRect(graphics.footerbuffer, SDL_MapRGB(fmt, 0, 0, 0));
    graphics.Makebfont();


    graphics.foregroundBuffer =  SDL_CreateRGBSurface(SDL_SWSURFACE,320 ,240 ,fmt->BitsPerPixel,fmt->Rmask,fmt->Gmask,fmt->Bmask,fmt->Amask  );
    SDL_SetSurfaceBlendMode(graphics.foregroundBuffer, SDL_BLENDMODE_NONE);

    graphics.screenbuffer = &gameScreen;

    graphics.menubuffer = SDL_CreateRGBSurface(SDL_SWSURFACE ,320 ,240 ,fmt->BitsPerPixel,fmt->Rmask,fmt->Gmask,fmt->Bmask,fmt->Amask );
    SDL_SetSurfaceBlendMode(graphics.menubuffer, SDL_BLENDMODE_NONE);

    graphics.towerbuffer =  SDL_CreateRGBSurface(SDL_SWSURFACE ,320 ,240 ,fmt->BitsPerPixel,fmt->Rmask,fmt->Gmask,fmt->Bmask,fmt->Amask  );
    SDL_SetSurfaceBlendMode(graphics.towerbuffer, SDL_BLENDMODE_NONE);

    graphics.tempBuffer = SDL_CreateRGBSurface(SDL_SWSURFACE ,320 ,240 ,fmt->BitsPerPixel,fmt->Rmask,fmt->Gmask,fmt->Bmask,fmt->Amask  );
    SDL_SetSurfaceBlendMode(graphics.tempBuffer, SDL_BLENDMODE_NONE);

    game.gamestate = PRELOADER;

    game.menustart = false;
    game.mainmenu = 0;

    map.ypos = (700-29) * 8;
    map.bypos = map.ypos / 2;

    //Moved screensetting init here from main menu V2.1
    game.loadstats();
    if (game.skipfakeload)
        game.gamestate = TITLEMODE;
    if(game.usingmmmmmm==0) music.usingmmmmmm=false;
    if(game.usingmmmmmm==1) music.usingmmmmmm=true;
    if (game.slowdown == 0) game.slowdown = 30;

    switch(game.slowdown){
        case 30: game.gameframerate=34; break;
        case 24: game.gameframerate=41; break;
        case 18: game.gameframerate=55; break;
        case 12: game.gameframerate=83; break;
        default: game.gameframerate=34; break;
    }

    /*
    //Check to see if you've already unlocked some achievements here from before the update
    if (game.swnbestrank > 0){
        if(game.swnbestrank >= 1) NETWORK_unlockAchievement("vvvvvvsupgrav5");
        if(game.swnbestrank >= 2) NETWORK_unlockAchievement("vvvvvvsupgrav10");
        if(game.swnbestrank >= 3) NETWORK_unlockAchievement("vvvvvvsupgrav15");
        if(game.swnbestrank >= 4) NETWORK_unlockAchievement("vvvvvvsupgrav20");
        if(game.swnbestrank >= 5) NETWORK_unlockAchievement("vvvvvvsupgrav30");
        if(game.swnbestrank >= 6) NETWORK_unlockAchievement("vvvvvvsupgrav60");
    }

    if(game.unlock[5]) NETWORK_unlockAchievement("vvvvvvgamecomplete");
    if(game.unlock[19]) NETWORK_unlockAchievement("vvvvvvgamecompleteflip");
    if(game.unlock[20]) NETWORK_unlockAchievement("vvvvvvmaster");

    if (game.bestgamedeaths > -1) {
        if (game.bestgamedeaths <= 500) {
            NETWORK_unlockAchievement("vvvvvvcomplete500");
        }
        if (game.bestgamedeaths <= 250) {
            NETWORK_unlockAchievement("vvvvvvcomplete250");
        }
        if (game.bestgamedeaths <= 100) {
            NETWORK_unlockAchievement("vvvvvvcomplete100");
        }
        if (game.bestgamedeaths <= 50) {
            NETWORK_unlockAchievement("vvvvvvcomplete50");
        }
    }

    if(game.bestrank[0]>=3) NETWORK_unlockAchievement("vvvvvvtimetrial_station1_fixed");
    if(game.bestrank[1]>=3) NETWORK_unlockAchievement("vvvvvvtimetrial_lab_fixed");
    if(game.bestrank[2]>=3) NETWORK_unlockAchievement("vvvvvvtimetrial_tower_fixed");
    if(game.bestrank[3]>=3) NETWORK_unlockAchievement("vvvvvvtimetrial_station2_fixed");
    if(game.bestrank[4]>=3) NETWORK_unlockAchievement("vvvvvvtimetrial_warp_fixed");
    if(game.bestrank[5]>=3) NETWORK_unlockAchievement("vvvvvvtimetrial_final_fixed");
    */

    obj.init();

    volatile Uint32 time, timePrev = 0;
    game.infocus = true;
    key.isActive = true;

    while(!key.quitProgram)
    {
        time = SDL_GetTicks();

        // Update network per frame.
        // NETWORK_update();

        //framerate limit to 30
        Uint32 timetaken = time - timePrev;
        if(game.gamestate==EDITORMODE)
        {
            if (timetaken < 34)
            {
                volatile Uint32 delay = 1000 * (34 - timetaken);
                // SDL_Delay( delay );
                sceKernelDelayThread(delay);
                time = SDL_GetTicks();
            }
            timePrev = time;

        }else{
            if (timetaken < game.gameframerate)
            {
                volatile Uint32 delay = 1000 * (game.gameframerate - timetaken);
                // SDL_Delay( delay );
                sceKernelDelayThread(delay);
                time = SDL_GetTicks();
            }
            timePrev = time;
        }



        key.Poll();
        if(key.toggleFullscreen)
        {
            if(!gameScreen.isWindowed)
            {
                SDL_ShowCursor(SDL_DISABLE);
                SDL_ShowCursor(SDL_ENABLE);
            }
            else
            {
                SDL_ShowCursor(SDL_ENABLE);
            }


            if(game.gamestate == EDITORMODE)
            {
                SDL_ShowCursor(SDL_ENABLE);
            }

            gameScreen.toggleFullScreen();
            game.fullscreen = !game.fullscreen;
            key.toggleFullscreen = false;

            key.keymap.clear(); //we lost the input due to a new window.
            game.press_left = false;
            game.press_right = false;
            game.press_action = true;
            game.press_map = false;
        }

        game.infocus = key.isActive;
        if(!game.infocus)
        {
            if(game.getGlobalSoundVol()> 0)
            {
                game.setGlobalSoundVol(0);
            }
            FillRect(graphics.backBuffer, 0x00000000);
            graphics.bprint(5, 110, "Game paused", 196 - help.glow, 255 - help.glow, 196 - help.glow, true);
            graphics.bprint(5, 120, "[click to resume]", 196 - help.glow, 255 - help.glow, 196 - help.glow, true);
            graphics.bprint(5, 230, "Press M to mute in game", 164 - help.glow, 196 - help.glow, 164 - help.glow, true);
            graphics.render();
            //We are minimised, so lets put a bit of a delay to save CPU
            SDL_Delay(100);
        }
        else
        {
            switch(game.gamestate)
            {
            case PRELOADER:
                //Render
                preloaderrender();
                break;
#if !defined(NO_CUSTOM_LEVELS)
            case EDITORMODE:
                graphics.flipmode = false;
                //Input
                editorinput();
                //Render
                editorrender();
                ////Logic
                editorlogic();
                break;
#endif
            case TITLEMODE:
                //Input
                titleinput();
                //Render
                titlerender();
                ////Logic
                titlelogic();
                break;
            case GAMEMODE:
                if (map.towermode)
                {
                    gameinput();

                    J_AddJob(job(entitywrapper, MELIB_EXEC_DEFAULT));
                    J_AddJob(job(clearwrapper, MELIB_EXEC_DEFAULT));
                    J_AddJob(job(towermapwrapper, MELIB_EXEC_DEFAULT));
                    J_AddJob(job(barwrapper, MELIB_EXEC_DEFAULT));
                    J_AddJob(job(spikewrapper, MELIB_EXEC_DEFAULT));
                    J_AddJob(job(towerentitydrawwrapper, MELIB_EXEC_DEFAULT));
                    J_AddJob(job(blitwrapper, MELIB_EXEC_DEFAULT));
                    J_AddJob(job(guiwrapper, MELIB_EXEC_DEFAULT));

                    J_Update(0.1f); //No dynamic rebalancing so this doesn't matter.

                    towerrender();
                    towerlogic();

                }
                else
                {

                    if (script.running)
                    {
                        script.run();
                    }

                    gameinput();

                    if(!game.completestop) {
                        // entityrender();
                        J_AddJob(job(entitywrapper, MELIB_EXEC_DEFAULT));
                        J_AddJob(job(backgroundwrapper, MELIB_EXEC_DEFAULT));
                        J_AddJob(job(mapwrapper, MELIB_EXEC_DEFAULT));
                        // graphics.drawentities();
                        // J_AddJob(job(entitydrawwrapper, MELIB_EXEC_DEFAULT));

                        J_Update(0.1f); //No dynamic rebalancing so this doesn't matter.
                    }
                    //
                    // J_AddJob(job(entitydrawwrapper, MELIB_EXEC_DEFAULT));
                    graphics.drawentities();

                    gamerender();
                    gamelogic();


                    break;
                case MAPMODE:
                    maprender();
                    mapinput();
                    maplogic();
                    break;
                case TELEPORTERMODE:
                    teleporterrender();
                    if(game.useteleporter)
                    {
                        teleporterinput();
                    }
                    else
                    {
                        if (script.running)
                        {
                            script.run();
                        }
                        gameinput();
                    }
                    maplogic();
                    break;
                case GAMECOMPLETE:
                    gamecompleterender();
                    //Input
                    gamecompleteinput();
                    //Logic
                    gamecompletelogic();
                    break;
                case GAMECOMPLETE2:
                    gamecompleterender2();
                    //Input
                    gamecompleteinput2();
                    //Logic
                    gamecompletelogic2();
                    break;
                case CLICKTOSTART:
                    help.updateglow();
                    break;
                default:

                break;
                }

            }

        }

        //We did editorinput, now it's safe to turn this off
        key.linealreadyemptykludge = false;

        if (game.savemystats)
        {
            game.savemystats = false;
            game.savestats();
        }

        //Mute button
#if !defined(NO_CUSTOM_LEVELS)
        bool inEditor = ed.textentry || ed.scripthelppage == 1;
#else
        bool inEditor = false;
#endif
        if (key.isDown(KEYBOARD_m) && game.mutebutton<=0 && !inEditor)
        {
            game.mutebutton = 8;
            if (game.muted)
            {
                game.muted = false;
            }
            else
            {
                game.muted = true;
            }
        }
        if(game.mutebutton>0)
        {
            game.mutebutton--;
        }

        if (game.muted)
        {
            game.globalsound = 0;
            Mix_VolumeMusic(0) ;
            Mix_Volume(-1,0);
        }

        if (!game.muted && game.globalsound == 0)
        {
            game.globalsound = 1;
            Mix_VolumeMusic(MIX_MAX_VOLUME) ;
            Mix_Volume(-1,MIX_MAX_VOLUME);
        }

        if (key.resetWindow)
        {
            key.resetWindow = false;
            gameScreen.ResizeScreen(-1, -1);
        }

        music.processmusic();
        graphics.processfade();
        game.gameclock();
        gameScreen.FlipScreen();
    }

    game.savestats();
    J_Cleanup();
    // NETWORK_shutdown();
    SDL_Quit();
    FILESYSTEM_deinit();

    return 0;
}
