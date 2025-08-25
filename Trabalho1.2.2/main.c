#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>


#define AU 26

int main (int argc, char* args[]){


    int winHLimit = 1920;
    int winVLimit = 1080;

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* win = SDL_CreateWindow("Solar System",
                         SDL_WINDOWPOS_UNDEFINED,
                         SDL_WINDOWPOS_UNDEFINED,
                         winHLimit, winVLimit, SDL_WINDOW_SHOWN
                      );
    SDL_Renderer* ren = SDL_CreateRenderer(win, -1, 0);

    //Fundo
    SDL_SetRenderDrawColor(ren, 0x00,0x00,0x00,0x00);
    SDL_RenderClear(ren);

    /* Sol */
    int SunR = 3;
    int Sunx = winHLimit/2;
    int Suny = winVLimit/2;
    filledCircleRGBA(ren, Sunx, Suny, SunR, 0xFF, 0xFF, 0xFF, 0xFF);

    //Planetas e Orbitas

    /* Mercurio */
    int MercR = 1;		// Raio do planeta
    int MercOrbRx =10;		// Raio Horizontal da orbita
    int MercOrbRy =10;		// Raio Vertical da orbita
    /*
     * Excentricidade real da orbita -> 0.2056
     * Excentricidade representada da orbita -> 0.0000
     */

    double MercOrbHTilt = (-0.0803)*((double)AU);	// Desvio da orbita

    int MercOrbHCenter = Sunx + ((int)MercOrbHTilt);	// Centro orbital corrigido

    int Mercx = MercOrbHCenter + MercOrbRx;		// Posicao inicial do planeta no eixo x
    int Mercy = Suny;					// Posicao inicial do planeta no eixo y
    ellipseRGBA(ren, MercOrbHCenter, Suny, MercOrbRx, MercOrbRy, 0xFF, 0xFF, 0xFF, 0xFF); // Orbita
    filledCircleRGBA(ren, Mercx, Mercy, MercR, 0xA5, 0xA8, 0xAD, 0xFF);	// Planeta
    circleRGBA(ren, Mercx, Mercy, 6, 0xA5, 0xA8, 0xAD, 0xFF);	// Marcador (melhora visualizacao)

    /* Venus */
    int VenR = 1;
    int VenOrbRx =18;
    int VenOrbRy =18;
    /*
     * Excentricidade real da orbita -> 0.0068
     * Excentricidade representada da orbita -> 0.0000
    */
    double VenOrbHTilt = (-0.005)*((double)AU);

    int VenOrbHCenter = Sunx + ((int)VenOrbHTilt);

    int Venx = VenOrbHCenter;
    int Veny = Suny - VenOrbRy;
    ellipseRGBA(ren, VenOrbHCenter, Suny, VenOrbRx, VenOrbRy, 0xFF, 0xFF, 0xFF, 0xFF);
    filledCircleRGBA(ren, Venx, Veny, VenR, 0x66, 0xCC, 0x44, 0xFF);
    circleRGBA(ren, Venx, Veny, 6, 0x66, 0xCC, 0x44, 0xFF);

    /* Terra */
    int EarR = 1;
    int EarOrbRx =26;
    int EarOrbRy =26;
    /*
     * Excentricidade real da orbita -> 0.00167
     * Excentricidade representada da orbita -> 0.0000
    */
    double EarOrbHTilt = (-0.0167)*((double)AU);

    int EarOrbHCenter = Sunx + ((int)EarOrbHTilt);

    int Earx = EarOrbHCenter + EarOrbRx;
    int Eary = Suny;
    ellipseRGBA(ren, EarOrbHCenter, Suny, EarOrbRx, EarOrbRy, 0xFF, 0xFF, 0xFF, 0xFF);
    filledCircleRGBA(ren, Earx, Eary, EarR, 0x2E, 0x3A, 0x92, 0xFF);
    circleRGBA(ren, Earx, Eary, 6, 0x2E, 0x3A, 0x92, 0xFF);

    /* Marte */
    int MarsR = 1;
    int MarsOrbRx =38;
    int MarsOrbRy =38;
    /*
     * Excentricidade real da orbita -> 0.0934
     * Excentricidade representada da orbita -> 0.0000
    */
    double MarsOrbHTilt = (-0.142)*((double)AU);

    int MarsOrbHCenter = Sunx + ((int)MarsOrbHTilt);
    int Marsx = MarsOrbHCenter;
    int Marsy = Suny + MarsOrbRy;
    ellipseRGBA(ren, MarsOrbHCenter, Suny, MarsOrbRx, MarsOrbRy, 0xFF, 0xFF, 0xFF, 0xFF);
    filledCircleRGBA(ren, Marsx, Marsy, MarsR, 0xDD, 0x33, 0x00, 0xFF);
    circleRGBA(ren, Marsx, Marsy, 6, 0xDD, 0x33, 0x00, 0xFF);

    /* Jupiter */
    int JupR = 3;
    int JupOrbRx =133;
    int JupOrbRy =133;
    /*
     * Excentricidade real da orbita -> 0.0484
     * Excentricidade representada da orbita -> 0.0000
    */

    /*Aneis de Jupiter nao seriam visiveis a esta distancia*/
    double JupOrbHTilt = (-0.2543)*((double)AU);

    int JupOrbHCenter = Sunx + ((int)JupOrbHTilt);

    int Jupx =JupOrbHCenter + JupOrbRx;
    int Jupy = Suny;
    ellipseRGBA(ren, JupOrbHCenter, Suny, JupOrbRx, JupOrbRy, 0xFF, 0xFF, 0xFF, 0xFF);
    filledCircleRGBA(ren, Jupx, Jupy, JupR, 0xC8, 0x8B, 0x3A, 0xFF);
    circleRGBA(ren, Jupx, Jupy, 6, 0xC8, 0x8B, 0x3A, 0xFF);

    /* Saturno */
    int SatR = 2;
    int SatRingR = 3;
    int SatOrbRx =244;
    int SatOrbRy =243;
    /*
     * Excentricidade real da orbita -> 0.0541
     * Excentricidade representada da orbita -> 0.0904
    */

    /*Apesar de aneis serem elipses, representar em pixels seria dificil pela escala pequena, entao optei por manter como circulo*/
    double SatOrbHTilt = (-0.5416)*((double)AU);

    int SatOrbHCenter = Sunx + ((int)SatOrbHTilt);
    int Satx = SatOrbHCenter - SatOrbRx;
    int Saty =Suny;
    ellipseRGBA(ren, SatOrbHCenter, Suny, SatOrbRx, SatOrbRy, 0xFF, 0xFF, 0xFF, 0xFF); // Orbita
    filledCircleRGBA(ren, Satx, Saty, SatR, 0xC3, 0x92, 0x4F, 0xFF); // Planeta
    circleRGBA(ren, Satx, Saty, SatRingR, 0x7B, 0x78, 0x69, 0xFF); // Aneis
    circleRGBA(ren, Satx, Saty, 6, 0xC3, 0x92, 0x4F, 0xFF);	// Marcador (melhora visualizacao)

    /* Urano */
    int UraR = 2;
    int UraOrbRx =492;
    int UraOrbRy =491;
    /*
     * Excentricidade real da orbita -> 0.0472
     * Excentricidade representada da orbita -> 0.0637
    */

    /*Aneis de Urano nao seriam visiveis a esta distancia*/
    double UraOrbHTilt = (-0.8789)*((double)AU);

    int UraOrbHCenter = Sunx + ((int)UraOrbHTilt);
    int Urax = UraOrbHCenter - UraOrbRx;
    int Uray = Suny;
    ellipseRGBA(ren, UraOrbHCenter, Suny, UraOrbRx, UraOrbRy, 0xFF, 0xFF, 0xFF, 0xFF);
    filledCircleRGBA(ren, Urax, Uray, UraR, 0x27, 0xA9, 0xDF, 0xFF);
    circleRGBA(ren, Urax, Uray, 6, 0x27, 0xA9, 0xDF, 0xFF);

    /* Netuno */
    int NeptR = 2;
    int NeptOrbRx =769;
    int NeptOrbRy =769;
    /*
     * Excentricidade real da orbita -> 0.0086
     * Excentricidade representada da orbita -> 0.0000
    */

    /*Aneis de Netuno nao seriam visiveis a esta distancia*/
    double NeptOrbHTilt = (-0.3396)*((double)AU);

    int NeptOrbHCenter = Sunx + ((int) NeptOrbHTilt);
    int Neptx = NeptOrbHCenter + NeptOrbRx;
    int Nepty = Suny;
    ellipseRGBA(ren, NeptOrbHCenter, Suny, NeptOrbRx, NeptOrbRy, 0xFF, 0xFF, 0xFF, 0xFF);
    filledCircleRGBA(ren, Neptx, Nepty, NeptR, 0x3E, 0x54, 0xE8, 0xFF);
    circleRGBA(ren, Neptx, Nepty, 6, 0x3E, 0x54, 0xE8, 0xFF); 


    //Atualiza tela
    SDL_RenderPresent(ren);
    SDL_Delay(10000);


    //Fim do processo
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
