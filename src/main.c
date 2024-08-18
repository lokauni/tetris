#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
        SDL_Window *window = NULL;
        SDL_Renderer *renderer = NULL;

        if(SDL_Init(SDL_INIT_VIDEO) < 0) {
                printf("SDL could initialize! SDL_Error: %s\n", SDL_GetError());
                return 1;

        }

        window = SDL_CreateWindow("My Window",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,320,180,SDL_WINDOW_SHOWN);

        if(window == NULL) {
                printf("Window could not be created! SDL_Error: %s\n",SDL_GetError());
                SDL_Quit ();
                return 1;
        }
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if(renderer == NULL) {
                printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
                SDL_DestroyWindow(window);
                SDL_Quit;
                return 1;
        }

        int running = 1;
        SDL_Event event;
        while(running) {
                while(SDL_PollEvent(&event)) {
                        if(event.type == SDL_QUIT) {
                        }
                }
                SDL_SetRenderDrawColor(renderer,255,255,255,255);
                SDL_RenderClear(renderer);

                SDL_RenderPresent(renderer);
        }

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit;
	
	return 0;
}
