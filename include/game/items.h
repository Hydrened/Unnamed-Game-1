#ifndef ITEMS_H
#define ITEMS_H

#include "game.h"
class Game;

class Item {
private:
    Game* game;
    H2DE_LevelPos pos;
    H2DE_LevelVelocity velocity = { 0.0f, 0.0f };

    std::string textureName;
    int nbFrames;
    H2DE_LevelObject* object = nullptr;
    bool pickedUp = false;
    H2DE_Timeline* pickUpSpeedTimeline = nullptr;
    float pickUpSpeed;
    bool remove = false;

    void initVelocity();
    void initObject();

    void collided();
    virtual void collidedImpl() = 0;
    
    virtual void updateImpl() = 0;
    void updatePos();
    void updateIndex();
    void updateForCollisionWithPlayer();

public:
    Item(Game* game, H2DE_LevelPos pos, std::string textureName, int nbFrames);
    virtual ~Item();

    void update();

    void pickUp();

    H2DE_LevelObjectData* getObjectData() const;
    bool toRemove() const;
};



class Xp : public Item {
private:
    Game* game;
    int level;

    void collidedImpl() override;

    void updateImpl() override;

public:
    Xp(Game* game, H2DE_LevelPos pos, int level);
    ~Xp();
};



class Coin : public Item {
private:
    Game* game;

    void collidedImpl() override;

    void updateImpl() override;

public:
    Coin(Game* game, H2DE_LevelPos pos);
    ~Coin();
};

#endif
