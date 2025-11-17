#include "Snake.h"
#include "raylib.h"
#include "vector"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 800;
Snake::Snake(Grid &grid)
	: grid(grid)
{
	reset();
	// head = grid.to_map(Vector2{static_cast<float>(grid.get_grid_size() / 2), static_cast<float>(grid.get_grid_size() / 2)});
	// direction = RIGHT;
	// for (int i = 0; i <= 3; i++)
	//{

	// body.push_back(Vector2{
	////.x = static_cast<float>(rand_x),
	//.x = float(grid.to_grid(head).x - i) * 64,
	//.y = float(grid.to_grid(head).y) * 64,
	//});
	//}
}

void Snake::draw_snake()
{
	// draw head, followed by body

	for (unsigned long i = 0; i < body.size(); i++)
	{
		DrawRectangle(body[i].position.x, body[i].position.y, 64, 64, body[i].color);
	}
}

Vector2 Snake::get_head()
{
	return head;
}

std::vector<Snake::BodyCell> Snake::get_body()
{
	return body;
}

static bool same_color(Color a, Color b)
{
	return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
}

void Snake::append_body(Color color)
{
	// Decide where the new segment spawns: at current tail position or head if empty
	Vector2 pos;
	if (body.empty())
	{
		pos = head;
	}
	else
	{
		pos = body.back().position;
	}

	// Add the new segment
	body.push_back(BodyCell{pos, color});

	// --- Sliding window from the end: remove 3+ same-colored cells at the tail ---

	const int k = 3; // minimum run length to remove

	if (body.size() < static_cast<size_t>(k))
		return; // nothing to check yet

	// Start at the last element (tail) and move left while color is the same
	Color tailColor = body.back().color;
	int runLength = 1;

	int idx = static_cast<int>(body.size()) - 2; // second-to-last index
	while (idx >= 0 && same_color(body[idx].color, tailColor))
	{
		runLength++;
		idx--;
	}

	// If we found 3 or more in a row at the end, erase them
	if (runLength >= k)
	{
		body.erase(body.end() - runLength, body.end());
	}
}

Directions Snake::get_direction()
{
	return direction;
}

void Snake::set_direction(Directions new_direction)
{

	if ((new_direction == UP && direction != DOWN) ||
		(new_direction == DOWN && direction != UP) ||
		(new_direction == LEFT && direction != RIGHT) ||
		(new_direction == RIGHT && direction != LEFT))
	{
		direction = new_direction;
	}
}

void Snake::reset()
{

	float tile_size = 64.0f;
	int center_x = 12;
	int center_y = 12;

	head = {static_cast<float>(center_x) * tile_size, static_cast<float>(center_y) * tile_size};

	body.clear();
	for (int i = 1; i <= 4; ++i)
	{
		body.push_back(BodyCell{Vector2{static_cast<float>(center_x - i) * tile_size, static_cast<float>(center_y) * tile_size}, WHITE});
	}

	direction = RIGHT;
	move_timer = 0.0f;
}
void Snake::check_body_collision()
{
	for (int i = 0; i <= body.size() - 1; i++)
	{
		if (head.x == body[i].position.x && head.y == body[i].position.y)
		{
			reset();
		}
	}
}

void Snake::update(float delta)
{

	for (int i = 0; i < grid.food.size(); i++)
	{

		if (grid.food[i].position.x == head.x && grid.food[i].position.y == head.y)
		{
			append_body(grid.food[i].color);
			grid.remove_food(i);
		}
	}
	if (IsKeyPressed(KEY_R))
	{
		grid.spawn_food();
	}
	if (IsKeyPressed(KEY_UP))
	{
		set_direction(Directions::UP);
	}
	else if (IsKeyPressed(KEY_DOWN))
	{
		set_direction(Directions::DOWN);
	}
	else if (IsKeyPressed(KEY_LEFT))
	{
		set_direction(Directions::LEFT);
	}
	else if (IsKeyPressed(KEY_RIGHT))
	{
		set_direction(Directions::RIGHT);
	}
	move_timer += delta;
	if (move_timer < move_interval)
		return;

	move_timer = 0.0f;
	Vector2 old_head = head;
	Vector2 grid_pos = grid.to_grid(head);
	switch (direction)
	{
	case UP:
		grid_pos.y -= 1;
		break;
	case DOWN:
		grid_pos.y += 1;
		break;
	case LEFT:
		grid_pos.x -= 1;
		break;
	case RIGHT:
		grid_pos.x += 1;
		break;
	}

	if (grid_pos.x < 0 || grid_pos.y < 0 || grid_pos.x > grid.get_grid_size() || grid_pos.y > grid.get_grid_size())
	{
		reset();
		return;
	}
	check_body_collision();
	head = grid.to_map(grid_pos);
	for (size_t i = body.size() - 1; i > 0; i--)
	{
		body[i].position = body[i - 1].position;
		// body[i].color = body[i-1].color;
	}

	if (!body.empty())
	{
		body[0].position = old_head;
	}
}