#include "items.h"

// INIT
Item::Item(Game* g, H2DE_LevelPos p, std::string t) : game(game) {
    initObject(t);
}

void Item::initObject(std::string textureName) {
    // static H2DE_Engine* engine = game->getEngine();

    // H2DE_CreateLevelObject();
}

// CLEANUP
Item::~Item() {
    std::cout << "└─> Item cleared" << std::endl;
}
