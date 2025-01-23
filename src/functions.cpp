#include "functions.h"

std::string getRandomTexture(std::vector<TextureProbability> textureProbabilities) {
    static std::random_device rd;
    static std::default_random_engine e(rd());
    std::uniform_int_distribution<int> dist(0.0f, 100.0f);

    float percentage = dist(e);
    float total = 0.0f;
    for (TextureProbability texPro : textureProbabilities) {
        total += texPro.probability;
        if (percentage <= total) return texPro.texture;
    }
    return "";
}

bool getRandomBool() {
    static std::random_device rd;
    static std::default_random_engine e(rd());
    std::uniform_int_distribution<int> dist(0, 1);

    return (dist(e) == 0);
}

int getIndex(float yPos, int index) {
    return yPos * 10 + index;
}

bool rand(float value) {
    return (rand(0.0f, 100.0f) < value);
}

float rand(float min, float max) {
    static std::random_device rd;
    static std::default_random_engine e(rd());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(e);
}

float lerp(float min, float max, float blend) {
    return min - blend * (max - min);
}
