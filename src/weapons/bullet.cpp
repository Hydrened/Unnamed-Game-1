#include "bullet.h"

// INIT
Bullet::Bullet(Game* g, Weapon* w, BulletData d, float a) : game(g), weapon(w), data(d), angle(a) {
    initVelocity();
    initObject();
}

void Bullet::initVelocity() {
    velocity.x = std::cos(angle) * data.speed;
    velocity.y = std::sin(angle) * data.speed;
}

void Bullet::initObject() {
    static H2DE_Engine* engine = game->getEngine();

    Entity* owner = weapon->getOwner();
    H2DE_LevelPos ownerCenter = owner->getObjectData()->pos + owner->getData().damageHitbox.getCenter();
    H2DE_LevelPos bulletCenter = data.rect.getCenter();
    H2DE_LevelPos pos = ownerCenter - bulletCenter;

    H2DE_LevelObjectData objData = H2DE_LevelObjectData();
    objData.pos = pos;

    H2DE_TextureData objTextureData = H2DE_TextureData();
    objTextureData.name = "9mm-bullet.png";
    objTextureData.scaleMode = H2DE_SCALE_MODE_NEAREST;
    objTextureData.size = data.rect.getSize();
    objData.texture = H2DE_CreateTexture(engine, objTextureData);

    H2DE_Hitbox hitbox = H2DE_Hitbox();
    hitbox.rect = data.rect;
    hitbox.color = { 255, 127, 127, 255 };
    hitbox.collisionIndex = 1;
    hitbox.onCollide = [](H2DE_LevelObject* obj) {

    };
    objData.hitboxes.push_back(hitbox);

    object = H2DE_CreateLevelObject(engine, objData);
}

// CLEANUP
Bullet::~Bullet() {
    static H2DE_Engine* engine = game->getEngine();
    H2DE_DestroyLevelObject(engine, object);
    std::cout << "Bullet Cleared" << std::endl;
}

// UPDATE
void Bullet::update() {
    H2DE_LevelObjectData* objData = H2DE_GetObjectData(object);
    objData->pos = objData->pos + velocity;
}
