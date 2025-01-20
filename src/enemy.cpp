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

void Enemy::kill() {
    Map* map = game->getMap();

    map->dropXp(pos, data.xpLevel);
    map->killEnemy(this);
}

// UPDATE
void Enemy::update() {
    updateSprite();
    updateForPlayer();
    updateWeapon();
}

void Enemy::updateForPlayer() {
    if (!isNearPlayer()) {
        sprite->setAnimation(WALK);
        LevelPos playerPos = map->getPlayer()->getPos();

        float angle = std::atan2(playerPos.y - pos.y, playerPos.x - pos.x);
        LevelVelocity velocity = { data.stats.speed * std::cos(angle), data.stats.speed * std::sin(angle) };
        pos = pos + velocity;

        if (velocity.x != 0) facing = (velocity.x > 0) ? RIGHT : LEFT;
    } else sprite->setAnimation(IDLE);
}
