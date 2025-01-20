#ifndef XP_H
#define XP_H

#include "game.h"
class Game;

class Xp {
private:
    Game* game;
    int level;

    LevelPos pos;
    LevelVelocity velocity;
    bool pickedUp = false;

public:
    Xp(Game* game, LevelPos pos, int level);
    ~Xp();

    void update();
    void render();

    void increaseLevel();
    void pickUp();

    LevelPos getPos() const;
    LevelVelocity getVelocity() const;
    int getLevel() const;
    bool isPickedUp() const;
};

#endif
