#include "entity.h"
#undef min
#undef max

// INIT
Entity::Entity(Game* g, Map* m, H2DE_LevelPos p, EntityData d) : game(g), map(m), data(d), health(d.stats.health) {
    initObject(p);
    if (data.stats.regeneration > 0.0f) initRegeneration();
}

void Entity::initObject(H2DE_LevelPos pos) {
    static H2DE_Engine* engine = game->getEngine();

    H2DE_LevelObjectData objectData = H2DE_LevelObjectData();
    objectData.pos = pos;

    H2DE_TextureData textureData = H2DE_TextureData();
    textureData.name = data.textureName;
    textureData.size = data.textureSize;
    textureData.scaleMode = H2DE_SCALE_MODE_NEAREST;

    H2DE_SpriteData spriteData = H2DE_SpriteData();
    spriteData.defaultAnimationName = "idle";
    spriteData.delay = 200;
    spriteData.nbFrame = 4;

    objectData.texture = H2DE_CreateSprite(engine, textureData, spriteData);

    if (data.collisionHitbox.has_value()) {
        H2DE_Hitbox objectCollisionHitbox = H2DE_Hitbox();
        objectCollisionHitbox.rect = data.collisionHitbox.value();
        objectCollisionHitbox.color = { 127, 127, 255, 255 };
        objectCollisionHitbox.collisionIndex = 0;
        objectCollisionHitbox.snap = true;
        objectData.hitboxes["collision"] = objectCollisionHitbox;
    }

    H2DE_Hitbox objectDamageHitbox = H2DE_Hitbox();
    objectDamageHitbox.rect = data.damageHitbox;
    objectDamageHitbox.color = { 255, 127, 127, 255 };
    objectDamageHitbox.collisionIndex = 1;
    objectData.hitboxes["damage"] = objectDamageHitbox;

    object = H2DE_CreateLevelObject(engine, objectData);
}

void Entity::initRegeneration() {
    static H2DE_Engine* engine = game->getEngine();

    regenerationTimline = H2DE_CreateTimeline(engine, 1000, LINEAR, NULL, [this]() {
        int beforeRegen = health;
        health = std::min(health + data.stats.regeneration, data.stats.health);

        if (beforeRegen != health) {
            if (dynamic_cast<Player*>(this)) map->getUi()->refreshHealthBar();

            H2DE_LevelObjectData* objData = H2DE_GetObjectData(object);
            H2DE_LevelPos textPos = objData->hitboxes["damage"].rect.getCenter() + objData->pos;
            textPos.y -= 0.5f;
            map->displayHealthActivity(textPos, data.stats.regeneration, { 0, 255, 0, 255 });
        }
    }, -1);
}

// CLEANUP
Entity::~Entity() {
    destroyObject();
    destroyWeapon();
    destroyTimelines();
    if (game->isDebuging()) std::cout << "└─> Entity cleared" << std::endl;
}

void Entity::destroyObject() {
    static H2DE_Engine* engine = game->getEngine();
    H2DE_DestroyLevelObject(engine, object);
}

void Entity::destroyWeapon() {
    if (weapon) delete weapon;
}

void Entity::destroyTimelines() {
    if (redFilterTimline) delete redFilterTimline;
    if (regenerationTimline) delete regenerationTimline;
}

// EVENTS
void Entity::kill() {
    dead = true;
    killImpl();
}

void Entity::inflictDamages(int damages, float crit) {
    static H2DE_Engine* engine = game->getEngine();
    static float critDamageMultiplier = game->getData()->critDamageMultiplier;

    bool isCrit = H2DE_RandomFloatInRange(0.0f, 100.0f) < crit;
    bool isPlayer = dynamic_cast<Player*>(this) != nullptr;

    if (isCrit) damages *= critDamageMultiplier;
    damages = std::max(damages - data.stats.defence, 0);
    health = std::max(health - damages, 0);

    H2DE_LevelPos textPos = getObjectData()->hitboxes["damage"].rect.getCenter() + getObjectData()->pos;
    textPos.y -= 0.5f;
    H2DE_ColorRGB textColor = (isPlayer) ? H2DE_ColorRGB{ 255, 0, 0, 255 } : (isCrit) ? H2DE_ColorRGB{ 255, 255, 0, 255 } : H2DE_ColorRGB{ 255, 255, 255, 255 };

    map->displayHealthActivity(textPos, damages, textColor);

    inflictDamagesImpl(damages, isCrit);

    if (health == 0) return kill();

    redFilterTimline = H2DE_CreateTimeline(engine, 200, LINEAR, [this](float blend) {
        Uint8 otherThanRed = H2DE_Lerp(0, UINT8_MAX, blend);
        H2DE_GetTextureData(H2DE_GetObjectData(object)->texture)->color = { 255, otherThanRed, otherThanRed, 255 };
    }, [this]() {
        H2DE_GetTextureData(H2DE_GetObjectData(object)->texture)->color = { 255, 255, 255, 255 };
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
    updateRegeneration();
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

void Entity::updateRegeneration() {
    if (regenerationTimline) H2DE_TickTimeline(regenerationTimline);
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

int Entity::getHealth() const {
    return health;
}

bool Entity::isDead() const {
    return dead;
}
