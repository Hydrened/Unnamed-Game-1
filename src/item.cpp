#include "item.h"

// INIT
Item::Item(Game* g, LevelPos p, std::string t) : game(g), pos(p), sprite(new Sprite(g, t, 150)), texture(t) {
    float angle = rand(0.0f, M_PI * 2.0f);
    velocity = { std::cos(angle), std::sin(angle) };
}

// CLEANUP
Item::~Item() {
    
}

// EVENTS
void Item::pickUp() {
    pickedUp = true;
}

// UPDATE
void Item::update() {
    sprite->update();

    if (pickedUp) {
        updatePickedUpPosition();
        updateCollisionWithPlayer();
    } else updateDropVelocity();

    updateImpl();
}

void Item::updateDropVelocity() {
    static GameData* gameData = game->getData();
    static float airResistance = gameData->physics->airResistance;
    static float itemSpeed = gameData->physics->itemSpeed;

    if (velocity.isNull()) return;

    if (velocity.x != 0.0f) {
        if (velocity.x < 0.0f) velocity.x = std::min(0.0f, velocity.x + airResistance);
        else velocity.x = std::max(0.0f, velocity.x - airResistance);
    }
    if (velocity.y != 0.0f) {
        if (velocity.y < 0.0f) velocity.y = std::min(0.0f, velocity.y + airResistance);
        else velocity.y = std::max(0.0f, velocity.y - airResistance);
    }

    pos = pos + velocity * (itemSpeed / 2);
}

void Item::updatePickedUpPosition() {
    static float itemSpeed = game->getData()->physics->itemSpeed;

    LevelPos playerPos = game->getMap()->getPlayer()->getPos();

    float angle = std::atan2(playerPos.y - pos.y, playerPos.x - pos.x);
    velocity = { std::cos(angle), std::sin(angle) };
    pos = pos + velocity * itemSpeed;
}

void Item::updateCollisionWithPlayer() {
    static std::unordered_map<std::string, LevelRect> itemHitboxes = game->getData()->physics->itemHitboxes;
    static LevelRect defaultPlayerHitbox = game->getData()->others->entities[0].hitbox;

    LevelRect playerHitbox = defaultPlayerHitbox + game->getMap()->getPlayer()->getPos();
    LevelRect itemHitbox = itemHitboxes[texture] + pos;
    if (playerHitbox.collides(itemHitbox) != NONE) collides();
}

// RENDER
void Item::render() {
    static H2DE_Engine* engine = game->getEngine();
    static Calculator* cal = game->getCalculator();
    static GameData* gameData = game->getData();
    static Camera* camera = game->getCamera();

    static std::unordered_map<std::string, LevelSize> texturesSizes = gameData->sizes->textures;
    static std::unordered_map<std::string, LevelPos> texturesOffsets = gameData->offsets->textures;
    static std::unordered_map<std::string, LevelRect> itemHitboxes = gameData->physics->itemHitboxes;

    LevelPos texturePos = pos + texturesOffsets[texture];
    LevelSize textureSize = texturesSizes[texture];
    LevelRect defaultHitbox = itemHitboxes[texture];
    LevelRect hitbox = defaultHitbox + pos;

    if (!camera->contains(texturePos.makeRect(textureSize))) return;

    H2DE_GraphicObject* textureSprite = H2DE_CreateGraphicObject();
    textureSprite->type = IMAGE;
    textureSprite->texture = sprite->getTexture();
    textureSprite->pos = cal->convertToPx(texturePos);
    textureSprite->size = cal->convertToPx(textureSize);
    textureSprite->index = getIndex(std::ceil(pos.y), 2);
    H2DE_AddGraphicObject(engine, textureSprite);

    game->getMap()->displayHitbox(hitbox, { 255, 0, 255, 255 });
}

// GETTER
LevelPos Item::getPos() const {
    return pos;
}

LevelVelocity Item::getVelocity() const {
    return velocity;
}

std::string Item::getTexture() const {
    return texture;
}

// SETTER
void Item::setTexture(std::string t) {
    texture = t;
    sprite->setTexture(texture.substr(0, texture.find(".png")));
}
