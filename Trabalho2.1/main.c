#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>

#define TEMPO_ATUALIZAR_TELA 0
#define TEMPO_CONTAR_CLIQUES 1



// Struct de tempos para facilitar lidar com multiplos relogios ao mesmo tempo
typedef struct{

        Uint32 tamanho;

        int* total;
        int* atual;

} tempos;


// Struct usado para criar e salvar estado dos baloes
typedef struct{

	SDL_Rect pos;
	SDL_Color cor;

	Uint32 elipseRaioX;
	Uint32 elipseRaioY;

} balao;

// Aloca memoria para struct de tempos
void iniciaTempos(tempos* structTempos, int nTempos, int* vetorTempos);
// Desaloca memoria do struct de tempos
void destroiTempos(tempos* structTempos);

// Criado para alterar nTempos no struct de tempos
// Cada tempo a ser alterado sera definido por atualizar
//
// Retorna 1 se:
// 	- Se evento foi lido no periodo
//
// Retorna (posicao do tempo esgotado no struck + 10)*(-1) se:		-> Retorno "estranho" para evitar reutilizar o 0 e diferenciar os tempos no array
// 	- Sucesso e o tempo de espera esgotou
//
// * Foi tentado usar 0 para ler caso de erro, mas SDL eh estranho e retorna erro mesmo quando nao tem (????)
//
int AUX_EventoOuMultiplosTempos(SDL_Event* evt, tempos* structTempos, int* atualizar, int nMudancas);


// Altera um dos tempos
// Baseado na funcao do exercicio 1.5.2, mas usa struct de tempos ao inves de tempo de espera
int AUX_EventoOuUmTempo(SDL_Event* evt, tempos* structTempos, Uint32 atualizar);

int criaEventoCliques(SDL_Event* evt, int cliques);	// Edita evento passado por referencia
							// Para ser UserEvent de multiclick e da PushEvent.
							// Retorna resultado de Pushevent

// Cria balao no array de baloes usando a heap
void criaBalao(balao** baloes,int* baloesInicio,int* baloesFim,SDL_Event* evt, SDL_Color*cores);

// Destroi um balao na heap
void destroiBalao(balao** baloes, int* baloesInicio,int* baloesFim, int posBalao);

// Desaloca array de baloes
void desalocaTodosBaloes(balao**baloes, int* baloesInicio,int* baloesFim);

// Movimenta todos os baloes o array
void movimentaBaloes(balao**baloes, int* baloesInicio,int* baloesFim);

// Tenta estourar um dos baloes alocados
void tentaEstourarBalao(balao**baloes, int* baloesInicio,int* baloesFim, int mouseX, int mouseY);

int colisaoBalaoMouse(balao**baloes, int posBalao, int mouseX, int mouseY);

/*
 *
 *	Percebi que SDL_MouseButtonEvent ja tem uma variavel de contagem de clicks
 *	Mas para efeitos de exercicio, fiz usando o SDL_UserEvent como proposto
 *
 *
 *
 *
 */




