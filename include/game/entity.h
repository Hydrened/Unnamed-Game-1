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
    LevelVelocity velocity = { 0.0f, 0.0f };
    EntityData data;
    Sprite* sprite;

    H2DE_TimelineManager* tm = H2DE_CreateTimelineManager();

    Weapon* weapon = nullptr;
    Face facing = LEFT;
    Uint8 redFilterOpacity = 0;

    virtual void updateImpl() = 0;
    void updateSprite();
    void updateWeapon();
    void renderTexture();
    void renderWeapon();
    virtual void kill() = 0;

public:
    Entity(Game* game, Map* map, LevelPos pos, EntityData data);
    virtual ~Entity();

    void update();
    void render();

    void equipWeapon(int id); 
    void inflictDamages(float damages, float crit);

    LevelPos getPos() const;
    EntityData getData() const;
};



class Player : public Entity {
private:
    int xp = 0;
    int coins = 0;

    void updateForControls();
    void updateForWorldCollisions();
    void updateAnimation(LevelPos defaultPos);
    void updateFacing();
    void updateForItems();
    void kill() override;

public:
    Player(Game* game, Map* map, LevelPos pos, EntityData data);
    ~Player() override;

    void increaseXp(int level);
    void increaseCoins(int nb);

    void updateImpl() override;

    int getCoins() const;
};



class Enemy : public Entity {
private:
    bool isNearPlayer();
    void kill() override;

public:
    Enemy(Game* game, Map* map, LevelPos pos, EntityData data);
    ~Enemy() override;

    void updateImpl() override;
};

#endif
