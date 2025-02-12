#include "items.h"

// INIT
Coin::Coin(Game* g, H2DE_LevelPos p) : Item(g, p, "coins.php") {

}

// CLEANUP
Coin::~Coin() {
    std::cout << "Coin cleared" << std::endl;
}

// UPDATE
void Coin::update() {

}
