#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "game.h"
#include "gameplay.h"
#include "timer.h"
#include "util.h"

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const string TITLE = "Earthquake Survival";

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* Message[5];
SDL_Surface* screen;
bool gamePlay = 1, onMenu = 1, hardMode = 0;
int numberLand;

void openResources();

int main(int argc, char* argv[])
{
    openResources();
    while(gamePlay) {
        if(onMenu)
            renderMenu(window, renderer, gamePlay, onMenu, hardMode);
        else {
            gamePlay = 0;
            if(!hardMode) numberLand = 5;
            else numberLand = 2;
            renderGame(window, renderer, screen, gamePlay, onMenu, numberLand);
        }
    }
    unloadComponents(window, renderer);
    return 0;
}

void openResources() {
    TTF_Init();
    IMG_Init(IMG_INIT_JPG);
    IMG_Init(IMG_INIT_PNG);
    Mix_Init(MIX_INIT_MP3);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    SDL_Init(SDL_INIT_VIDEO);
    initSDL(window, renderer, TITLE, SCREEN_WIDTH, SCREEN_HEIGHT);
    screen = SDL_GetWindowSurface(window);
    for(int i=0; i<5; i++) {
        Message[i] = NULL;
    }
}
