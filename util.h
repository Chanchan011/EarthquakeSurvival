#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <SDL.h>
#include <string>

void logSDLError(std::ostream& os, const std::string &msg, bool fatal);
void initSDL(SDL_Window* &window, SDL_Renderer* &renderer, std::string WINDOW_TITLE, int SCREEN_WIDTH, int SCREEN_HEIGHT);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
void waitUntilKeyPressed();
void drawFilledRect(SDL_Renderer* renderer, SDL_Rect* rect, int r, int g, int b, int a);
SDL_Rect createNewRect(int x, int y, int h, int w);
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren);
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, int w, int h);

#endif // UTIL_H_INCLUDED
