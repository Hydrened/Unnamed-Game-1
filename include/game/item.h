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
    void updateDropVelocity();
    void updatePickedUpPosition();
    void updateCollisionWithPlayer();
    virtual void updateImpl() = 0;

public:
    Item(Game* game, LevelPos pos, std::string texture);
    virtual ~Item();

    void update();
    void render();

    void pickUp();
    virtual void collides() = 0;

    LevelPos getPos() const;
    LevelVelocity getVelocity() const;
    std::string getTexture() const;
};



class Xp : public Item {
private:
    int level;

    void updateMerge();

public:
    Xp(Game* game, LevelPos pos, int level);
    ~Xp() override;

    void increaseLevel();
    void collides() override;

    void updateImpl() override;

    int getLevel() const;
};



class Coin : public Item {
private:

public:
    Coin(Game* game, LevelPos pos);
    ~Coin() override;

    void collides() override;

    void updateImpl() override;
};

#endif
