#include "Paddle.h"
#include "Globals.h"
#include <iostream>
#include "raymath.h"

Paddle::Paddle()
    :x(0), y(0), width(20), height(60),
    velocity(200.0f), isAI(false)
{
}

Paddle::Paddle(float startX, float startY)
    :x(startX), y(startY),
    width(20), height(60),
    velocity(200.0f), isAI(startX > 400)
{
    //PaddleRect = { 0.0f, 225.0f, 20.0f, 60.0f }; // x, y, width, height
}

Paddle::~Paddle() 
{
}

void Paddle::Update(float ballY) {
    float deltaTime = GetFrameTime();
    //deltaTime = (deltaTime > 0.02f) ? 0.02f : deltaTime;
    deltaTime = Clamp(deltaTime, 0.001f, 0.016f); // Ensures stable movement


    if (!isAI) {
        // Player controls
        if (IsKeyDown(KEY_W)) y -= velocity * deltaTime;
        if (IsKeyDown(KEY_S)) y += velocity * deltaTime;
    }
    else {
        // AI follows the ball with reaction delay
        float aiSpeedFactor = 0.7f;
        if (y + height / 2 < ballY - 10) y += velocity * aiSpeedFactor * deltaTime;
        if (y + height / 2 > ballY + 10) y -= velocity * aiSpeedFactor * deltaTime;
    }

    // Keep inside screen bounds
    y = Clamp(y, 0, screenHeight - height);
}

void Paddle::Draw() const {
    
    DrawRectangle(x, y, width, height, BLACK);
}
