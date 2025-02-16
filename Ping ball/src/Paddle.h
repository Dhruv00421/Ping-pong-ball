#pragma once

#include <iostream>
#include <raylib.h>
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui_impl_raylib.h"
#include "rlImGui.h"
#include <GLFW/glfw3.h>

class Paddle
{
public:
	
	Paddle();
	Paddle(float startX, float startY);
	~Paddle();


	void Update(float ballY);
	void Draw() const;
	//void OnImguiRender();

	//Rectangle PaddleRect;
	Rectangle getRect() const { return Rectangle{ x, y, width, height }; };

	// Gettter
	float getPaddleX() const { return x; }
	float getPaddleY() const { return y; }
	float getPaddleWidth() const { return width; }
	float getPaddleHeight() const { return height; }
	float getPaddleVelocity() const { return velocity; }

	// Setter
	void setPaddleX(float newX) { x = newX; }
	void setPaddleY(float newY) { y = newY; }
	void setPaddleWidth(float newWidth) { width = newWidth; }
	void setPaddleHeight(float newHeight) { height = newHeight; }
	void setPaddleVelocity(float newVelocity) { velocity = newVelocity; }

private:
	float x, y, width, height;
	float velocity;
	bool isAI;

};