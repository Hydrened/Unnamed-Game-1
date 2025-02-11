#ifndef UTILS_H
#define UTILS_H

namespace std {
    template<>
    struct hash<H2DE_LevelPos> {
        size_t operator()(const H2DE_LevelPos& pos) const {
            return hash<int>()(pos.x) ^ (hash<int>()(pos.y) << 1);
        }
    };
}

struct EntityStats {
    float health;
    float attack;
    float defence;
    float speed;
    float crit;
    float pickup;
    float regeneration;
};

struct TextureData {
    H2DE_LevelPos offset;
    H2DE_LevelSize size;
};

struct EntityData {
    std::string textureName;
    EntityStats stats;
    std::optional<H2DE_LevelRect> collisionHitbox;
    H2DE_LevelRect damageHitbox;
};

struct TileGround {
    std::string textureName;
};

struct TileDecoration {
    std::string textureName;
    std::optional<H2DE_Hitbox> hitbox;
};

struct TileData {
    TileGround ground;
    std::optional<TileDecoration> decoration;
};

struct Tile {
    TileData data;
    H2DE_LevelObject* groundObject;
    std::optional<H2DE_LevelObject*> decorationObject;
};

struct BulletData {
    std::string textureName;
    H2DE_LevelRect rect;
    float speed;
    float damage;
    bool piercing;
    bool explosive;
};

struct WeaponData {
    std::string name;
    std::string textureName;
    int delay;
    BulletData bullet;
};

#endif
