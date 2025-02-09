#ifndef DATA_H
#define DATA_H

struct GameData {
    std::unordered_map<std::string, TextureData> texturesData = {
        { "ground.png", { { 0.0f, 0.0f }, { 1.0f, 1.0f } } },
        { "bush.png", { { -0.5f, 0.0f }, { 2.0f, 1.0f } } },
    };

    std::unordered_map<H2DE_LevelPos, TileData> mapData = {
        {{ 0.0f, 0.0f }, { TileGround{ "ground.png" }, std::nullopt }},
        {{ 1.0f, 0.0f }, { TileGround{ "ground.png" }, std::nullopt }},
        {{ 2.0f, 0.0f }, { TileGround{ "ground.png" }, std::nullopt }},
        {{ 3.0f, 0.0f }, { TileGround{ "ground.png" }, TileDecoration{ "bush.png", H2DE_Hitbox{ { 0.0f, 0.0f, 1.0f, 1.0f }, { 255, 0, 0, 255 }, 0, true, std::nullopt } } }},
        {{ 4.0f, 0.0f }, { TileGround{ "ground.png" }, std::nullopt }},
    };
};

#endif
