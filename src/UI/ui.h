#pragma once
#include "raylib.h"
#include "string"
#include "./snake/Snake.h"
#include "../utils/Points.h"
#include "./grid/Grid.h"

enum class AppScreen
{
    MainMenu,
    MainGame,
    Settings,
    GameOver
};

class UI
{
private:
    enum Anchor
    {
        TopLeft,
        TopRight,
        Center,
        BottomLeft,
        BottomRight,
        None

    };
    int screen_width;
    int screen_height;
    AppScreen current_screen = AppScreen::MainMenu;

public:
    Vector2 position;
    Anchor anchor;
    AppScreen get_current_screen();
    void set_current_screen(AppScreen new_screen);
    UI();
    void render();
    void draw_main_menu();
    void draw_game_ui(const Grid &grid, const Snake &snake, const Points &points);
    void draw_settings_menu();
    void draw_game_over_screen();
};
