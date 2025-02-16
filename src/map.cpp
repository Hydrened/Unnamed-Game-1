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
    data.index = Map::getIndex(data.pos.y, 4);

    H2DE_Hitbox collisionHitbox = H2DE_Hitbox();
    collisionHitbox.rect = { 0.5f, 0.0f, 1.0f, 1.0f };
    collisionHitbox.color = { 127, 127, 255, 255 };
    collisionHitbox.collisionIndex = 0;
    data.hitboxes["collision"] = collisionHitbox;

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

void Map::dropXp(H2DE_LevelPos pos, int level) {
    static int maxXpLevel = game->getData()->maxXpLevel;

    int i = maxXpLevel;
    while (i != 0) {
        if (level >= i) {
            Xp* xp = new Xp(game, pos, i);
            items.push_back(xp);
            level -= i;
        } else i--;
    }
}

void Map::dropCoin(H2DE_LevelPos pos) {
    items.push_back(new Coin(game, pos));
}

void Map::displayDamages(H2DE_LevelPos pos, float damages, H2DE_ColorRGB color) {
    static H2DE_Engine* engine = game->getEngine();
    
    H2DE_LevelObjectData objData = H2DE_LevelObjectData();
    objData.pos = pos;
    objData.index = Map::getIndex(pos.y, 30);

    H2DE_TextData textData = H2DE_TextData();
    textData.text = std::to_string(static_cast<int>(damages));
    textData.font = "test";
    textData.charSize = { 0.3f, 0.5f };
    textData.spacing = 0.05f;
    textData.color = color;
    textData.textAlign = H2DE_TEXT_ALIGN_CENTER;
    textData.scaleMode = H2DE_SCALE_MODE_NEAREST;
    objData.texture = H2DE_CreateText(engine, textData);

    H2DE_LevelObject* damageObj = H2DE_CreateLevelObject(engine, objData);

    H2DE_Timeline* damageTimeline = H2DE_CreateTimeline(engine, 400, EASE_OUT, [damageObj, pos](float blend) {
        H2DE_LevelObjectData* objData = H2DE_GetObjectData(damageObj);
        objData->pos.y = H2DE_Lerp(pos.y, pos.y - 1.0f, std::clamp(0.0f, 0.75f, blend));

    }, [this, damageObj]() {
        H2DE_DestroyLevelObject(engine, damageObj);
    }, 1);

    damageTimelines.push_back(damageTimeline);
}

// UPDATE
void Map::update() {
    H2DE_TickTimeline(t);
    updatePlayer();
    updateEnemies();
    updateItems();
    updateDamageDisplay();
}

void Map::updatePlayer() {
    player->update();
}

void Map::updateEnemies() {
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](Enemy* enemy) {
        bool isDead = enemy->isDead();
        if (isDead) {
            delete enemy;
            return true;
        } else return false;
    }), enemies.end());

    for (Enemy* enemy : enemies) enemy->update();
}

void Map::updateItems() {
    items.erase(std::remove_if(items.begin(), items.end(), [](Item* item) {
        bool toRemove = item->toRemove();
        if (toRemove) {
            delete item;
            return true;
        } else return false;
    }), items.end());

    for (Item* item : items) item->update();
}

void Map::updateDamageDisplay() {
    for (auto it = damageTimelines.begin(); it != damageTimelines.end(); ) {
        if (!H2DE_TickTimeline(*it)) {
            it = damageTimelines.erase(it);
        } else ++it;
    }
}

// GETTER
int Map::getIndex(float yPos, int index) {
    return std::ceil(yPos) * 10 + index;
}

Player* Map::getPlayer() const {
    return player;
}

std::vector<Bullet*> Map::getAllBullets() const {
    std::vector<Bullet*> res;

    std::vector<Bullet*> playerBullets = player->getWeapon()->getBullets();
    res.insert(res.end(), playerBullets.begin(), playerBullets.end());

    for (const Enemy* enemy : enemies) {
        Weapon* weapon = enemy->getWeapon();
        if (!weapon) continue;

        std::vector<Bullet*> enemyBullets = weapon->getBullets();
        res.insert(res.end(), enemyBullets.begin(), enemyBullets.end());
    }

    return res;
}

Enemy* Map::getEnemy(H2DE_LevelObject* object) const {
    auto it = std::find_if(enemies.begin(), enemies.end(), [object](Enemy* enemy) {
        return object == enemy->getObject();
    });
    return (it != enemies.end()) ? *it : nullptr;
}

Bullet* Map::getBullet(H2DE_LevelObject* object) const {
    std::vector<Bullet*> allBullets = getAllBullets();

    auto it = std::find_if(allBullets.begin(), allBullets.end(), [object](Bullet* bullet) {
        return object == bullet->getObject();
    });

    return (it != allBullets.end()) ? *it : nullptr;
}

std::vector<Item*> Map::getItems() const {
    return items;
}
