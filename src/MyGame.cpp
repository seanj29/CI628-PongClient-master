#include "MyGame.h"

void MyGame::on_receive(std::string cmd, std::vector<std::string>& args) {
    if (cmd == "GAME_DATA") {
        // we should have exactly 5 arguments
        if (args.size() == 5) {
            game_data.board = args.at(0);
            game_data.GameFinished = args.at(1) == "true";
            game_data.inputEnabled = args.at(2) == "true";
            game_data.yourTurn = args.at(3) == "true";
            game_data.clientNum = stoi(args.at(4));
        }
    } else {
        std::cout << "Received: " << cmd << std::endl;
    }
}

void MyGame::send(std::string message) {
    messages.push_back(message);
}

void MyGame::input(SDL_Event& event) {
    if (game_data.inputEnabled && game_data.yourTurn){
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        game_data.mousePos.x = event.motion.x;
        game_data.mousePos.y = event.motion.y;
            send(event.type == SDL_MOUSEBUTTONDOWN ?"LEFT_DOWN": "LEFT_UP");
            for (int y = 0; y < 3; y++) {
                for (int x = 0; x < 3; x++) {
                    if (SDL_PointInRect(&game_data.mousePos, &game_data.grid[x][y])){
                        send(std::to_string(x));
                        send(std::to_string(y));
                }
            }
        }
    }
 
}
}

void MyGame::update() {
}
void MyGame::createGrid(SDL_Renderer* renderer, SDL_Texture* texture1, SDL_Texture* texture2) {
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            game_data.grid[x][y].x = x * game_data.width / 3;
            game_data.grid[x][y].y = y * game_data.height / 3;
            game_data.grid[x][y].w = game_data.width / 3;
            game_data.grid[x][y].h = game_data.height / 3;

            //Render SDL copies;
            if (game_data.board[x + y * 3] == *"X") {
                SDL_RenderCopy(renderer, texture1, NULL, &game_data.grid[x][y]);
            }
            else if (game_data.board[x + y * 3] == *"O") {
                SDL_RenderCopy(renderer, texture2, NULL, &game_data.grid[x][y]);
            }
            else if (game_data.board[x + y * 3] == *"E") {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(renderer, &game_data.grid[x][y]);
            }
  
        }
    }
}
void MyGame::createButton(SDL_Window* window) {
    if (game_data.clientNum == 1) {
        const SDL_MessageBoxButtonData buttons[] = {
            { /* .flags, .buttonid, .text */        SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "X" },
            { 0, 1, "O" },
        };
        const SDL_MessageBoxColorScheme colorScheme = {
            { /* .colors (.r, .g, .b) */
                /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
                { 255,   0,   0 },
                /* [SDL_MESSAGEBOX_COLOR_TEXT] */
                {   0, 255,   0 },
                /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
                { 255, 255,   0 },
                /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
                {   0,   0, 255 },
                /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
                { 255,   0, 255 }
            }
        };
        const SDL_MessageBoxData messageboxdata = {
            SDL_MESSAGEBOX_INFORMATION, /* .flags */
            window, /* .window */
            "Start the game", /* .title */
            "Pick which side you want to play", /* .message */
            SDL_arraysize(buttons), /* .numbuttons */
            buttons, /* .buttons */
            &colorScheme /* .colorScheme */
        };
        int buttonid;
        if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
            SDL_Log("error displaying message box");
        }
        if (buttonid == -1) {
            SDL_Log("no selection");
        }
        else if(buttonid == 0) {
            SDL_Log("selection was %s", buttons[buttonid].text);
            send("X");
        }
        else if (buttonid == 1) {
            SDL_Log("selection was %s", buttons[buttonid].text);
            send("O");
        }
    }
}
char* MyGame::fForWin() {
    if(game_data.yourTurn){
        return "Winner!";
    }
    else {
        return "You Lost!";
    }

}
void MyGame::createEnd(SDL_Window* window) {
    if (game_data.GameFinished) {
        const SDL_MessageBoxButtonData buttons[] = {
            { /* .flags, .buttonid, .text */        SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 0, "Quit!" },
        };
        const SDL_MessageBoxColorScheme colorScheme = {
            { /* .colors (.r, .g, .b) */
                /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
                { 255,   0,   0 },
                /* [SDL_MESSAGEBOX_COLOR_TEXT] */
                {   0, 255,   0 },
                /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
                { 255, 255,   0 },
                /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
                {   0,   0, 255 },
                /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
                { 255,   0, 255 }
            }
        };
        const SDL_MessageBoxData messageboxdata = {
            SDL_MESSAGEBOX_INFORMATION, /* .flags */
            window, /* .window */
           "Game Over!" , /* .title */
            fForWin(), /* .message */
            SDL_arraysize(buttons), /* .numbuttons */
            buttons, /* .buttons */
            &colorScheme /* .colorScheme */
        };
        int buttonid;
        if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
            SDL_Log("error displaying message box");
        }
        if (buttonid == -1) {
            SDL_Log("no selection");
            SDL_Quit();
        }
        else if (buttonid == 0) {
            SDL_Log("selection was %s", buttons[buttonid].text);
            SDL_Quit();
        }
    }
}

void MyGame::render(SDL_Renderer* renderer) {
    if(game_data.inputEnabled == true) {
        SDL_Surface* surface1 = IMG_Load("X.png");
        SDL_Texture* texture1 = SDL_CreateTextureFromSurface(renderer, surface1);
        SDL_Surface* surface2 = IMG_Load("O.png");
        SDL_Texture* texture2 = SDL_CreateTextureFromSurface(renderer, surface2);
        SDL_FreeSurface(surface1);
        SDL_FreeSurface(surface2);
        createGrid(renderer, texture1, texture2);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderDrawLine(renderer, 0, game_data.height / 3, game_data.width, game_data.height / 3);
        SDL_RenderDrawLine(renderer, 0, game_data.height * 2 / 3, game_data.width, game_data.height * 2 / 3);
        SDL_RenderDrawLine(renderer, game_data.width / 3, 0, game_data.width / 3, game_data.height);
        SDL_RenderDrawLine(renderer, game_data.width * 2 / 3, 0, game_data.width * 2 / 3, game_data.height);
        SDL_RenderPresent(renderer);

    }
}