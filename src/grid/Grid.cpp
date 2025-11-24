#include "vector"
#include "raylib.h"
#include "Grid.h"

std::vector<Vector2> const &Grid::get_tiles()
{
	return tiles;
}

Grid::Grid(RandomGenerator &rng)
	: random(rng)
{
	regenerate_preview_colors();
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

std::vector<Color> Grid::generate_random_color_batch(int count)
{
	std::vector<Color> palette = {RED, GREEN, BLUE};
	std::vector<Color> batch;
	batch.reserve(count);

	for (int i = 0; i < count; ++i)
	{
		batch.push_back(random.random_color(palette));
	}

	return batch;
}

void Grid::regenerate_preview_colors()
{
	const int preview_count = 3;
	preview_colors = generate_random_color_batch(preview_count);
}

void Grid::spawn_food()
{
	if (preview_colors.empty())
	{
		regenerate_preview_colors();
	}

	std::vector<Color> colors_to_spawn = preview_colors;
	preview_colors.clear();

	const int target_food_count = static_cast<int>(colors_to_spawn.size());
	int spawned = 0;
	int safety_attempts = 0;

	while (spawned < target_food_count && safety_attempts < 1000)
	{
		safety_attempts++;

		Vector2 grid_cell = random.random_cell(grid_size);
		int gx = static_cast<int>(grid_cell.x);
		int gy = static_cast<int>(grid_cell.y);

		bool occupied = false;
		for (const auto &f : food)
		{
			Vector2 f_grid = to_grid(f.position);
			if (static_cast<int>(f_grid.x) == gx && static_cast<int>(f_grid.y) == gy)
			{
				occupied = true;
				break;
			}
		}

		if (occupied)
		{
			continue;
		}

		Color c = colors_to_spawn[spawned];
		Vector2 map_pos = to_map(Vector2{static_cast<float>(gx), static_cast<float>(gy)});
		food.push_back(Food{map_pos, c});
		spawned++;
	}

	regenerate_preview_colors();
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

const std::vector<Color> &Grid::get_preview_colors() const
{
	return preview_colors;
}
