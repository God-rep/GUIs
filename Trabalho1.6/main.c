#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


#include <assert.h>

/*Funcoes*/
int AUX_WaitEventTimeoutCount (SDL_Event* evt, Uint32* ms);     // Funcao herdada do exercicio 1.5.2
                                                                // Espera padrao mantida em 500

int colisaoRetangulo(SDL_Rect r1, SDL_Rect r2);	// Calcula colisao. 1-> colidiu. 0-> nao colidiu.
						// Pensei em usar passagem por referencia por economia de memoria/eficiencia
						// Mas optei assim

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    
    SDL_Window* win = SDL_CreateWindow("Corrida com 3 Retângulos",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         200, 100, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* Bloco relacionado aos textos     */
    TTF_Init();
    TTF_Font* fnt = TTF_OpenFont("Tiny5-Regular.ttf", 20);
    assert(fnt != NULL);
    // Texto vitoria azul
    SDL_Color clrAzul = {0x00, 0x00, 0xFF, 0xFF};
    SDL_Surface* srcAzul = TTF_RenderText_Blended(fnt, "Vitoria Azul!", clrAzul);
    assert(srcAzul != NULL);
    SDL_Texture* vitAzulTxt = SDL_CreateTextureFromSurface(ren, srcAzul);
    assert(vitAzulTxt != NULL);
    // Texto vitoria vermelha
    SDL_Color clrVermelha = {0xFF, 0x00, 0x00, 0xFF};
    SDL_Surface* srcVermelha = TTF_RenderText_Blended(fnt, "Vitoria Vermelha!", clrVermelha);
    assert(srcVermelha != NULL);
    SDL_Texture* vitVermelhaTxt = SDL_CreateTextureFromSurface(ren, srcVermelha);
    assert(vitVermelhaTxt != NULL);
    // Texto vitoria verde
    SDL_Color clrVerde = {0x00, 0xFF, 0x00, 0xFF};
    SDL_Surface* srcVerde = TTF_RenderText_Blended(fnt, "Vitoria Verde!", clrVerde);
    assert(srcVerde != NULL);
    SDL_Texture* vitVerdeTxt = SDL_CreateTextureFromSurface(ren, srcVerde);
    assert(vitVerdeTxt != NULL);
    // Free srcs
    SDL_FreeSurface(srcAzul);
    SDL_FreeSurface(srcVermelha);
    SDL_FreeSurface(srcVerde);


    /* EXECUÇÃO */

    int reiniciar = 1;  //Variavel de retorno da funcao
    while(reiniciar){
	    //Variaveis gerais
	    int primeiro = -1;	// -1 -> nenhum chegou. 0 -> r chegou.
				// 1 -> r2 chegou. 2 -> r3 chegou.

	    int chegou[3] = {0, 0, 0};	// Diz de r, r2 e r3, respectivamente chegaram
			  	     	// na chegada. 0 -> nao chegou, >0 -> chegou.

	    // Dimensoes da janela
	    int xWin = -1;
	    int yWin = -1;
	    SDL_GetWindowSize(win, &xWin, &yWin);

    //Retangulos
    SDL_Rect r = { 40, 20, 10,10 };
    SDL_Rect r2 = { 40, 40, 10,10 };
    SDL_Rect r3 = { 40, 60, 10,10 };

    //Linha de chegada
    SDL_Rect chegada;
    chegada.y = 3;
    chegada.w = 6;

    //Eventos e tempos
    Uint32 espera = 500;
    SDL_Event evt;

   while (!(chegou[0]) || !(chegou[1]) || !(chegou[2]) ) {

	//Preenche fundo
        SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
        SDL_RenderClear(ren);

	//Desenha linha de chegada
	SDL_GetWindowSize(win, &xWin, &yWin);
	chegada.x = xWin-13;
	chegada.h = yWin-6;
	SDL_SetRenderDrawColor(ren, 0x00,0x00,0x00,0x00);
	SDL_RenderFillRect(ren, &chegada);

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
        int isevt = AUX_WaitEventTimeoutCount(&evt, &espera);

	if(isevt){	//Houve evento nao-temporal?

		//Tratamento de eventos nao-temporais
		switch(evt.type){

		    case SDL_QUIT:	// Ir para rotina de finalizar processo
					// com variavel de retorno zerada
					// (nao reiniciar jogo)
			reiniciar = 0;
			goto FIM;
		
		    case SDL_MOUSEMOTION:	// Mover retangulo guiado por mouse

			//Se ja chegou na meta
			if (chegou[2]);

			//Se ainda nao chegou na meta, trata movimento
			else{
	        	        //Pega posicao do mouse
				int xMouse = -1;
				int yMouse = -1;
	                	SDL_GetMouseState(&xMouse, &yMouse);

	        	        //Atualiza posicao do retangulo que se move com mouse
				r3.x = xMouse;

				// Removida movimentacao em y
				//r3.y = yMouse;
			

				//Checa se ja chegou na meta
				chegou[2] = colisaoRetangulo(chegada, r3);
				// Checa se foi o primeiro
                                if(!chegou[0] && !chegou[1] && chegou[2]){
                                        primeiro = 2;
                                }
			}

			break;

		    case SDL_KEYDOWN:	// Mover retangulo guiado por setas

			//Se ja chegou antes
			if (chegou[0]);

			//Se ainda nao chegou na meta, trata movimento
			else{
				switch (evt.key.keysym.sym) {


					/*	Removido movimentacao em y 
        	        		case SDLK_UP:
						if (r.y>=5){	// Retangulo sobe 5px se houver 5 ou mais pixels acima
	        				r.y -= 5;
					    }
					    break;
                			case SDLK_DOWN:
					    SDL_GetMouseState(&xWin, &yWin);
	                        	    if ((yWin-r.y)>= (5+r.h)){ // Retangulo desce 5px se houver 15 (altura do retangulo+5) ou mais pixels abaixo
                		                r.y += 5;
	                        	    }
		        	            break;
					*/

	        			case SDLK_LEFT:
	                        	    if (r.x >= 5){ // Retangulo vai 5px a esquerda se houver 5 ou mais pixels a esquerda
        	                        	r.x -= 5;
	        	                    }
        	        	            break;
	        	        	case SDLK_RIGHT:
					    SDL_GetWindowSize(win, &xWin, &yWin);
        	                	    if ((xWin-r.x) >= (5+r.w)){ // r.x += 5px a direita se houver 15px (largura do retangulo+5) ou mais a direita
                	                	r.x += 5;
	                	            }
        	                	    break;
		        	    }
				
				// Checa se ja chegou na meta
				chegou[0] = colisaoRetangulo(chegada, r);
				// Checa se foi o primeiro
				if(chegou[0] && !chegou[1] && !chegou[2]){
					primeiro = 0;
				}
			}

	        }
	}
	else{	//Tratamento de evento temporal
		
		espera = 500;
		//Se ja chegou na meta
		if(chegou[1]);
		//Se ainda nao chegou na meta, trata movimento
		else{

			//Move retangulo guiado por tempo
			r2.x +=10;
			// Removida movimentacao em y
			//r2.y +=1;

			//Checa se ja chegou na meta
			chegou[1] = colisaoRetangulo(chegada, r2);
			// Checa se foi o primeiro
                        if(!chegou[0] && chegou[1] && !chegou[2]){
                                primeiro = 1;
                        }

		}
	}
    }

    reiniciar = 1;

     /* Redesenha apos loop */
        
        //Preenche fundo
        SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
        SDL_RenderClear(ren);

        //Desenha linha de chegada
        SDL_GetWindowSize(win, &xWin, &yWin);
        chegada.x = xWin-13;
        chegada.h = yWin-6;
        SDL_SetRenderDrawColor(ren, 0x00,0x00,0x00,0x00);
        SDL_RenderFillRect(ren, &chegada);

        //Desenha retangulo que se move com setas
        SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
        SDL_RenderFillRect(ren, &r);
        //Desenha retangulo que se move com tempo
        SDL_SetRenderDrawColor(ren, 0xFF,0x00,0x00,0x00);
        SDL_RenderFillRect(ren, &r2);
        //Desenha retangulo que se move com mouse
        SDL_SetRenderDrawColor(ren, 0x00,0xFF,0x00,0x00);
	SDL_RenderFillRect(ren, &r3);


     //Constroi caixa de texto
     SDL_GetWindowSize(win, &xWin, &yWin);
     SDL_Rect caixaTexto = {(xWin/4), (yWin/4), (xWin/2), (yWin/2)};
     //Desenha caixa na tela
     SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0x00,0x00);
     SDL_RenderFillRect(ren, &caixaTexto);

     // Tamanho e posicao da mensagem
     SDL_Rect msgVit = { (5+ caixaTexto.x), ((caixaTexto.y + (caixaTexto.h/2))-10), 90, 20};

     // Escolhe mensagem
     switch(primeiro){
		case 0: SDL_RenderCopy(ren, vitAzulTxt, NULL, &msgVit);
			break;

		case 1: SDL_RenderCopy(ren, vitVermelhaTxt, NULL, &msgVit);
			break;
		case 2: SDL_RenderCopy(ren, vitVerdeTxt, NULL, &msgVit);
                        break;
     }

     // Apresenta tela final
     SDL_RenderPresent(ren);


     //Maior espera para manter mensagem de finalizacao por mais tempo
     espera = 10000;

     //Loop do estado da mensagem final
     while(1){
	     int isevt = AUX_WaitEventTimeoutCount(&evt, &espera);

	     if(isevt){
     		//Reutilizacao do switch por maior facilidade de manutencao (e.g. adicao de novas transicoes no futuro)
                switch(evt.type){			

                    case SDL_QUIT:      // Ir para rotina de finalizar processo
                                        // com variavel de retorno zerada
                                        // (nao reiniciar jogo)
                        reiniciar = 0;
			goto FIM;

		}
     	     }
             else{	//Caso tempo acabe, atualiza variavel de retorno para reiniciar programa
		reiniciar = 1;
		break;
	     }
	}

    }
    /* FINALIZACAO */
FIM:

    SDL_DestroyTexture(vitAzulTxt);
    SDL_DestroyTexture(vitVermelhaTxt);
    SDL_DestroyTexture(vitVerdeTxt);
    TTF_CloseFont(fnt);
    TTF_Quit();
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return reiniciar;
}

int colisaoRetangulo(SDL_Rect r1, SDL_Rect r2){
	return (((r1.x <= (r2.x+r2.w)) && (r2.x<=(r1.x+r1.w)))
                &&((r1.y <= (r2.y+r2.h)) && (r2.y<=(r1.y+r1.h))));
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