int main(int argc, char* args[]){

	/* Inicia SDL */
	SDL_Init(SDL_INIT_EVERYTHING);
            SDL_Window* win = SDL_CreateWindow("99 baloes",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         700, 600, SDL_WINDOW_SHOWN
                      );

        SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

        SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
        SDL_RenderClear(ren);

	/* Inicia variaveis*/
	 SDL_Color cores[10] = {
		 			{0xAA,0x00,0xFF,0xFF},	// Roxo
                                        {0xFF,0x00,0x00,0xFF},	// Vermelho
                                        {0x00,0xFF,0x00,0xFF},	// Verde
                                        {0x00,0x00,0xFF,0xFF},	// Azul
                                        {0xFF,0xFF,0x00,0xFF},	// Amarelo
                                        {0x00,0x00,0x00,0xFF},	// Branco
                                        {0xFF,0xBB,0xAA,0xFF},	// Rosa
                                        {0x00,0xFF,0xFF,0xFF},	// Azul claro
                                        {0xFF,0xAA,0x00,0xFF},	// Laranja
                                        {0xBB,0xBB,0xBB,0xFF}	// Cinza
                                };

        balao* baloes[100]; // Apesar de 100 espacos, so usa no maximo 99
	for(int i = 0; i < 100; i++){
		baloes[i] = NULL;
	}

	int baloesInicio = 0;
	int baloesFim = 0;

	SDL_Event evt;
	Uint32 estado = 0; // 0 -> 0CLicks. 1-> 1+Clicks.
	Uint32 cliques = 0;

	// 0-> tempo de atualizar a tela (17ms = ~60 fps)
	// 1-> tempo de contar cliques
	int vetorTempos[2] = {17, 250};

	tempos TemposMS;
	iniciaTempos(&TemposMS, 2, vetorTempos);

	while(1){
		/* Desenho */
		
		//Fundo
                SDL_SetRenderDrawColor(ren, 0xFF,0xFF,0xFF,0x00);
                SDL_RenderClear(ren);

		//Baloes
		for(int i = baloesInicio; i != baloesFim; i = (i+1)%100){
			if (baloes[i]){

				fprintf(stderr, "Desenhando balao na posicao (%d, %d)\n", baloes[i]->pos.x, baloes[i]->pos.y);

				filledEllipseRGBA(ren,
                                                baloes[i]->pos.x, baloes[i]->pos.y, baloes[i]->elipseRaioX, baloes[i]->elipseRaioY,
                                                baloes[i]->cor.r,baloes[i]->cor.g,baloes[i]->cor.b,baloes[i]->cor.a);

				const Sint16 linhaBalaoX[4] = {	baloes[i]->pos.x,
								(baloes[i]->pos.x - 5),
								(baloes[i]->pos.x + 2),
								(baloes[i]->pos.x - 7)
								};

				const Sint16 linhaBalaoY[4] = {	(baloes[i]->pos.y + baloes[i]->elipseRaioY),
								(baloes[i]->pos.y + baloes[i]->elipseRaioY + 2),
								(baloes[i]->pos.y + baloes[i]->elipseRaioY + 4),
								(baloes[i]->pos.y + baloes[i]->elipseRaioY + 6)
								};
				bezierRGBA(ren,
						linhaBalaoX, linhaBalaoY, 4, 4,
						0x00,0x00,0x00,0xFF);

			}
		}

		SDL_RenderPresent(ren);
		/* Execucao */

		switch(estado){
			case 0:
				int isevt_e0 = AUX_EventoOuUmTempo(&evt, &TemposMS, TEMPO_ATUALIZAR_TELA);
				if(isevt_e0){
					switch(evt.type){
						case SDL_QUIT:
							goto FIM;
							break;
						case SDL_MOUSEBUTTONDOWN:
							// Botao esquerdo clicado
							switch(evt.button.button){
								case SDL_BUTTON_LEFT:	// Botao esquerdo clicado -> vai para estado de criar balao
									// Atualiza variaveis relacionadas ao clique
									TemposMS.atual[TEMPO_CONTAR_CLIQUES] = TemposMS.total[TEMPO_CONTAR_CLIQUES];
		                                                        cliques = 1;
                		                                        // Troca estado
									++estado;
                                		                        estado %= 2;
									break;
								case SDL_BUTTON_RIGHT:   // Botao direito clicado -> tenta consumir balao
                                                                        // Atualiza variaveis relacionadas ao clique
									tentaEstourarBalao(baloes, &baloesInicio, &baloesFim, evt.button.x, evt.button.y);
                                                                        break;
							}
							break;
						case SDL_MOUSEMOTION:
							SDL_FlushEvent(SDL_MOUSEMOTION);
							break;
						case SDL_USEREVENT:
							criaBalao(baloes, &baloesInicio, &baloesFim, &evt, cores);
							// Desaloca memoria do evento
							free(evt.user.data1); evt.user.data1 = NULL;
							free(evt.user.data2); evt.user.data2 = NULL;
							break;
					}
				}
				else{	// Acabou tempo de atualizacao de tela
					movimentaBaloes(baloes, &baloesInicio, &baloesFim);
				}
				break;

			case 1:
				int atualizar[2] = {0, 1};
				int isevt_e1 = AUX_EventoOuMultiplosTempos(&evt, &TemposMS, atualizar, 2);
				switch(isevt_e1){
					case -TEMPO_CONTAR_CLIQUES-10:	// Tempo de 250ms dos cliques esgotou
						criaEventoCliques(&evt, cliques);
                                                ++estado;
                                                estado %= 2;
                                                break;
					case -TEMPO_ATUALIZAR_TELA-10:	// Tempo de 17ms de atualizacao da tela esgotou
						movimentaBaloes(baloes, &baloesInicio, &baloesFim);
                                                break;
					case 0:				// Um erro ocorreu
						/*
						 * A ideia era realizar rotina de erro aqui
						 * mas nao consegui entender como funciona no SDL
						 * ele retorna erro mesmo onde nao tem em WaitEventTimeout
						 *
						 */
						break;
					case 1:				// Um evento ocorreu
						switch(evt.type){
                                                	case SDL_QUIT:          // Sai do programa
                                                        	goto FIM;
	                                                        break;
        	                                        case SDL_MOUSEBUTTONDOWN:
                	                                        switch(evt.button.button){
                        	                                        case SDL_BUTTON_LEFT: // ButEsquerdo -> nao troca estado, so atualiza variaveis de clique
                                	                                        TemposMS.atual[TEMPO_CONTAR_CLIQUES] = TemposMS.total[TEMPO_CONTAR_CLIQUES];
                                        	                                cliques++;
                                                	                        break;
									case SDL_BUTTON_RIGHT:   // Botao direito clicado -> tenta consumir balao
												 // Depois, troca de estado
        	                                                                tentaEstourarBalao(baloes, &baloesInicio, &baloesFim, evt.button.x, evt.button.y);
										++estado;
										estado%=2;
                	                                                        break;
                                                        	}
	                                                        break;
        	                                        case SDL_MOUSEMOTION:   // Muda de estado
								criaEventoCliques(&evt, cliques);
		                                                ++estado;
                		                                estado %= 2;
                                		                break;
                                        	}
						break;

                                }
				break;

			}
		}

FIM:
	destroiTempos(&TemposMS);
	desalocaTodosBaloes(baloes, &baloesInicio,&baloesFim);

	SDL_DestroyRenderer(ren);
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
}



	// Aloca um struct de tempos
