package main
import "core:fmt"
import "core:math/rand"
import rl "vendor:raylib"

GRID_SIZE :: 20
CELL_SIZE :: 35

GridCell :: struct {
	x: i32,
	y: i32,
}

Food :: struct {
	position: GridCell,
	color:    rl.Color,
}

Colors := [4]rl.Color{rl.RED, rl.BLUE, rl.GREEN, rl.WHITE}

Grid :: struct {
	gridCells: [dynamic]GridCell,
	food:      [dynamic]Food,
    next_food: [dynamic]Food,
	update:    proc(delta: f32),
}

grid: Grid

make_grid :: proc() {
	grid.gridCells = make([dynamic]GridCell)
	grid.food = make([dynamic]Food)
	grid.next_food = make([dynamic]Food)
    

	for i: i32 = 0; i <= GRID_SIZE; i += 1 {
		for j: i32 = 0; j <= GRID_SIZE; j += 1 {
			append(&grid.gridCells, GridCell{i, j})
		}
	}

	for i in 0 ..< 3 {
		new_food := Food {
			position = random_cell(),
			color    = random_color(),
		}

		append(&grid.food, new_food)
	}
	// Pre-populate next queue so UI has data on first frame
	for i in 0 ..< 3 {
		new_food := Food{
			position = random_cell(),
			color    = random_color(),
		}
		append(&grid.next_food, new_food)
	}

	grid.update = update

}

random_color :: proc() -> rl.Color {
	rand_int := rand.int31_max(4)
	return Colors[rand_int]
}

random_non_white_color :: proc() -> rl.Color { 
    rand_int := rand.int31_max(3)
    return Colors[rand_int]
}

random_cell :: proc() -> GridCell {
	randx := rand.int31_max(GRID_SIZE)
	randy := rand.int31_max(GRID_SIZE)

	return GridCell{x = randx, y = randy}
}


check_bounds :: proc(cell: GridCell) -> bool {
	if cell.x < 0 || cell.x > GRID_SIZE || cell.y < 0 || cell.y > GRID_SIZE {
		return true
	} else {
		return false
	}

}

to_grid :: proc(gridCell: GridCell) -> GridCell {
	cell_x := gridCell.x / CELL_SIZE
	cell_y := gridCell.y / CELL_SIZE

	return GridCell{cell_x, cell_y}
}

to_map :: proc(gridCell: GridCell) -> GridCell {
	cell_x := gridCell.x * CELL_SIZE
	cell_y := gridCell.y * CELL_SIZE

	return GridCell{cell_x, cell_y}
}

@(private = "file")
draw_grid :: proc() {
	for &cell in grid.gridCells {
		rl.DrawRectangleLines(to_map(cell).x, to_map(cell).y, CELL_SIZE, CELL_SIZE, rl.WHITE)
	}

	for food in grid.food {
		rl.DrawRectangle(
			to_map(food.position).x,
			to_map(food.position).y,
			CELL_SIZE,
			CELL_SIZE,
			food.color,
		)
	}
}

spawn_food :: proc() {
	if len(grid.next_food) == 0 {
		for i in 0 ..< 3 {
			new_food := Food{
				position = random_cell(),
				color    = random_color(),
			}
			append(&grid.next_food, new_food)
		}
	}

	for food in grid.next_food {
		new_food := Food {
			position = food.position,
			color    = food.color,
		}
		append(&grid.food, new_food)
	}

	clear(&grid.next_food)
	for i in 0 ..< 3 {
		new_food := Food{
			position = random_cell(),
			color    = random_color(),
		}
		append(&grid.next_food, new_food)
	}
}

@(private = "file")
update :: proc(delta: f32) {
	draw_grid()

	if len(grid.food) == 0 {
		spawn_food()
	}


}
