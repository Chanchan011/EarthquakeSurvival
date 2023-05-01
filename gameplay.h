#ifndef GAMEPLAY_H_INCLUDED
#define GAMEPLAY_H_INCLUDED

#include <bits/stdc++.h>
#include <SDL.h>
using namespace std;

void renderAnimation(int i, int j, SDL_Renderer* renderer);
void renderCorruptLand(vector <pair<int, int>> &corruptPos, int playTime, SDL_Renderer* renderer);
void renderLoseAnimation(SDL_Renderer* renderer);
void askPlayAgain(SDL_Window* window, SDL_Renderer* renderer, bool &gamePlay, bool &onMenu);
void renderGame(SDL_Window* window, SDL_Renderer* renderer, SDL_Surface* &screen, bool &gamePlay,
                bool &onMenu, int &numberLand);

#endif // GAMEPLAY_H_INCLUDED

