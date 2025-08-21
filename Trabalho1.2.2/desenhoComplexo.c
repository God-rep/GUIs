#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

int main (int argc, char* args[]){


    int winHLimit = 800;
    int winVLimit = 650;

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

    //Sol
    int SunR = 50;
    int Sunx =((winHLimit/2)-SunR);
    int Suny = ((winVLimit/2)-SunR);
    filledCircleColor(ren, Sunx, Suny, SunR, 0xFFFFFF00);

    //Orbitas planetarias

    int MercOrbRx =;
    int MercOrbRy =;
    ellipseColor(ren, Sunx, Suny, MercOrbRx, MercOrbRy, 0xFFFFFF00);

    int VenOrbRx =;
    int VenOrbRy =;
    ellipseColor(ren, Sunx, Suny, VenOrbRx, VenOrbRy, 0xFFFFFF00);

    int EarOrbRx =;
    int EarOrbRy =;
    ellipseColor(ren, Sunx, Suny, EarOrbRx, EarOrbRy, 0xFFFFFF00);

    int MarsOrbRx =;
    int MarsOrbRy =;
    ellipseColor(ren, Sunx, Suny, MarsOrbRx, MarsOrbRy, 0xFFFFFF00);

    int JupOrbRx =;
    int JupOrbRy =;
    ellipseColor(ren, Sunx, Suny, JupOrbRx, JupOrbRy, 0xFFFFFF00);

    int SatOrbRx =;
    int SatOrbRy =;
    ellipseColor(ren, Sunx, Suny, SatOrbRx, SatOrbRy, 0xFFFFFF00);

    int UraOrbRx =;
    int UraOrbRy =;
    ellipseColor(ren, Sunx, Suny, SatOrbRx, SatOrbRy, 0xFFFFFF00);

    int NeptOrbRx =;
    int NeptOrbRy =;
    ellipseColor(ren, Sunx, Suny, NeptOrbRx, NeptOrbRy, 0xFFFFFF00);

    //Mercurio
    int Mercx =0;
    int Mercy =0;
    int MercR = 1;
    filledCircleColor(ren, Mercx, Mercy, MercR, 0xB1ADAD00);

    //Venus
    int Venx =0;
    int Veny =0;
    int VenR = 1;
    filledCircleColor(ren, Venx, Veny, VenR, 0xC18F1700);

    //Terra
    int Earx =0;
    int Eary =0;
    int EarR = 2;
    filledCircleColor(ren, Earx, Eary, EarR, 0x023CA700);

    //Marte
    int Marsx =0;
    int Marsy =0;
    int MarsR = 2;
    filledCircleColor(ren, Marsx, Marsy, MarsR, 0xE27B5800);

    //Jupiter
    int Jupx =0;
    int Jupy =0;
    int JupR = 5;
    /*Aneis de Jupiter nao seriam visiveis a esta distancia*/
    filledCircleColor(ren, Jupx, Jupy, JupR, 0x90614D00);

    //Saturno
    int Satx =0;
    int Saty =0;
    int SatR = 4;
    int SatRingR = 5;
    /*Apesar de aneis serem elipses, representar em pixels seria dificil pela escala pequena, entao optei por manter como circulo*/
    filledCircleColor(ren, Satx, Saty, SatR, 0xC5AB6E00);

    //Urano
    int Urax =0;
    int Uray =0;
    int UraR = 3;
    /*Aneis de Urano nao seriam visiveis a esta distancia*/
    filledCircleColor(ren, Urax, Uray, UraR, 0xD1EDF200);

    //Netuno
    int Neptx =0;
    int Nepty =0;
    int NeptR = 3;
    /*Aneis de Netuno nao seriam visiveis a esta distancia*/
    filledCircleColor(ren, Neptx, Nepty, NeptR, 0x0033FF00);

    SDL_RenderPresent(ren);
    SDL_Delay(5000);


    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
