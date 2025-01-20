#ifndef ENTITY_H
#define ENTITY_H

#include "game.h"
#include "weapon.h"
class Game;
class Map;
class Sprite;
class Weapon;

class Entity {
protected:
    Game* game;
    Map* map;
    LevelPos pos;
    EntityData data;
    Sprite* sprite;

    Face facing = LEFT;
    Weapon* weapon = nullptr;

    void updateSprite();
    void updateWeapon();
    void renderTexture();
    void renderWeapon();
    virtual void kill() = 0;

public:
    Entity(Game* game, Map* map, LevelPos pos, EntityData data);
    virtual ~Entity();

    virtual void update() = 0;
    void render();

    void equipWeapon(int id); 
    void inflictDamages(float damages, float crit);

    LevelPos getPos() const;
    EntityData getData() const;
};



class Player : public Entity {
private:
    int xp = 0;

    void updateForControls();
    void updateForWorldCollisions();
    void updateAnimation(LevelPos defaultPos);
    void updateFacing();
    void updateForXp();
    void kill() override;

public:
    Player(Game* game, Map* map, LevelPos pos, EntityData data);
    ~Player() override;

    void increaseXp(int level);

    void update() override;
};



class Enemy : public Entity {
private:
    bool isNearPlayer();
    void updateForPlayer();
    void kill() override;

public:
    Enemy(Game* game, Map* map, LevelPos pos, EntityData data);
    ~Enemy() override;

    void update() override;
};

#endif