void iniciaTempos(tempos* structTempos, int nTempos, int* vetorTempos){
	structTempos->tamanho = nTempos;
	
	structTempos->total = (int*) malloc( nTempos* sizeof(Uint32));
	structTempos->atual = (int*) malloc( nTempos* sizeof(Uint32));

	for(int i = 0; i < nTempos; i++){
		structTempos->total[i] = structTempos->atual[i] = vetorTempos[i];
	}

}
	// Desaloca um struct de tempos
void destroiTempos(tempos* structTempos){
	free(structTempos->total);
	free(structTempos->atual);
}


int criaEventoCliques(SDL_Event* evt, int cliques){
	// Cria evento e aloca memoria
	evt->user.type = SDL_USEREVENT;
	evt->user.code = cliques;
	evt->user.data1 = (int*) malloc(sizeof(int));
	evt->user.data2 = (int*) malloc(sizeof(int));

	// Salva posicao do mouse em data1 e data2
	SDL_GetMouseState(evt->user.data1, evt->user.data2);

	// Tenta jogar evento na fila
	int result = SDL_PushEvent(evt);


	// Se falhar, diz que falhou e desaloca memoria (seta para NULL para evitar dangling pointer)
	if (result < 0){
		fprintf(stderr, "SDL_PushEvent falhou: %s\n", SDL_GetError());

		free(evt->user.data1); evt->user.data1 = NULL;
		free(evt->user.data2); evt->user.data2 = NULL;
	}

	// Retorna se o evento foi enviado ou nao para a fila
	return result;

}

