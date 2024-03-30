#include <iostream>
#include <SDL_image.h>
#include <SDL_mixer.h>

using namespace std;

bool initializeSDL();
void cleanupSDL();
int displayMenu();
int displayInstruction();
int exitGame();
int startGame();

Game* gameInstance;
MainScreen* mainScreenInstance;
Instruct* instructionInstance;

void LoadData() {
}

enum MenuOption {
	PLAY,
	INSTRUCTION,
	EXIT_GAME,
	DISPLAY_MENU
};

int main(int argc, char* argv[]) {
	initializeSDL();

	int menuOption = DISPLAY_MENU;
	while (menuOption != EXIT_GAME) {
		gameInstance = new Game();
		switch (menuOption) {
			case DISPLAY_MENU:
				menuOption = displayMenu();
				break;
			case INSTRUCTION:
				menuOption = displayInstructions();
				break;
			case EXIT_GAME:
				menuOption = exitGame();
				break;
			case PLAY:
				menuOption = startGame();
				break;
			default:
				break;
		}
		gameInstance->cleanUp();
		delete gameInstance;
	}
	cleanupSDL();
	return 0;
}

bool initializeSDL() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		cerr << "Failed to initialize SDL. Error: " << SDL.GetError() << endl;
		return false;
	}
	else {

	}
	LoadData();
	return true;
}