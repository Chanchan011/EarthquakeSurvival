#include <bits/stdc++.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "game.h"
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
const int landWidth = 150, landHeight = 100;

void handlePlayerEvent(int *currentDir, int *animationFlip, SDL_Rect *position) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
        if ((e.type == SDL_QUIT) || (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
            exit(1);
        }
        else if(e.type == SDL_KEYDOWN) {
            //cout << "keyboard detected" << '\n';
            if(e.key.keysym.sym == SDLK_d || e.key.keysym.sym == SDLK_RIGHT) {
                *currentDir = DIR_RIGHT;
                *animationFlip = 1 - *animationFlip;
                if(position->x + SPRITE_STEP <= 625 - SPRITE_SIZE) {
                    position->x += SPRITE_STEP;
                }
                else position->x = 625 - SPRITE_SIZE;
                break;
            }
            else if(e.key.keysym.sym == SDLK_w || e.key.keysym.sym == SDLK_UP) {
                *currentDir = DIR_UP;
                *animationFlip = 1 - *animationFlip;
                if(position->y - SPRITE_STEP >= 150) {
                    position->y -= SPRITE_STEP;
                }
                else position->y = 150;
                break;
            }
            else if(e.key.keysym.sym == SDLK_s || e.key.keysym.sym == SDLK_DOWN) {
                *currentDir = DIR_DOWN;
                *animationFlip = 1 - *animationFlip;
                if(position->y + SPRITE_STEP <= 450 - SPRITE_SIZE) {
                    position->y += SPRITE_STEP;
                }
                else position->y = 450 - SPRITE_SIZE;
                break;
            }
            else if(e.key.keysym.sym == SDLK_a || e.key.keysym.sym == SDLK_LEFT) {
                *currentDir = DIR_LEFT;
                *animationFlip = 1 - *animationFlip;
                if(position->x - SPRITE_STEP >= 175) {
                    position->x -= SPRITE_STEP;
                }
                else position->x = 175;
                break;
            }
        }
        else {
            break;
        }
    }
}
