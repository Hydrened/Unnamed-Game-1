#include "map.h"

// INIT
Map::Map(Game* g) : game(g) {
    initPlayer();
    generate();
}

void Map::initPlayer() {
    EntityData playerData = EntityData();
    player = new Player(game, this, { 0.0f, 0.0f }, playerData);
}

void Map::generate() {
    static H2DE_Engine* engine = game->getEngine();
    static std::unordered_map<std::string, TextureData> texturesData = game->getData()->texturesData;

    for (const auto& [pos, tileData] : game->getData()->mapData) {
        Tile* tile = new Tile();
        tile->data = tileData;

        TextureData gameGroundTextureData = texturesData[tileData.ground.textureName];
        H2DE_LevelObjectData groundObjectData = getGroundObjectData(pos, tileData, gameGroundTextureData);
        tile->groundObject = H2DE_CreateLevelObject(engine, groundObjectData);

        if (tileData.decoration.has_value()) {
            TextureData gameDecorationTextureData = texturesData[tileData.decoration.value().textureName];
            H2DE_LevelObjectData decorationObjectData = getDecorationObjectData(pos, tileData, gameDecorationTextureData);
            tile->decorationObject = H2DE_CreateLevelObject(engine, decorationObjectData);
        }

        tiles[pos] = tile;
    }
}

H2DE_TextureData Map::getGroundTileTextureData(TileData tileData, TextureData textureData) const {
    H2DE_TextureData data = H2DE_TextureData();
    data.name = tileData.ground.textureName;
    data.size = textureData.size;
    data.scaleMode = H2DE_SCALE_MODE_NEAREST;
    return data;
}

H2DE_TextureData Map::getDecorationTileTextureData(TileData tileData, TextureData textureData) const {
    H2DE_TextureData data = H2DE_TextureData();
    data.name = tileData.decoration.value().textureName;
    data.size = textureData.size;
    data.scaleMode = H2DE_SCALE_MODE_NEAREST;
    return data;
}

H2DE_LevelObjectData Map::getGroundObjectData(H2DE_LevelPos pos, TileData tileData, TextureData textureData) const {
    static H2DE_Engine* engine = game->getEngine();

    H2DE_LevelObjectData data = H2DE_LevelObjectData();
    data.pos = pos + textureData.offset;
    data.texture = H2DE_CreateTexture(engine, getGroundTileTextureData(tileData, textureData));
    data.index = 0;
    return data;
}

H2DE_LevelObjectData Map::getDecorationObjectData(H2DE_LevelPos pos, TileData tileData, TextureData textureData) const {
    static H2DE_Engine* engine = game->getEngine();

    H2DE_LevelObjectData data = H2DE_LevelObjectData();
    data.pos = pos + textureData.offset;
    data.texture = H2DE_CreateTexture(engine, getDecorationTileTextureData(tileData, textureData));
    data.index = 1;
    return data;
}

// CLEANUP
Map::~Map() {
    static H2DE_Engine* engine = game->getEngine();

    for (const auto& [pos, tile] : tiles) {
        H2DE_DestroyLevelObject(engine, tile->groundObject);
        if (tile->decorationObject.has_value()) H2DE_DestroyLevelObject(engine, tile->decorationObject.value());

        delete tile;
        std::cout << "Tile cleared" << std::endl;
    }
    delete player;
    std::cout << "Map cleared" << std::endl;
}

// UPDATE
void Map::update() {
    player->update();
}
