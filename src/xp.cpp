#include "xp.h"

// INIT
Xp::Xp(Game* g, LevelPos p, int l) : game(g), pos(p), level(l) {
    float angle = rand(0.0f, M_PI * 2.0f);
    velocity = { std::cos(angle), std::sin(angle) };
}

// CLEANUP
Xp::~Xp() {

}

// EVENTS
void Xp::increaseLevel() {
    if (level < 5) level++;
}

void Xp::pickUp() {
    pickedUp = true;
}

// UPDATE
void Xp::update() {
    static float airResistance = game->getData()->physics->airResistance;
    static float xpSpeed = game->getData()->physics->xpSpeed;

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

        pos = pos + velocity * (xpSpeed / 2);
    } else {
        LevelPos playerPos = game->getMap()->getPlayer()->getPos();

        float angle = std::atan2(playerPos.y - pos.y, playerPos.x - pos.x);
        velocity = { std::cos(angle), std::sin(angle) };
        pos = pos + velocity * xpSpeed;
    }
}

// RENDER
void Xp::render() {
    static H2DE_Engine* engine = game->getEngine();
    static Calculator* cal = game->getCalculator();
    static GameData* gameData = game->getData();
    static Camera* camera = game->getCamera();

    static std::unordered_map<std::string, LevelSize> texturesSizes = gameData->sizes->textures;
    static std::unordered_map<std::string, LevelPos> texturesOffsets = gameData->offsets->textures;
    static LevelRect defaultXpHitbox = gameData->physics->xpHitbox;

    std::string xpTextureName = "xp-lvl-" + std::to_string(level) + ".png";

    LevelPos xpTexturePos = pos + texturesOffsets[xpTextureName];
    LevelSize xpTextureSize = texturesSizes[xpTextureName];
    LevelRect xpHitbox = defaultXpHitbox + pos;

    if (!camera->contains(xpTexturePos.makeRect(xpTextureSize))) return;

    H2DE_GraphicObject* xpSprite = H2DE_CreateGraphicObject();
    xpSprite->type = IMAGE;
    xpSprite->texture = xpTextureName;
    xpSprite->pos = cal->convertToPx(xpTexturePos);
    xpSprite->size = cal->convertToPx(xpTextureSize);
    xpSprite->index = getIndex(std::ceil(pos.y), 2);
    H2DE_AddGraphicObject(engine, xpSprite);

    game->getMap()->displayHitbox(xpHitbox, { 255, 0, 255, 255 });
}

// GETTER
LevelPos Xp::getPos() const {
    return pos;
}

LevelVelocity Xp::getVelocity() const {
    return velocity;
}

int Xp::getLevel() const {
    return level;
}

bool Xp::isPickedUp() const {
    return pickedUp;
}
