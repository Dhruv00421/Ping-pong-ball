#pragma once

#include <iostream>
#include <raylib.h>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui_impl_raylib.h"
#include "rlImGui.h"
#include <GLFW/glfw3.h>

#include "Paddle.h"
#include "Ball.h"
#include "Globals.h"


class Ball
{
public:
	Ball();
	~Ball();

	void Update(Paddle& playerPaddle, Paddle& aiPaddle, Ball& ball);
	void Draw() const;
	void OnImguiRender();
    bool checkCollisionWithPaddle(const Paddle& Paddle) const;
	bool checkCollisionWithWall(const Ball& ball) const;

	// Getters
	float getX() const { return x; }
	float getY() const { return y; }

	// Setters
	void setX(float newX) { x = newX; }
	void setY(float newY) { y = newY; }


private:
	float x, y, r;
	float speedx, speedy;
	float speedInc;

};
