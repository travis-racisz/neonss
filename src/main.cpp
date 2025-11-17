#include "raylib.h"
#include "./grid/Grid.h"
#include <algorithm>
#include "./snake/Snake.h"
// #include "resource_dir.h"
// utility header for SearchAndSetResourceDir

const int GAME_WIDTH = 1200; const int GAME_HEIGHT = 800;

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


	Grid grid;
	Snake snake(grid);
	Camera2D camera;
	camera.zoom = 0.45;
	camera.offset = Vector2{100,0}; 
	camera.rotation = 0.0; 
	camera.target = Vector2{0,0}; 
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_WINDOW_MAXIMIZED);
	SetTargetFPS(60);

	// Create the window and OpenGL context
	InitWindow(GAME_WIDTH, GAME_HEIGHT, "Hello Raylib");
	RenderTexture2D target = LoadRenderTexture(GAME_WIDTH, GAME_HEIGHT);
	SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR); // smoother scaling

	// game loop
	while (!WindowShouldClose()) // run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		float delta = GetFrameTime();

		BeginTextureMode(target);
    		ClearBackground(BLACK);
    		BeginMode2D(camera);
        		grid.update(delta);
        		snake.update(delta);
        		snake.draw_snake();
    		EndMode2D();
		EndTextureMode();
		BeginDrawing();
    		ClearBackground(BLACK);
    		float screenW = (float)GetScreenWidth();
    		float screenH = (float)GetScreenHeight();
    		float scale = std::min(screenW / GAME_WIDTH, screenH / GAME_HEIGHT);
    		float destW = GAME_WIDTH * scale;
    		float destH = GAME_HEIGHT * scale;
    		float destX = (screenW - destW) * 0.5f;
    		float destY = (screenH - destH) * 0.5f;
    		Rectangle src = { 0, 0, (float)target.texture.width, -(float)target.texture.height };
    		Rectangle dst = { destX, destY, destW, destH };
    		DrawTexturePro(target.texture, src, dst, {0, 0}, 0.0f, WHITE);
		EndDrawing();

	}

	UnloadRenderTexture(target);

	CloseWindow();
	return 0;
}
