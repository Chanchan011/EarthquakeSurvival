#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "gameplay.h"
#include "util.h"
using namespace std;

TTF_Font* gameFont = NULL;
SDL_Color color;
SDL_Surface* surfaceMessage = NULL;
SDL_Surface* surfaceImage = NULL;
SDL_Texture* image = NULL;
SDL_Texture* text = NULL;
Mix_Music* music = NULL;
bool isPlay = 0, isHelp = 0, isExit = 0, isBack = 0;

void renderMenu(SDL_Window* window, SDL_Renderer* renderer, bool &gamePlay, bool &onMenu, bool &hardMode);

void createMessage(const char* s, int fontSize, SDL_Color color, int x, int y, SDL_Texture* mess,
                   SDL_Renderer* renderer)
{
    int iW, iH;
    gameFont = TTF_OpenFont("Font.ttf", fontSize);
    surfaceMessage = TTF_RenderText_Solid(gameFont, s, color);
    mess = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
    SDL_QueryTexture(mess, NULL, NULL, &iW, &iH);
    x -= iW / 2;
    y -= iH / 2;
    renderTexture(mess, renderer, x, y, iW, iH);
}

void loadImage(const char* path, SDL_Renderer* renderer) {
    surfaceImage = IMG_Load(path);
    image = SDL_CreateTextureFromSurface(renderer, surfaceImage);
    SDL_RenderCopy(renderer, image, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void unloadComponents(SDL_Window* &window, SDL_Renderer* &renderer) {
    TTF_CloseFont(gameFont);
    TTF_Quit();
    IMG_Quit();
    Mix_FreeMusic(music);
	Mix_CloseAudio();
    SDL_FreeSurface(surfaceMessage);
    quitSDL(window, renderer);
}

void getHelp(SDL_Window* &window, SDL_Renderer* &renderer, bool &gamePlay, bool &onMenu, bool &hardMode) {
    SDL_RenderClear(renderer);
    loadImage("images/help.jpg", renderer);
    createMessage("Back", 24, {155, 203, 254}, 50, 30, text, renderer);
    SDL_RenderPresent(renderer);
    SDL_Event e;
    while (true) {
        SDL_Delay(10);
        if ( SDL_WaitEvent(&e) == 0) continue;
        if ((e.type == SDL_QUIT) || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
            unloadComponents(window, renderer);
            exit(0);
        }
        if (e.type == SDL_MOUSEMOTION) {
            int x = e.button.x;
            int y = e.button.y;
            if((x >= 22 && x <= 78) && (y >= 2 && y <= 58)) isBack = 1;
            else isBack = 0;

            if(isBack) createMessage("Back", 24, {255, 106, 106}, 50, 30, text, renderer);
            else createMessage("Back", 24, {155, 203, 254}, 50, 30, text, renderer);
            SDL_RenderPresent(renderer);
        }
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            int x = e.button.x;
            int y = e.button.y;
            if((x >= 22 && x <= 78) && (y >= 2 && y <= 58)) {
                renderMenu(window, renderer, gamePlay, onMenu, hardMode);
                break;
            }
        }
    }
}

void playMusic(const char* path) {
    music = Mix_LoadMUS(path);
    Mix_PlayMusic(music, -1);
}

void renderMenu(SDL_Window* window, SDL_Renderer* renderer, bool &gamePlay, bool &onMenu, bool &hardMode) {
    SDL_RenderClear(renderer);
    loadImage("images/mainmenu.jpg", renderer);
    createMessage("Earthquake Survival", 38, {155, 203, 254}, 400, 170, text, renderer);
    createMessage("Play", 30, {255, 106, 106}, 400, 270, text, renderer);
    //createMessage("Mode: Hard", 24, {255, 106, 106}, 400, 320, text, renderer);
    createMessage("Help", 24, {255, 106, 106}, 400, 360, text, renderer);
    createMessage("Exit", 24, {255, 36, 0}, 400, 400, text, renderer);

    renderTexture(loadTexture("images/easy1.bmp", renderer), renderer, 320, 300, 175, 42);
    SDL_RenderPresent(renderer);
    if(!Mix_PlayingMusic()) playMusic("music/backgroundMusic.wav");

    SDL_Event e;
    while (true) {
        SDL_Delay(10);
        if ( SDL_WaitEvent(&e) == 0) continue;
        if ((e.type == SDL_QUIT) || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
            unloadComponents(window, renderer);
            exit(0);
        }
        if (e.type == SDL_MOUSEMOTION) {
            int x = e.button.x;
            int y = e.button.y;
            if((x >= 372 && x <= 428) && (y >= 389 && y <= 411)) isExit = 1;
            else isExit = 0;
            if((x >= 372 && x <= 428) && (y >= 349 && y <= 371)) isHelp = 1;
            else isHelp = 0;
            if((x >= 364 && x <= 436) && (y >= 257 && y <= 283)) isPlay = 1;
            else isPlay = 0;

            if(isExit) createMessage("Exit", 24, {200, 200, 211}, 400, 400, text, renderer);
            else createMessage("Exit", 24, {255, 36, 0}, 400, 400, text, renderer);
            if(isHelp) createMessage("Help", 24, {200, 200, 211}, 400, 360, text, renderer);
            else createMessage("Help", 24, {255, 106, 106}, 400, 360, text, renderer);
            if(isPlay) createMessage("Play", 30, {200, 200, 211}, 400, 270, text, renderer);
            else createMessage("Play", 30, {255, 106, 106}, 400, 270, text, renderer);
            SDL_RenderPresent(renderer);
        }
        else if (e.type == SDL_MOUSEBUTTONDOWN) {
            int x = e.button.x;
            int y = e.button.y;
            if((x >= 372 && x <= 428) && (y >= 389 && y <= 411)) {
                unloadComponents(window, renderer);
                exit(0);
            }
            if((x >= 372 && x <= 428) && (y >= 349 && y <= 371)) {
                getHelp(window, renderer, gamePlay, onMenu, hardMode);
            }
            if((x >= 320 && x <= 495) && (y >= 300 && y <= 342)) {
                if(hardMode)
                {
                    renderTexture(loadTexture("images/easy1.bmp", renderer), renderer, 320, 300, 175, 42);
                    hardMode = 0;
                }
                else
                {
                    renderTexture(loadTexture("images/hard1.bmp", renderer), renderer, 320, 300, 175, 42);
                    hardMode = 1;
                }
                SDL_RenderPresent(renderer);
            }
            if((x >= 364 && x <= 436) && (y >= 257 && y <= 283)) {
                onMenu = 0;
                return ;
            }
        }
    }
}
