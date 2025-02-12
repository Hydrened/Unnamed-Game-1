#ifndef MAP_H
#define MAP_H

#include "game.h"
#include "entity.h"
class Game;
class Player;
class Enemy;
class Bullet;

class Map {
private:
    Game* game;
    
    Player* player;
    std::vector<Enemy*> enemies;
    std::unordered_map<H2DE_LevelPos, Tile*> tiles;

    H2DE_Timeline* t = nullptr;

    void initPlayer();
    void generate();
    H2DE_TextureData getGroundTileTextureData(TileData tileData, TextureData textureData) const;
    H2DE_TextureData getDecorationTileTextureData(TileData tileData, TextureData textureData) const;
    H2DE_LevelObjectData getGroundObjectData(H2DE_LevelPos pos, TileData tileData, TextureData textureData) const;
    H2DE_LevelObjectData getDecorationObjectData(H2DE_LevelPos pos, TileData tileData, TextureData textureData) const;

    void destroyTiles();
    void destroyEnemies();
    void destroyPlayer();

    void updatePlayer();
    void updateEnemies();

    void summonEnemy(int id, float size);
    void summonEnemy(int id, H2DE_LevelPos pos, float size);

    std::vector<Bullet*> getAllBullets() const;

public:
    Map(Game* game);
    ~Map();

    void update();

    static int getIndex(float yPos, int index);
    Player* getPlayer() const;
    Enemy* getEnemy(H2DE_LevelObject* object) const;
    Bullet* getBullet(H2DE_LevelObject* object) const;
};

#endif
