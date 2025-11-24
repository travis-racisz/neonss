#pragma once
#include <random>
#include <vector>
#include "raylib.h"

class RandomGenerator
{
public:
	RandomGenerator()
	{
		std::random_device rd;
		engine = std::mt19937(rd());
	}

	int random_int(int min, int max)
	{
		std::uniform_int_distribution<int> dist(min, max);
		return dist(engine);
	}

	Vector2 random_cell(int grid_size)
	{
		if (grid_size <= 0)
		{
			return {0.0f, 0.0f};
		}

		int max_index = grid_size - 1;
		float x = static_cast<float>(random_int(0, max_index));
		float y = static_cast<float>(random_int(0, max_index));
		return {x, y};
	}

	Color random_color(const std::vector<Color> &options)
	{
		if (options.empty())
		{
			return WHITE;
		}

		int idx = random_int(0, static_cast<int>(options.size()) - 1);
		return options[idx];
	}

private:
	std::mt19937 engine;
};
