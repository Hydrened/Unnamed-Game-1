#include "map.h"

// INIT
Map::Map(Game* g) : game(g) {
    initPlayer();
    game->setState(PLAYING);

    static H2DE_Engine* engine = game->getEngine();
    H2DE_Timeline* t = H2DE_CreateTimeline(engine, 1000, LINEAR, NULL, [this]() {
        summonEnemy(1, 1.0f);
    }, -1);
    H2DE_AddTimelineToManager(tm, t);
}

void Map::initPlayer() {
    static std::unordered_map<int, EntityData> entities = game->getData()->others->entities;
    player = new Player(game, this, { 0.25f, 0.25f }, entities[0]);
}

// CLEANUP
Map::~Map() {
    delete player;
    for (Enemy* enemy : enemies) delete enemy;
    enemies.clear();
    for (const auto& [pos, tile] : tiles) delete tile;
    tiles.clear();
}

// EVENTS
void Map::generate() {
    static int blocksOnWidth = static_cast<int>(std::ceil(game->getData()->sizes->blocksOnWidth));
    static int blocksOnHeight = static_cast<int>(std::ceil(game->getData()->sizes->blocksOnHeight));
    static Camera* camera = game->getCamera();

    LevelPos camPos = camera->getPos();

    for (int y = -1; y <= blocksOnHeight + 4; y++) { // + tallest obj
        for (int x = -1; x <= blocksOnWidth; x++) { // -+ widest obj
            LevelPos tilePos = { (float)x + std::round(camPos.x), (float)y + std::round(camPos.y) };
            if (tiles.find(tilePos) != tiles.end()) continue;
            generateTile(tilePos);
        }
    }
}

void Map::generateTile(LevelPos pos) {
    static GameData* gameData = game->getData();

    static std::random_device rd;
    static std::default_random_engine e(rd());
    static std::vector<TextureProbability> groundProbabilities = gameData->probabilities->ground;
    static std::vector<TextureProbability> decorationProbabilities = gameData->probabilities->decoration;
    static float haveDecorationPorbabily = gameData->probabilities->haveDecoration;
    static float nearSpawnDistance = gameData->sizes->nearSpawnDistance;

    bool haveDecoration = rand(haveDecorationPorbabily);
    bool nearSpawn = (std::abs(pos.x) <= nearSpawnDistance && std::abs(pos.y) <= nearSpawnDistance);

    Tile* tile = new Tile();
    tile->ground = getRandomTexture(groundProbabilities);
    tile->fliped = getRandomBool();

    if (haveDecoration && !nearSpawn) {
        bool haveDecorationNear = false;
        for (const auto& [p, tile] : getPerimeter(pos, false)) {
            if (tile->decoration.has_value()) {
                haveDecorationNear = true;
                break;
            }
        }
        if (!haveDecorationNear) tile->decoration = getRandomTexture(decorationProbabilities);
    }

    tiles[pos] = tile;
}

void Map::generateStructure(LevelPos pos) {
    // générer à des endroits random
}

void Map::summonEnemy(int id, float sizeMultiplier) {
    static Camera* camera = game->getCamera();
    static GameData* gameData = game->getData();
    static float blocksOnWidth = gameData->sizes->blocksOnWidth;
    static float blocksOnHeight = gameData->sizes->blocksOnHeight;
    
    float rX = rand(0.0f, 1.0f);
    float rY = rand(0.0f, 1.0f);

    if (rand(50.0f)) rY = std::round(rY);
    else rX = std::round(rX);

    if (rX == 0.0f) rX -= 0.1f;
    else if (rX == 1.0f) rX += 0.1f;

    if (rY == 0.0f) rY -= 0.1f;
    else if (rY == 1.0f) rY += 0.1f;

    LevelPos pos = { rX * blocksOnWidth, rY * blocksOnHeight };
    summonEnemy(id, pos + camera->getPos(), sizeMultiplier);
}

void Map::summonEnemy(int id, LevelPos pos, float sizeMultiplier) {
    static std::unordered_map<int, EntityData> entities = game->getData()->others->entities;
    enemies.push_back(new Enemy(game, this, pos, entities[id]));
}

void Map::displayHitbox(LevelRect hitbox, H2DE_RGB color) {
    if (!game->getDebugMode()) return;

    static H2DE_Engine* engine = game->getEngine();
    static Calculator* cal = game->getCalculator();

    H2DE_Size absHitboxSize = cal->convertToPx(LevelSize{ hitbox.w, hitbox.h });

    H2DE_GraphicObject* h = H2DE_CreateGraphicObject();
    h->type = POLYGON;
    h->pos = cal->convertToPx(LevelPos{ hitbox.x, hitbox.y });
    h->points = {
        { 0, 0 },
        { absHitboxSize.w, 0 },
        { absHitboxSize.w, absHitboxSize.h },
        { 0, absHitboxSize.h },
    };
    h->color = color;
    h->index = 1000000;
    H2DE_AddGraphicObject(engine, h);
}

void Map::killEnemy(Enemy* enemy) {
    if (std::find(enemiesToRemove.begin(), enemiesToRemove.end(), enemy) != enemiesToRemove.end()) return;
    enemiesToRemove.push_back(enemy);
}

void Map::dropXp(LevelPos pos, int level) {
    static int maxXpLevel = game->getData()->others->maxXpLevel;

    int i = maxXpLevel;
    while (i != 0) {
        if (level >= i) {
            Xp* xp = new Xp(game, pos, i);
            items.push_back(xp);
            level -= i;
        } else i--;
    }
}

void Map::dropCoin(LevelPos pos) {
    items.push_back(new Coin(game, pos));
}

