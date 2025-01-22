#include "bullet.h"

// INIT
Bullet::Bullet(Game* g, Weapon* w, BulletData d, float angle) : game(g), weapon(w), data(d) {
    static LevelRect playerHitbox = game->getData()->others->entities[0].hitbox;
    LevelPos playerPos = game->getMap()->getPlayer()->getPos();

    LevelPos playerCenter = (playerHitbox + playerPos).getCenter();
    LevelPos bulletCenter = data.size.makeRect({ 0.0f, 0.0f }).getCenter();
    pos = playerCenter - bulletCenter;

    velocity = { data.speed * std::cos(angle), data.speed * std::sin(angle) };
}

// CLEANUP
Bullet::~Bullet() {
    damagedEntities.clear();
}

// EVENTS
void Bullet::destroy() {
    weapon->destroyBullet(this);
}

// UPDATE
void Bullet::update() {
    updatePos();
    checkEntityCollisions();
}

void Bullet::updatePos() {
    static Camera* camera = game->getCamera();
    LevelPos camPos = camera->getPos();

    pos = pos + velocity;
    if (!camera->contains(pos.makeRect(data.size))) destroy();
}

void Bullet::checkEntityCollisions() {
    static float piercingDamageMultiplier = game->getData()->physics->piercingDamageMultiplier;
    Entity* user = weapon->getUser();
    LevelRect bulletHitbox = pos.makeRect(data.size);

    for (Enemy* enemy : game->getMap()->getEnemies()) {
        LevelRect enemyHitbox = enemy->getData().hitbox + enemy->getPos();
        if (bulletHitbox.collides(enemyHitbox) == NONE) continue;
        if (std::find(damagedEntities.begin(), damagedEntities.end(), enemy) != damagedEntities.end()) continue;

        damagedEntities.push_back(enemy);
        enemy->inflictDamages(data.damage, user->getData().stats.crit);
        if (!data.piercing) {
            destroy();
            break;
        } else data.damage *= piercingDamageMultiplier;
    }
}

// RENDER
void Bullet::render() {
    static H2DE_Engine* engine = game->getEngine();
    static Calculator* cal = game->getCalculator();

    H2DE_GraphicObject* bulletSprite = H2DE_CreateGraphicObject();
    bulletSprite->type = IMAGE;
    bulletSprite->texture = data.texture;
    bulletSprite->pos = cal->convertToPx(pos);
    bulletSprite->size = cal->convertToPx(data.size);
    bulletSprite->index = getIndex(std::ceil(pos.y), 3);
    H2DE_AddGraphicObject(engine, bulletSprite);

    game->getMap()->displayHitbox(data.size.makeRect(pos), { 255, 0, 0, 255 });
}
