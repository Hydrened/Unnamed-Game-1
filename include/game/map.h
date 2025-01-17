#ifndef MAP_H
#define MAP_H

#include "game.h"
#include "entity.h"
class Game;
class Player;
class Enemy;

class Map {
private:
    Game* game;

    Player* player = nullptr;
    std::vector<Enemy*> enemies;
    std::unordered_map<LevelPos, Tile*, LevelPosHash> tiles;

    void initPlayer();

    void generate();
    void generateTile(LevelPos pos);
    void generateStructure(LevelPos pos);

    void renderAntiLineBug();
    void renderTiles();

    void summonEnemy(int id, LevelPos pos, float sizeMultiplier);

public:
    Map(Game* game);
    ~Map();

    void update();
    void render();

    void displayHitbox(LevelRect hitbox, H2DE_RGB color);

    Player* getPlayer() const;
    std::unordered_map<LevelPos, Tile*, LevelPosHash>* getTiles();
    std::unordered_map<LevelPos, Tile*, LevelPosHash> getPerimeter(LevelPos pos, bool withCenter);
};

#endif
