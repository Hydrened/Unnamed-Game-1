#include "functions.h"

std::string getRandomTexture(std::vector<TextureProbability> textureProbabilities) {
    static std::random_device rd;
    static std::default_random_engine e(rd());
    std::uniform_int_distribution<int> dist(0.0f, 100.0);

    float percentage = dist(e);
    float total = 0.0f;
    for (TextureProbability texPro : textureProbabilities) {
        total += texPro.probability;
        if (percentage <= total) return texPro.texture;
    }
    return "";
}

SDL_RendererFlip getRandomFlipX() {
    static std::random_device rd;
    static std::default_random_engine e(rd());
    std::uniform_int_distribution<int> dist(0, 1);

    return (dist(e) == 0) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
}

int getIndex(float yPos, int index) {
    return yPos * 10 + index;
}

bool rand(float value) {
    static std::random_device rd;
    static std::default_random_engine e(rd());
    std::uniform_int_distribution<int> dist(0.0f, 100.0f);

    return (dist(e) < value);
}
