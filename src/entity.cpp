#include "entity.h"

// ENTITY

// INIT
Entity::Entity(Game* g, Map* m, LevelPos p, EntityData d) : game(g), map(m), pos(p), data(d), sprite(new Sprite(g, d.texture.name, 200)) {

}

// CLEANUP
Entity::~Entity() {

}

// UPDATE
void Entity::updateSprite() {
    sprite->update();
}

// RENDER
void Entity::render() {
    static H2DE_Engine* engine = game->getEngine();
    static Calculator* cal = game->getCalculator();
    static GameData* gameData = game->getData();

    static LevelSize textureSize = gameData->sizes->textures[data.texture.name];
    static LevelPos textureOffset = gameData->offsets->textures[data.texture.name];

    H2DE_GraphicObject* entitySprite = H2DE_CreateGraphicObject();
    entitySprite->type = IMAGE;
    entitySprite->texture = sprite->getTexture();
    entitySprite->pos = cal->convertToPx(pos + textureOffset);
    entitySprite->size = cal->convertToPx(textureSize);
    if (facing == RIGHT) entitySprite->flip = SDL_FLIP_HORIZONTAL;
    entitySprite->index = getIndex(std::ceil(pos.y), 1);
    H2DE_AddGraphicObject(engine, entitySprite);

    map->displayHitbox(data.hitbox + pos, { 255, 0, 0, 255 });
}

// GETTER
LevelPos Entity::getPos() const {
    return pos;
}
