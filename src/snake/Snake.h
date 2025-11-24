#pragma once
#include "../grid/Grid.h"
#include "../utils/Points.h"
#include <vector>
#include <deque>
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
    std::deque<Directions> direction_queue;
    Directions direction = DOWN;
    float move_timer = 0.0f;

    const float move_interval = 0.1f;
    Grid &grid;
    Points &points;
    void reset(); 
    void update_removal_animation(float delta);
    bool animation_playing = false;
    int pending_removals = 0;
    float removal_timer = 0.0f;
    const float removal_interval = 0.16f;
    bool are_opposite(Directions a, Directions b) const;

public:
    Vector2 get_head();
    std::vector<BodyCell> get_body();
    Directions get_direction();
    void set_direction(Directions new_direction);
    Snake(Grid &g, Points &p);
    void set_head();
    void append_body(Color color);
    void draw_snake();
    void check_body_collision();
    void update(float delta);
};
