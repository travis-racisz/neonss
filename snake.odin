package main
import rl "vendor:raylib"

@(private = "file")
hurt_cooldown: f32 = 0

@(private = "file")
is_animation_playing := false

@(private = "file")
pending_removals: i32 = 0

@(private = "file")
removal_interval: f32 = 0.16

@(private = "file")
removal_timer: f32 = 0.0

Directions :: enum {
	UP,
	DOWN,
	LEFT,
	RIGHT,
}

BodyCell :: struct {
	position: GridCell,
	color:    rl.Color,
}

Snake :: struct {
	direction:    Directions,
	updateTimer:  f32,
	head:         GridCell,
	body:         [dynamic]BodyCell,
	removalCount: i32,
	removalTimer: f32,
	update:       proc(delta: f32),
}

snake: Snake

direction_queue: [dynamic]Directions
REMOVAL_INTERVAL :: 0.2

init_snake :: proc() {

	snake.head = GridCell{13, 13} // in grid coordinates
	for i in 1 ..< 4 {
		// append(&snake.body, BodyCell{position = GridCell{13 - i32(i), 13}, color = random_non_white_color()})
        
	}
	snake.direction = .RIGHT
	snake.updateTimer = 0.1
	snake.removalCount = 0
	snake.removalTimer = 0
	snake.update = update

}


check_food_collision :: proc() {
	for food, i in grid.food {
		if snake.head.x == food.position.x && snake.head.y == food.position.y {

			// remove the food
			unordered_remove(&grid.food, i)
            if len(snake.body) == 0 { 

			new_cell := BodyCell {
				position = snake.head,
				color    = food.color == rl.WHITE ? random_non_white_color(): food.color,
			}
			append(&snake.body, new_cell)
            } else { 

			new_cell := BodyCell {
				position = snake.body[len(snake.body) - 1].position,
				color    = food.color == rl.WHITE ? random_non_white_color(): food.color,
			}
			append(&snake.body, new_cell)
            }

			//After growth, check for a tail run of matching colors and queue removals
		}

	}
}

@(private = "file")
check_body_collision :: proc() {

	    for cell in snake.body {
		    if snake.head.x == cell.position.x && snake.head.y == cell.position.y {
                if hurt_cooldown >= 5{ 
			// game over
                    game_manager.heart -= 1
                    hurt_cooldown = 0.0
		        }
	    }
    }
}


@(private = "file")
draw_snake :: proc() {
	rl.DrawRectangle(to_map(snake.head).x, to_map(snake.head).y, CELL_SIZE, CELL_SIZE, rl.WHITE)
	for cell in snake.body {
		rl.DrawRectangle(
			to_map(cell.position).x,
			to_map(cell.position).y,
			CELL_SIZE,
			CELL_SIZE,
			cell.color,
		)
	}

}

@(private = "file")
check_tail_match :: proc() -> bool {

	return false
}


@(private = "file")
same_color :: proc(a: rl.Color, b: rl.Color) -> bool {
	if a.r == b.r && a.g == b.g && a.b == b.b {
		return true
	} else {return false}
}

@(private = "file")
handle_removals :: proc(delta: f32) {
	if !is_animation_playing {
		return
	}
	if len(snake.body) == 0 {
		is_animation_playing = false
		pending_removals = 0
		return
	}

	removal_timer += delta
	if removal_timer >= removal_interval && pending_removals > 0 {
        spawn_particle(50, to_map(snake.body[len(snake.body) - 1].position), snake.body[len(snake.body) - 1].color)
		pop(&snake.body)
		game_manager.points += 100
		pending_removals -= 1
		removal_timer = 0.0
		if pending_removals == 0 {
			is_animation_playing = false
		}
	}
}

@(private = "file")
move_snake :: proc() {
	prev_head := snake.head

	switch snake.direction {
	case .UP:
		snake.head = GridCell{snake.head.x, snake.head.y - 1}
	case .DOWN:
		snake.head = GridCell{snake.head.x, snake.head.y + 1}
	case .LEFT:
		snake.head = GridCell{snake.head.x - 1, snake.head.y}

	case .RIGHT:
		snake.head = GridCell{snake.head.x + 1, snake.head.y}
	}

	if snake.head.x < 0 {
		snake.head.x = GRID_SIZE
	}

	if snake.head.x > GRID_SIZE {
		snake.head.x = 0
	}

	if snake.head.y < 0 {
		snake.head.y = GRID_SIZE
	}

	if snake.head.y > GRID_SIZE {
		snake.head.y = 0
	}

	// Shift each body cell forward so it follows the previous segment
	for i in 0 ..< len(snake.body) {
		next := snake.body[i].position
		snake.body[i].position = prev_head
		prev_head = next
	}
}


@(private = "file")
is_opposite :: proc(a: Directions, b: Directions) -> bool {
	return(
		(a == .UP && b == .DOWN) ||
		(a == .DOWN && b == .UP) ||
		(a == .LEFT && b == .RIGHT) ||
		(a == .RIGHT && b == .LEFT) \
	)
}

@(private = "file")
check_three_in_a_row::proc(){ 

			if len(snake.body) > 0 && !is_animation_playing && pending_removals == 0 {
				tail_color := snake.body[len(snake.body) - 1].color
				run_length: i32 = 1
				for idx := len(snake.body) - 2; idx >= 0; idx -= 1 {
					if same_color(tail_color, snake.body[idx].color) {
						run_length += 1
					} else {
						break
					}
				}

				if run_length >= 3 {
					pending_removals = run_length
					removal_timer = 0.0
					is_animation_playing = true
				}
			}
}

@(private = "file")
update :: proc(delta: f32) {
    if hurt_cooldown <= 5 { 
        hurt_cooldown += delta
    }


	check_food_collision()
	check_body_collision()
	draw_snake()

	if (snake.updateTimer <= 0) {
		if len(direction_queue) > 0 {
			snake.direction = direction_queue[0]
			pop_front(&direction_queue)
		}

		move_snake()
		snake.updateTimer = 0.1

        check_three_in_a_row()
	}
    if is_animation_playing{ 
        handle_removals(delta)
        return 
    } 


	if rl.IsKeyPressed(.ESCAPE) {
		screen_manager.current_screen = .Settings
		screen_manager.previous_screen = .ConnectThree
		return
	}

	snake.updateTimer -= delta
	last_direction := snake.direction
	if len(direction_queue) > 0 {
		last_direction = direction_queue[len(direction_queue) - 1]
	}

	enqueue_direction :: proc(dir: Directions, last: ^Directions) {
		if is_opposite(dir, last^) || dir == last^ {
			return
		}
		append(&direction_queue, dir)
		last^ = dir
	}

	if rl.IsKeyPressed(.RIGHT) {
		enqueue_direction(.RIGHT, &last_direction)
	}
	if rl.IsKeyPressed(.LEFT) {
		enqueue_direction(.LEFT, &last_direction)
	}
	if rl.IsKeyPressed(.UP) {
		enqueue_direction(.UP, &last_direction)
	}
	if rl.IsKeyPressed(.DOWN) {
		enqueue_direction(.DOWN, &last_direction)
	}


	// Process collisions after movement so new tiles count immediately

	// Draw before removal timer so new segments are visible at least one frame

}
