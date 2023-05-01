#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <bits/stdc++.h>
#include <SDL.h>

void createMessage(const char* s, int fontSize, SDL_Color color, int x, int y, SDL_Texture* mess,
                   SDL_Renderer* renderer);
void loadImage(const char* path, SDL_Renderer* renderer);
void playMusic(const char* path);
void renderMenu(SDL_Window* window, SDL_Renderer* renderer, bool &gamePlay, bool &onMenu, bool &hardMode);
void getHelp(SDL_Window* window, SDL_Renderer* renderer, bool &gamePlay, bool &onMenu, bool &hardMode);
void unloadComponents(SDL_Window* &window, SDL_Renderer* &renderer);

#endif // GAME_H_INCLUDED
