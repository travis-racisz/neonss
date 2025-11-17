#include "raylib.h"
#include "vector"
#pragma once
class Grid
{
private:
    int grid_size = 25;
    std::vector<Vector2> tiles;

public:
    struct Food
    {
       Vector2 position; 
       Color color; 
    };
        Grid();
    bool has_food = false;
    std::vector<Food> food;
    void make_grid();
    void spawn_food();
    void remove_food(int i);
    int get_grid_size();
    std::vector<Vector2> const &get_tiles();
    Vector2 to_grid(Vector2 position);
    Vector2 to_map(Vector2 position);
    void update(float delta);
};