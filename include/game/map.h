#ifndef MAP_H
#define MAP_H

#include "game.h"
#include "entity.h"
#include "item.h"
class Game;
class Player;
class Enemy;
class Item;

class Map {
private:
    Game* game;

    Player* player = nullptr;
    std::vector<Enemy*> enemies;
    std::vector<Enemy*> enemiesToRemove;
    std::vector<Item*> items;
    std::vector<Item*> itemsToRemove;
    std::unordered_map<LevelPos, Tile*, LevelPosHash> tiles;

    H2DE_TimelineManager* tm = H2DE_CreateTimelineManager();

    void initPlayer();

    void generate();
    void generateTile(LevelPos pos);
    void generateStructure(LevelPos pos);
    void summonEnemy(int id, float sizeMultiplier);
    void summonEnemy(int id, LevelPos pos, float sizeMultiplier);

    void updateEnemies();
    void updateItems();
    void renderAntiLineBug();
    void renderTiles();

public:
    Map(Game* game);
    ~Map();

    void update();
    void render();

    void displayHitbox(LevelRect hitbox, H2DE_RGB color);
    void killEnemy(Enemy* enemy);
    void dropXp(LevelPos pos, int level);
    void dropCoin(LevelPos pos);
    void removeItem(Item* item);

    Player* getPlayer() const;
    std::unordered_map<LevelPos, Tile*, LevelPosHash>* getTiles();
    std::unordered_map<LevelPos, Tile*, LevelPosHash> getPerimeter(LevelPos pos, bool withCenter);
    std::vector<Enemy*> getEnemies() const;
    std::vector<Item*> getItems() const;
    bool hasToBeRemoved(Item* item) const;
};

#endif
