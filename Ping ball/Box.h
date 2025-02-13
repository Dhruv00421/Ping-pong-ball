#pragma once

#include <iostream>
#include "raylib.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_glfw.h"
#include "Imgui/imgui_impl_opengl3.h"
#include "rllm/imgui_impl_raylib.h"
#include "rllm/rlImGui.h"
#include <GLFW/glfw3.h>

class Box
{
public:
	
	Box();
	Box(float startX, float startY);
	~Box();


	void Update(float ballY);
	void Draw() const;
	//void OnImguiRender();

	//Rectangle boxRect;
	Rectangle getRect() const { return Rectangle{ x, y, width, height }; };

	// Gettter
	float getBoxX() const { return x; }
	float getBoxY() const { return y; }
	float getBoxWidth() const { return width; }
	float getBoxHeight() const { return height; }

	// Setter
	void setBoxX(float newX) { x = newX; }
	void setBoxY(float newY) { y = newY; }
	void setBoxWidth(float newWidth) { width = newWidth; }
	void setBoxHeight(float newHeight) { height = newHeight; }

private:
	float x, y, width, height;
	float velocity;
	bool isAI;

};