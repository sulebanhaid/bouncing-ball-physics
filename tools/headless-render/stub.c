#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include "fontdata.h"

static int W, H, frame = 0;
static unsigned char *fb;
static unsigned char font[128*128];
static int recX[224], recY[224];

/* ---- scripted input, chosen by SCENE env var ---- */
typedef struct { int frame; int kind; float x, y; int key; } Ev; /* kind 0=left click,1=right click,2=key press */
static int scene; static int lastFrame; static int shots[8]; static int nshots;
static Ev evs[256]; static int nev;
static int holdKey = -1, holdFrom = 0, holdTo = 0;

static void addClick(int f, float x, float y) { evs[nev++] = (Ev){f,0,x,y,0}; }
static void addKey(int f, int k) { evs[nev++] = (Ev){f,2,0,0,k}; }

static void setupScene(void) {
    srand(7 + scene);
    if (scene == 1) {          /* lots of balls mid-bounce */
        for (int i = 0; i < 45; i++) addClick(10 + i*4, 120 + (i*137)%560, 210 + (i*53)%160);
        lastFrame = getenv("GIF") ? 420 : 260; shots[nshots++] = 150;
    } else if (scene == 2) {   /* balls settled into a pile, low bounce, then paused */
        for (int i = 0; i < 70; i++) addClick(10 + i*3, 100 + (i*97)%600, 200 + (i*31)%120);
        holdKey = KEY_LEFT; holdFrom = 20; holdTo = 50;      /* bounce 0.80 -> 0.50 */
        addKey(560, KEY_SPACE);
        lastFrame = 575; shots[nshots++] = 570;
    } else {                   /* high gravity and full bounce */
        for (int i = 0; i < 30; i++) addClick(10 + i*5, 150 + (i*173)%500, 220 + (i*41)%140);
        holdKey = KEY_UP; holdFrom = 5; holdTo = 35;          /* gravity 0.30 -> 0.60 */
        evs[nev++] = (Ev){40,3,0,0,0};                         /* marker, unused */
        lastFrame = 230; shots[nshots++] = 95;
    }
}

static int pressedThisFrame(int kind, int key, Ev **out) {
    for (int i = 0; i < nev; i++)
        if (evs[i].frame == frame && evs[i].kind == kind && (kind != 2 || evs[i].key == key)) { if (out) *out = &evs[i]; return 1; }
    return 0;
}
static Vector2 mouse = {0,0};
bool IsMouseButtonPressed(int b) { Ev *e; if (pressedThisFrame(b == MOUSE_BUTTON_LEFT ? 0 : 1, 0, &e)) { mouse = (Vector2){e->x, e->y}; return true; } return false; }
Vector2 GetMousePosition(void) { return mouse; }
bool IsKeyPressed(int k) { return pressedThisFrame(2, k, NULL); }
bool IsKeyDown(int k) {
    return k == holdKey && frame >= holdFrom && frame < holdTo;
}

/* ---- window / drawing ---- */
void InitWindow(int w, int h, const char *t) {
    (void)t; W = w; H = h; fb = calloc(w*h*3, 1);
    char *s = getenv("SCENE"); scene = s ? atoi(s) : 1; setupScene();
    for (int i = 0, c = 0; i < 128*128; i += 32, c++)
        for (int j = 31; j >= 0; j--) font[i+j] = (defaultFontData[c] >> j) & 1;
    int line = 0, cur = 1, test = 1;
    for (int i = 0; i < 224; i++) {
        recX[i] = cur; recY[i] = 1 + line*11; test += charsWidth[i] + 1;
        if (test >= 128) { line++; cur = 2 + charsWidth[i]; test = cur; recX[i] = 1; recY[i] = 1 + line*11; }
        else cur = test;
    }
}
void SetTargetFPS(int f) { (void)f; }
bool WindowShouldClose(void) { return frame > lastFrame; }
void CloseWindow(void) {}
void BeginDrawing(void) {}
void EndDrawing(void) {
    for (int i = 0; i < nshots; i++) if (shots[i] == frame) {
        char name[64]; snprintf(name, sizeof name, "scene%d.ppm", scene);
        FILE *f = fopen(name, "wb"); fprintf(f, "P6\n%d %d\n255\n", W, H); fwrite(fb, 1, W*H*3, f); fclose(f);
    }
    if (getenv("GIF") && frame % 2 == 0 && frame >= 5) { char n[64]; snprintf(n, sizeof n, "gif/f%04d.ppm", frame/2); FILE *g = fopen(n, "wb"); fprintf(g, "P6\n%d %d\n255\n", W, H); fwrite(fb, 1, W*H*3, g); fclose(g); }
    frame++;
}
static void px(int x, int y, Color c) {
    if (x < 0 || y < 0 || x >= W || y >= H) return;
    unsigned char *p = fb + (y*W + x)*3; float a = c.a/255.0f;
    p[0] = c.r*a + p[0]*(1-a); p[1] = c.g*a + p[1]*(1-a); p[2] = c.b*a + p[2]*(1-a);
}
void ClearBackground(Color c) { for (int i = 0; i < W*H; i++) { fb[i*3]=c.r; fb[i*3+1]=c.g; fb[i*3+2]=c.b; } }
void DrawCircleV(Vector2 c, float r, Color col) {
    for (int y = (int)(c.y - r) - 1; y <= (int)(c.y + r) + 1; y++)
        for (int x = (int)(c.x - r) - 1; x <= (int)(c.x + r) + 1; x++) {
            float dx = x + 0.5f - c.x, dy = y + 0.5f - c.y;
            if (dx*dx + dy*dy <= r*r) px(x, y, col);
        }
}
void DrawText(const char *t, int x, int y, int size, Color col) {
    if (size < 10) size = 10; int scale = size/10, spacing = size/10; float ox = 0;
    for (; *t; t++) {
        int g = (unsigned char)*t - 32; if (g < 0 || g >= 224) g = '?' - 32;
        int w = charsWidth[g];
        for (int gy = 0; gy < 10; gy++) for (int gx = 0; gx < w; gx++)
            if (font[(recY[g]+gy)*128 + recX[g]+gx])
                for (int sy = 0; sy < scale; sy++) for (int sx = 0; sx < scale; sx++)
                    px(x + (int)ox + gx*scale + sx, y + gy*scale + sy, col);
        ox += w*scale + spacing;
    }
}
const char *TextFormat(const char *fmt, ...) { static char buf[4][256]; static int i; i = (i+1)%4; va_list a; va_start(a, fmt); vsnprintf(buf[i], 256, fmt, a); va_end(a); return buf[i]; }
int GetRandomValue(int min, int max) { if (min > max) { int t = max; max = min; min = t; } return rand()%(abs(max-min)+1) + min; }
bool CheckCollisionPointCircle(Vector2 p, Vector2 c, float r) { float dx = p.x-c.x, dy = p.y-c.y; return dx*dx+dy*dy <= r*r; }
int GetScreenWidth(void) { return W; }
int GetScreenHeight(void) { return H; }
