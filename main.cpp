#include <SDL_events.h>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <SDL.h>
using namespace std;

#define stepValue long double(0.001)
#define SCREEN_WIDTH  int(1000)
#define SCREEN_HEIGHT int(550)
#define FOOTER_HEIGHT int(49)

#define COLOR_BLACK  0x00000000
#define COLOR_WHITE  0xffffffff
#define COLOR_YELLOW 0x00F6ED36
#define COLOR_PURPLE 0x00ab85ea

float SCALE_FACTOR = 1;


struct Coordinate {
	long double x = 0;
	long double y = 51;
};

struct Equation {
	long double d0 = 0;
	long double d1 = 0.01;
	long double d2 = 0.002;
	long double d3 = -0.001;
	long double d4 = 0.00001;
	long double d5 = 0;
};

static void PaintPixel(SDL_Surface* surface, int x, int y, Uint32 color, int a = 1) {
	SDL_Rect pixel = SDL_Rect({ x,y,a,a });
	SDL_FillRect(surface, &pixel, color);
}

void DrawEquation (SDL_Surface* surface,Coordinate* origin, Equation* equation, Uint32 color) {
	float xOld = 0;
	long double yOld = 0;
	for (float x = (-origin->x) / SCALE_FACTOR;
		x <= (SCREEN_WIDTH - origin->x) / SCALE_FACTOR;) {
		long double y = (
			pow(x, 0) * equation->d0 +
			pow(x, 1) * equation->d1 +
			pow(x, 2) * equation->d2 +
			pow(x, 3) * equation->d3 +
			pow(x, 4) * equation->d4 +
			pow(x, 5) * equation->d5)*SCALE_FACTOR + origin->y;
		PaintPixel(surface, int(x*SCALE_FACTOR + origin->x), SCREEN_HEIGHT - y, color);
		if (SCALE_FACTOR <= 12) x += 0.1;
		else x += 0.01;
	}
}

void DrawOordinatesAndFooterLine (SDL_Surface* surface,Coordinate* origin, Uint32 color) {
	for (float x = (-origin->x); x <= (SCREEN_WIDTH - origin->x); x += 1){
		PaintPixel(surface, int(x + origin->x), SCREEN_HEIGHT - origin->y, color);
	}
	for (float y = (-origin->y); y <= (SCREEN_WIDTH - origin->y); y += 1){
		PaintPixel(surface, origin->x, int(y + origin->y), color);
	}
}

int main(int argc, char* args[]) {
	struct Equation equation1;
	struct Coordinate origin;

	SDL_Delay(1000);
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Rect blackScreen = SDL_Rect({ 0,0, SCREEN_WIDTH, SCREEN_HEIGHT });
	SDL_Window* window = SDL_CreateWindow("Graphing Calculator", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	SDL_Surface* surface = SDL_GetWindowSurface(window);

	int runSim = 1;
	SDL_Event event;
	while (runSim) {
		DrawOordinatesAndFooterLine(surface, &origin, COLOR_WHITE);
		DrawEquation(surface, &origin, &equation1, COLOR_PURPLE);
		PaintPixel(surface, origin.x,SCREEN_HEIGHT - origin.y, COLOR_YELLOW);
		SDL_UpdateWindowSurface(window);
		SDL_FillRect(surface, &blackScreen, COLOR_BLACK);
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) runSim = 0;
			if (event.wheel.preciseY >= 0.1 && event.wheel.preciseY > 0) SCALE_FACTOR += 0.1;;
			if (event.wheel.preciseY <= 0.0 && event.wheel.preciseY < 0 && SCALE_FACTOR > 0.2) SCALE_FACTOR -= 0.1;
			if (event.motion.state == SDL_PRESSED) {
				origin.x += event.motion.xrel;
				origin.y -= event.motion.yrel;
			}
		}
	} return 0;
}