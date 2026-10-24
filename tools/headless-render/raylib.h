/* Minimal software stand-in for raylib, used only to render screenshots of the
   unmodified game code (main.c + ball.c) without a GPU or display. */
#ifndef RAYLIB_STUB_H
#define RAYLIB_STUB_H
#include <stdbool.h>
typedef struct { float x, y; } Vector2;
typedef struct { unsigned char r, g, b, a; } Color;
#define WHITE    (Color){255,255,255,255}
#define YELLOW   (Color){253,249,0,255}
#define DARKBLUE (Color){0,82,172,255}
enum { MOUSE_BUTTON_LEFT = 0, MOUSE_BUTTON_RIGHT = 1 };
enum { KEY_SPACE = 32, KEY_C = 67, KEY_R = 82, KEY_RIGHT = 262, KEY_LEFT = 263, KEY_DOWN = 264, KEY_UP = 265 };
void InitWindow(int w, int h, const char *title);
void SetTargetFPS(int fps);
bool WindowShouldClose(void);
void CloseWindow(void);
void BeginDrawing(void);
void EndDrawing(void);
void ClearBackground(Color c);
void DrawCircleV(Vector2 c, float r, Color col);
void DrawText(const char *t, int x, int y, int size, Color col);
const char *TextFormat(const char *fmt, ...);
bool IsMouseButtonPressed(int b);
Vector2 GetMousePosition(void);
bool IsKeyPressed(int k);
bool IsKeyDown(int k);
int GetRandomValue(int min, int max);
bool CheckCollisionPointCircle(Vector2 p, Vector2 c, float r);
int GetScreenWidth(void);
int GetScreenHeight(void);
#endif
