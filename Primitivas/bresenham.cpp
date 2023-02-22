#include <SDL.h>
#include <math.h>
#include <iostream>

#define TITLE "Bresenham"
#define HEIGHT 600
#define WIDTH 800
#define XI 100
#define YI -100
#define XF -275
#define YF 45

/* 
    Implementado considerando os diferentes quadrantes e as
    simetrias entre o plano xy;

    Os pontos de limite do segmento de linha podem ser passados
    em qualquer ordem - não é necessário, por exemplo, passar o ponto mais
    à esquerda como inicial...
*/
void desenharLinha(SDL_Renderer* r, int xi, int yi, int xf, int yf){
    int dx = std::abs(xf-xi), dy = std::abs(yf-yi);
    int a, b, wa_offset, wb_offset, end, x=xi, y=yi;
    int* wa;
    int* wb;

    if (dx > dy){
        a = dx;
        b = dy;
        wa = &x;
        wb = &y;
        end = xf;
        wa_offset = std::round((xf-xi)/(dx+1.f));
        wb_offset = std::round((yf-yi)/(dy+1.f));

    }
    else {
        a = dy;
        b = dx;
        wa = &y;
        wb = &x;
        end = yf;
        wb_offset = std::round((xf-xi)/(dx+1.f));
        wa_offset = std::round((yf-yi)/(dy+1.f));
    }

    int twoB = 2*b, twoBA = 2*(b-a);
    int p = 2*b-a;
    
    SDL_RenderDrawPoint(r, x+WIDTH/2, HEIGHT/2-y);
    
    while (*wa != end){
        *wa += wa_offset;
        
        if (p<0)
            p += twoB;

        else {
            *wb += wb_offset;
            p += twoBA;
        }

        SDL_RenderDrawPoint(r, x+WIDTH/2, HEIGHT/2-y);
    }
}


int main(){

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

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        desenharLinha(renderer, XI, YI, XF, YF);

        SDL_RenderPresent(renderer);

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}