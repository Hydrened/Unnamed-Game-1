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
    Entity* user;
    WeaponData data;

    bool shooting = false;
    bool canShoot = true;
    std::vector<Bullet*> bullets;
    std::vector<Bullet*> bulletsToRemove;
    
    void shot();

    void updateShooting();
    void updateBullets();

public:
    Weapon(Game* game, Entity* user, WeaponData data);
    ~Weapon();

    void update();
    void render();

    void destroyBullet(Bullet* bullet);

    Entity* getUser() const;
};

#endif
