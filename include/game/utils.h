#ifndef UTILS_H
#define UTILS_H

#include <algorithm>
#include <optional>
#include <iostream>
#include <string>
#include <SDL2/SDL_types.h>
#include <SDL2/SDL_render.h>
struct LevelRect;
struct LevelSize;
struct LevelVelocity;

enum GameState {
    MAIN_MENU,
    PLAYING,
};

enum Face {
    NONE,
    TOP,
    BOTTOM,
    LEFT,
    RIGHT,
};

enum SpriteAnimation {
    IDLE,
    WALK,
};

struct LevelPos {
    float x;
    float y;

    LevelPos operator+(const LevelPos& other) const;
    LevelPos operator+(const LevelVelocity& other) const;
    LevelPos operator-(const LevelPos& other) const;
    LevelPos operator/(const int& other) const;
    bool operator==(const LevelPos& other) const;
    LevelRect makeRect(LevelSize size) const;
};

struct LevelPosHash {
    std::size_t operator()(const LevelPos& pos) const;
};

struct LevelSize {
    float w;
    float h;

    LevelSize operator+(const LevelSize& other) const;
    LevelSize operator/(const int& other) const;
    LevelRect makeRect(LevelPos pos) const;
};

struct LevelVelocity {
    float x;
    float y;

    bool isNull() const;
    LevelVelocity operator*(const float& other) const;
    bool operator>(const LevelVelocity& other) const;
};

struct LevelRect {
    float x;
    float y;
    float w;
    float h;

    LevelRect operator+(const LevelPos& other) const;
    Face collides(const LevelRect& other) const;
    LevelPos getPos() const;
    LevelSize getSize() const;
    LevelPos getCenter() const;
};

struct Tile {
    std::string ground;
    std::optional<std::string> decoration;
    bool fliped;
};

struct TextureProbability {
    float probability;
    std::string texture;
};

struct EntityStats {
    float health;
    float attack;
    float defence;
    float speed;
    float crit;
    float pickup;
    float regeneration;
};

struct EntityData {
    std::string texture;
    LevelRect hitbox;
    int xpLevel;
    EntityStats stats;
};

struct BulletData {
    std::string texture;
    LevelSize size;
    float speed;
    float damage;
    bool piercing;
    bool explosive;
};

struct WeaponData {
    std::string name;
    std::string texture;
    int delay;
    BulletData bullet;
};

std::ostream& operator<<(std::ostream& os, const LevelPos& pos);
std::ostream& operator<<(std::ostream& os, const LevelSize& size);
std::ostream& operator<<(std::ostream& os, const LevelVelocity& velocity);
std::ostream& operator<<(std::ostream& os, const LevelRect& rect);

#endif
