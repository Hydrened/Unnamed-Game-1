#ifndef MAP_H
#define MAP_H

#include "game.h"
#include "entity.h"
class Game;
class Player;

class Map {
private:
    Game* game;
    
    Player* player;
    std::unordered_map<H2DE_LevelPos, Tile*> tiles;

    void initPlayer();
    void generate();
    H2DE_TextureData getGroundTileTextureData(TileData tileData, TextureData textureData) const;
    H2DE_TextureData getDecorationTileTextureData(TileData tileData, TextureData textureData) const;
    H2DE_LevelObjectData getGroundObjectData(H2DE_LevelPos pos, TileData tileData, TextureData textureData) const;
    H2DE_LevelObjectData getDecorationObjectData(H2DE_LevelPos pos, TileData tileData, TextureData textureData) const;

public:
    Map(Game* game);
    ~Map();

    void update();
};

#endif
