#ifndef DATA_H
#define DATA_H

struct GameData {
    struct Positions {

    };

    struct Sizes {
        float blocksOnWidth = 30.0f;
        float blocksOnHeight = 0.0f;
        int winWidth = 0;
        int winHeight = 0;
        int blockSize = 0;

        float nearSpawnDistance = 3.0f;

        std::unordered_map<std::string, LevelSize> textures = {
            { "player.png", { 1.0f, 1.0f } },
            { "ground.png", { 1.0f, 1.0f } },
            { "ground-with-grass.png", { 1.0f, 1.0f } },
            { "log-1.png", { 1.0f, 1.0f } },
            { "log-2.png", { 2.0f, 1.0f } },
            { "grass-1.png", { 1.0f, 1.0f } },
            { "grass-2.png", { 1.0f, 1.0f } },
            { "grass-3.png", { 1.0f, 1.0f } },
            { "bush.png", { 2.0f, 1.0f } },
            { "tree-1.png", { 3.0f, 3.0f } },
            { "tree-2.png", { 3.0f, 3.0f } },
        };
    };

    struct Offsets {
        float enemyCondiredNearPlayerRange = 0.2f;

        std::unordered_map<std::string, LevelPos> textures = {
            { "player.png", { -0.25f, -0.25f } },
            { "ground.png", { 0.0f, 0.0f } },
            { "ground-with-grass.png", { 0.0f, 0.0f } },
            { "log-1.png", { 0.0f, 0.0f } },
            { "log-2.png", { -0.5f, 0.0f } },
            { "grass-1.png", { 0.0f, 0.0f } },
            { "grass-2.png", { 0.0f, 0.0f } },
            { "grass-3.png", { 0.0f, 0.0f } },
            { "bush.png", { -0.5f, 0.0f } },
            { "tree-1.png", { -1.0f, -2.0f } },
            { "tree-2.png", { -1.0f, -2.0f } },
        };
    };

    struct Physics {
        std::unordered_map<std::string, LevelRect> decorationHitboxes = {
            { "tree-1.png", { 0.15f, 0.25f, 0.7f, 0.5f } },
            { "tree-2.png", { 0.15f, 0.25f, 0.7f, 0.5f } },
        };
    };

    struct Probabilities {
        float haveDecoration = 10.0f;

        std::vector<TextureProbability> ground = {
            { 10.0f, "ground-with-grass.png" },
            { 90.0f, "ground.png" },
        };
        std::vector<TextureProbability> decoration = {
            { 1.0f, "log-1.png" },
            { 1.0f, "log-2.png" },
            { 25.0f, "grass-1.png" },
            { 25.0f, "grass-2.png" },
            { 25.0f, "grass-3.png" },
            { 15.0f, "bush.png" },
            { 4.0f, "tree-1.png" },
            { 4.0f, "tree-2.png" },
        };
    };

    std::unordered_map<int, EntityData> entities = {
        { 0, {{ "player.png", SDL_FLIP_NONE }, { 0.0f, 0.0f, 0.5f, 0.5f }, {
            0.0f,   /*< health */
            0.0f,   /*< attack */
            0.0f,   /*< defence */
            0.1f,  /*< speed */
            0.0f,   /*< crit */
            0.0f,   /*< pickup */
            0.0f,   /*< haste */
            0.0f,   /*< regeneration */
        }}},
        { 1, {{ "player.png", SDL_FLIP_NONE }, { 0.0f, 0.0f, 0.5f, 0.5f }, {
            0.0f,   /*< health */
            0.0f,   /*< attack */
            0.0f,   /*< defence */
            0.05f,  /*< speed */
            0.0f,   /*< crit */
            0.0f,   /*< pickup */
            0.0f,   /*< haste */
            0.0f,   /*< regeneration */
        }}},
    };

    ~GameData() {
        delete positions;
        delete sizes;
        delete offsets;
        delete physics;
        delete probabilities;
    };

    void setWinSize(int width, int height) {
        sizes->winWidth = width;
        sizes->winHeight = height;
        sizes->blockSize = static_cast<int>(width / sizes->blocksOnWidth);
        sizes->blocksOnHeight = height / sizes->blockSize;
    }

    Positions* positions = new Positions();
    Sizes* sizes = new Sizes();
    Offsets* offsets = new Offsets();
    Physics* physics = new Physics();
    Probabilities* probabilities = new Probabilities();
};

#endif
