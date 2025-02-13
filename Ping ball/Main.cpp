#include "raylib.h"
#include <iostream>
//#include <Windows.h>
#include <stdio.h>


#define NO_FONT_AWESOME

#ifndef _DEBUG

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#endif

#include "Globals.h"
#include "Ball.h"
#include "Paddle.h"


// Imgui Includes
#include "Imgui/imgui.h"
#include "Imgui/imgui_impl_glfw.h"
#include "Imgui/imgui_impl_opengl3.h"
#include "rllm/imgui_impl_raylib.h"
#include "rllm/rlImGui.h"
#include <GLFW/glfw3.h>
//#include <wtypes.h>



enum GameState { MENU, PLAYING, GAME_OVER };
GameState gameState = MENU;

//void static resizeWindow(int newWidth, int newHeight) {
//	SetWindowSize(newWidth, newHeight);
//}

void static showImGuiMenu()
{
	ImGui::Begin("Game Launcher");
	
	ImGui::Text("Welcome to ping pong ball");
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

void static toggleFullscreenMode()
{
	if (!IsWindowFullscreen)
	{
		int monitor = GetCurrentMonitor();
		SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
		ToggleFullscreen();
	}
	else
	{
		ToggleFullscreen();
		SetWindowSize(screenWidth, screenHeight);
	}
}

//int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
int main(void)
{

	SetConfigFlags(FLAG_VSYNC_HINT);
	SetTargetFPS(60);
	InitWindow(500, 300, "Game Launcher");  // ImGui Launcher Window
	rlImGuiSetup(true);  // Initialize ImGui for Raylib

	while (!WindowShouldClose() && gameState == MENU) {  // Keep running until "Start Game" is pressed
		BeginDrawing();
		ClearBackground(DARKGRAY);

		rlImGuiBegin(); 

		showImGuiMenu();

		rlImGuiEnd();    // End ImGui frame

		EndDrawing();
		if (gameState == PLAYING) {
			break; // Exit menu loop and start the game
		}
	}

	CloseWindow();

	if (gameState != PLAYING) return 0;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_MAXIMIZED);
	InitWindow(screenWidth, screenHeight, "Ping Pong Ball");
	InitAudioDevice();
	Sound hitSound = LoadSound("Sounds/Paddle_hit.mp3");
	Sound wallHit = LoadSound("Sounds/Wall_hit.mp3");

	Music bgMusic = LoadMusicStream("Sounds/bg_music.mp3");
	SetMusicVolume(bgMusic, 0.1f);
	PlayMusicStream(bgMusic);

	SetWindowPosition(50, 50);

	rlImGuiSetup(false);

	Ball ball;
	Paddle playerPaddle(0, screenHeight/2);
	Paddle aiPaddle(screenWidth - 20, screenHeight/2);
	int score = 0;

	while (!WindowShouldClose())
	{
		UpdateMusicStream(bgMusic);
		ClearBackground(GRAY);

		BeginDrawing();
		rlImGuiBegin();

		if (IsKeyPressed(KEY_F))
		{
			toggleFullscreenMode();
		}

		if (gameState == GAME_OVER)
		{
			gameOverWindow();
		}
		else if (gameState == PLAYING)
		{
			ball.Update(playerPaddle, aiPaddle, ball);
			playerPaddle.Update(ball.getY());
			aiPaddle.Update(ball.getY());

			bool isCollidingWithPlayer = ball.checkCollisionWithPaddle(playerPaddle);
			bool isCollidingWithAI = ball.checkCollisionWithPaddle(aiPaddle);
			bool isCollidingWithWall = ball.checkCollisionWithWall(ball);

			if (isCollidingWithPlayer)
			{
				//PlaySound(TEXT("Sounds/Paddle_hit.mp3"), NULL, SND_FILENAME | SND_ASYNC);
				PlaySound(hitSound);
				score++;
			}

			if (isCollidingWithAI)
			{
				//PlaySound(TEXT("Sounds/Paddle_hit.mp3"), NULL, SND_FILENAME | SND_ASYNC);
				PlaySound(hitSound);
			}

			if (isCollidingWithWall)
			{
				//PlaySound(TEXT("Sounds/Wall_hit.mp3"), NULL, SND_FILENAME | SND_ASYNC);
				PlaySound(wallHit);
			}

			//std::cout << isCollidingPlayer << " " << isCollidingWithAI << std::endl;

			if (ball.getX() >= screenWidth || ball.getX() <= 0)
			{
				gameState = GAME_OVER;
			}

			DrawText("W for up", 10, 10, 20, RAYWHITE);
			DrawText("S for down", 10, 30, 20, RAYWHITE);
			DrawText("F for Fullscreen mode", 10,50, 20, RAYWHITE);
			DrawText("esc for exit", 10, 70, 20, RAYWHITE);
			DrawText(TextFormat("Score: %d", score), screenWidth/2, 10, 20, RAYWHITE);

			DrawFPS(100, 100);

			ball.Draw();
			playerPaddle.Draw();
			aiPaddle.Draw();
		}

		if (gameState == GAME_OVER)
		{
			ball = Ball();  // Reset ball position
			playerPaddle = Paddle(0, screenWidth/2);
			aiPaddle = Paddle(screenWidth - 20, screenHeight / 2);
		}

		rlImGuiEnd();
		EndDrawing();

	}

	UnloadSound(hitSound);
	UnloadSound(wallHit);
	UnloadMusicStream(bgMusic);

	rlImGuiShutdown();
	ImGui::DestroyContext();
	CloseWindow();

	return 0;
}



