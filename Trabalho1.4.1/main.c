#include <SDL2/SDL.h>

int main (int argc, char* args[])
{
    /* INICIALIZACAO */
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Movendo um Retângulo",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         200, 100, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    /* EXECUÇÃO */
    SDL_Rect r = { 40,20, 10,10 };
    SDL_Event evt;

    //Vetores de novos quadrados e cores dos quadrados
    SDL_Rect NewRs[10];
    int NewRsColor[40] ={	0xFF,0x99,0x00,0x00,	//Laranja
    				0xFF,0xFF,0x00,0x00,	//Amarelo
    				0xFF,0x00,0x00,0x00,	//Vermelho
                                0xAA,0x00,0xFF,0x00,	//Roxo
                                0xFF,0xDD,0xDD,0x00,	//Rosa
                                0x00,0x00,0x00,0x00,	//Preto
                                0xDD,0xDD,0xFF,0x00,	//Lilas	
                                0x00,0xFF,0x00,0x00,	//Verde
                                0x99,0x99,0x99,0x00,	//Cinza
                                0x00,0xFF,0xFF,0x00	//Azul claro
    			};

    int Pos = 0; // Variavel que conta o numero de quadrados fixos na tela
    while (1) {
	//Preenche fundo
        SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
        SDL_RenderClear(ren);

	//Desenha retangulo que move
        SDL_SetRenderDrawColor(ren, 0x00,0x00,0xFF,0x00);
        SDL_RenderFillRect(ren, &r);

	//Desenha retangulos criados por clique
	for (int i = 0; i < Pos; i ++){
	   //Pega numeros de 4 em 4 no vetor de cores para formar a cor de cada quadrado
	   SDL_SetRenderDrawColor(ren, NewRsColor[4*i],NewRsColor[(4*i)+1],NewRsColor[(4*i)+2],NewRsColor[(4*i)+3]);
           SDL_RenderFillRect(ren, &NewRs[i]);
	}

	//Desenha tela e espera por inputs do usuario
	SDL_RenderPresent(ren);
        SDL_WaitEvent(&evt);


	//Tratamento de eventos
	switch(evt.type){

	    case SDL_QUIT:	// Ir para rotina de finalizar processo
		goto FIM;
		break;
		
	    case SDL_MOUSEBUTTONDOWN:	// Criar retangulos em vetor de retangulos
		if(Pos<10){
                   //Pega posicao do mouse
                   int x, y;
                   SDL_GetMouseState(&x, &y);

                   //Cria retangulo base com as coordenadas e adiciona ao vetor
		   SDL_Rect a = { x,y, 10,10 };
                   NewRs[Pos] = a;

                   //Atualiza numero de quadrados fixos
                   Pos++;
                }
		break;

	    case SDL_KEYDOWN:	// Mover quadrado azul ao clicar nas setas

		
		// Pega dimensoes atuais da janela
		int w, h;
		SDL_GetWindowSize(win, &w, &h);
		
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


    /* FINALIZACAO */
FIM:
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
