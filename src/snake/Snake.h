#include "../grid/Grid.h"
#include "vector"
#include "raylib.h"
enum Directions
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Snake
{
private:
    struct BodyCell{ 
        Vector2 position; 
        Color color; 
    };
    Vector2 head;
    std::vector<BodyCell> body;
    Directions direction = DOWN;
    float move_timer = 0.0f;

    const float move_interval = 0.1f;
    Grid &grid;
    void reset(); 

public:
    Vector2 get_head();
    std::vector<BodyCell> get_body();
    Directions get_direction();
    void set_direction(Directions new_direction);
    Snake(Grid &g);
    void set_head();
    void append_body(Color color);
    void draw_snake();
    void check_body_collision();
    void update(float delta);
};