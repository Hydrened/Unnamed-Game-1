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

    bool hit = false;

    H2DE_LevelObject* object = nullptr;
    H2DE_LevelVelocity velocity;
    std::vector<Entity*> damagedEntities; 

    void initVelocity();
    void initObject();

public:
    Bullet(Game* game, Weapon* weapon, BulletData data, float angle);
    ~Bullet();

    void update();

    H2DE_LevelObject* getObject() const;
    Weapon* getWeapon() const;
    BulletData getData() const;
    bool getHit() const;

    void setHit();
};

#endif
