#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <limits.h>
#include <errno.h>


#include <stdio.h>

#define wPlat 80
#define hPlat 8


typedef struct{

	SDL_Rect posicao;	// guarda posicao e formato da plataforma
	int direcao; // 0-> direita. 1-> esquerda. outro -> erro
} plataforma;


int AUX_WaitEventTimeoutCount (SDL_Event* evt, Uint32* ms);	// Funcao herdada do exercicio 1.5.2
								// Espera atualizada de 500 para 100




int criaPlataforma(plataforma** aray, SDL_Window* win, int* inicio, int* fim, int yNovo); // Cria plataforma usando
											  // ponteiro de plataformas no array na heap
											  //
											  // 1 -> criada com sucesso. 0 -> Erro
							
int destroiPlataforma(plataforma** array, int* inicio, int* fim); // Destroi plataforma usando ponteiro de plataformas no array na heap
								  //
                                                		  // 1 -> criada com sucesso. 0 -> Erro



int main(int argc, char* args[]){
	
	SDL_Init(SDL_INIT_EVERYTHING);
	    SDL_Window* win = SDL_CreateWindow("Plataformas alinhadas",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         700, 600, SDL_WINDOW_SHOWN
                      );
	SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);
	
	SDL_SetRenderDrawColor(ren, 0x00,0x00,0x00,0x00);
	SDL_RenderClear(ren);
	
	
	SDL_Rect r = {340, 590, 1, 1};
	filledCircleRGBA(ren, r.x, r.y, 10, 0xFF, 0xFF, 0xFF, 0xFF);


	int ticksSubindo = 0; // Diz quantos ticks o jogador continuara pulando (se >0) ou se nao esta pulando no momento (se =0)


	int movDir = 1; //1 = cima, -1 = baixo
	int flag = 200;

	int contaTick = 49; // Varia de 0 a 999 e reseta. Comeca em 49 para acelerar criacao de plataformas iniciais

	Uint32 espera = 100;
	SDL_Event evt;
	
	// Armazena ponteiros para plataformas serem alocadas na heap
	plataforma* arrayPlataformas[30] = {
						NULL,
                                                NULL,
                                                NULL,
                                                NULL,
                                                NULL,
                                                NULL,
                                                NULL,
                                                NULL,
                                                NULL,
                                                NULL,
                                                NULL,
                                                NULL,
                                                NULL,
                                                NULL,
                                                NULL,
                                                NULL,
                                                NULL,
                                                NULL,
                                                NULL,
                                                NULL,
                                                NULL,
                                                NULL,
                                                NULL,
                                                NULL,
                                                NULL,
                                                NULL,
                                                NULL,
                                                NULL,
                                                NULL,
                                                NULL
					};

	//Lida com alocacao de plataformas
	int inicioPlatArray = 0;
	int fimPlatArray = 0;

	/*
	 * 	
	 *	Implementacao de array de plataformas similar a consumidores/produtores
	 *
	 * 	Usando buffer circular:
	 *
	 * 		Se (inicio == fim) -> buffer vazio
	 * 		Se (fimPlatArray==((inicioPlatArray-1)%30) -> buffer cheio
	 *				                    [limite]
	 *	
	 *		achar total de plataformas alocadas:
	 *					Se (fim < inicio) -> total = 30-inicio + fim;
	 *					Se (fim > inicio) -> total = fim-inicio;
	 * 
	 *
	 * */

	while(1){

		/*Desenhos*/

		//Fundo
		SDL_SetRenderDrawColor(ren, 0x00,0x00,0x00,0x00);
	        SDL_RenderClear(ren);
	
		//Jogador
		filledCircleRGBA(ren, r.x, r.y, 10, 0xFF, 0x00, 0x00, 0xFF);

		//Plataformas
		int z = inicioPlatArray;
		while(z != fimPlatArray){

			SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0x00,0xFF);
			SDL_RenderFillRect(ren, &(arrayPlataformas[z]->posicao));

			z = ((z+1)%30);
		}
	
		//Checa estado do array
		// printf("inicioArray:%d | fimArray:%d\n", inicioPlatArray, fimPlatArray);
		SDL_RenderPresent(ren);

		/* Lida com tempo */
		int isevt = AUX_WaitEventTimeoutCount(&evt, &espera);

		/* Execucao */
		if(isevt){	// Se houver evento nao-temporal no intervalo
			int xWin = -1;
                        int yWin = -1;

			switch(evt.type){

				case SDL_KEYDOWN:

					switch(evt.key.keysym.sym){
						case SDLK_UP:

							if(!ticksSubindo){
								ticksSubindo = 25;
							}
							break;

						case SDLK_RIGHT:
							SDL_GetWindowSize(win, &xWin, &yWin);
                                                        if((r.x+15)< xWin){	//Centro do circulo + raio + deslocamento
                                                                r.x+=5;
                                                        }
							else{
								r.x = xWin-10; //Circulo na extremidade direita
							}
                                                        break;
						case SDLK_LEFT:
                                                        if(r.x > 15){     //Centro do circulo + raio + deslocamento
                                                                r.x-=5;
                                                        }
                                                        else{
                                                                r.x = 10;	//Circulo na extremidade esquerda
                                                        }
                                                        break;
					}

					break;

				case SDL_QUIT:
					goto FIM;
					break;

			}

		}
		else{	// Se proximo evento no intervalo for temporal
			
			int xWin = -1;
			int yWin = -1;
		
			espera = 100;
			contaTick = ((contaTick+1)%1000);

			/* relacionado a gravidade agindo no jogador (bola vermelha) */
			SDL_GetWindowSize(win, &xWin, &yWin);
			if(ticksSubindo>0){
				r.y -= 5;
				ticksSubindo -=1;
			}
			else if((r.y+10) < yWin){ // Centro do circulo + raio
                                r.y +=5;
                	}

			int i = inicioPlatArray;
			while(i != fimPlatArray){
				int* platX = NULL;
				int* platDir = NULL;

				platX = &((arrayPlataformas[i]->posicao).x);
				platDir = &(arrayPlataformas[i]->direcao);

				if(*platDir){ // Se direcao = 1 -> va para esquerda
					(*platX) -= 5;
				}
				else{	//Senao, se direcao = 0 -> va para direita
					(*platX) +=5;

				}
				
				SDL_GetWindowSize(win, &xWin, &yWin);
				if( (*platX) == -(wPlat*2) || (*platX) == (xWin+wPlat)){	// Se plataforma chegou no limite, destroi
					int destruida = destroiPlataforma(arrayPlataformas, &inicioPlatArray, &fimPlatArray);
				}
				
				i = ((i+1)%30);
			}


			/* Cria novas plataformas */ 

			if (!(contaTick%50)){ // A cada 100 ticks, cria 1 plataforma por linha
				for(int j = 1; j < 6; j++){
					if( !(fimPlatArray==((inicioPlatArray-1)%30) )){
						int criada = criaPlataforma(arrayPlataformas, win, &inicioPlatArray, &fimPlatArray, 100*j);
					}
				}
			}

		
		}
		

	}
