#include "map.h"

// INIT
Map::Map(Game* g) : game(g) {
    initPlayer();
    generate();

    static H2DE_Engine* engine = game->getEngine();
    t = H2DE_CreateTimeline(engine, 1000, LINEAR, NULL, [this]() {
        summonEnemy(1, 1.0f);
    }, -1);
}

void Map::initPlayer() {
    static std::unordered_map<int, EntityData> entitiesData = game->getData()->entitiesData;

    EntityData playerData = entitiesData[0];
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
    data.index = Map::getIndex(data.pos.y, 0);

    return data;
}

H2DE_LevelObjectData Map::getDecorationObjectData(H2DE_LevelPos pos, TileData tileData, TextureData textureData) const {
    static H2DE_Engine* engine = game->getEngine();

    H2DE_LevelObjectData data = H2DE_LevelObjectData();
    data.pos = pos + textureData.offset;
    data.texture = H2DE_CreateTexture(engine, getDecorationTileTextureData(tileData, textureData));
    data.index = Map::getIndex(data.pos.y, 3);

    H2DE_Hitbox collisionHitbox = H2DE_Hitbox();
    collisionHitbox.rect = { 0.5f, 0.0f, 1.0f, 1.0f };
    collisionHitbox.color = { 127, 127, 255, 255 };
    collisionHitbox.collisionIndex = 0;
    data.hitboxes.push_back(collisionHitbox);

    return data;
}

// CLEANUP
Map::~Map() {
    delete t;
    destroyTiles();
    destroyEnemies();
    destroyPlayer();
    std::cout << "Map cleared" << std::endl;
}

void Map::destroyTiles() {
    static H2DE_Engine* engine = game->getEngine();

    for (const auto& [pos, tile] : tiles) {
        H2DE_DestroyLevelObject(engine, tile->groundObject);
        if (tile->decorationObject.has_value()) H2DE_DestroyLevelObject(engine, tile->decorationObject.value());

        delete tile;
    }
    std::cout << "Tiles cleared" << std::endl;
}

void Map::destroyEnemies() {
    for (Enemy* enemy : enemies) delete enemy;
}

void Map::destroyPlayer() {
    delete player;
}

// EVENTS
void Map::summonEnemy(int id, float size) {
    static H2DE_Engine* engine = game->getEngine();
    static H2DE_Camera* camera = H2DE_GetCamera(engine);
    static GameData* gameData = game->getData();
    H2DE_LevelSize camSize = H2DE_GetCameraSize(camera);

    float rX = H2DE_RandomFloatInRange(0.0f, 1.0f);
    float rY = H2DE_RandomFloatInRange(0.0f, 1.0f);

    if (H2DE_RandomBool()) rY = std::round(rY);
    else rX = std::round(rX);

    if (rX == 0.0f) rX -= 0.1f;
    else if (rX == 1.0f) rX += 0.1f;

    if (rY == 0.0f) rY -= 0.1f;
    else if (rY == 1.0f) rY += 0.1f;

    H2DE_LevelPos pos = { rX * camSize.w, rY * camSize.h };
    summonEnemy(id, pos + H2DE_GetCameraPos(camera), size);
}

void Map::summonEnemy(int id, H2DE_LevelPos pos, float size) {
    static std::unordered_map<int, EntityData> entitiesData = game->getData()->entitiesData;
    enemies.push_back(new Enemy(game, this, pos, entitiesData[id]));
}

// UPDATE
void Map::update() {
    H2DE_TickTimeline(t);
    player->update();
    for (Enemy* enemy : enemies) enemy->update();
}

// GETTER
int Map::getIndex(float yPos, int index) {
    return std::ceil(yPos) * 10 + index;
}

Player* Map::getPlayer() const {
    return player;
}

Enemy* Map::getEnemy(H2DE_LevelObject* object) const {
    auto it = std::find_if(enemies.begin(), enemies.end(), [object](Enemy* enemy) {
        return object == enemy->getObject();
    });
    return (it != enemies.end()) ? *it : nullptr;
}
