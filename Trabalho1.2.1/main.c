#include <stdio.h>
#include <SDL2/SDL.h>

int main (int argc, char* args[]){


    int winHLimit = 500;
    int winVLimit = 250;

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Hello World!",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         winHLimit, winVLimit, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
    SDL_RenderClear(ren);
    SDL_SetRenderDrawColor(ren, 0, 47, 108, 0x00);


    SDL_Rect hLine = { 0,((winVLimit/2)-(winVLimit/6)), winHLimit ,(winVLimit/3) };
    SDL_RenderFillRect(ren, &hLine);


    SDL_Rect vLine = { ((winHLimit/2)-(winHLimit/4)), 0, (winVLimit/3), winVLimit };
    SDL_RenderFillRect(ren, &vLine);
    
    
    SDL_RenderPresent(ren);
    SDL_Delay(5000);


    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
