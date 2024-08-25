#include <SDL2/SDL.h>
#include <stdio.h>
#include "blocks.h"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define CW 20
#define CH 20
#define ROWS WINDOW_HEIGHT / CH
#define COLS WINDOW_WIDTH / CW



void draw_block(SDL_Renderer *renderer, Block *block){
	for(int j = 0; j < 4; j++) {
		for(int i = 0; i < 4; i++) {
			int cell = BLOCK_TYPES[block->type][block->rotation % 4][j][i];
			if(cell) {
				SDL_Rect r ={ block->x + i * CW, block->y +j * CH, CW, CH };
				SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 0xFF);
				SDL_RenderFillRect(renderer, &r);
			}
		}
	}

}

void draw_grid(SDL_Renderer* renderer ){
	SDL_SetRenderDrawColor(renderer,0xbb, 0xbb,0xbb, 0xff);
	for(int j= 0; j < WINDOW_HEIGHT; j+= CH) {
		SDL_RenderDrawLine(renderer,0,j,WINDOW_WIDTH,j);
	}
	 for(int i= 0; i < WINDOW_WIDTH; i += CW) {
		 SDL_RenderDrawLine(renderer,i,0,i,WINDOW_HEIGHT);
	 }

}

int main(int argc, char* argv[]) {
        SDL_Window *window = NULL;
        SDL_Renderer *renderer = NULL;

        if(SDL_Init(SDL_INIT_VIDEO) < 0) {
                printf("SDL could initialize! SDL_Error: %s\n", SDL_GetError());
                return 1;

        }

        window = SDL_CreateWindow("My Window",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,WINDOW_WIDTH,WINDOW_HEIGHT,SDL_WINDOW_SHOWN);

        if(window == NULL) {
                printf("Window could not be created! SDL_Error: %s\n",SDL_GetError());
		SDL_Quit();
                return 1;
        }
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if(renderer == NULL) {
                printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
                SDL_DestroyWindow(window);
                SDL_Quit();
                return 1;

        }

        int running = 1;
        SDL_Event event;
	Block current ={0, 0,0,0};
	float y = 0;
        while(running) {
                while(SDL_PollEvent(&event)) {
                        if(event.type == SDL_QUIT) {
				running = 0;
                        }
			if(event.type == SDL_KEYDOWN) {
				switch(event.key.keysym.sym) {
					case SDLK_q:
						running = 0;
						break;
					case SDLK_SPACE:
						current.rotation ++;
						break;
					
					case SDLK_LEFT:
						current.x -= CW;
						break;
					case SDLK_RIGHT:
						current.x += CW;
						break;

				}
			}
                }
                SDL_SetRenderDrawColor(renderer,255,255,255,255);
                SDL_RenderClear(renderer);

		draw_grid(renderer);



		draw_block(renderer, &current);
		current.y = (int) y;
		y += 0.1;
		if(y > 720.0) {
			y = 0;
		}

                SDL_RenderPresent(renderer);
        }

        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
	
	return 0;
}
