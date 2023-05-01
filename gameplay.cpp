#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "game.h"
#include "timer.h"
#include "util.h"
using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int SPRITE_SIZE = 32;
const int SPRITE_STEP = 10;
const int DIR_UP = 0;
const int DIR_RIGHT = 1;
const int DIR_DOWN = 2;
const int DIR_LEFT = 3;
const int GAME_SIZE = 3;
const int landWidth = 150, landHeight = 100;
bool landCorrupt[GAME_SIZE][GAME_SIZE];
int corruptTime[GAME_SIZE][GAME_SIZE];
bool isYes = 0, isNo = 0;
int x, y, currentDir = DIR_RIGHT, animationFlip = 0;
int gameTime = 0, libraryTime = 0, checkTime = 0, lastPlay = 0;
vector<pair<int, int>> corruptPos;
SDL_Texture* mess, *spriteTexture, *background;
SDL_Rect spritePos, spriteImage;
SDL_Surface *temp;
SDL_Surface* imageLoader;
SDL_Texture* aniImage, *landImage, *paneImage;
SDL_Rect aniRect;
string s;
const char* timeDisplay;
Mix_Music* endingMusic = NULL;

string calculateTimer(double gameTime) {
    int x = floor(gameTime / 1000);
    int minute, second;
    minute = x/60, second = x%60;
    string minString = "", secString = "";
    if(minute < 10) minString = "0" + to_string(minute);
    else minString = to_string(minute);
    if(second < 10) secString = "0" + to_string(second);
    else secString = to_string(second);
    return minString + ":" + secString;
}

vector<pair<int, int>> getLandtoCorrupt(int numberInitLand) {
    srand(time(0));
    vector <int> v;
    for(int i=0; i<GAME_SIZE; i++) {
        for(int j=0; j<GAME_SIZE; j++)
            if(!landCorrupt[i][j])
                v.push_back(i*10 + j);
    }
    int a[10];
    for(int i=0; i<(int)v.size(); i++)
        a[i] = i;
    random_shuffle(a, a+(int)v.size());
    vector <pair<int, int>> r;
    for(int i = 0; i< (int)v.size() - numberInitLand; i++) {
        y = v[a[i]] % 10;
        x = (v[a[i]] - y) / 10;
        r.push_back({x, y});
    }
    return r;
}

bool onCrashLand(int posX, int posY) {
    for(int i=0; i<(int)corruptPos.size(); i++) {
        int landX = 175 + corruptPos[i].second*landWidth;
        int landY = 150 + corruptPos[i].first*landHeight;
        if(posX >= landX && posX <= landX + landWidth && posY >= landY &&
           posY <= landY + landHeight && landCorrupt[corruptPos[i].first][corruptPos[i].second])
            return 1;
    }
    return 0;
}

void renderAnimation(int typeofAni, int i, int j, SDL_Renderer* renderer) {
    imageLoader = IMG_Load(("images/ani" + to_string(typeofAni) + ".png").c_str());
    aniImage = SDL_CreateTextureFromSurface(renderer, imageLoader);
    aniRect.w = landWidth;
    aniRect.h = landHeight;
    aniRect.x = 175 + j*landWidth;
    aniRect.y = 150 + i*landHeight;
    SDL_RenderCopy(renderer, aniImage, NULL, &aniRect);
}

void renderCorruptLand(vector <pair<int, int>> &corruptPos, int playTime, SDL_Renderer* renderer) {
    for(int i=0; i<(int)corruptPos.size(); i++) {
        int typeofAni;
        x = corruptPos[i].first;
        y = corruptPos[i].second;
        corruptTime[x][y] = playTime;
        if(corruptTime[x][y] >= 1000 && corruptTime[x][y] < 2000) typeofAni = 1;
        else if(corruptTime[x][y] >= 2000 && corruptTime[x][y] < 3000) typeofAni = 2;
        else if(corruptTime[x][y] >= 3000) {
            typeofAni = 3;
            landCorrupt[x][y] = 1;
        }
        else typeofAni = 0;
        renderAnimation(typeofAni, x, y, renderer);
    }
}

void renderLoseAnimation(SDL_Renderer* renderer) {
    Mix_HaltMusic();
    endingMusic = Mix_LoadMUS("music/gameOver.wav");
    Mix_PlayMusic(endingMusic, 1);
    int fallPixel = 10;
    while(spritePos.y <= SCREEN_HEIGHT) {
        SDL_RenderClear(renderer);
        loadImage("images/gameimage.jpg", renderer);
        SDL_RenderCopy(renderer, spriteTexture, &spriteImage, &spritePos);
        spritePos.y += fallPixel;
        SDL_Delay(20);
        SDL_RenderPresent(renderer);
    }
}

