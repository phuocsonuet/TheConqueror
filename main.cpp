#include <iostream>
#include <vector>
// SDL 2
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

using namespace std;

SDL_Window* win = SDL_CreateWindow("The Conqueror", 50, 50, 400, 640, 0);
SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
SDL_Event event;

SDL_Texture* base = IMG_LoadTexture(ren, "assets/base.png");
SDL_Texture* block = IMG_LoadTexture(ren, "assets/block.png");
SDL_Texture* sign = IMG_LoadTexture(ren, "assets/sign.png");
SDL_Texture* wall = IMG_LoadTexture(ren, "assets/wall.png");
SDL_Texture* block_icon = IMG_LoadTexture(ren, "assets/block-icon.png");
SDL_Texture* balloon = IMG_LoadTexture(ren, "assets/balloon.png");
class Block {
public:
	float bx, by, bvy, bay, b;
	Block(int x, int y, int base) {
		bx = x;
		by = y;
		bay = 0.4;
		bvy = 0;
		b = base;
	}
	void anchor(int x, int y) {
		bx = x;
		by = y;
	}

	void update(int x, int y) {

	}

	void drop() {
		bvy += bay;
		by += bvy;
	}

	bool collideWithBlock(Block block) {
		if (bx <= block.bx + 64 && bx + 64 >= block.bx) {

			if (by + 64 >= block.by) {
				by = block.by - 64;
				bvy = 0;
				bay = 0;
				return true;
			}
		}
		return false;
	}

	bool collideWithGround(int camera) {
		if (by + 64 >= (520 + camera)) {
			bvy = 0;
			bay = 0;
			by = (520 + camera) - 64;

			return true;
		}
		return false;
	}

	void moveCamera(int cm) {
		by = by + cm;
	}



	void draw() {
		SDL_SetRenderDrawColor(ren, 251, 133, 0, 255);
		SDL_Rect rect = { (int)bx - 32, (int)by, 64, 64 };
		if (b == 1) {
			SDL_RenderCopy(ren, base, NULL, &rect);
		}
		else {
			SDL_RenderCopy(ren, block, NULL, &rect);
		}
	}
};

void drawText(string text, int x, int y, TTF_Font* font) {
	SDL_Color color = { 255, 255, 255 };
	SDL_Surface* tmp = TTF_RenderText_Blended(font, text.c_str(), color);
	SDL_Rect geo = { x, y, tmp->w, tmp->h };
	SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, tmp);
	SDL_FreeSurface(tmp);
	SDL_RenderCopy(ren, texture, NULL, &geo);
}


int main(int argc, char* argv[]) {
	IMG_Init(IMG_INIT_PNG);
	TTF_Init();
	TTF_Font* poppins = TTF_OpenFont("fonts/poppins.ttf", 16);
	if (poppins) {
		cout << "Font Loaded Successfully" << endl;
	}
	else {
		cout << "Error on loading Fonts. Error : " << TTF_GetError() << endl;
	}
	bool isRunning = true;
	int t = 0;
	float cx = 320, cy = 100;
	vector<Block> blocks;
	blocks.push_back(Block(cx, cy, 1));
	float angle = 1;
	float angleA = 0;
	float angleV = 0;
	float gravity = 0.0004;
	int base = 1;
	int current = 0;
	int camera = 0;
	bool droping = false;
	float balloonx = -100;
	while (isRunning) {
		SDL_SetRenderDrawColor(ren, 33, 158, 188, 255);
		SDL_RenderClear(ren);
		// Rope
		float force = gravity * sin(angle);
		angleA = force * -1;
		angleV += angleA;
		angle += angleV;
		cx = 130 * sin(angle) + 200;
		cy = 130 * cos(angle);
		blocks[current].anchor(cx, cy);
		if (blocks[0].collideWithGround(camera)) {
			if (blocks.size() <= 2) droping = false;
		}
		if (base == 0) blocks[0].drop();
		for (unsigned i = 1; i < blocks.size() - 1; i++) {
			if (i > 0) {
				if (blocks[i].collideWithBlock(blocks[i - 1])) {
					droping = false;
				}
			}
			blocks[i].drop();
			if (blocks[i].collideWithGround(camera)) {
				blocks.erase(blocks.begin() + i);
				current = blocks.size() - 1;
				droping = false;
				if (blocks.size() >= 4) {
					camera -= 64;
				}
			}
		}
		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				isRunning = false;
			}
			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {

				case SDLK_RETURN:
					if (droping == false) {
						if (blocks.size() >= 4)
						{
							camera += 64;
							for (unsigned i = 0; i < blocks.size() - 1; i++) {
								blocks[i].moveCamera(64);
							}
						}
						blocks.push_back(Block(cx, cy, 0));
						current = blocks.size() - 1;
						base = 0;
						droping = true;
					}
					break;
				}
			}
		}
		if (blocks.size() >= 12) 
			balloonx += 0.2;
		SDL_FRect brect = { balloonx, (100 + (float)camera) - 768, 100, 134 };
		SDL_RenderCopyF(ren, balloon, NULL, &brect);
		SDL_SetRenderDrawColor(ren, 43, 147, 72, 255);
		SDL_Rect rect = { 0, 540 + camera, 400, 100 };
		SDL_RenderFillRect(ren, &rect);
		SDL_Rect wrect = { 0, 540 + camera - 118, 400, 118 };
		SDL_RenderCopy(ren, wall, NULL, &wrect);
		SDL_SetRenderDrawColor(ren, 200, 200, 200, 255);
		SDL_Rect rect2 = { 20, 520 + camera, 360, 20 };
		SDL_RenderFillRect(ren, &rect2);
		SDL_SetRenderDrawColor(ren, 2, 48, 71, 255);
		SDL_RenderDrawLine(ren, 200, 0, cx, cy);
		for (unsigned i = 0; i < blocks.size(); i++) 
			blocks[i].draw();
		SDL_Rect srect = { 260, 640 + camera - 157, 128, 157 };
		SDL_RenderCopy(ren, sign, NULL, &srect);
		SDL_Rect birect = { 10, 10, 16, 16 };
		SDL_RenderCopy(ren, block_icon, NULL, &birect);
		drawText(to_string(blocks.size() - 1), 32, 8, poppins);
		SDL_Delay(1000 / 60);
		SDL_RenderPresent(ren);
		t += 1;
	}
	return 0;
}
