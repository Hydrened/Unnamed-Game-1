#ifndef GAME_H
#define GAME_H

#include <H2DE/H2DE.h>
#include "utils.h"
#include "data.h"
#include "map.h"
class Map;

class Game {
private:
    H2DE_Engine* engine;
    GameData* data = new GameData();
    Map* map;

    std::vector<SDL_Keycode> keysDown = {};
    std::vector<int> mouseButtonsDown = {};
    H2DE_AbsPos mousePos = { 0, 0 };
    bool debug = false;

    H2DE_LevelObject* cursor = nullptr;

    void initEngine();
    void initFont();
    void initCursor();
    void initMap();
    void initCamera();

    void destroyMap();
    void destroyData();
    
    void updateCursor();
    void updateMap();

public:
    Game();
    ~Game();

    void run();

    void handleEvents(SDL_Event event);
    void update();

    H2DE_Engine* getEngine() const;
    GameData* getData() const;
    Map* getMap() const;
    std::vector<SDL_Keycode> getKeysDown() const;
    std::vector<int> getMouseButtonsDown() const;
    H2DE_AbsPos getMousePos() const;
    bool isDebuging() const;
};

#endif
