#include "raylib.h"
#include "string"

#ifndef UI
#define UI

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

public:
    Vector2 position;
    Anchor anchor;



    void render();
    void draw_text(const std::string &text, int x, int y, int font_size, Color color);
    void draw_button(const std::string &label, int x, int y, int height, int width, int font_size, Color bg, Color border, Color font_color, Color hover_color);
    void draw_panel(int x, int y, int height, int width, Color bg, Color border);
};

#endif