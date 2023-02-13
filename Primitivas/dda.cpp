#include <SDL.h>
#include <math.h>
#include <iostream>

#define ROUND(n) ((int)(n+0.5))

#define HEIGHT 600
#define WIDTH 800
#define XI WIDTH/2 
#define YI HEIGHT/2
#define XF WIDTH/2-175 
#define YF HEIGHT/2-180
#define TITLE "DDA"

void desenharLinhaDDA(SDL_Renderer* r, int xi, int yi, int xf, int yf){
    
    int k, steps, dx=xf-xi, dy=yf-yi;
    float xIncrement, yIncrement, x=xi, y=yi;

    if (std::abs(dx) > std::abs(dy))
        steps = std::abs(dx);
    else
        steps = std::abs(dy);

    xIncrement = dx / (float) steps;
    yIncrement = dy / (float) steps;

    SDL_RenderDrawPoint(r, ROUND(x), ROUND(y));
    
    for (k=0; k<steps; k++){
        x += xIncrement;
        y += yIncrement;
    
        SDL_RenderDrawPoint(r, ROUND(x), ROUND(y));
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
        desenharLinhaDDA(renderer, XI, YI, XF, YF);

        SDL_RenderPresent(renderer);

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}