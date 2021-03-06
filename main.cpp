#include <iostream>
#include <SDL2/SDL.h>
#include "Config.h"
#include "Map.h"
#include "Snake.h"
#include "Fruit.h"
#include "Level.h"


using namespace std;

int main(int argc,char *argv[]){
    srand(time(nullptr));

    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow(
            "NMSL-Retro-Snaker",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            MAP_WIDTH * TILE_SIZE,MAP_HEIGHT * TILE_SIZE,
            SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);//use hardware and enable the present vsync

    auto &snake = Snake::instance();
    Fruit::generate();

    int frame_count = 0;

    Level::genLevel1();

    bool is_quit = false;
    SDL_Event event;
    while(!is_quit){
        SDL_SetRenderDrawColor(renderer,0,0,0,255);//set clear color to black
        SDL_RenderClear(renderer);
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_WINDOWEVENT:
                    if(event.window.event == SDL_WINDOWEVENT_CLOSE){//clicked red cross
                        is_quit = true;
                    }
                    break;
                case SDL_KEYDOWN:
                    if(event.key.keysym.scancode == SDL_SCANCODE_UP){//up arrow
                        snake.set_direction(Snake::Direction::up);
                    }else if(event.key.keysym.scancode == SDL_SCANCODE_LEFT){//left arrow
                        snake.set_direction(Snake::Direction::left);
                    }else if(event.key.keysym.scancode == SDL_SCANCODE_RIGHT){//right arrow
                        snake.set_direction(Snake::Direction::right);
                    }else if(event.key.keysym.scancode == SDL_SCANCODE_DOWN){//down arrow
                        snake.set_direction(Snake::Direction::down);
                    }else if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){//esc
                        is_quit = true;
                    }else if(event.key.keysym.scancode == SDL_SCANCODE_SPACE){
                        snake.increse_length();
                    }
                    break;
                default:
                    break;
            }
        }

        if(frame_count == 5){
            frame_count = 0;
            if(snake.update()){//died
                is_quit = true;
            }
        }
        ++frame_count;

        Map::instance().draw(renderer);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}
