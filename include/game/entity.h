#ifndef ENTITY_H
#define ENTITY_H

#include "game.h"
class Game;
class Map;

class Entity {
protected:
    Game* game;
    Map* map;
    H2DE_LevelPos pos;
    H2DE_LevelVelocity velocity = { 0.0f, 0.0f };
    EntityData data;

    H2DE_LevelObject* object = nullptr;

//     H2DE_TimelineManager* tm = H2DE_CreateTimelineManager();

//     Weapon* weapon = nullptr;
//     std::vector<AutoWeapon*> autoWeapons;
//     Face facing = LEFT;
//     Uint8 redFilterOpacity = 0;

    virtual void updateImpl() = 0;
//     void updateSprite();
//     void updateWeapon();

    virtual void kill() = 0;

public:
    Entity(Game* game, Map* map, H2DE_LevelPos pos, EntityData data);
    ~Entity();

    void update();

//     void equipWeapon(int id); 
//     void inflictDamages(float damages, float crit);

//     H2DE_LevelPos getPos() const;
//     EntityData getData() const;
};



class Player : public Entity {
private:
//     int xp = 0;
//     int coins = 0;

    void updateImpl() override;
//     void updateForControls();
//     void updateForWorldCollisions();
//     void updateAnimation(H2DE_LevelPos defaultPos);
//     void updateFacing();
//     void updateForItems();
//     void updateAutoWeapons();

    void kill() override;

public:
    Player(Game* game, Map* map, H2DE_LevelPos pos, EntityData data);
    ~Player();

//     void increaseXp(int level);
//     void increaseCoins(int nb);
//     void equipAutoWeapon(AutoWeapon* autoWeapon);

//     int getCoins() const;
};



class Enemy : public Entity {
private:
    void updateImpl() override;

    void kill() override;
//     bool isNearPlayer();

public:
    Enemy(Game* game, Map* map, H2DE_LevelPos pos, EntityData data);
    ~Enemy();
};

#endif
