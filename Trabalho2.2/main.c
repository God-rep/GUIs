#include <SDL2/SDL.h>

#define ESTADO_PARADO 0
#define ESTADO_CLIQUE 1
#define ESTADO_ARRASTA 2


int AUX_WaitEventTimeoutCount (SDL_Event* evt, Uint32* ms); // Herdado de 1.5.2
							// Como so um evento de tempo eh utilizado, acredito que
							// faz sentido evitar a complexidade de usar o struct de tempos de 2.1

int cliqueRetangulo(SDL_Window* win, SDL_Rect* r1); 	// Retorno:
							//	0 (estado_parado) -> mouse fora do quadrado
							//	1 (estado_clique) -> mouse dentro do quadrado


void arrastaRetangulo(SDL_Window* win, SDL_Rect* quadradoMovendo, SDL_Rect* distanciaClique);

/*
 *
 * Apesar de nenhum movimento por tempo,
 * mantive os else's para dar enfase
 *
 *
 */

int main(int argc, char* args[]){

	/* Inicia SDL */
	SDL_Init(SDL_INIT_EVERYTHING);
            SDL_Window* win = SDL_CreateWindow("Clica e arrasta",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         700, 600, SDL_WINDOW_SHOWN
                      );

        SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

        SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
        SDL_RenderClear(ren);

	//Usado para salvar posicao pre-movimento do quadrado
	SDL_Rect quadradoParado = {10, 10, 50, 50};
	//Usado para mover e desenhar quadrado na tela
	SDL_Rect quadradoMovendo = {10, 10, 50, 50};

	// So salva a distancia da ancora do quadradoParado para o click
	SDL_Rect distanciaClique = {0, 0, 0, 0};

	SDL_Event evt;
	Uint32 estado = ESTADO_PARADO;

	int tempoAtualizarTela = 17; // ~60fps

	while(1){
		/* Desenho */
		
		//Fundo
                SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
                SDL_RenderClear(ren);
		
		// Retangulo
		switch(estado){
			case ESTADO_PARADO:
				SDL_SetRenderDrawColor(ren, 0xFF,0x00,0x00,0xFF); // Vermelho
				break;
			case ESTADO_CLIQUE:
				SDL_SetRenderDrawColor(ren, 0xAA,0x00,0xFF,0xFF); // Roxo
				break;
			case ESTADO_ARRASTA:
				SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0x00,0xFF); // Amarelo
                                break;
		}
		SDL_RenderFillRect(ren, &quadradoMovendo);	// Sempre desenha o quadradoMovendo
								// O parado so serve para salvar posicao

		SDL_RenderPresent(ren);
		/* Execucao */

		// O lag do mouse por excessivos eventos de mousemotion estava me incomodando.
		// Esse loop parece lidar bem com isso.
		//
		// Senti um pouco de screen tearing quando ao arrastar o quadrado muito rapido,
		// mas em geral melhorou a experiencia de uso
		while (SDL_PeepEvents(&evt, 1, SDL_GETEVENT, SDL_MOUSEMOTION, SDL_MOUSEMOTION));

		switch(estado){
			case ESTADO_PARADO:
				int isevt_e0 = AUX_WaitEventTimeoutCount (&evt, &tempoAtualizarTela);
				if(isevt_e0){
					switch(evt.type){
						case SDL_QUIT:
							goto FIM;
							break;
						case SDL_MOUSEBUTTONDOWN:
							// Botao esquerdo clicado
							switch(evt.button.button){
								case SDL_BUTTON_LEFT:	// Botao esquerdo clicado -> checa se clique acertou quadrado
									if (estado = cliqueRetangulo(win, &quadradoMovendo)){	// Se clique bem sucedido -> salva distancia para ancora e muda estado
										int mouseX = -1;
								        	int mouseY = -1;
									        SDL_GetMouseState(&mouseX, &mouseY);
										distanciaClique.x = quadradoParado.x - mouseX;
								                distanciaClique.y = quadradoParado.y - mouseY;
									}
									break;
							}
							break;
					}
				}

				else{	// Acabou tempo de atualizacao de tela
					/* Nada deve mudar aqui,
					* chamar funcao de mover
					* quadrado seria desnecessario
					*/
				}
				break;

			case ESTADO_CLIQUE:
				int isevt_e1 = AUX_WaitEventTimeoutCount(&evt, &tempoAtualizarTela);
				if(isevt_e1){
					switch(evt.type){
                        	                case SDL_QUIT:          // Sai do programa
                                	                goto FIM;
	                                	        break;
        	                                case SDL_MOUSEBUTTONUP:
                	                                switch(evt.button.button){
                        	                                case SDL_BUTTON_LEFT: // ButEsquerdo -> nao troca estado, so atualiza variaveis de clique
									estado = 0;
                                	                        	break;
	                                        	}
                                                        break;
        		                        case SDL_MOUSEMOTION:   // Muda de estado
                		                        estado = 2;
                			                break;
						case SDL_KEYDOWN:
	                                        	switch(evt.key.keysym.sym){
        	                        	                case SDLK_ESCAPE:	// Cancela clique -> vai para estado parado
									estado = 0;
                                                        	        break;
	                                                }
							break;
        	                        }
				}
				
				else{ // Acabou tempo de atualizacao de tela
					/* Nada deve mudar aqui,
                                        * chamar funcao de mover
                                        * quadrado seria desnecessario
                                        */
				}
				break;
			case ESTADO_ARRASTA:
				int isevt_e2 = AUX_WaitEventTimeoutCount(&evt, &tempoAtualizarTela);
				if(isevt_e2){
					switch(evt.type){
						case SDL_QUIT:          // Sai do programa
                                                        goto FIM;
                                                        break;
                                                case SDL_MOUSEBUTTONUP:
                                                        switch(evt.button.button){
                                                                case SDL_BUTTON_LEFT: // ButEsquerdo -> atualiza posicao e para quadrado
									quadradoParado = quadradoMovendo;	// Atualiza quadrado PARADO
                                                                        estado = 0;	// Volta para estado parado
                                                                        break;
                                                        }
                                                        break;
                                                case SDL_KEYDOWN:
                                                        switch(evt.key.keysym.sym){
                                                                case SDLK_ESCAPE:	// esc -> cancela movimento
									quadradoMovendo = quadradoParado; // Atualiza quadrado MOVENDO
                                                                        estado = 0;	// Volta para estado parado
                                                                        break;
                                                        }
                                                        break;
						case SDL_MOUSEMOTION:          // Move quadrado
							arrastaRetangulo(win, &quadradoMovendo, &distanciaClique);
                                                        break;
					}
				}
				
				else{ // Acabou tempo de atualizacao de tela
					/* Nada deve mudar aqui,
                                        * chamar funcao de mover
                                        * quadrado seria desnecessario
                                        */	
				}


			}
		}

