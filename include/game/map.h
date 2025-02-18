#ifndef MAP_H
#define MAP_H

#include <iomanip>
#include <sstream>
#include "game.h"
#include "interface.h"
#include "entity.h"
#include "items.h"
class Game;
class Interface;
class Player;
class Enemy;
class Item;
class Bullet;

class Map {
private:
    Game* game;
    Interface* ui;
    
    Player* player;
    std::vector<Enemy*> enemies;
    std::vector<Item*> items;
    std::unordered_map<H2DE_LevelPos, Tile*> tiles;

    H2DE_Timeline* t = nullptr;
    std::vector<H2DE_Timeline*> damageTimelines;

    int killCount = 0;

    void initPlayer();
    void initInterface();
    void generate();
    H2DE_TextureData getGroundTileTextureData(TileData tileData, TextureData textureData) const;
    H2DE_TextureData getDecorationTileTextureData(TileData tileData, TextureData textureData) const;
    H2DE_LevelObjectData getGroundObjectData(H2DE_LevelPos pos, TileData tileData, TextureData textureData) const;
    H2DE_LevelObjectData getDecorationObjectData(H2DE_LevelPos pos, TileData tileData, TextureData textureData) const;

    void destroyTiles();
    void destroyEnemies();
    void destroyPlayer();
    void destroyDamageTimelines();
    void destroyInterface();

    void updatePlayer();
    void updateEnemies();
    void updateItems();
    void updateDamageDisplay();
    void updateUi();

    void summonEnemy(int id, bool big);
    void summonEnemy(int id, H2DE_LevelPos pos, bool big);

    std::vector<Bullet*> getAllBullets() const;
    int xpForLevel(unsigned int level) const;
    int totalXpForLevel(unsigned int level) const;

public:
    Map(Game* game);
    ~Map();

    void update();

    void dropXp(H2DE_LevelPos pos, int level);
    void dropCoin(H2DE_LevelPos pos);
    void enemyKilled();

    void displayHealthActivity(H2DE_LevelPos pos, int healthActivity, H2DE_ColorRGB color);

    static int getIndex(float yPos, int index);
    Interface* getUi() const;
    Player* getPlayer() const;
    Enemy* getEnemy(H2DE_LevelObject* object) const;
    Bullet* getBullet(H2DE_LevelObject* object) const;
    std::vector<Item*> getItems() const;
    float getXpLevelPercentage() const;
};

#endif
