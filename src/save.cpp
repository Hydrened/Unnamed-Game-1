#include "save.h"

// INIT
Save::Save(Game* g) : game(g) {
    if (fs::exists("save.dat")) {
        json save = H2DE_ReadJson("save.dat");
        coins = static_cast<int>(save["coins"]);
    } else coins = 0;
}

// CLEANUP
Save::~Save() noexcept(false) {
    json save;
    save["coins"] = coins;

    if (!H2DE_WriteJson("save.dat", &save)) {
        throw std::runtime_error("AAAAA-301: Error saving data");
    }
}

// GETTER
int Save::getCoins() const {
    return coins;
}

// SETTER
void Save::setCoins(int c) {
    coins = c;
}
