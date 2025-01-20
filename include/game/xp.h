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

public:
    Xp(Game* game, LevelPos pos, int level);
    ~Xp();

    void update();
    void render();

    void increaseLevel();

    LevelPos getPos() const;
    LevelVelocity getVelocity() const;
    int getLevel() const;
};

#endif
