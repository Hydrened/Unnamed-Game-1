#ifndef BULLET_H
#define BULLET_H

#include "game.h"
class Game;
class Entity;
class Weapon;

class Bullet {
private:
    Game* game;
    Weapon* weapon;
    BulletData data;
    float angle;
    LevelPos pos;

    std::vector<Entity*> damagedEntities; 

    void destroy();

    void updatePos();
    void checkEntityCollisions();

public:
    Bullet(Game* game, Weapon* weapon, BulletData data, float angle);
    ~Bullet();

    void update();
    void render();
};

#endif
