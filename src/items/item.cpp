#include "items.h"
#undef min
#undef max

// INIT
Item::Item(Game* g, H2DE_LevelPos p, std::string t, int n) : game(g), pos(p), textureName(t), nbFrames(n) {
    initVelocity();
    initObject();
}

void Item::initVelocity() {
    float angle = H2DE_RandomFloatInRange(0.0f, M_PI * 2.0f);
    velocity = { std::cos(angle) * 0.1f, std::sin(angle) * 0.1f };
}

void Item::initObject() {
    static H2DE_Engine* engine = game->getEngine();
    static std::unordered_map<std::string, TextureData> texturesData = game->getData()->texturesData;
    static std::unordered_map<std::string, H2DE_LevelRect> itemHitboxes = game->getData()->itemHitboxes;

    H2DE_LevelObjectData objData = H2DE_LevelObjectData();
    objData.pos = pos + texturesData[textureName].offset - (texturesData[textureName].size / 2.0f).toPos();

    H2DE_TextureData textureData = H2DE_TextureData();
    textureData.name = textureName;
    textureData.scaleMode = H2DE_SCALE_MODE_NEAREST;
    textureData.size = texturesData[textureName].size;
    
    H2DE_SpriteData spriteData = H2DE_SpriteData();
    spriteData.defaultAnimationName = "idle";
    spriteData.delay = 150;
    spriteData.nbFrame = nbFrames;

    objData.texture = H2DE_CreateSprite(engine, textureData, spriteData);

    H2DE_Hitbox hitbox = H2DE_Hitbox();
    hitbox.collisionIndex = 1;
    hitbox.color = { 255, 127, 255, 255 };
    hitbox.rect = itemHitboxes[textureName];
    objData.hitboxes["collision"] = hitbox;

    object = H2DE_CreateLevelObject(engine, objData);
}

// CLEANUP
Item::~Item() {
    static H2DE_Engine* engine = game->getEngine();
    if (pickUpSpeedTimeline) delete pickUpSpeedTimeline;
    H2DE_DestroyLevelObject(engine, object);
    if (game->isDebuging()) std::cout << "└─> Item cleared" << std::endl;
}

// EVENTS
void Item::pickUp() {
    static H2DE_Engine* engine = game->getEngine();
    static float speed = game->getData()->itemSpeed;
    static float speedMultiplier = game->getData()->pickUpItemSpeedMultiplier;

    if (pickedUp) return;

    pickedUp = true;
    pickUpSpeed = speed;

    pickUpSpeedTimeline = H2DE_CreateTimeline(engine, 0, LINEAR, NULL, [this]() {
        pickUpSpeed *= speedMultiplier;
    }, -1);
}

void Item::collided() {
    collidedImpl();
    remove = true;
}

// UPDATE
void Item::update() {
    updatePos();
    updateIndex();
    updateImpl();
    updateForCollisionWithPlayer();
}

void Item::updatePos() {
    static float airResistance = game->getData()->itemAirResistance;
    static float speed = game->getData()->itemSpeed;
    
    if (pickedUp) {
        H2DE_TickTimeline(pickUpSpeedTimeline);

        H2DE_LevelObjectData* thisData = getObjectData();
        H2DE_LevelObjectData* playerData = game->getMap()->getPlayer()->getObjectData();

        H2DE_LevelPos playerPos = playerData->pos + playerData->hitboxes["damage"].rect.getCenter();
        H2DE_LevelPos futurePos = thisData->pos + thisData->hitboxes["collision"].rect.getCenter() + velocity;

        float angle = std::atan2(playerPos.y - futurePos.y, playerPos.x - futurePos.x);
        H2DE_LevelVelocity velocityToAdd = { std::cos(angle), std::sin(angle) };

        velocity += velocityToAdd;
        velocity *= pickUpSpeed;
        
    } else {
        if (velocity.isNull()) return;

        if (velocity.x != 0.0f) {
            if (velocity.x < 0.0f) velocity.x = std::min(0.0f, velocity.x + airResistance);
            else velocity.x = std::max(0.0f, velocity.x - airResistance);
        }
        if (velocity.y != 0.0f) {
            if (velocity.y < 0.0f) velocity.y = std::min(0.0f, velocity.y + airResistance);
            else velocity.y = std::max(0.0f, velocity.y - airResistance);
        }
    }

    H2DE_LevelObjectData* data = H2DE_GetObjectData(object);
    data->pos = data->pos + velocity;
}

void Item::updateIndex() {
    H2DE_LevelObjectData* data = H2DE_GetObjectData(object);
    data->index = Map::getIndex(data->pos.y, 1);
}

void Item::updateForCollisionWithPlayer() {
    H2DE_LevelObjectData* thisData = getObjectData();
    H2DE_LevelObjectData* playerData = game->getMap()->getPlayer()->getObjectData();

    H2DE_LevelRect rect = thisData->hitboxes["collision"].rect + thisData->pos;
    H2DE_LevelRect playerRect = playerData->hitboxes["collision"].rect + playerData->pos;

    if (rect.collides(playerRect) != H2DE_NO_FACE) collided();
}

// GETTER
H2DE_LevelObject* Item::getObject() const {
    return object;
}

H2DE_LevelObjectData* Item::getObjectData() const {
    return H2DE_GetObjectData(object);
}

H2DE_LevelVelocity Item::getVelocity() const {
    return velocity;
}

bool Item::toRemove() const {
    return remove;
}