FIM:

	SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
}

//Funcao uxiliar que esconde o uso de "SDL_WaitEventTimeout"
//Reutilizada de 1.5.2a
int AUX_WaitEventTimeoutCount(SDL_Event* evt, Uint32* ms){

        //Pega tempo anterior
        Uint32 antes = SDL_GetTicks();

        int isevt =  SDL_WaitEventTimeout( evt , *ms );

        //Lida com atualizacao do valor de espera
        (*ms) = (isevt)? ((*ms)-( SDL_GetTicks()- antes )):(*ms);

        return isevt;
}

int cliqueRetangulo(SDL_Window* win, SDL_Rect* r1){
	int mouseX = -1;
	int mouseY = -1;
	SDL_GetMouseState(&mouseX, &mouseY);

	return (((r1->x <= mouseX) && (mouseX <= (r1->x+r1->w)))
                &&((r1->y <= mouseY) && (mouseY <= (r1->y+r1->h))));

}

void arrastaRetangulo(SDL_Window* win, SDL_Rect* quadradoMovendo, SDL_Rect* distanciaClique){
        int mouseX = -1;
        int mouseY = -1;
        SDL_GetMouseState(&mouseX, &mouseY);

	// Atualiza posicao do quadrado, mantendo distancia do clique para a ancora salva
	quadradoMovendo->x = mouseX + distanciaClique->x;
	quadradoMovendo->y = mouseY + distanciaClique->y;

}
