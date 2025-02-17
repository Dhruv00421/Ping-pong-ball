//#include <Windows.h>
#define PLATFORM_DESKTOP
#include <raylib.h>
#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream> 

#define NO_FONT_AWESOME

#ifndef _DEBUG

#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#endif

#include "Globals.h"
#include "Ball.h"
#include "Paddle.h"

// Include sound header files
//#include "bg_music.h"
//#include "Pubg-Theme.h"
//#include "Wall_hit.h"
//#include "Paddle_hit.h"
//#include "grenade-explosion.h"


// Imgui Includes
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui_impl_raylib.h"
#include "rlImGui.h"
#include <GLFW/glfw3.h>


enum GameState { MENU, PLAYING, GAME_OVER };
GameState gameState = MENU;

static bool pubg = false;
static bool normal = true;

/*// embedding sound function
std::string SaveToTempFile(const std::string& filename, const unsigned char* data, unsigned int length) {
	std::ofstream file(filename, std::ios::binary);  // Open file in binary mode
	if (!file) {
		std::cerr << "Error: Could not create temp file: " << filename << std::endl;
		return "";
	}
	file.write(reinterpret_cast<const char*>(data), length);
	file.close();
	return filename; // Return the file path
}*/

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


int main(void)
{
	//std::string exePath = GetExecutablePath();
    /*std::string soundsPath = exePath + "\\Sounds\\";*/


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

	// Sounds & Musics
	InitAudioDevice();

	/*{
	*	// Tried to make sound in binary and embedding it in exe file but the music is not working
		Wave gewave = LoadWaveFromMemory(".wav", (unsigned char*)grenadeexplosionData, grenadeexplosionData_len);
		Sound grenade_explosion = LoadSoundFromWave(gewave);
		Wave pdwave = LoadWaveFromMemory(".wav", (unsigned char*)PaddleHitData, PaddleHitData_len);
		Sound paddle_hit = LoadSoundFromWave(pdwave);
		Wave wlwave = LoadWaveFromMemory(".wav", (unsigned char*)wallHitData, wallHitData_len);
		Sound wall_hit = LoadSoundFromWave(wlwave);

		std::string tempFile = "temp_bg_music.wav";
		SaveToTempFile(tempFile, bgMusicData, bgMusicData_len);
		Music bgMusic = LoadMusicStream(tempFile.c_str());
		std::string tempFile2 = "temp_bg_music.wav";
		SaveToTempFile(tempFile2, pubgThemeData, pubgThemeData_len);
		Music pubgMusic = LoadMusicStream(tempFile2.c_str());
	}*/

	Sound paddle_hit = LoadSound("Sounds/Paddle_hit.wav");
	Sound wall_hit = LoadSound("Sounds/Wall_hit.wav");
	Sound grenade_explosion = LoadSound("Sounds/grenade-explosion.wav");

	Music bgMusic = LoadMusicStream("Sounds/bg_music.wav");
	Music pubgMusic = LoadMusicStream("Sounds/Pubg-Theme.wav");

	/*SetMusicVolume(bgMusic, 0.1f);
	SetMusicVolume(pubgMusic, 0.6f);*/

	if (pubg)
	{
		normal = false;
		PlayMusicStream(pubgMusic);
	}
	if (normal)
	{
		pubg = false;
		PlayMusicStream(bgMusic);
	}



	SetWindowPosition(50, 50);

	rlImGuiSetup(false);

	Ball ball;
	Paddle playerPaddle(0, screenHeight/2);
	Paddle aiPaddle(screenWidth - 20, screenHeight/2);
	int score = 0;

	while (!WindowShouldClose())
	{
		//UpdateMusicStream(bgMusic);
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
				if (normal)
				{
					PlaySound(paddle_hit);
				}
				if (pubg)
				{
					PlaySound(grenade_explosion);
				}
				score++;
			}

			if (isCollidingWithAI)
			{
				if (normal)
				{
					PlaySound(paddle_hit);
				}
				if (pubg)
				{
					PlaySound(grenade_explosion);
				}
			}

			if (isCollidingWithWall)
			{
				if (normal)
				{
					PlaySound(wall_hit);
				}
				if (pubg)
				{
					PlaySound(grenade_explosion);
				}
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

			ImGui::Begin("Music And Sound Control");
			if (ImGui::Checkbox("Pubg", &pubg)) {
				if (pubg) {
					normal = false;
					StopMusicStream(bgMusic);
					PlayMusicStream(pubgMusic);
					SetSoundVolume(grenade_explosion, 0.5f);

				}
			}

			if (ImGui::Checkbox("Normal", &normal)) {
				if (normal) {
					pubg = false;
					StopMusicStream(pubgMusic);
					PlayMusicStream(bgMusic);

				}
			}

			UpdateMusicStream(bgMusic);
			UpdateMusicStream(pubgMusic);

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
		ImGui::End();

		rlImGuiEnd();
		EndDrawing();

	}

	UnloadSound(paddle_hit);
	UnloadSound(wall_hit);
	UnloadSound(grenade_explosion);
	UnloadMusicStream(bgMusic);
	UnloadMusicStream(pubgMusic);

	rlImGuiShutdown();
	ImGui::DestroyContext();
	CloseWindow();

	return 0;
}



