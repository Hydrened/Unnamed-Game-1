#include "game.h"

// INIT
Game::Game(int argc, char** argv) {
    createWindow();
    H2DE_LoadAssets(engine, "assets");

    H2DE_DebugGraphicObjects(engine, false);

    int width, height;
    SDL_GetWindowSize(window, &width, &height);
    data->setWinSize(width, height);

    H2DE_SetTextureScaleMode(engine, SDL_ScaleModeNearest);


    calculator = new Calculator(this);
    save = new Save(this);
    camera = new Camera(this);
    map = new Map(this);
}

void Game::createWindow() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        throw std::runtime_error("AAAAA-101: Error creating window => SDL_Init failed: " + std::string(SDL_GetError()));
    }

    window = SDL_CreateWindow("AAAAA - 1.0.0", 0, 0, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (!window) {
        SDL_Quit();
        throw std::runtime_error("AAAAA-102: Error creating window => SDL_CreateWindow failed: " + std::string(SDL_GetError()));
    }
    
    int winWidth, winHeight;
    SDL_GetWindowSize(window, &winWidth, &winHeight);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        throw std::runtime_error("AAAAA-103: Error creating window => SDL_CreateRenderer failed: " + std::string(SDL_GetError()));
    }

    engine = H2DE_CreateEngine(renderer, winWidth, winHeight, FPS);
    if (!engine) {
        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
        SDL_Quit();
        throw std::runtime_error("AAAAA-104: Error creating window => H2DE_CreateEngine failed");
    }

    SDL_SetWindowGrab(window, SDL_TRUE);
    SDL_SetWindowMinimumSize(window, winWidth, winHeight);
    SDL_SetWindowMaximumSize(window, winWidth, winHeight);
    H2DE_SetEngineMinimumSize(engine, winWidth, winHeight);
    H2DE_SetEngineMaximumSize(engine, winWidth, winHeight);
}

// CLEANUP
Game::~Game() {
    if (map) delete map;
    delete save;

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    H2DE_DestroyEngine(engine);
    SDL_Quit();
}

void Game::quit() {
    isRunning = false;
}

// RUN
void Game::run() {
    Uint32 now = SDL_GetTicks();
    int frameTime;
    SDL_Event event;
    int timePerFrame = 1000 / FPS;

    while (isRunning) {
        now = SDL_GetTicks();

        handleEvents(event);
        update();
        render();
        H2DE_RenderEngine(engine);
        
        frameTime = SDL_GetTicks() - now;
        currentFPS = 1000.0f / static_cast<float>((frameTime > 0) ? frameTime : 1);
        if (timePerFrame >= frameTime) SDL_Delay(timePerFrame - frameTime);
    }
}

// EVENTS
void Game::handleEvents(SDL_Event event) {
    while (SDL_PollEvent(&event)) switch (event.type) {
        case SDL_QUIT: quit(); break;

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
    map->update();
}

// RENDER
void Game::render() {
    map->render();
}

// STATIC
void Game::delay(int ms, std::function<void()> callback) {
    std::thread([ms, callback]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
        callback();
    }).detach();
}

// GETTER
H2DE_Engine* Game::getEngine() const {
    return engine;
}

int Game::getFPS() const {
    return FPS;
}

GameData* Game::getData() const {
    return data;
}

GameState Game::getState() const {
    return state;
}

Calculator* Game::getCalculator() const {
    return calculator;
}

Save* Game::getSave() const {
    return save;
}

Camera* Game::getCamera() const {
    return camera;
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

void Game::getMousePos(int* x, int* y) const {
    *x = mousePos.x;
    *y = mousePos.y;
}

bool Game::getDebugMode() const {
    return debugMode;
}

// SETTER
void Game::setState(GameState s) {
    state = s;
}
