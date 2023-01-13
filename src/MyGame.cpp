#include "MyGame.h"

void MyGame::on_receive(std::string cmd, std::vector<std::string>& args) {
    if (cmd == "GAME_DATA") {
        // we should have exactly 4 arguments
        if (args.size() == 4) {
            game_data.board = args.at(0);
            game_data.playerXTurn = args.at(1) == "true";
            game_data.GameFinished = args.at(2) == "true";
            game_data.clientNum = stoi(args.at(3));
        }
    } else {
        std::cout << "Received: " << cmd << std::endl;
    }
}

void MyGame::send(std::string message) {
    messages.push_back(message);
}

void MyGame::input(SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        game_data.mousePos.x = event.motion.x;
        game_data.mousePos.y = event.motion.y;
        switch (event.button.button)
        {
        case SDL_BUTTON_LEFT:
            send(event.type == SDL_MOUSEBUTTONDOWN ?"LEFT_DOWN": "LEFT_UP");
            for (int y = 0; y < 3; y++) {
                for (int x = 0; x < 3; x++) {
                    if (SDL_PointInRect(&game_data.mousePos, &game_data.grid[x][y])){
                        send(std::to_string(x));
                        send(std::to_string(y));
                        send(std::to_string(game_data.clientNum));
                }
            }
        }
            break;
        case SDL_BUTTON_RIGHT:
            send(event.type == SDL_MOUSEBUTTONDOWN ? "RIGHT_DOWN": "RIGHT_UP");
            for (int y = 0; y < 3; y++) {
                for (int x = 0; x < 3; x++) {
                    if (SDL_PointInRect(&game_data.mousePos, &game_data.grid[x][y])) {
                        send(std::to_string(x));
                        send(std::to_string(y));
                    }
                }
            }
            break;
        default:
            send(event.type == SDL_MOUSEBUTTONDOWN ? "OTHER_DOWN" :"OTHER_UP");
            for (int y = 0; y < 3; y++) {
                for (int x = 0; x < 3; x++) {
                    if (SDL_PointInRect(&game_data.mousePos, &game_data.grid[x][y])) {
                        send(std::to_string(x));
                        send(std::to_string(y));
                    }
                }
            }
            break;
        }
    }
 
}

void MyGame::update() {
}

void MyGame::createGrid(SDL_Renderer* renderer, SDL_Texture* texture1, SDL_Texture* texture2) {
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            game_data.grid[x][y].x = x * 800 / 3;
            game_data.grid[x][y].y = y * 200;
            game_data.grid[x][y].w = game_data.width / 3;
            game_data.grid[x][y].h = game_data.height / 3;

            //Render SDL copies;
            if (game_data.board[x + y * 3] == *"X") {
                SDL_RenderCopy(renderer, texture1, NULL, &game_data.grid[x][y]);
            }
            else if (game_data.board[x + y * 3] == *"O") {
                SDL_RenderCopy(renderer, texture2, NULL, &game_data.grid[x][y]);
            }
  
        }
    }
}

void MyGame::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Surface* surface1 = IMG_Load("X.png");
    SDL_Texture* texture1 = SDL_CreateTextureFromSurface(renderer, surface1);
    SDL_Surface* surface2 = IMG_Load("O.png");
    SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer, surface2);
    SDL_FreeSurface(surface1);
    SDL_FreeSurface(surface2);
    createGrid(renderer, texture1, texture2);
    SDL_RenderPresent(renderer);
    SDL_RenderDrawLine(renderer, 0, game_data.height/3, game_data.width, game_data.height/3);
    SDL_RenderDrawLine(renderer, 0, game_data.height * 2 / 3, game_data.width, game_data.height * 2 / 3);
    SDL_RenderDrawLine(renderer, game_data.width /3, 0, game_data.width /3, game_data.height);
    SDL_RenderDrawLine(renderer, game_data.width* 2 / 3 , 0, game_data.width * 2 / 3, game_data.height);
    

}