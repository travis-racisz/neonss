#include "vector"
#include "raylib.h"
#include "Grid.h"
#include "random"

std::vector<Vector2> const &Grid::get_tiles()
{
	return tiles;
}

Grid::Grid()
{
	spawn_food();
	for (int i = 0; i <= grid_size; i++)
	{
		for (int j = 0; j <= grid_size; j++)
		{
			tiles.push_back(Vector2{.x = static_cast<float>(i), .y = static_cast<float>(j)});
		}
	}
}

Vector2 Grid::to_grid(Vector2 position)
{
	return Vector2{static_cast<float>(int(position.x / 64)), static_cast<float>(int(position.y / 64))};
}

Vector2 Grid::to_map(Vector2 position)
{
	return Vector2{position.x * 64, position.y * 64};
}

void Grid::spawn_food()
{
    std::vector<Color> colors = { RED, GREEN, BLUE };

    // Random engines (you could move these to static/global if you want)
    std::random_device rd;
    std::mt19937 gen(rd());

    // If grid_size is the number of tiles per side, valid grid indices are [0, grid_size - 1]
    std::uniform_int_distribution<int> distr(0, grid_size - 1);
    std::uniform_int_distribution<int> distc(0, static_cast<int>(colors.size()) - 1);

    int target_food_count = 3;           // how many food pieces to spawn total
    int safety_attempts   = 0;           // avoid infinite loops on tiny grids

    while (static_cast<int>(food.size()) < target_food_count && safety_attempts < 1000)
    {
        safety_attempts++;

        int gx = distr(gen);
        int gy = distr(gen);

        // Check if there is already food at this grid location
        bool occupied = false;
        for (const auto &f : food)
        {
            Vector2 f_grid = to_grid(f.position);  // convert from pixel coords back to grid coords
            if (static_cast<int>(f_grid.x) == gx && static_cast<int>(f_grid.y) == gy)
            {
                occupied = true;
                break;
            }
        }

        if (occupied)
        {
            continue; // pick another random cell
        }

        // Choose a random color and place new food
        Color c = colors[distc(gen)];
        Vector2 map_pos = to_map(Vector2{ static_cast<float>(gx), static_cast<float>(gy) });
        food.push_back(Food{ map_pos, c });
    }
}
void Grid::remove_food(int i)
{
	food.erase(food.begin() + i);
}

int Grid::get_grid_size()
{
	return grid_size;
}

void Grid::update(float delta)
{
	if(food.size() == 0){ 
		spawn_food();
	}
	for (unsigned long i = 0; i < get_tiles().size(); i++)
	{
		DrawRectangleLines(get_tiles()[i].x * 64, get_tiles()[i].y * 64, 64, 64, GRAY);
	};
	for (unsigned long i = 0; i < food.size(); i++)
	{

		DrawRectangle(food[i].position.x, food[i].position.y, 64, 64, food[i].color);
	}
}