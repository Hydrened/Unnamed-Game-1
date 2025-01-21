#include "item.h"

// INIT
Item::Item(Game* g, LevelPos p, std::string t) : game(g), pos(p), sprite(new Sprite(g, t, 200)), texture(t) {
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
    static float airResistance = game->getData()->physics->airResistance;
    static float itemSpeed = game->getData()->physics->itemSpeed;

    sprite->update();

    if (!pickedUp) {
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
    } else {
        LevelPos playerPos = game->getMap()->getPlayer()->getPos();

        float angle = std::atan2(playerPos.y - pos.y, playerPos.x - pos.x);
        velocity = { std::cos(angle), std::sin(angle) };
        pos = pos + velocity * itemSpeed;
    }
}

// RENDER
void Item::render() {
    static H2DE_Engine* engine = game->getEngine();
    static Calculator* cal = game->getCalculator();
    static GameData* gameData = game->getData();
    static Camera* camera = game->getCamera();

    static std::unordered_map<std::string, LevelSize> texturesSizes = gameData->sizes->textures;
    static std::unordered_map<std::string, LevelPos> texturesOffsets = gameData->offsets->textures;
    static std::unordered_map<std::string, LevelRect> itemHitobxes = gameData->physics->itemHitobxes;

    LevelPos texturePos = pos + texturesOffsets[texture];
    LevelSize textureSize = texturesSizes[texture];
    LevelRect defaultHitbox = itemHitobxes[texture];
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

bool Item::isPickedUp() const {
    return pickedUp;
}

std::string Item::getTexture() const {
    return texture;
}

// SETTER
void Item::setTexture(std::string t) {
    texture = t;
    sprite->setTexture(texture.substr(0, texture.find(".png")));
}
