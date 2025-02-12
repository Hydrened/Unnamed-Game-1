#ifndef WEAPON_H
#define WEAPON_H

#include "game.h"
#include "bullet.h"
class Game;
class Entity;
class Bullet;

class Weapon {
private:
    Game* game;
    Entity* owner;
    WeaponData data;
        
    H2DE_Timeline* shotTimeline = nullptr;
    std::vector<Bullet*> bullets;

    void updateBullets();
    void updateBulletsToRemove();

public:
    Weapon(Game* game, Entity* owner, WeaponData data);
    ~Weapon();

    void shot(H2DE_LevelPos targetPos);

    void update();

    Entity* getOwner() const;
    std::vector<Bullet*> getBullets() const;
};

#endif