FIM:
	//Desaloca memoria das plataformas
	while(inicioPlatArray != fimPlatArray){

		//Nao gera loop infinito, funcao incrementa inicioPlatArray ate chegar em fimPlatArray
		int destuida = destroiPlataforma(arrayPlataformas, &inicioPlatArray, &fimPlatArray);
	}

	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();

	return 1;
}


int criaPlataforma(plataforma** array, SDL_Window* win, int* inicio, int* fim, int yNovo){
	
	/* Checa erros */

	if(((*fim) - (*inicio)) == 29){
		printf("Impossivel criar mais plataformas! Array cheio!");
		return 0;
	}

	/* Executa rotina de criar plataforma de fato */

	int xWin = -1;
        int yWin = -1;

	int direcao = ((yNovo/100) % 2); // Se linha for par, vai pra direita.
					 // Se linha for impar, esquerda

	//Cria retangulo com as caracteristicas desejadas
	SDL_Rect posicao;
	posicao.w = wPlat;
	posicao.h = hPlat;
	posicao.y = yNovo;

	SDL_GetWindowSize(win, &xWin, &yWin);
       	posicao.x = (direcao)? (xWin):-(wPlat);



	//Aloca memoria na heap usando o array e copia o retangulo e a direcao escolhidas
	array[ (*fim) ] = (plataforma*) malloc(sizeof(plataforma));
	(array[(*fim)]->posicao) = posicao;
	(array[(*fim)]->direcao) = direcao;

	(*fim) = (( (*fim) +1)%30); // Atualiza fim do array

	return 1;
}


int destroiPlataforma(plataforma** array, int* inicio, int* fim){

	/* Checa erros */

	if(((*fim) - (*inicio)) == 0){
                printf("Impossivel destruir mais plataformas! Array vazio!");
		return 0;
        }

        /* Executa rotina de destruir plataforma de fato */

	// Desaloca ponteiro e anula para evitar ponteiro pendente
	free(array[ (*inicio) ]);
	array[ (*inicio) ] = NULL;

	(*inicio) = (( (*inicio) +1)%30); //Atualiza inicio do array
	
	return 1;
}



//Funcao auxiliar que esconde o uso de "SDL_WaitEventTimeout"
//Reutilizada de 1.5.2
//
//
//Atualizada -> Agora nao precisa de getTicks() anterior a chamada,
//mas nao redefine valor de espera automaticamente (poderia ser feito com a adicao de mais uma variavel,
//ou utilizando a versao anterior)
int AUX_WaitEventTimeoutCount(SDL_Event* evt, Uint32* ms){

        //Pega tempo anterior
        Uint32 antes = SDL_GetTicks();

        int isevt =  SDL_WaitEventTimeout( evt , *ms );

        //Lida com atualizacao do valor de espera
        (*ms) = (isevt)? ((*ms)-( SDL_GetTicks()- antes )):(*ms);

        return isevt;
}
