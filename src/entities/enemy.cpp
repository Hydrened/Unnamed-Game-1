#include "entity.h"

// INIT
Enemy::Enemy(Game* g, Map* m, H2DE_LevelPos p, EntityData d) : Entity(g, m, p, d) {

}

// CLEANUP
Enemy::~Enemy() {
    std::cout << "Enemy cleared" << std::endl;
}

// EVENTS
void Enemy::kill() {
    
}

// UPDATE
void Enemy::updateImpl() {
    
}
