#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>

using namespace std;

bool initializeSDL(SDL_Window*& window, SDL_Renderer*& renderer, int screenWidth, int screenHeight) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
        return false;
    }

    window = SDL_CreateWindow("Collision", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_RESIZABLE);
    if (window == NULL) {
        cout << "Error: " << SDL_GetError() << endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        cout << "Error: " << SDL_GetError() << endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return false;
    }

    return true;
}

void drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius, Uint8 r, Uint8 g, Uint8 b) {
    SDL_SetRenderDrawColor(renderer, r, g, b, 255);
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x * x + y * y <= radius * radius) {
                SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
            }
        }
    }
}

int main(int argc, char *argv[]) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (!initializeSDL(window, renderer, 500, 500)) {
        return 1;
    }

    int c1x = 50; int c1y = 250; int c1r = 50; 
    int c2x = 250; int c2y = 50; int c2r = 50; 

    SDL_Event event;
    bool game = true;

    while (game) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) { game = false; break; }
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP: if (c2y - 10 >= c2r) { c2y -= 10; } break;
                    case SDLK_DOWN: if (c2y + 10 <= 500 - c2r) { c2y += 10; } break;
                    case SDLK_LEFT: if (c2x - 10 >= c2r) { c2x -= 10; } break;
                    case SDLK_RIGHT: if (c2x + 10 <= 500 - c2r) { c2x += 10; } break;
                    default: break;
                }
            }
        }

        c1x += 2;
        if (c1x - c1r > 500) { c1x = c1r; }
        int dx = c1x - c2x;
        int dy = c1y - c2y;
        int distance = sqrt(dx * dx + dy * dy);
        int md = c1r + c2r;
        Uint8 c1_red = 255;
        Uint8 c1_green = 0;
        Uint8 c1_blue = 0;
        Uint8 c2_red = (distance <= md) ? 255 : 0; 
        Uint8 c2_green = (distance <= md) ? 255 : 0;
        Uint8 c2_blue = (distance <= md) ? 0 : 255;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        drawCircle(renderer, c1x, c1y, c1r, c1_red, c1_green, c1_blue);

        drawCircle(renderer, c2x, c2y, c2r, c2_red, c2_green, c2_blue);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
