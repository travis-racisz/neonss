#include "raylib.h"
#include <string>
#include "ui.h"

UI::UI()
{
    screen_height = GetScreenHeight();
    screen_width = GetScreenWidth();
}

void UI::draw_main_menu()
{
    Vector2 mouse_pos = GetMousePosition();
    Rectangle start_button = Rectangle{
        .x = float(screen_width / 2),
        .y = float(screen_height / 2),
        .height = 100,
        .width = 200

    };
    Color start_button_color = WHITE;

    if (CheckCollisionPointRec(mouse_pos, start_button))
    {
        start_button_color = BLUE;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            set_current_screen(AppScreen::MainGame);
        }
    }

    DrawRectangleRec(start_button, start_button_color);
    DrawText("Start", start_button.x, start_button.y, 28, BLACK);
}

void UI::draw_game_ui(const Grid &grid, const Snake &snake, const Points &points)
{
    // TODO: will update later cast it to void for now
    (void)snake;
    Rectangle next_color_panel = Rectangle{
        .x = float(screen_width - 308), // padding of 8 on top and right side
        .y = 8,
        .height = 200,
        .width = 300

    };

    DrawRectangleRec(next_color_panel, Color{20, 20, 20, 200});
    DrawRectangleLines(next_color_panel.x, next_color_panel.y, next_color_panel.width, next_color_panel.height, WHITE);
    DrawText("Next Colors", next_color_panel.x + 12, next_color_panel.y + 12, 18, RAYWHITE);

    const auto &previews = grid.get_preview_colors();
    const int slot_size = 40;
    const int slot_padding = 12;

    for (size_t i = 0; i < previews.size(); ++i)
    {
        float square_x = next_color_panel.x + 12;
        float square_y = next_color_panel.y + 40 + i * (slot_size + slot_padding);
        DrawRectangle(square_x, square_y, slot_size, slot_size, previews[i]);
        DrawRectangleLines(square_x, square_y, slot_size, slot_size, BLACK);
    }

    DrawText(TextFormat("%d", points.get_points()), float(screen_width - 300), 400, 28, WHITE);
}

AppScreen UI::get_current_screen()
{
    return current_screen;
}

void UI::set_current_screen(AppScreen new_screen)
{
    current_screen = new_screen;
}
