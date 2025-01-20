#ifndef GAME_H
#define GAME_H

#include <H2DE/H2DE.h>
#include <algorithm>
#include <filesystem>
#include <functional>
#include <iostream>
#include <random>
#include <thread>
#include "utils.h"
#include "sprite.h"
#include "calculator.h"
#include "functions.h"
#include "data.h"
#include "camera.h"
#include "map.h"
class Calculator;
class Camera;
class Map;

class Game {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    H2DE_Engine* engine;
    int FPS = 60;
    float currentFPS = 0;
    bool isRunning = true;

    GameData* data = new GameData();
    GameState state = MAIN_MENU;
    bool debugMode = true;

    Calculator* calculator = nullptr;
    Camera* camera = nullptr;
    Map* map = nullptr;

    std::vector<SDL_Keycode> keysDown = {};
    std::vector<int> mouseButtonsDown = {};
    H2DE_Pos mousePos = { 0, 0 };

    void createWindow();

public:
    Game(int argc, char** argv);
    ~Game();

    void run();
    void quit();
    
    void handleEvents(SDL_Event event);
    void update();
    void render();

    static void delay(int ms, std::function<void()> callback);

    H2DE_Engine* getEngine() const;
    int getFPS() const;
    GameData* getData() const;
    GameState getState() const;
    Calculator* getCalculator() const;
    Camera* getCamera() const;
    Map* getMap() const;
    std::vector<SDL_Keycode> getKeysDown() const;
    std::vector<int> getMouseButtonsDown() const;
    void getMousePos(int* x, int* y) const;
    bool getDebugMode() const;

    void setState(GameState state);
};

#endif
