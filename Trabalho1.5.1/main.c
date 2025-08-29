#include <SDL2/SDL.h>

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Movendo 3 Retângulos",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         200, 100, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    SDL_Rect r = { 40,20, 10,10 };
    SDL_Rect r2 = { 50,20, 10,10 };
    SDL_Rect r3 = { 60,20, 10,10 };

    SDL_Event evt;
    int espera = 500;

   while (1) {

	//Pega dimensoes atuais da janela
        int w, h;
        SDL_GetWindowSize(win, &w, &h);


	//Preenche fundo
        SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
        SDL_RenderClear(ren);

	//Desenha retangulo que se move com setas
        SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
        SDL_RenderFillRect(ren, &r);
	//Desenha retangulo que se move com tempo
        SDL_SetRenderDrawColor(ren, 0xFF,0x00,0x00,0x00);
        SDL_RenderFillRect(ren, &r2);
	//Desenha retangulo que se move com mouse
        SDL_SetRenderDrawColor(ren, 0x00,0xFF,0x00,0x00);
        SDL_RenderFillRect(ren, &r3);

	//Desenha tela
	SDL_RenderPresent(ren);


	//Lida com tempo
        Uint32 antes = SDL_GetTicks();
        int isevt = SDL_WaitEventTimeout(&evt, espera);

	if(isevt){	//Houve evento nao-temporal?

		//Atualiza espera
		espera -= (SDL_GetTicks()-antes);

		//Tratamento de eventos nao-temporais
		switch(evt.type){

		    case SDL_QUIT:	// Ir para rotina de finalizar processo
			goto FIM;
			break;
		
		    case SDL_MOUSEMOTION:	// Mover retangulo guiado por mouse
        	        //Pega posicao do mouse
                	int x, y;
	                SDL_GetMouseState(&x, &y);

        	        //Atualiza posicao do retangulo que se move com mouse
			r3.x = x;
			r3.y = y;
			break;

		    case SDL_KEYDOWN:	// Mover retangulo guiado por setas
			switch (evt.key.keysym.sym) {
                		case SDLK_UP:
				    if (r.y>=5){	// Retangulo sobe 5px se houver 5 ou mais pixels acima
	        			r.y -= 5;
				    }
				    break;
                		case SDLK_DOWN:
                        	    if ((h-r.y)>= (5+r.h)){ // Retangulo desce 5px se houver 15 (altura do retangulo+5) ou mais pixels abaixo
                                	r.y += 5;
	                            }
        	                    break;
	        	        case SDLK_LEFT:
                        	    if (r.x >= 5){ // Retangulo vai 5px a esquerda se houver 5 ou mais pixels a esquerda
                                	r.x -= 5;
	                            }
        	                    break;
	        	        case SDLK_RIGHT:
                        	    if ((w-r.x) >= (5+r.w)){ // r.x += 5px a direita se houver 15px (largura do retangulo+5) ou mais a direita
                                	r.x += 5;
	                            }
        	                    break;
	        	    }
	        }
	}
	else{	//Tratamento de evento temporal

		//Reinicia a variavel espera
		espera = 500;

		//Move retangulo guiado por tempo
		r2.x +=1;
		r2.y +=1;

	}
    }


    /* FINALIZACAO */
FIM:
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
