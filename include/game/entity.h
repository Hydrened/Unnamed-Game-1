#ifndef ENTITY_H
#define ENTITY_H

#include "game.h"
class Game;
class Map;
class Sprite;

class Entity {
protected:
    Game* game;
    Map* map;
    LevelPos pos;
    EntityData data;
    Sprite* sprite;

    Face facing = LEFT;

    void updateSprite();

public:
    Entity(Game* game, Map* map, LevelPos pos, EntityData data);
    virtual ~Entity();

    virtual void update() = 0;
    void render();

    LevelPos getPos() const;
};



class Player : public Entity {
private:
    void updateForControls();
    void updateForWorldCollisions();
    void updateAnimation(LevelPos defaultPos);
    void updateFacing();

public:
    Player(Game* game, Map* map, LevelPos pos, EntityData data);
    ~Player() override;

    void update() override;
};



class Enemy : public Entity {
private:
    bool isNearPlayer();
    void updateForPlayer();

public:
    Enemy(Game* game, Map* map, LevelPos pos, EntityData data);
    ~Enemy() override;

    void update() override;
};

#endif
