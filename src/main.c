#include <SDL2/SDL.h>
#include <stdio.h>

#include "blocks.h"
#define WINDOW_WIDTH 360
#define WINDOW_HEIGHT 720
#define CW 20
#define CH 20
#define ROWS WINDOW_HEIGHT / CH
#define COLS WINDOW_WIDTH / CW
#define SPEED 0.001

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    int running;
} Context;

typedef int BlockStack[ROWS][COLS];

typedef struct {
    int no;
    Block current;
    Block next;
    BlockStack stack;
    double difficulty;
    double y;
} Level;

int init(Context *ctx) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could initialize! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    ctx->window = SDL_CreateWindow("My Window", SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH,
                                   WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

    if (ctx->window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }
    ctx->renderer =
        SDL_CreateRenderer(ctx->window, -1, SDL_RENDERER_ACCELERATED);
    if (ctx->renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n",
               SDL_GetError());
        SDL_DestroyWindow(ctx->window);
        SDL_Quit();
        return 0;
    }
    ctx->running = 1;
    return 1;
}

void draw_block_stack(SDL_Renderer *renderer, BlockStack stack) {
    for (int j = 0; j < ROWS; j++) {
        for (int i = 0; i < COLS; i++) {
            if (stack[j][i]) {
                SDL_Rect r = {i * CW, j * CH, CW, CH};
                SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
                SDL_RenderFillRect(renderer, &r);
            }
        }
    }
}

void draw_block(SDL_Renderer *renderer, Block *block) {
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
            int cell = BLOCK_TYPES[block->type][block->rotation % 4][j][i];
            if (cell) {
                SDL_Rect r = {(block->x + i) * CW, (block->y + j) * CH, CW, CH};
                switch (block->type) {
                    case 'I':
                        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF,
                                               0xFF);
                        printf("I");
                        break;
                    case 'O':
                        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0x00,
                                               0xFF);
                        break;
                    case 'T':
                        SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00,
                                               0xFF);
                        break;
                    case 'L':
                        printf("L");
                        SDL_SetRenderDrawColor(renderer, 0x80, 0x00, 0x80,
                                               0xFF);
                        break;
                    case 'S':
                        SDL_SetRenderDrawColor(renderer, 0xA5, 0x2A, 0x2A,
                                               0xFF);
                        break;
                    case 'Z':
                        SDL_SetRenderDrawColor(renderer, 0x80, 0x80, 0x80,
                                               0xFF);
                        break;
                }
                SDL_RenderFillRect(renderer, &r);
            }
        }
    }
}

void draw_grid(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0xbb, 0xbb, 0xbb, 0xff);
    for (int j = 0; j < WINDOW_HEIGHT; j += CH) {
        SDL_RenderDrawLine(renderer, 0, j, WINDOW_WIDTH, j);
    }
    for (int i = 0; i < WINDOW_WIDTH; i += CW) {
        SDL_RenderDrawLine(renderer, i, 0, i, WINDOW_HEIGHT);
    }
}

int has_collision(Block block, BlockStack stack) {
    int x = block.x;
    int y = block.y + 1;
    int h = block_get_height(block.type, block.rotation);
    if (block.y >= ROWS - h) {
        return 1;
    }

    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
            int cell = BLOCK_TYPES[block.type][block.rotation % 4][j][i];
            if (cell) {
                if (stack[y + j][x + i]) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

void fix_block(Block *block, BlockStack stack) {
    int row = block->y;
    int col = block->x;
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
            int cell = BLOCK_TYPES[block->type][block->rotation % 4][j][i];
            if (cell) {
                stack[row + j][col + i] = 1;
            }
        }
    }
    block->x = 0;
    block->y = 0;
}

void update(Context *ctx, Level *level) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            ctx->running = 0;
        }
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_q:
                    ctx->running = 0;
                    break;
                case SDLK_SPACE:
                    level->current.rotation++;
                    int bw = block_get_width(level->current.type,
                                             level->current.rotation);
                    if (level->current.x > COLS - bw) {
                        level->current.x = COLS - bw;
                    }
                    break;
                case SDLK_LEFT:
                    if (level->current.x > 0) {
                        level->current.x -= 1;
                    }
                    break;
                case SDLK_RIGHT:
                    if (level->current.x <
                        COLS - block_get_width(level->current.type,
                                               level->current.rotation)) {
                        level->current.x += 1;
                    }
                    break;
            }
        }
    }
    level->current.y = (int)level->y;
    level->y += level->difficulty * SPEED;
    if (has_collision(level->current, level->stack)) {
        level->y = 0;
        fix_block(&level->current, level->stack);
    }
}

void render(Context *ctx, Level *level) {
    SDL_SetRenderDrawColor(ctx->renderer, 255, 255, 255, 255);
    SDL_RenderClear(ctx->renderer);
    draw_block_stack(ctx->renderer, level->stack);
    draw_grid(ctx->renderer);
    draw_block(ctx->renderer, &level->current);
    SDL_RenderPresent(ctx->renderer);
}

void cleanup(Context *ctx) {
    SDL_DestroyRenderer(ctx->renderer);
    SDL_DestroyWindow(ctx->window);
    SDL_Quit();
}

int main(int argc, char *argv[]) {
    Context ctx;
    if (!init(&ctx)) {
        return 1;
    }
    Level level;
    level.current.type = 0;
    level.current.rotation = 0;
    level.current.x = 0;
    level.current.y = 0;
    for (int j = 0; j < ROWS; j++) {
        for (int i = 0; i < COLS; i++) {
            level.stack[j][i] = 0;
        }
    }

    level.y = 0;
    level.difficulty = 1;
    while (ctx.running) {
        update(&ctx, &level);
        render(&ctx, &level);
    }
    cleanup(&ctx);

    return 0;
}
