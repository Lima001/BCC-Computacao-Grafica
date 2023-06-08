#include <SDL.h>
#include <cmath>
#include "matriz.h"

#define ROUND(n) ((int)(n+0.5))

#define T_INCREMENT 0.001

#define HEIGHT 600
#define WIDTH 800
#define TITLE "SPLINES"

#define CIRC_PRECISION 30
#define ANGLE (360/CIRC_PRECISION)*(M_PI/180.0f)
#define RADIUS 10

void desenharCircunferencia(SDL_Renderer* renderer, double xc, double yc, double raio, Uint8 r=255, Uint8 g=255, Uint8 b=255, Uint8 a=255){

    Matriz<double> pontos(2, CIRC_PRECISION);

    for (int i=0; i<CIRC_PRECISION; i++){
        pontos[0][i] = xc + cos(ANGLE*i) * raio;
        pontos[1][i] = yc + sin(ANGLE*i) * raio;
    }

    SDL_SetRenderDrawColor(renderer, r, g, b, a);

    for(int i=1; i<CIRC_PRECISION; i++)
        SDL_RenderDrawLine(renderer,  WIDTH/2+pontos[0][i-1], HEIGHT/2-pontos[1][i-1],  WIDTH/2+pontos[0][i], HEIGHT/2-pontos[1][i]);

    SDL_RenderDrawLine(renderer,  WIDTH/2+pontos[0][CIRC_PRECISION-1], HEIGHT/2-pontos[1][CIRC_PRECISION-1],  WIDTH/2+pontos[0][0], HEIGHT/2-pontos[1][0]);

}

int main(){

    double t = 0;
    int i;

    Matriz<double> f(1,2);
    
    Matriz<double> tm(1,4); 
    tm[0][0] = 1;
    tm[0][1] = t;
    tm[0][2] = t*t;
    tm[0][3] = tm[0][2]*t;

    Matriz<double> m = lerMatrizArquivo("Dados/matriz_polinomios.txt",4,4);
    Matriz<double> p = lerMatrizArquivo("Dados/pontos.txt",4,2);

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0){
        std::cout << "SDL Init Error - " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Event event;
    
    window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

    while (1){

        SDL_PollEvent(&event);
        
        if (event.type == SDL_QUIT)
            break;
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        f = tm*m*p;

        for (i=0; i<p.nLinha(); i++)
            desenharCircunferencia(renderer, p[i][0], p[i][1], RADIUS, 255, 165, 0);

        desenharCircunferencia(renderer, f[0][0], f[0][1], RADIUS, 177,156,217);
    
        SDL_RenderPresent(renderer);

        t = std::fmod((t+=T_INCREMENT),1);
        tm[0][1] = t;
        tm[0][2] = t*t;
        tm[0][3] = tm[0][2]*t;

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
};