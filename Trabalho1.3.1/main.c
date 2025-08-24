#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <limits.h>

int main(int argc, char* args[]){
	
	SDL_Init(SDL_INIT_EVERYTHING);
	    SDL_Window* win = SDL_CreateWindow("Animacao Simples",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         200, 100, SDL_WINDOW_SHOWN
                      );
	SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
	
	SDL_SetRenderDrawColor(ren, 0x00,0x00,0x00,0x00);
	SDL_RenderClear(ren);
	
	
	SDL_Rect r = {99, 89, 1, 1};
	filledCircleColor(ren, r.x, r.y, 10, 0xFFFFFFFF);

	int movDir = 1; //1 = cima, -1 = baixo
	
	int flag = 200;
	while(flag >0){
		SDL_SetRenderDrawColor(ren, 0x00,0x00,0x00,0x00);
	        SDL_RenderClear(ren);
	
		if(r.y > 49){
			if (movDir == 1){
				r.y -=1;
			}
			else if (movDir == -1){
				if (r.y < 90){
					r.y +=1;
				}
				else if (r.y == 90){
					movDir = 1;
				}
			}
		}
		else if (r.y == 49){
			movDir = -1;
			r.y += 1;
		}

		flag -=1;
		filledCircleColor(ren, r.x, r.y, 10, 0xFFFFFFFF);
		SDL_RenderPresent(ren);
		SDL_Delay(100);
	}
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 1;
}
