#include "entity.h"

// INIT
Entity::Entity(Game* g, Map* m, H2DE_LevelPos p, EntityData d) : game(g), map(m), pos(p), data(d) {

}

// CLEANUP
Entity::~Entity() {
    static H2DE_Engine* engine = game->getEngine();
    H2DE_DestroyLevelObject(engine, object);
    std::cout << "Entity cleared" << std::endl;
}

// UPDATE
void Entity::update() {
    updateImpl();
}
