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

    void initEngine();

public:
    Game();
    ~Game();

    void run();

    void handleEvents(SDL_Event event);
    void update();

    H2DE_Engine* getEngine() const;
    GameData* getData() const;
};

#endif
