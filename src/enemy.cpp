#include "entity.h"

// INIT
Enemy::Enemy(Game* g, Map* m, LevelPos p, EntityData d) : Entity(g, m, p, d) {

}

// CLEANUP
Enemy::~Enemy() {

}

// EVENTS
bool Enemy::isNearPlayer() {
    static float condiredNearPlayerRange = game->getData()->offsets->enemyCondiredNearPlayerRange;
    LevelPos playerPos = map->getPlayer()->getPos();
    return (std::abs(playerPos.x - pos.x) < condiredNearPlayerRange && std::abs(playerPos.y - pos.y) < condiredNearPlayerRange);
}

// UPDATE
void Enemy::update() {
    updateSprite();
    updateForPlayer();
}

void Enemy::updateForPlayer() {
    if (!isNearPlayer()) {
        sprite->setAnimation(WALK);
        LevelPos playerPos = map->getPlayer()->getPos();

        float angle = std::atan2(playerPos.y - pos.y, playerPos.x - pos.x);
        float xVelocity = data.stats.speed * std::cos(angle);
        float yVelocity = data.stats.speed * std::sin(angle);
        pos.x += xVelocity;
        pos.y += yVelocity;

        if (xVelocity != 0) facing = (xVelocity > 0) ? RIGHT : LEFT;
    } else sprite->setAnimation(IDLE);
}
