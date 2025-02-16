#ifndef ITEMS_H
#define ITEMS_H

#include "game.h"
class Game;

class Item {
private:
    Game* game;
    H2DE_LevelPos pos;
    H2DE_LevelVelocity velocity = { 0.0f, 0.0f };

    H2DE_LevelObject* object = nullptr;
    std::string textureName;
    int nbFrames;
    H2DE_Timeline* pickUpSpeedTimeline = nullptr;
    float pickUpSpeed;

    void initVelocity();
    void initObject();

    void collided();
    virtual void collidedImpl() = 0;
    
    virtual void updateImpl() = 0;
    void updatePos();
    void updateIndex();
    void updateForCollisionWithPlayer();

protected:
    bool remove = false;
    bool pickedUp = false;

public:
    Item(Game* game, H2DE_LevelPos pos, std::string textureName, int nbFrames);
    virtual ~Item();

    void update();

    void pickUp();

    H2DE_LevelObject* getObject() const;
    H2DE_LevelObjectData* getObjectData() const;
    H2DE_LevelVelocity getVelocity() const;
    bool toRemove() const;
};



class Xp : public Item {
private:
    Game* game;
    int level;

    void collidedImpl() override;
    void increaseLevel();

    void updateImpl() override;
    void updateMerge();

public:
    Xp(Game* game, H2DE_LevelPos pos, int level);
    ~Xp();

    int getLevel() const;
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
