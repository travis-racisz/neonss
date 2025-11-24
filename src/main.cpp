#include "raylib.h"
#include "./grid/Grid.h"
#include <algorithm>
#include "./snake/Snake.h"
#include "./UI/ui.h"
#include "./utils/RandomGenerator.h"
#include "./utils/Points.h"
// #include "resource_dir.h"
// utility header for SearchAndSetResourceDir

const int GAME_WIDTH = 1200;
const int GAME_HEIGHT = 800;
void clean_up()
{
	// delete grid,
	// delete snake
	// unload textures which we have none right now
	// delete &grid;
	// delete &snake;
}

int main()
{

	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_MAXIMIZED);
	SetTargetFPS(60);

	// Create the window and OpenGL context
	InitWindow(GAME_WIDTH, GAME_HEIGHT, "Hello Raylib");
	UI ui;
	RandomGenerator random;
	Grid grid(random);
	Points points;
	Snake snake(grid, points);
	Camera2D camera;
	camera.zoom = 0.45;
	camera.offset = Vector2{100, 0};
	camera.rotation = 0.0;
	camera.target = Vector2{0, 0};
	// Tell the window to use vsync and work on high DPI displays
	Shader shader = LoadShader(0, "src/shaders/bloom.fs");

	// game loop
	while (!WindowShouldClose()) // run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		float delta = GetFrameTime();

		BeginDrawing();
		ClearBackground(BLACK);
		switch (ui.get_current_screen())
		{
		case AppScreen::MainMenu:
			ui.draw_main_menu();
			break;
		case AppScreen::MainGame:
			ui.draw_game_ui(grid, snake, points);
			BeginMode2D(camera);
			grid.update(delta);
			snake.draw_snake();
			snake.update(delta);
			break;
		default:
			break;
		}
		EndMode2D();

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
