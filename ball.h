#ifndef BALL_H
#define BALL_H

#include "raylib.h"

#define MAX_BALLS 100

typedef struct
{
    Vector2 position;
    Vector2 velocity;
    float radius;
    Color colour;
} Ball;

void AddBall(float x, float y);
void RemoveBallAt(float x, float y);
void ClearBalls(void);
void UpdateBalls(float gravity, float bounceEnergy);
void DrawBalls(void);
int GetBallCount(void);

#endif