int AUX_EventoOuMultiplosTempos(SDL_Event* evt, tempos* structTempos, int* atualizar, int nMudancas){
	// Pega tempo anterior
        int antes = SDL_GetTicks();

	// Busca primeiro dos tempos selecionados a acabar
	int posicaoMinimo = -1;
	int valorMinimo = INT_MAX;
	for(int i = 0; i < nMudancas; i++){

		if (structTempos->atual[ atualizar[i] ] < valorMinimo){
			valorMinimo = structTempos->atual[ atualizar[i] ];
			posicaoMinimo = atualizar[i];
		}
	}
	
	// Usa tempo achado para chamar WatiEventTimeout
        int isevt =  SDL_WaitEventTimeout( evt , valorMinimo );
	/*
	const char* error = SDL_GetError();
	if (error && *error){   // Achou erro -> Retorna 0
                        fprintf(stderr, "Erro em SDL_WaitEventTimeout: %s\nValorminimo: %d\nposicaoMinimo: %d\nAUX_EventoOuMultiplosTempos\n", error, valorMinimo, posicaoMinimo);
                        return 0;
        }

	*/

	// Atualiza todos os tempos
	// `agora` fora do loop para evitar problemas com escalonamento do SO.
	// Todos tem mesma variacao
	int agora = SDL_GetTicks();
	for(int i = 0; i < nMudancas; i++){

		// If-Else para evitar atribuicao de negativo para Uint32
		if( (agora-antes) < structTempos->atual[ atualizar[i] ]){
			structTempos->atual[ atualizar[i] ] -= (agora-antes);
		}
		else{
			structTempos->atual[ atualizar[i] ] = 0;
		}
        }


	// Reseta o valor escolhido para WaitEventTimeout e retorna posicao do tempo resetado
	if (!isevt){

		structTempos->atual[posicaoMinimo] = structTempos->total[ posicaoMinimo ];

		return (-posicaoMinimo-10);
	}

	// Retorna que evento ocorreu
	else{
		return 1;
	}
}


int AUX_EventoOuUmTempo(SDL_Event* evt, tempos* structTempos, Uint32 atualizar){

        //Pega tempo anterior
        Uint32 antes = SDL_GetTicks();

        int isevt =  SDL_WaitEventTimeout( evt , structTempos->atual[atualizar] );
	
	/*
	const char* error = SDL_GetError();
	if (error && *error){   // Achou erro -> Retorna 0
                fprintf(stderr, "Erro em SDL_WaitEventTimeout: %s\nAUX_EventoOuUmTempo\n", error);
        }
	*/

        //Lida com atualizacao do valor de espera
	Uint32 agora = SDL_GetTicks();
	if(isevt){
		//If-Else para evitar atriubuicao de negativo em Uint32
		if ((agora- antes) < structTempos->atual[atualizar]){
			structTempos->atual[atualizar] -= ( agora- antes );
		}
		else{
			structTempos->atual[atualizar] = 0;
		}
	}
	else{	// Se tempo acabou e nao houve evento, reseta tempo
		structTempos->atual[atualizar] = structTempos->total[atualizar];
	}
        return isevt;
}

