#ifndef ENTITY_H
#define ENTITY_H

#include "game.h"
#include "weapon.h"
class Game;
class Map;
class Weapon;

class Entity {
protected:
    Game* game;
    Map* map;
    H2DE_LevelVelocity velocity = { 0.0f, 0.0f };
    EntityData data;

    bool dead = false;
    H2DE_LevelObject* object = nullptr;
    Weapon* weapon = nullptr;

    H2DE_Face facing = H2DE_LEFT_FACE;
    H2DE_Timeline* redFilterTimline = nullptr;

    void kill();
    virtual void killImpl() = 0;

    virtual void updateImpl() = 0;
    void updatePos();
    virtual void updateFacingImpl() = 0;
    void updateFacing();
    void updateAnimation(H2DE_LevelPos defaultPos);
    void updateRedFilter();
    void updateIndex();
    void updateWeapon();

    H2DE_Surface* getSprite() const;
    std::vector<H2DE_Hitbox> getHitboxes() const;

public:
    Entity(Game* game, Map* map, H2DE_LevelPos pos, EntityData data);
    ~Entity();

    void update();

    void inflictDamages(float damages, float crit);
    void equipWeapon(int id); 

    EntityData getData() const;
    H2DE_LevelObject* getObject() const;
    H2DE_LevelObjectData* getObjectData() const;
    Weapon* getWeapon() const;
    bool isDead() const;
};



class Player : public Entity {
private:
    int xp = 0;
    int coins = 0;

    void killImpl() override;

    void updateImpl() override;
    void updateForControls();
    void updateFacingImpl() override;
    void updateForItems();

public:
    Player(Game* game, Map* map, H2DE_LevelPos pos, EntityData data);
    ~Player();

    void increaseXp(int level);
    void increaseCoins(int nb);
};



class Enemy : public Entity {
private:
    bool canAttackNow = true;
    
    void killImpl() override;

    void updateImpl() override;
    void updateFacingImpl() override;

    bool isNearPlayer();

public:
    Enemy(Game* game, Map* map, H2DE_LevelPos pos, EntityData data);
    ~Enemy();

    void attacked();
    bool canAttack() const;
};

#endif
