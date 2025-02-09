#include "game.h"

// INIT
Game::Game() {
    initEngine();
    map = new Map(this);
    
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
    engineData.window.title = "Unnamed_Game-2.0.0";
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
    while (SDL_PollEvent(&event)) switch (event.type) {
        
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
