#include <SDL_events.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <SDL.h>

using namespace std;

#define FOOTER_HEIGHT  49
#define SCREEN_HEIGHT  800
#define SCREEN_WIDTH   1600
#define COLOR_BLACK    0x00000000
#define COLOR_WHITE    0xffffffff
#define COLOR_YELLOW   0x00F6ED36
#define COLOR_PURPLE   0x00ab85ea
#define COLOR_ORANGE   0x00fb5607
#define COLOR_GREEN    0x00588157
#define COLOR_RED      0x00ff0000

int Anim = 0;
int AnimSpeed = 20;
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
typedef struct PolyEq {
    long double d0 = 0;
    long double d1 = 0;
    long double d2 = 0;
    long double d3 = 0;
    long double d4 = 0;
    long double d5 = 0;
    Uint32 color = COLOR_RED;
}PolyEq;
typedef struct TrigEq {
    int FuncType = 0;
    long double CoeffA = 10;
    long double CoeffX = 0.1;
    long double CoeffPhi = 0;
    Uint32 color = COLOR_RED;
}TrigEq;

struct TrigEq {
    long double CoeffA = 1;
    long double CoeffX = 1;
    long double CoeffPhi = 0;
	int FuncType = 0;
};

static void PaintPixel(SDL_Surface* surface, int x, int y, Uint32 color, int a = 1) {
    SDL_Rect pixel = SDL_Rect({ x, y, a, a });
    SDL_FillRect(surface, &pixel, color);

static void inline PaintPixel(SDL_Surface* surface, int x, int y, Uint32 color, int a = 1) {
    SDL_Rect pixel = SDL_Rect({ x, y, a, a });
    SDL_FillRect(surface, &pixel, color);
}
void DrawEquation(SDL_Window* window, SDL_Surface* surface, Coordinate* origin, PolyEq* equation, Uint32 color) {
    float Res = 15;
    if (SCALE_FACTOR > 1) Res = 15 * SCALE_FACTOR;
    else Res = 15;
    for (float x = (-origin->x) * Res / SCALE_FACTOR; x <= (SCREEN_WIDTH - origin->x) * Res / SCALE_FACTOR; x++) {
        float a = x / Res;
        long double y = 0;
        y = (
            pow(a, 0) * equation->d0 +
            pow(a, 1) * equation->d1 +
            pow(a, 2) * equation->d2 +
            pow(a, 3) * equation->d3 +
            pow(a, 4) * equation->d4 +
            pow(a, 5) * equation->d5) * SCALE_FACTOR + origin->y;
        PaintPixel(surface, int(a * SCALE_FACTOR + origin->x), SCREEN_HEIGHT - y, color);
    }
}
void DrawEquation(SDL_Window* window, SDL_Surface* surface, Coordinate* origin, TrigEq* equation, Uint32 color) {
    float Res = 15 * SCALE_FACTOR;
    for (float x = (-origin->x) * Res / SCALE_FACTOR; x <= (SCREEN_WIDTH - origin->x) * Res / SCALE_FACTOR; x++) {
        float a = x / Res;
        long double y = 0;
        switch (equation->FuncType) {
        case 0: y = equation->CoeffA * sin(equation->CoeffX * a + equation->CoeffPhi) * SCALE_FACTOR + origin->y; break;
        case 1: y = equation->CoeffA * cos(equation->CoeffX * a + equation->CoeffPhi) * SCALE_FACTOR + origin->y; break;
        case 2: y = equation->CoeffA * tan(equation->CoeffX * a + equation->CoeffPhi) * SCALE_FACTOR + origin->y; break;
        case 3: y = equation->CoeffA * asin(equation->CoeffX * a + equation->CoeffPhi) * SCALE_FACTOR + origin->y; break;
        case 4: y = equation->CoeffA * acos(equation->CoeffX * a + equation->CoeffPhi) * SCALE_FACTOR + origin->y; break;
        case 5: y = equation->CoeffA * atan(equation->CoeffX * a + equation->CoeffPhi) * SCALE_FACTOR + origin->y; break;
        default: break;
        }
        PaintPixel(surface, int(a * SCALE_FACTOR + origin->x), SCREEN_HEIGHT - y, color);
    }
}
void DrawOrdinateLine(SDL_Window* window, SDL_Surface* surface, Coordinate* origin, Uint32 color) {
    float Res = 15;
    int pp = 150;

    SDL_Rect line = { (int)origin->x,0,1,SCREEN_HEIGHT };
    SDL_FillRect(surface, &line, color);
    line = { (int)origin->x - 1,0,1,SCREEN_HEIGHT };
    SDL_FillRect(surface, &line, color);
    line = { (int)origin->x + 1,0,1,SCREEN_HEIGHT };
    SDL_FillRect(surface, &line, color);
    line = { 0,int(SCREEN_HEIGHT - origin->y),SCREEN_WIDTH,1 };
    SDL_FillRect(surface, &line, color);
    line = { 0,int(SCREEN_HEIGHT - origin->y - 1),SCREEN_WIDTH,1 };
    SDL_FillRect(surface, &line, color);
    line = { 0,int(SCREEN_HEIGHT - origin->y + 1),SCREEN_WIDTH,1 };
    SDL_FillRect(surface, &line, color);

    Uint32 COLOR_GREY = 0x00606060;


    if (SCALE_FACTOR >= 1) {
        for (float x = 0; x >= (-origin->x) * SCALE_FACTOR; x -= pp * SCALE_FACTOR) {
            SDL_Rect line = { x + origin->x,0,1,SCREEN_HEIGHT };
            SDL_FillRect(surface, &line, COLOR_GREY);
        }
        for (float x = 0; x <= (SCREEN_WIDTH - origin->x) * SCALE_FACTOR; x += pp * SCALE_FACTOR) {
            SDL_Rect line = { x + origin->x,0,1,SCREEN_HEIGHT };
            SDL_FillRect(surface, &line, COLOR_GREY);
        }
        //for (float y = (SCREEN_HEIGHT) * SCALE_FACTOR - origin->y; y >= 0; y -= pp * SCALE_FACTOR) {
        //    SDL_Rect line = { 0,y,SCREEN_WIDTH,1 };
        //    SDL_FillRect(surface, &line, 0xbbbbbbbb);
        //    //cout << y << endl;
        //}

    }
    else {
        for (float x = 0; x >= (-origin->x) / SCALE_FACTOR; x -= pp * SCALE_FACTOR) {
            SDL_Rect line = { x + origin->x,0,1,SCREEN_HEIGHT };
            SDL_FillRect(surface, &line, COLOR_GREY);
        }
        for (float x = 0; x <= (SCREEN_WIDTH - origin->x) / SCALE_FACTOR; x += pp * SCALE_FACTOR) {
            SDL_Rect line = { x + origin->x,0,1,SCREEN_HEIGHT };
            SDL_FillRect(surface, &line, COLOR_GREY);
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
void ClearScreen(SDL_Window* window, SDL_Surface* surface, Coordinate* origin, Uint32 color) {

void DrawCoordinatesAndFooterLine(SDL_Surface* surface, Coordinate* origin, Uint32 color) {
    for (float x = (-origin->x); x <= (SCREEN_WIDTH - origin->x); x += 1) {
        PaintPixel(surface, int(x + origin->x), SCREEN_HEIGHT - origin->y, color);
    }
    for (float y = (-origin->y); y <= (SCREEN_WIDTH - origin->y); y += 1) {
        PaintPixel(surface, origin->x, int(y + origin->y), color);
    }
    if (Anim) SDL_Delay(5000);

    for (int x = 0; x <= SCREEN_WIDTH; x += 1) {
        SDL_Rect scanline = SDL_Rect({ x, 0, 1, SCREEN_HEIGHT });
        SDL_FillRect(surface, &scanline, color);
        if (Anim && int(x) % AnimSpeed == 0) SDL_UpdateWindowSurface(window);
    }
}

void DrawFrame(SDL_Window* window, SDL_Surface* surface, Coordinate* origin,
    vector <TrigEq>* TrigEqs, vector <PolyEq>* PolyEqs) {

    for (PolyEq eq : *PolyEqs) {
        DrawEquation(window, surface, &*origin, &eq, eq.color);
    }
    for (TrigEq eq : *TrigEqs) {
        DrawEquation(window, surface, &*origin, &eq, eq.color);
    }


    SDL_UpdateWindowSurface(window);
    ClearScreen(window, surface, origin, COLOR_BLACK);
    DrawOrdinateLine(window, surface, origin, COLOR_WHITE);

}

static void ZoomAtMouse(SDL_Window* window, SDL_Surface* surface, SDL_Event* event,
    Coordinate* origin, vector <TrigEq>* TrigEqs, vector <PolyEq>* PolyEqs) {

    float c = 0.2 + SCALE_FACTOR * 0.1;
    if (event->wheel.y > 0) {
        if (SCALE_FACTOR > 200) SCALE_FACTOR = 200;
        else {
            for (int z = 0; z <= 100; z++) {
                SCALE_FACTOR += c / 100;
                if (z % 10 == 0) DrawFrame(window, surface, origin, TrigEqs, PolyEqs);
            }
        }
    }
    else if (event->wheel.y < 0) {
        if (SCALE_FACTOR > 0.9) {
            for (int z = 0; z <= 100; z++) {
                SCALE_FACTOR -= c / 100;
                if (z % 10 == 0) DrawFrame(window, surface, origin, TrigEqs, PolyEqs);
            }
        }
        else {
            if (SCALE_FACTOR > 0.1) SCALE_FACTOR -= 0.02;
            else {
                while (SCALE_FACTOR < 0.1) {
                    SCALE_FACTOR += 0.01;
                    DrawFrame(window, surface, origin, TrigEqs, PolyEqs);
                }
            }
        }
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
    SDL_Delay(1000);
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Rect blackScreen = SDL_Rect({ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT });
    SDL_Window* window = SDL_CreateWindow("Graphing Calculator", 0, 30, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    SDL_Surface* surface = SDL_GetWindowSurface(window);
    int runSim = 1;
    SDL_Event event;

    struct Coordinate origin;

    vector <TrigEq> TrigEqs;
    vector <PolyEq> PolyEqs;

    TrigEq Teq1;
    Teq1.CoeffA = 150;
    Teq1.CoeffX = static_cast<long double>(1) / 150;
    Teq1.FuncType = 0;
    Teq1.color = COLOR_PURPLE;
    TrigEqs.push_back(Teq1);

    struct TrigEq Teq2;
    Teq2.CoeffA = 150;
    Teq2.CoeffX = static_cast<long double>(1) / 150;
    Teq2.FuncType = 1;
    Teq2.color = COLOR_ORANGE;
    TrigEqs.push_back(Teq2);

    struct PolyEq PolyEq1;
    PolyEq1.d1 = 1;
    PolyEq1.color = COLOR_YELLOW;
    PolyEqs.push_back(PolyEq1);

    struct PolyEq PolyEq2;
    PolyEq2.d2 = -0.1;
    PolyEq2.d1 = 10;
    PolyEq2.color = COLOR_GREEN;
    PolyEqs.push_back(PolyEq2);

    while (runSim) {
        DrawFrame(window, surface, &origin, &TrigEqs, &PolyEqs);
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) runSim = 0;
            if (event.type == SDL_MOUSEWHEEL) ZoomAtMouse(window, surface, &event, &origin, &TrigEqs, &PolyEqs);
            if (event.type == SDL_MOUSEMOTION && event.motion.state == SDL_PRESSED) {
                origin.x += event.motion.xrel;
                origin.y -= event.motion.yrel;
            }
        }
    }
    return 0;
}