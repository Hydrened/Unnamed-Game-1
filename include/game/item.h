#ifndef ITEM_H
#define ITEM_H

#include "game.h"
class Game;

class Item {
protected:
    Game* game;

    LevelPos pos;
    LevelVelocity velocity;
    Sprite* sprite;
    std::string texture;
    bool pickedUp = false;

    void setTexture(std::string texture);

public:
    Item(Game* game, LevelPos pos, std::string texture);
    virtual ~Item() = 0;

    void update();
    void render();

    void pickUp();

    LevelPos getPos() const;
    LevelVelocity getVelocity() const;
    bool isPickedUp() const;
    std::string getTexture() const;
};



class Xp : public Item {
private:
    int level;

public:
    Xp(Game* game, LevelPos pos, int level);
    ~Xp() override;

    void increaseLevel();

    int getLevel() const;
};



class Coin : public Item {
private:

public:
    Coin(Game* game, LevelPos pos);
    ~Coin() override;
};

#endif
