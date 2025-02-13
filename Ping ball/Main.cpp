#include <iostream>
#include "raylib.h"
#include "Globals.h"

#define NO_FONT_AWESOME

#include "Ball.h"
#include "Box.h"

#include <stdio.h>

// Imgui Includes
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_glfw.h"
#include "Imgui/imgui_impl_opengl3.h"
#include "rllm/imgui_impl_raylib.h"
#include "rllm/rlImGui.h"
#include <GLFW/glfw3.h>

enum GameState { MENU, PLAYING, GAME_OVER };
GameState gameState = MENU;

//void static resizeWindow(int newWidth, int newHeight) {
//	SetWindowSize(newWidth, newHeight);
//}

void static showImGuiMenu()
{
	ImGui::Begin("Game Launcher");
	
	ImGui::Text("Welcome to ping ball");
	if (ImGui::Button("Start Game"))
	{
		//startGame = true;
		gameState = PLAYING;
	}

	if (ImGui::Button("Exit"))
	{
		rlImGuiShutdown();
		//ImGui_ImplRaylib_Shutdown();
		ImGui::DestroyContext();
		CloseWindow();
	}

	ImGui::End();
}

void static gameOverWindow()
{
	ImGui::Begin("Game Over!");

	ImGui::Text("GAME OVER");
	if (ImGui::Button("Play Again"))
	{
		//startGame = true;
		gameState = PLAYING;
	}

	if (ImGui::Button("Exit"))
	{
		rlImGuiShutdown();
		//ImGui_ImplRaylib_Shutdown();
		ImGui::DestroyContext();
		CloseWindow();
	}

	ImGui::End();
}

int main(void)
{
	/*const int screenWidth = 800;
	const int screenHeight = 450;*/

	SetConfigFlags(FLAG_VSYNC_HINT);
	SetTargetFPS(60);
	InitWindow(500, 300, "Game Launcher");  // ImGui Launcher Window
	rlImGuiSetup(true);  // Initialize ImGui for Raylib

	while (!WindowShouldClose() && gameState == MENU) {  // Keep running until "Start Game" is pressed
		BeginDrawing();
		ClearBackground(DARKGRAY);

		rlImGuiBegin();  // Start ImGui frame
		//if (gameState != PLAYING)
		//{
		//	gameOverWindow();
		//}
		//else
		//{
		//	showImGuiMenu();
		//}

		showImGuiMenu();

		rlImGuiEnd();    // End ImGui frame

		EndDrawing();
	}

	CloseWindow();

	if (gameState != PLAYING) return 0;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
	InitWindow(screenWidth, screenHeight, "Ping Ball");

	rlImGuiSetup(false);

	Ball ball;
	Box playerBox(0, screenHeight/2);
	Box aiBox(screenWidth-10, screenHeight/2);
	//InitWindow(screenWidth, screenHeight, "Ping Ball");

	while (!WindowShouldClose())
	{
		if (IsWindowResized()) {
			screenWidth = GetScreenWidth();
			screenHeight = GetScreenHeight();
		}
		ClearBackground(GRAY);

		BeginDrawing();
		rlImGuiBegin();


		if (gameState == GAME_OVER)
		{
			gameOverWindow();
		}
		else if (gameState == PLAYING)
		{
			ball.Update(playerBox, aiBox);
			playerBox.Update(ball.getY());
			aiBox.Update(ball.getY());

			bool isCollidingPlayer = ball.checkCollisionWithBox(playerBox);
			bool isCollidingWithAI = ball.checkCollisionWithBox(aiBox);

			//std::cout << isCollidingPlayer << " " << isCollidingWithAI << std::endl;

			if (ball.getX() >= screenWidth || ball.getX() <= 0)
			{
				gameState = GAME_OVER;
			}


			DrawText("W for up", 10, 10, 20, RAYWHITE);
			DrawText("S for down", 10, 30, 20, RAYWHITE);

			DrawFPS(100, 100);

			ball.Draw();
			playerBox.Draw();
			aiBox.Draw();
		}

		if (gameState == GAME_OVER)
		{
			ball = Ball();  // Reset ball position
			playerBox = Box(20, 225);
			aiBox = Box(760, 225);
		}

		rlImGuiEnd();
		EndDrawing();

	}
	rlImGuiShutdown();
	ImGui::DestroyContext();
	CloseWindow();

	return 0;
}

