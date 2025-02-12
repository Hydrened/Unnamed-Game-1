#include "game.h"

// INIT
Game::Game() {
    initEngine();
    initMap();
    initCamera();

    H2DE_DebugObjectNumber(engine, false);
    H2DE_DebugObjectHitboxes(engine, true);
}

void Game::initEngine() {
    H2DE_EngineData engineData = H2DE_EngineData();
    engineData.camera.width = 20.0f;
    engineData.camera.smoothing = 0.0f;
    engineData.fps = 60;
    engineData.window.fullscreen = false;
    engineData.window.saveState = false;
    engineData.window.title = "Unnamed_Game-2.0.3";
    engineData.window.pos = { 100, 100 };
    engineData.window.size = { 1280, 720 };
    engine = H2DE_CreateEngine(engineData);

    H2DE_LoadAssets(engine, "assets");

    H2DE_SetGameHandleEventCall(engine, [this](SDL_Event event) {
        handleEvents(event);
    });

    H2DE_SetGameUpdateCall(engine, [this]() {
        update();
    });
}

void Game::initMap() {
    map = new Map(this);
}

void Game::initCamera() {
    static H2DE_Camera* camera = H2DE_GetCamera(engine);
    static Player* player = map->getPlayer();
    static H2DE_LevelObject* playerObject = player->getObject();
    static H2DE_LevelObjectData* playerObjectData = player->getObjectData();

    H2DE_LevelSize playerTextureSize = { 1.0f, 1.0f };

    H2DE_LevelPos camPos = H2DE_GetCameraPos(camera);
    H2DE_LevelPos playerPos = playerObjectData->pos;

    H2DE_LevelSize camSize = H2DE_GetCameraSize(camera);
    H2DE_LevelPos camOffset = { (camSize.w - playerTextureSize.w) / 2, (camSize.h - playerTextureSize.h) / 2 };

    H2DE_SetCameraPos(camera, camPos + playerPos - camOffset);
    H2DE_SetCameraReference(camera, playerObject);
    H2DE_SetCameraLockedToReference(camera, true);
}

// CLEANUP
Game::~Game() {
    delete map;
    delete data;
    std::cout << "Game cleared" << std::endl;
    H2DE_DestroyEngine(engine);
    std::cout << "Engine cleared" << std::endl;
}

// RUN
void Game::run() {
    H2DE_RunEngine(engine);
}

// EVENTS
void Game::handleEvents(SDL_Event event) {
    switch (event.type) {
        case SDL_KEYDOWN: {
            SDL_Keycode key = event.key.keysym.sym;
            if (std::find(keysDown.begin(), keysDown.end(), key) == keysDown.end()) keysDown.push_back(key);
            break;
        }

        case SDL_KEYUP:
            keysDown.erase(std::find(keysDown.begin(), keysDown.end(), event.key.keysym.sym));
            break;

        case SDL_MOUSEMOTION: mousePos = { event.button.x, event.button.y }; break;

        case SDL_MOUSEBUTTONDOWN: {
            int button = event.button.button;
            if (std::find(mouseButtonsDown.begin(), mouseButtonsDown.end(), button) == mouseButtonsDown.end()) mouseButtonsDown.push_back(button);
            break;
        }

        case SDL_MOUSEBUTTONUP:
            mouseButtonsDown.erase(std::find(mouseButtonsDown.begin(), mouseButtonsDown.end(), event.button.button));
            break;

        default: break;
    }
}

// UPDATE
void Game::update() {
    if (map) map->update();
}

// GETTER
H2DE_Engine* Game::getEngine() const {
    return engine;
}

GameData* Game::getData() const {
    return data;
}

Map* Game::getMap() const {
    return map;
}

std::vector<SDL_Keycode> Game::getKeysDown() const {
    return keysDown;
}

std::vector<int> Game::getMouseButtonsDown() const {
    return mouseButtonsDown;
}

H2DE_AbsPos Game::getMousePos() const {
    return mousePos;
}
