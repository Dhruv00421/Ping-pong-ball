#pragma once

#include <iostream>
#include "raylib.h"
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_glfw.h"
#include "Imgui/imgui_impl_opengl3.h"
#include "rllm/imgui_impl_raylib.h"
#include "rllm/rlImGui.h"
#include <GLFW/glfw3.h>

#include "Box.h"
#include "Globals.h"


class Ball
{
public:
	Ball();
	~Ball();

	void Update(Box& playerBox, Box& aiBox);
	void Draw() const;
	void OnImguiRender();
    bool checkCollisionWithBox(const Box& box) const;

	// Getters
	float getX() const { return x; }
	float getY() const { return y; }

	// Setters
	void setX(float newX) { x = newX; }
	void setY(float newY) { y = newY; }


private:
	float x, y, r;
	float speedx, speedy;

};
