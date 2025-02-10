CXX = g++
APP_NAME = Unnamed_Game_1.exe
CPP_VERSION = c++17

BIN_DIR = bin
OBJECT_DIR = $(BIN_DIR)\objects
INCLUDE_DIR = include
LIB_DIR = lib
SRC_DIR = src

CXX_FLAGS = -std=$(CPP_VERSION) -m64 -I$(INCLUDE_DIR) -I$(INCLUDE_DIR)/game -I$(INCLUDE_DIR)/H2DE
SDL_FLAGS = -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_gfx
LD_FLAGS = -L$(LIB_DIR) -lmingw32 $(SDL_FLAGS) -lbase64 -linih

SRC = $(wildcard $(SRC_DIR)/**/*.cpp) $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJECT_DIR)/%.o, $(SRC))

all:
	make app -j
	make run

app: $(OBJ)
	$(CXX) $(CXX_FLAGS) -o $(BIN_DIR)/$(APP_NAME) $^ $(LD_FLAGS) -lH2DE

$(OBJECT_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@

data:
	del /q $(OBJECT_DIR)\game.o 2>nul || true
	$(MAKE)

run:
	chcp 65001
	cd $(BIN_DIR) && $(APP_NAME)

clean:
	for /r $(OBJECT_DIR) %%f in (*.o) do del /q "%%f"
	del /q $(BIN_DIR)\$(APP_NAME) 2>nul || true

rebuild: clean all
