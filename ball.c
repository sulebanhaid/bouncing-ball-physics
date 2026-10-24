#include "ball.h"
#include <math.h>

static Ball balls[MAX_BALLS];
static int ballCount = 0;

static void ResolveBallCollisions(float bounceEnergy)
{
    for (int i = 0; i < ballCount; i++)
    {
        for (int j = i + 1; j < ballCount; j++)
        {
            Ball *first = &balls[i];
            Ball *second = &balls[j];

            float dx = second->position.x - first->position.x;
            float dy = second->position.y - first->position.y;

            float distanceSquared = dx * dx + dy * dy;
            float minimumDistance = first->radius + second->radius;

            if (distanceSquared == 0.0f ||
                distanceSquared >= minimumDistance * minimumDistance)
            {
                continue;
            }

            float distance = sqrtf(distanceSquared);

            float normalX = dx / distance;
            float normalY = dy / distance;

            float overlap = minimumDistance - distance;

            first->position.x -= normalX * overlap * 0.5f;
            first->position.y -= normalY * overlap * 0.5f;

            second->position.x += normalX * overlap * 0.5f;
            second->position.y += normalY * overlap * 0.5f;

            float relativeVelocityX =
                second->velocity.x - first->velocity.x;

            float relativeVelocityY =
                second->velocity.y - first->velocity.y;

            float velocityAlongNormal =
                relativeVelocityX * normalX +
                relativeVelocityY * normalY;

            if (velocityAlongNormal > 0.0f)
            {
                continue;
            }

            float impulse =
                -(1.0f + bounceEnergy) *
                velocityAlongNormal / 2.0f;

            first->velocity.x -= impulse * normalX;
            first->velocity.y -= impulse * normalY;

            second->velocity.x += impulse * normalX;
            second->velocity.y += impulse * normalY;
        }
    }
}

void AddBall(float x, float y)
{
    if (ballCount >= MAX_BALLS)
    {
        return;
    }

    Ball *ball = &balls[ballCount];

    ball->position = (Vector2){x, y};

    ball->velocity = (Vector2){
        (float)GetRandomValue(-4, 4),
        (float)GetRandomValue(-4, 0)
    };

    ball->radius = (float)GetRandomValue(10, 20);

    ball->colour = (Color){
        GetRandomValue(80, 255),
        GetRandomValue(80, 255),
        GetRandomValue(80, 255),
        255
    };

    ballCount++;
}

void RemoveBallAt(float x, float y)
{
    Vector2 point = {x, y};

    for (int i = ballCount - 1; i >= 0; i--)
    {
        if (CheckCollisionPointCircle(
                point,
                balls[i].position,
                balls[i].radius))
        {
            balls[i] = balls[ballCount - 1];
            ballCount--;
            return;
        }
    }
}

void ClearBalls(void)
{
    ballCount = 0;
}

void UpdateBalls(float gravity, float bounceEnergy)
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    for (int i = 0; i < ballCount; i++)
    {
        Ball *ball = &balls[i];

        ball->velocity.y += gravity;

        ball->position.x += ball->velocity.x;
        ball->position.y += ball->velocity.y;

        if (ball->position.x - ball->radius <= 0)
        {
            ball->position.x = ball->radius;
            ball->velocity.x *= -bounceEnergy;
        }

        if (ball->position.x + ball->radius >= screenWidth)
        {
            ball->position.x = screenWidth - ball->radius;
            ball->velocity.x *= -bounceEnergy;
        }

        if (ball->position.y - ball->radius <= 0)
        {
            ball->position.y = ball->radius;
            ball->velocity.y *= -bounceEnergy;
        }

        if (ball->position.y + ball->radius >= screenHeight)
        {
            ball->position.y = screenHeight - ball->radius;
            ball->velocity.y *= -bounceEnergy;
        }
    }

    ResolveBallCollisions(bounceEnergy);
}

void DrawBalls(void)
{
    for (int i = 0; i < ballCount; i++)
    {
        DrawCircleV(
            balls[i].position,
            balls[i].radius,
            balls[i].colour
        );
    }
}

int GetBallCount(void)
{
    return ballCount;
}