#ifndef BULLET_H
#define BULLET_H

#include "game.h"
class Game;

class Item {
private:
    Game* game;

    H2DE_LevelObject* object = nullptr;

    void initObject(std::string textureName);

public:
    Item(Game* game, H2DE_LevelPos pos, std::string textureName);
    virtual ~Item();

    virtual void update() = 0;
};



class Xp : public Item {
private:
    Game* game;
    int level;

public:
    Xp(Game* game, H2DE_LevelPos pos, int level);
    ~Xp();

    void update();
};



class Coin : public Item {
private:
    Game* game;

public:
    Coin(Game* game, H2DE_LevelPos pos);
    ~Coin();

    void update();
};

#endif
