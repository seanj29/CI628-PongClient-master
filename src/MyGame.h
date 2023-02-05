#ifndef __MY_GAME_H__
#define __MY_GAME_H__

#include <iostream>
#include <vector>
#include <string>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "SDL.h"
#include "SDL_image.h"


static struct GameData {
    std::string board;
    int clientNum;
    bool GameFinished = false;
    bool inputEnabled = false;
    bool yourTurn = false;
    SDL_Rect grid[3][3] = {};
    int height = 600;
    int width = 800;
    SDL_Point mousePos;
} game_data;

class MyGame {

    private:

    public:
        std::vector<std::string> messages;

        void on_receive(std::string message, std::vector<std::string>& args);
        void send(std::string message);
        void input(SDL_Event& event);
        void update();
        char* fForWin();
        void createButton(SDL_Window* window);
        void createEnd(SDL_Window* window);
        void createGrid(SDL_Renderer* renderer, SDL_Texture* texture1, SDL_Texture* texture2);
        void render(SDL_Renderer* renderer);
};

#endif