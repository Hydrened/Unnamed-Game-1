#include "entity.h"
#undef max

// INIT
Entity::Entity(Game* g, Map* m, H2DE_LevelPos p, EntityData d) : game(g), map(m), data(d) {
    H2DE_Engine* engine = game->getEngine();

    H2DE_LevelObjectData objectData = H2DE_LevelObjectData();
    objectData.pos = p;
    objectData.texture = getSprite();
    objectData.hitboxes = getHitboxes();
    object = H2DE_CreateLevelObject(engine, objectData);
}

H2DE_Surface* Entity::getSprite() const {
    H2DE_Engine* engine = game->getEngine();

    H2DE_TextureData textureData = H2DE_TextureData();
    textureData.name = data.textureName;
    textureData.size = { 1.0f, 1.0f };
    textureData.scaleMode = H2DE_SCALE_MODE_NEAREST;

    H2DE_SpriteData spriteData = H2DE_SpriteData();
    spriteData.defaultAnimationName = "idle";
    spriteData.delay = 200;
    spriteData.nbFrame = 4;

    return H2DE_CreateSprite(engine, textureData, spriteData);
}

std::vector<H2DE_Hitbox> Entity::getHitboxes() const {
    std::vector<H2DE_Hitbox> res;

    if (data.collisionHitbox.has_value()) {
        H2DE_Hitbox objectCollisionHitbox = H2DE_Hitbox();
        objectCollisionHitbox.rect = data.collisionHitbox.value();
        objectCollisionHitbox.color = { 127, 127, 255, 255 };
        objectCollisionHitbox.collisionIndex = 0;
        objectCollisionHitbox.snap = true;
        res.push_back(objectCollisionHitbox);
    }

    H2DE_Hitbox objectDamageHitbox = H2DE_Hitbox();
    objectDamageHitbox.rect = data.damageHitbox;
    objectDamageHitbox.color = { 255, 127, 127, 255 };
    objectDamageHitbox.collisionIndex = 1;
    res.push_back(objectDamageHitbox);

    return res;
}

// CLEANUP
Entity::~Entity() {
    static H2DE_Engine* engine = game->getEngine();
    if (weapon) delete weapon;
    if (redFilterTimline) delete redFilterTimline;
    H2DE_DestroyLevelObject(engine, object);
    std::cout << "└─> Entity cleared" << std::endl;
}

// EVENTS
void Entity::kill() {
    dead = true;
    killImpl();
}

void Entity::inflictDamages(float damages, float crit) {
    static H2DE_Engine* engine = game->getEngine();
    static float critDamageMultiplier = game->getData()->critDamageMultiplier;

    if (H2DE_RandomFloatInRange(0.0f, 100.0f) < crit) damages *= critDamageMultiplier;
    damages = std::max(damages - data.stats.defence, 0.0f);
    data.stats.health = std::max(data.stats.health - damages, 0.0f);

    if (data.stats.health == 0) kill();

    redFilterTimline = H2DE_CreateTimeline(engine, 100, LINEAR, [this](float blend) {
        Uint8 otherThanRed = H2DE_Lerp(0, UINT8_MAX, blend);
        H2DE_GetObjectData(object)->texture->getData()->color = { 255, otherThanRed, otherThanRed, 255 };
    }, [this]() {
        H2DE_GetObjectData(object)->texture->getData()->color = { 255, 255, 255, 255 };
        delete redFilterTimline;
        redFilterTimline = nullptr;
    }, 0);
}

void Entity::equipWeapon(int id) {
    static std::unordered_map<int, WeaponData> weaponsData = game->getData()->weaponsData;
    if (weapon) delete weapon;
    weapon = new Weapon(game, this, weaponsData[id]);
}

// UPDATE
void Entity::update() {
    H2DE_LevelPos defaultPos = H2DE_GetObjectData(object)->pos;
    updateImpl();
    updatePos();
    updateFacingImpl();
    updateFacing();
    updateAnimation(defaultPos);
    updateRedFilter();
    updateIndex();
    updateWeapon();
}

void Entity::updatePos() {
    H2DE_LevelObjectData* objData = getObjectData();
    objData->pos = objData->pos + velocity;
}

void Entity::updateFacing() {
    H2DE_LevelObjectData* objData = getObjectData();
    if (facing == H2DE_RIGHT_FACE) objData->transform.flip = H2DE_FLIP_HORIZONTAL;
    else objData->transform.flip = H2DE_NO_FLIP;
}

void Entity::updateAnimation(H2DE_LevelPos defaultPos) {
    H2DE_LevelObjectData* objData = getObjectData();
    H2DE_SetSpriteAnimation(dynamic_cast<H2DE_Sprite*>(objData->texture), (defaultPos == objData->pos) ? "idle" : "walk");
}

void Entity::updateRedFilter() {
    if (redFilterTimline) H2DE_TickTimeline(redFilterTimline);
}

void Entity::updateIndex() {
    H2DE_LevelObjectData* objData = getObjectData();
    objData->index = Map::getIndex(objData->pos.y, 3);
}

void Entity::updateWeapon() {
    if (weapon) weapon->update();
}

// GETTER
EntityData Entity::getData() const {
    return data;
}

H2DE_LevelObject* Entity::getObject() const {
    return object;
}

H2DE_LevelObjectData* Entity::getObjectData() const {
    return H2DE_GetObjectData(getObject());
}

Weapon* Entity::getWeapon() const {
    return weapon;
}

bool Entity::isDead() const {
    return dead;
}
