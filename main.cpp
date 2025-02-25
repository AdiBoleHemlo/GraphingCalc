#include <SDL_events.h>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <SDL.h>

using namespace std;

#define SCREEN_WIDTH 1500
#define SCREEN_HEIGHT 550
#define FOOTER_HEIGHT 49
#define COLOR_BLACK 0x00000000
#define COLOR_WHITE 0xffffffff
#define COLOR_YELLOW 0x00F6ED36
#define COLOR_PURPLE 0x00ab85ea

float SCALE_FACTOR = 1;

struct Coordinate {
    long double x = SCREEN_WIDTH / 2;
    long double y = SCREEN_HEIGHT / 2;
};

struct PolyEquation {
    long double d0 = 0;
    long double d1 = 0;
    long double d2 = 0;
    long double d3 = 0;
    long double d4 = 0;
    long double d5 = 0;
};

struct TrigEq {
    long double CoeffA = 1;
    long double CoeffX = 1;
    long double CoeffPhi = 0;
	int FuncType = 0;
};

static void PaintPixel(SDL_Surface* surface, int x, int y, Uint32 color, int a = 1) {
    SDL_Rect pixel = SDL_Rect({ x, y, a, a });
    SDL_FillRect(surface, &pixel, color);
}

void DrawEquation(SDL_Surface* surface, Coordinate* origin, PolyEquation* equation, Uint32 color) {
    float xOld = 0;
    long double yOld = 0;
    for (float x = (-origin->x) / SCALE_FACTOR; x <= (SCREEN_WIDTH - origin->x) / SCALE_FACTOR;) {
        long double y = (
            pow(x, 0) * equation->d0 +
            pow(x, 1) * equation->d1 +
            pow(x, 2) * equation->d2 +
            pow(x, 3) * equation->d3 +
            pow(x, 4) * equation->d4 +
            pow(x, 5) * equation->d5) * SCALE_FACTOR + origin->y;
        PaintPixel(surface, int(x * SCALE_FACTOR + origin->x), SCREEN_HEIGHT - y, color);
        if (SCALE_FACTOR <= 12) x += 0.1;
        else x += 0.01;
    }
}
void DrawEquation(SDL_Surface* surface, Coordinate* origin, TrigEq* equation, Uint32 color) {
    float xOld = 0;
    long double yOld = 0;
    for (float x = (-origin->x) / SCALE_FACTOR; x <= (SCREEN_WIDTH - origin->x) / SCALE_FACTOR;) {
		long double y = 0;
        switch (equation->FuncType) {
		case 0:
			y = equation->CoeffA * sin(equation->CoeffX * x + equation->CoeffPhi) * SCALE_FACTOR + origin->y;
			break;
		case 1:
			y = equation->CoeffA * cos(equation->CoeffX * x + equation->CoeffPhi) * SCALE_FACTOR + origin->y;
			break;
		case 2:
			y = equation->CoeffA * tan(equation->CoeffX * x + equation->CoeffPhi) * SCALE_FACTOR + origin->y;
			break;
		case 3:
            y = equation->CoeffA * asin(equation->CoeffX * x + equation->CoeffPhi) * SCALE_FACTOR + origin->y;
			break;
		case 4:
			y = equation->CoeffA * acos(equation->CoeffX * x + equation->CoeffPhi) * SCALE_FACTOR + origin->y;
			break;
		case 5:
			y = equation->CoeffA * atan(equation->CoeffX * x + equation->CoeffPhi) * SCALE_FACTOR + origin->y;
			break;
        default:
            break;
        }
        PaintPixel(surface, int(x * SCALE_FACTOR + origin->x), SCREEN_HEIGHT - y, color);
        if (SCALE_FACTOR <= 12) x += 0.1;
        else x += 0.01;
    }
}

void DrawCoordinatesAndFooterLine(SDL_Surface* surface, Coordinate* origin, Uint32 color) {
    for (float x = (-origin->x); x <= (SCREEN_WIDTH - origin->x); x += 1) {
        PaintPixel(surface, int(x + origin->x), SCREEN_HEIGHT - origin->y, color);
    }
    for (float y = (-origin->y); y <= (SCREEN_WIDTH - origin->y); y += 1) {
        PaintPixel(surface, origin->x, int(y + origin->y), color);
    }
}


static void ZoomAtMouse(SDL_Event * event, Coordinate * origin) {
    float c = 0.5;
    float d = 75;
	if (event->wheel.y > 0) {
		if (SCALE_FACTOR > 12) SCALE_FACTOR = 12;
        else {
		    SCALE_FACTOR += c;
            if (event->wheel.mouseX < origin->x) origin->x += event->wheel.mouseX * d / (SCREEN_WIDTH / SCALE_FACTOR);
            else origin->x -= event->wheel.mouseX * d / (SCREEN_WIDTH / SCALE_FACTOR);
            if (event->wheel.mouseY < origin->y) origin->y -= event->wheel.mouseY * d / (SCREEN_HEIGHT / SCALE_FACTOR);
            else origin->y += event->wheel.mouseY * d / (SCREEN_HEIGHT / SCALE_FACTOR);
        }
    }
	else if (event->wheel.y < 0) {
        if (SCALE_FACTOR > 0.9) {
            SCALE_FACTOR -= c;
            if (event->wheel.mouseX < origin->x) origin->x -= event->wheel.mouseX * d / (SCREEN_WIDTH / SCALE_FACTOR);
            else origin->x += event->wheel.mouseX * d / (SCREEN_WIDTH / SCALE_FACTOR);
            if ((SCREEN_HEIGHT - event->wheel.mouseY - origin->y) < origin->y) {
                origin->y -= event->wheel.mouseY * d / (SCREEN_HEIGHT / SCALE_FACTOR);
            }
            else origin->y -= event->wheel.mouseY * d / (SCREEN_HEIGHT / SCALE_FACTOR);
		}
        else {
			if (SCALE_FACTOR > 0.1) SCALE_FACTOR -= 0.01;
            else SCALE_FACTOR = 0.1;
        }
	}
}



int main(int argc, char* args[]) {
    struct PolyEquation equation0;
    equation0.d0 = 0;
    equation0.d1 = 5;
    equation0.d2 = -1;



	struct TrigEq equation1;
	equation1.CoeffA = 100;
	equation1.CoeffX = 0.1;
	equation1.CoeffPhi = 0;
	equation1.FuncType = 0;

    struct Coordinate origin;

    SDL_Delay(1000);
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Rect blackScreen = SDL_Rect({ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT });
    SDL_Window* window = SDL_CreateWindow("Graphing Calculator", 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Surface* surface = SDL_GetWindowSurface(window);

    int runSim = 1;
    SDL_Event event;
    while (runSim) {
        SDL_FillRect(surface, &blackScreen, COLOR_BLACK);
        DrawCoordinatesAndFooterLine(surface, &origin, COLOR_WHITE);



        DrawEquation(surface, &origin, &equation1, COLOR_YELLOW);
        DrawEquation(surface, &origin, &equation0, COLOR_PURPLE);





        SDL_UpdateWindowSurface(window);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                runSim = 0;
            }
            if (event.type == SDL_MOUSEMOTION && event.motion.state == SDL_PRESSED) {
                origin.x += event.motion.xrel;
                origin.y -= event.motion.yrel;
            }
            if (event.type == SDL_MOUSEWHEEL) {
                ZoomAtMouse(&event, &origin);
            }

        }
    }
    return 0;
}