void Map::removeItem(Item* item) {
    itemsToRemove.push_back(item);
}

// UPDATE
void Map::update() {
    static Camera* camera = game->getCamera();

    player->update();
    camera->update();
    generate();
    updateEnemies();
    updateItems();
}

void Map::updateEnemies() {
    H2DE_TickTimelineManager(tm);

    for (Enemy* enemy : enemies) enemy->update();

    for (Enemy* enemy : enemiesToRemove) {
        auto it = std::find(enemies.begin(), enemies.end(), enemy);
        if (it != enemies.end()) {
            delete *it;
            enemies.erase(it);
        }
    }
    enemiesToRemove.clear();
}

void Map::updateItems() {
    for (Item* item : items) item->update();
    for (Item* item : itemsToRemove) {
        items.erase(std::find(items.begin(), items.end(), item));
        delete item;
    }
    itemsToRemove.clear();
}

// RENDER
void Map::render() {
    player->render();
    renderAntiLineBug();
    renderTiles();
    for (Enemy* enemy : enemies) enemy->render();
    for (Item* item : items) item->render();
}

void Map::renderAntiLineBug() {
    static H2DE_Engine* engine = game->getEngine();
    static Calculator* cal = game->getCalculator();
    static GameData* gameData = game->getData();

    static int winWidth = gameData->sizes->winWidth;
    static int winHeight = gameData->sizes->winHeight;

    H2DE_GraphicObject* antiLineBug = H2DE_CreateGraphicObject();
    antiLineBug->type = POLYGON;
    antiLineBug->pos = { 0, 0 };
    antiLineBug->points = {
        { 0, 0 },
        { winWidth, 0 },
        { winWidth, winHeight },
        { 0, winHeight },
    };
    antiLineBug->color = { 124, 182, 114, 255 };
    antiLineBug->filled = true;
    antiLineBug->index = INT_MIN;
    H2DE_AddGraphicObject(engine, antiLineBug);
}

void Map::renderTiles() {
    static H2DE_Engine* engine = game->getEngine();
    static Calculator* cal = game->getCalculator();
    static GameData* gameData = game->getData();
    static Camera* camera = game->getCamera();

    static std::unordered_map<std::string, LevelSize> texturesSizes = gameData->sizes->textures;
    static std::unordered_map<std::string, LevelPos> texturesOffsets = gameData->offsets->textures;
    static std::unordered_map<std::string, LevelRect> decorationHitboxes = gameData->physics->decorationHitboxes;
    for (const auto& [pos, tile] : tiles) {

        LevelPos groundTexturePos = pos + texturesOffsets[tile->ground];
        LevelSize groundTextureSize = texturesSizes[tile->ground];

        if (camera->contains(groundTexturePos.makeRect(groundTextureSize))) {
            H2DE_GraphicObject* ground = H2DE_CreateGraphicObject();
            ground->type = IMAGE;
            ground->texture = tile->ground;
            ground->pos = cal->convertToPx(groundTexturePos);
            ground->size = cal->convertToPx(groundTextureSize);
            ground->flip = (tile->fliped) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
            ground->index = getIndex(pos.y, 0);
            H2DE_AddGraphicObject(engine, ground);

            displayHitbox(groundTexturePos.makeRect(groundTextureSize), { 0, 0, 0, 31 });
        }

        if (!tile->decoration.has_value()) continue;

        LevelPos decorationTexturePos = pos + texturesOffsets[tile->decoration.value()];
        LevelSize decorationTextureSize = texturesSizes[tile->decoration.value()];

        if (camera->contains(decorationTexturePos.makeRect(decorationTextureSize))) {
            H2DE_GraphicObject* decoration = H2DE_CreateGraphicObject();
            decoration->type = IMAGE;
            decoration->texture = tile->decoration.value();
            decoration->pos = cal->convertToPx(decorationTexturePos);
            decoration->size = cal->convertToPx(decorationTextureSize);
            decoration->flip = (tile->fliped) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
            decoration->index = getIndex(pos.y, 5);
            H2DE_AddGraphicObject(engine, decoration);

            auto it = decorationHitboxes.find(tile->decoration.value());
            if (it != decorationHitboxes.end()) displayHitbox(decorationHitboxes[tile->decoration.value()] + pos, { 0, 0, 255, 255 });
        }
    }
}

// GETTER
Player* Map::getPlayer() const {
    return player;
}

std::unordered_map<LevelPos, Tile*, LevelPosHash>* Map::getTiles() {
    return &tiles;
}

std::unordered_map<LevelPos, Tile*, LevelPosHash> Map::getPerimeter(LevelPos pos, bool withCenter) {
    std::unordered_map<LevelPos, Tile*, LevelPosHash> res;

    pos.x = std::floor(pos.x);
    pos.y = std::floor(pos.y);

    std::vector<LevelPos> positions = {
        { pos.x - 1, pos.y - 1 },
        { pos.x, pos.y - 1 },
        { pos.x + 1, pos.y - 1 },
        { pos.x + 1, pos.y },
        { pos.x + 1, pos.y + 1 },
        { pos.x, pos.y + 1 },
        { pos.x - 1, pos.y + 1 },
        { pos.x - 1, pos.y },
    };
    if (withCenter) positions.push_back(pos);

    for (const LevelPos p : positions) {
        if (tiles.find(p) != tiles.end()) res[p] = tiles[p];
    }
    return res;
}    

std::vector<Enemy*> Map::getEnemies() const {
    return enemies;
}

std::vector<Item*> Map::getItems() const {
    return items;
}

bool Map::hasToBeRemoved(Item* item) {
    return (std::find(itemsToRemove.begin(), itemsToRemove.end(), item) != itemsToRemove.end());
}
