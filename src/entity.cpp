#include "entity.h"

// INIT
Entity::Entity(Game* g, Map* m, LevelPos p, EntityData d) : game(g), map(m), pos(p), data(d), sprite(new Sprite(g, d.texture, 200)) {

}

// CLEANUP
Entity::~Entity() {

}

// EVENT
void Entity::equipWeapon(int id) {
    if (weapon) delete weapon;
    static std::unordered_map<int, WeaponData> weapons = game->getData()->others->weapons;
    weapon = new Weapon(game, this, weapons[id]);
}

void Entity::inflictDamages(float damages, float crit) {
    static float critDamageMultiplier = game->getData()->physics->critDamageMultiplier;

    if (rand(crit)) damages *= critDamageMultiplier;
    damages = std::max(damages - data.stats.defence, 0.0f);
    data.stats.health -= damages;

    if (data.stats.health <= 0) kill();
}

// UPDATE
void Entity::updateSprite() {
    sprite->update();
}

void Entity::updateWeapon() {
    if (weapon) weapon->update();
}

// RENDER
void Entity::render() {
    renderTexture();
    renderWeapon();
}

void Entity::renderTexture() {
    static H2DE_Engine* engine = game->getEngine();
    static Calculator* cal = game->getCalculator();
    static GameData* gameData = game->getData();

    static LevelSize textureSize = gameData->sizes->textures[data.texture];
    static LevelPos textureOffset = gameData->offsets->textures[data.texture];

    H2DE_GraphicObject* entitySprite = H2DE_CreateGraphicObject();
    entitySprite->type = IMAGE;
    entitySprite->texture = sprite->getTexture();
    entitySprite->pos = cal->convertToPx(pos + textureOffset);
    entitySprite->size = cal->convertToPx(textureSize);
    if (facing == RIGHT) entitySprite->flip = SDL_FLIP_HORIZONTAL;
    entitySprite->index = getIndex(std::ceil(pos.y), 4);
    H2DE_AddGraphicObject(engine, entitySprite);

    map->displayHitbox(data.hitbox + pos, { 255, 0, 0, 255 });
}

void Entity::renderWeapon() {
    if (weapon) weapon->render();
}

// GETTER
LevelPos Entity::getPos() const {
    return pos;
}

EntityData Entity::getData() const {
    return data;
}
