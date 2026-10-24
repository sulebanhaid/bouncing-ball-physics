#include "raylib.h"
#include "ball.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define DEFAULT_GRAVITY 0.3f
#define DEFAULT_BOUNCE_ENERGY 0.8f

static float gravity = DEFAULT_GRAVITY;
static float bounceEnergy = DEFAULT_BOUNCE_ENERGY;
static bool paused = false;

static void ResetSimulation(void)
{
    ClearBalls();
    gravity = DEFAULT_GRAVITY;
    bounceEnergy = DEFAULT_BOUNCE_ENERGY;
    paused = false;
}

static void HandleInput(void)
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        Vector2 mousePosition = GetMousePosition();
        AddBall(mousePosition.x, mousePosition.y);
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        Vector2 mousePosition = GetMousePosition();
        RemoveBallAt(mousePosition.x, mousePosition.y);
    }

    if (IsKeyPressed(KEY_C))
    {
        ClearBalls();
    }

    if (IsKeyPressed(KEY_R))
    {
        ResetSimulation();
    }

    if (IsKeyPressed(KEY_SPACE))
    {
        paused = !paused;
    }

    if (IsKeyDown(KEY_UP))
    {
        gravity += 0.01f;
    }

    if (IsKeyDown(KEY_DOWN))
    {
        gravity -= 0.01f;
    }

    if (IsKeyDown(KEY_RIGHT))
    {
        bounceEnergy += 0.01f;

        if (bounceEnergy > 1.0f)
        {
            bounceEnergy = 1.0f;
        }
    }

    if (IsKeyDown(KEY_LEFT))
    {
        bounceEnergy -= 0.01f;

        if (bounceEnergy < 0.0f)
        {
            bounceEnergy = 0.0f;
        }
    }
}

static void DrawInterface(void)
{
    DrawText("Left click: add ball", 10, 10, 20, WHITE);
    DrawText("Right click: remove ball", 10, 35, 20, WHITE);
    DrawText("Space: pause", 10, 60, 20, WHITE);
    DrawText("C: clear balls", 10, 85, 20, WHITE);
    DrawText("R: reset simulation", 10, 110, 20, WHITE);
    DrawText("Up/Down: gravity", 10, 135, 20, WHITE);
    DrawText("Left/Right: bounce energy", 10, 160, 20, WHITE);

    DrawText(
        TextFormat("Balls: %d / %d", GetBallCount(), MAX_BALLS),
        560,
        10,
        20,
        WHITE
    );

    DrawText(
        TextFormat("Gravity: %.2f", gravity),
        560,
        35,
        20,
        WHITE
    );

    DrawText(
        TextFormat("Bounce: %.2f", bounceEnergy),
        560,
        60,
        20,
        WHITE
    );

    if (paused)
    {
        DrawText("PAUSED", 340, 280, 30, YELLOW);
    }
}

int main(void)
{
    InitWindow(
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        "Interactive Bouncing Ball Physics"
    );

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        HandleInput();

        if (!paused)
        {
            UpdateBalls(gravity, bounceEnergy);
        }

        BeginDrawing();

        ClearBackground(DARKBLUE);
        DrawBalls();
        DrawInterface();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}