void criaBalao(balao** baloes,int* baloesInicio,int* baloesFim,SDL_Event* evt, SDL_Color*cores){

	fprintf(stderr, "cria balao: %d\n", * baloesFim);
	// Buffer circular tem espaco livre
	if ( ( (1 + (*baloesFim))%100) != (*baloesInicio) ){
		// Aloca fim do array
		baloes[*baloesFim] = (balao*) malloc(sizeof(balao));

		// Define posicao inicial do balao
		baloes[*baloesFim]->pos.w = baloes[*baloesFim]->pos.h = 1;
		baloes[*baloesFim]->pos.x = *((int*) evt->user.data1);
		baloes[*baloesFim]->pos.y = *((int*) evt->user.data2);

		// Define cor do balao
		baloes[*baloesFim]->cor = cores[(evt->user.code)%10];	// Ate 10 cores

		// Define tamanho do balao
		baloes[*baloesFim]->elipseRaioX = 5;
		baloes[*baloesFim]->elipseRaioY = 9;

		// Atualiza fim do vetor
		(*baloesFim) = (1+(*baloesFim))%100;

	}
	else{
		 fprintf(stderr, "Erro: buffer cheio (incapaz de criar mais baloes!)\n");
	}
	
}

void destroiBalao(balao** baloes, int* baloesInicio,int* baloesFim, int posBalao){
	fprintf(stderr, "destroi balao: %d\n", posBalao);	
	// Desaloca balao se nao for NULL
	if (baloes[posBalao]){
                free(baloes[posBalao]); baloes[posBalao] = NULL;
	}
	else{
                 fprintf(stderr, "Erro: buffer posicao vazia (impossivel desalocar balao)\n");
        }

	// Tenta atualizar baloesInicio a cada desalocacao
	while(	   !baloes[*baloesInicio]
		&& ( (*baloesInicio) != ( (*baloesFim)))
		){
		(*baloesInicio) = ((1+ (*baloesInicio))%100);
	}
	fprintf(stderr, "inicio baloes: %d\nfim baloes %d\n\n", *baloesInicio, *baloesFim);
}

void desalocaTodosBaloes(balao**baloes, int* baloesInicio,int* baloesFim){
	// Desaloca todos os baloes nao nulos no buffer
	for(int i = *baloesInicio; i  != *baloesFim; i = (i+1)%100){
		if (baloes[i]){
         	       free(baloes[i]); baloes[i] = NULL;
	        }
	}
}

void movimentaBaloes(balao**baloes, int* baloesInicio,int* baloesFim){
	// Tenta movimentar todos os baloes do array
	for(int i = *baloesInicio; i != *baloesFim; i= (i+1)%100){
		if (baloes[i]){
			baloes[i]->pos.x +=1;
			baloes[i]->pos.y -=1;

			// Se balao for muito para cima da tela, desaloca
			if(baloes[i]->pos.y <= -20){
				destroiBalao(baloes, baloesInicio, baloesFim, i);
			}
		}
	}
}


void tentaEstourarBalao(balao**baloes, int* baloesInicio,int* baloesFim, int mouseX, int mouseY){
	// Procura no array balar nas coordenadas
	for(int i = *baloesInicio; i  != *baloesFim; i = (i+1)%100){
		if(baloes[i]){
			if(colisaoBalaoMouse(baloes, i, mouseX, mouseY)){

				destroiBalao(baloes, baloesInicio, baloesFim, i);
			}
		}
	}
}

int colisaoBalaoMouse(balao**baloes, int posBalao, int mouseX, int mouseY){
	if (!baloes[posBalao]){
		fprintf(stderr, "Erro: Tentando calcular colisao de mouse com balao nao criado\n");
		return 0; 
	}

	int diferencaHorizontal = ( (mouseX - baloes[posBalao]->pos.x) * (mouseX - baloes[posBalao]->pos.x) );
	int diferencaVertical = ( (mouseY - baloes[posBalao]->pos.y) * (mouseY - baloes[posBalao]->pos.y) );

	double norma = (
			((double) (diferencaHorizontal)) / ((baloes[posBalao]->elipseRaioX) * (baloes[posBalao]->elipseRaioX) )
		+	(((double) (diferencaVertical))/ ((baloes[posBalao]->elipseRaioY) * (baloes[posBalao]->elipseRaioY)))
		);
	return (norma <= 1.0);
}

