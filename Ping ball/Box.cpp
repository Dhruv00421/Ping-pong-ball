#include "Box.h"
#include "Globals.h"
#include <iostream>
#include "raymath.h"

Box::Box()
    :x(0), y(0), width(20), height(60),
    velocity(200.0f), isAI(false)
{
}

Box::Box(float startX, float startY)
    :x(startX), y(startY),
    width(20), height(60),
    velocity(200.0f), isAI(startX > 400)
{
    //boxRect = { 0.0f, 225.0f, 20.0f, 60.0f }; // x, y, width, height
}

Box::~Box() 
{
}

void Box::Update(float ballY) {
    float deltaTime = GetFrameTime();
    //deltaTime = (deltaTime > 0.02f) ? 0.02f : deltaTime;
    deltaTime = Clamp(deltaTime, 0.001f, 0.016f); // Ensures stable movement

    if (IsWindowResized()) {
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();
    }

    if (x < screenWidth / static_cast<float>(2)) {  // Player controlled box
        if (IsKeyDown(KEY_W)) y -= velocity * deltaTime;
        if (IsKeyDown(KEY_S)) y += velocity * deltaTime;
    }
    else {  // AI box logic (follows the ball)
        if (y + 30 < ballY) y += velocity * deltaTime;
        if (y + 30 > ballY) y -= velocity * deltaTime;
    }

    // Keep inside screen bounds
    if (y < 0) y = 0;
    if (y > screenHeight - height) y = screenHeight - height;
}

void Box::Draw() const {
    // Draw logic here
    //std::cout << "Drawing Box at (" << x << ", " << y << ") with velocity " << Velocity << std::endl;
    DrawRectangle(x, y, width, height, BLACK);
}