void askPlayAgain(SDL_Window* window, SDL_Renderer* renderer, bool &gamePlay, bool &onMenu) {
    imageLoader = IMG_Load("images/infopane.png");
    paneImage = SDL_CreateTextureFromSurface(renderer, imageLoader);
    SDL_Rect dstrect;
    dstrect.w = 336;
    dstrect.h = 224;
    dstrect.x = (SCREEN_WIDTH - 336)/2;
    dstrect.y = (SCREEN_HEIGHT - 224)/2;
    SDL_RenderCopy(renderer, paneImage, NULL, &dstrect);
    createMessage("Yes", 24, {255, 255, 255}, (400 + 232) / 2, 2* dstrect.y - 20, mess, renderer);
    createMessage("No", 24, {0, 0, 0}, (568 + 400) / 2, 2* dstrect.y - 20, mess, renderer);
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

            if((x >= 295 && x <= 337) && (y >= 345 && y <= 367)) isYes = 1;
            else isYes = 0;
            if((x >= 470 && x <= 498) && (y >= 345 && y <= 367)) isNo = 1;
            else isNo = 0;

            if(isYes) createMessage("Yes", 24, {155, 203, 254}, (400 + 232) / 2, 2* dstrect.y - 20, mess, renderer);
            else createMessage("Yes", 24, {255, 255, 255}, (400 + 232) / 2, 2* dstrect.y - 20, mess, renderer);
            if(isNo) createMessage("No", 24, {155, 203, 254}, (568 + 400) / 2, 2* dstrect.y - 20, mess, renderer);
            else createMessage("No", 24, {0, 0, 0}, (568 + 400) / 2, 2* dstrect.y - 20, mess, renderer);
            SDL_RenderPresent(renderer);
        }
        if (e.type == SDL_MOUSEBUTTONDOWN) {
            int x = e.button.x;
            int y = e.button.y;
            if((x >= 295 && x <= 337) && (y >= 345 && y <= 367)) {
                gamePlay = 1;
                break;
            }
            else if((x >= 470 && x <= 498) && (y >= 345 && y <= 367))
            {
                onMenu = 1;
                gamePlay = 1;
                break;
            }
        }
    }
}

void renderGame(SDL_Window* window, SDL_Renderer* renderer, SDL_Surface* &screen, bool &gamePlay,
                bool &onMenu, int &numberLand) {
    SDL_RenderClear(renderer);

    Mix_HaltMusic();
    playMusic("music/gameMusic.wav");

    temp          = IMG_Load("images/sprite.png");
    spriteTexture = SDL_CreateTextureFromSurface(renderer, temp);
    temp       = IMG_Load("images/gameimage.jpg");
    background = SDL_CreateTextureFromSurface(renderer, temp);
    temp      = IMG_Load("images/land.jpg");
    landImage = SDL_CreateTextureFromSurface(renderer, temp);
    SDL_FreeSurface(temp);

    checkTime = 0;
    currentDir = DIR_RIGHT, animationFlip = 0;

    spritePos.x = (SCREEN_WIDTH - SPRITE_SIZE) / 2;
    spritePos.y = (SCREEN_HEIGHT - SPRITE_SIZE) / 2;
    spritePos.w = SPRITE_SIZE;
    spritePos.h = SPRITE_SIZE;

    SDL_RenderPresent(renderer);
    libraryTime = SDL_GetTicks();
    while(true) {
        gameTime = SDL_GetTicks() - libraryTime;
        handlePlayerEvent(&currentDir, &animationFlip, &spritePos);
        spriteImage.x = SPRITE_SIZE*(2*currentDir + animationFlip);
        spriteImage.y = 0;
        spriteImage.w = SPRITE_SIZE;
        spriteImage.h = SPRITE_SIZE;
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, background, NULL, NULL);
        for(int i=0; i<GAME_SIZE; i++) {
            for(int j=0; j<GAME_SIZE; j++) {
                if(!landCorrupt[i][j])
                    renderTexture(landImage, renderer,
                                175 + j*landWidth, 150 + i*landHeight, landWidth, landHeight);
            }
        }
        s = calculateTimer(gameTime);
        timeDisplay = s.c_str();
        createMessage(timeDisplay, 26, {205, 0, 0}, 400, 50, mess, renderer);
        SDL_Delay(10);
        if(gameTime - checkTime >= 4000 || checkTime == 0) {
            corruptPos = getLandtoCorrupt(numberLand);
            for(int p=0; p<GAME_SIZE; p++) {
                for(int j=0; j<GAME_SIZE; j++) {
                    corruptTime[p][j] = 0;
                    landCorrupt[p][j] = 0;
                }
            }
            checkTime = gameTime;
        }
        renderCorruptLand(corruptPos, gameTime - checkTime, renderer);
        SDL_RenderCopy(renderer, spriteTexture, &spriteImage, &spritePos);
        SDL_RenderPresent(renderer);
        if(onCrashLand(spritePos.x, spritePos.y)) {
            renderLoseAnimation(renderer);
            askPlayAgain(window, renderer, gamePlay, onMenu);
            if(gamePlay && !onMenu) {
                libraryTime = SDL_GetTicks();
                checkTime = 0;
                spritePos.x = (SCREEN_WIDTH - SPRITE_SIZE) / 2;
                spritePos.y = (SCREEN_HEIGHT - SPRITE_SIZE) / 2;
                spritePos.w = SPRITE_SIZE;
                spritePos.h = SPRITE_SIZE;
                currentDir = DIR_RIGHT, animationFlip = 0;
                playMusic("music/gameMusic.wav");
            }
            else {
                Mix_HaltMusic();
                return ;
            }
        }
        spriteImage.x = spritePos.x;
        spriteImage.y = spritePos.y;
    }
}

