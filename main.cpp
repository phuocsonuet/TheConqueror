#include <iostream>
#include <SDL.h>

using namespace std;

const int screen_width = 640;
const int screen_height = 480;

SDL_Window* gWindow = nullptr;
SDL_Surface* gScreenSurface = nullptr;

bool init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		cout << "Error: \n" << SDL_GetError;
		return false;
	}
	gWindow = SDL_CreateWindow("The Conqueror game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_SHOWN);
	if (gWindow == nullptr) {
		cout << "Error: \n" << SDL_GetError;
		return false;
	}
	gScreenSurface = SDL_GetWindowSurface(gWindow);
	SDL_FillRect(gScreenSurface, nullptr, SDL_MapRGB(gScreenSurface->format, 0xFF, 0xFF, 0xFF));
	SDL_UpdateWindowSurface(gWindow);
	return true;
}

void close() {
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	SDL_Quit();
}

int main(int argc, char* args[]) {
	if (!init()) {
		cout << "Failed";
	}
	else {
		SDL_Event e;
		bool quit = false;
		while (!quit) {
			while (SDL_PollEvent((&e))) {
				if (e.type == SDL_QUIT) {
					quit = true;
				}
			}
		}
	}
	close();
	return 0;
}
