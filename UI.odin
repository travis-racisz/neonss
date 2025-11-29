package main
import "core:fmt"
import "core:strings"
import rl "vendor:raylib"


start_button_color := rl.ColorAlpha(rl.RAYWHITE, 1.0)
quit_button_color := rl.ColorAlpha(rl.RAYWHITE, 1.0)
button_padding_top := 50


draw_main_menu :: proc() {
	mouse_pos := rl.GetMousePosition()
	start_button := rl.Rectangle {
		x      = f32(screen_width / 2 - 50),
		y      = f32(screen_height / 2 - 25),
		width  = 100,
		height = 50,
	}

	settings_button := rl.Rectangle {
		x      = f32(screen_width / 2) - 50,
		y      = f32(screen_height / 2) + f32(25 + button_padding_top),
		height = 50,
		width  = 100,
	}
	if rl.GuiButton(start_button, "START") {
		screen_manager.current_screen = .ConnectThree
		screen_manager.previous_screen = .MainMenu
	}

	if rl.GuiButton(settings_button, "SETTINGS") {
		screen_manager.current_screen = .Settings
		screen_manager.previous_screen = .MainMenu
	}

}


draw_connect_three_ui :: proc() {
	panel_w: f32 = 240
	panel_h := f32(screen_height) - 20
	panel_x := f32(screen_width) - panel_w - 10
	panel_y: f32 = 10
	panel := rl.Rectangle {
		x      = panel_x,
		y      = panel_y,
		width  = panel_w,
		height = panel_h,
	}

	rl.GuiPanel(panel, "")

	padding: f32 = 24
	section_y := panel_y + padding + 4

	// Next food preview
	rl.GuiLabel(rl.Rectangle{panel_x + padding, section_y, panel_w - padding * 2, 24}, "Next Food")
	section_y += 28
	cell_size: f32 = 40
	for food, i in grid.next_food {
		rect := rl.Rectangle {
			panel_x + padding,
			section_y + f32(i) * (cell_size + 10),
			cell_size,
			cell_size,
		}
		rl.DrawRectangleRec(rect, food.color)
		rl.DrawRectangleLinesEx(rect, 1, rl.BLACK)
	}
	if len(grid.next_food) == 0 {
		rect := rl.Rectangle{panel_x + padding, section_y, cell_size, cell_size}
		rl.DrawRectangleRec(rect, rl.Color{60, 60, 60, 200})
		rl.DrawRectangleLinesEx(rect, 1, rl.BLACK)
		rl.GuiLabel(
			rl.Rectangle {
				rect.x + cell_size + 8,
				rect.y + 8,
				panel_w - padding * 2 - cell_size - 8,
				24,
			},
			"No queued food",
		)
		section_y += cell_size + 10
	} else {
		section_y += f32(len(grid.next_food)) * (cell_size + 10)
	}

	// Hearts
	section_y += 12
	rl.GuiLabel(rl.Rectangle{panel_x + padding, section_y, panel_w - padding * 2, 24}, "Hearts")
	section_y += 28
	for i: i32 = 0; i < game_manager.heart; i += 1 {
		heart := rl.Rectangle {
			panel_x + padding + f32(i) * (cell_size * 0.8 + 6),
			section_y,
			cell_size * 0.8,
			cell_size * 0.8,
		}
		rl.DrawRectangleRec(heart, rl.Color{200, 60, 90, 255})
		rl.DrawRectangleLinesEx(heart, 1, rl.BLACK)
	}
	section_y += cell_size * 0.8 + 16

	// Points
	rl.GuiLabel(rl.Rectangle{panel_x + padding, section_y, panel_w - padding * 2, 24}, "Points")
	section_y += 26
	points_text := fmt.aprintf("%v", game_manager.points)
	rl.GuiLabel(rl.Rectangle{panel_x + padding, section_y, panel_w - padding * 2, 24}, strings.clone_to_cstring(points_text))
}


draw_settings :: proc() {
	quit_button_rect := rl.Rectangle {
		x      = f32(screen_width / 2) - 150,
		y      = f32(screen_height / 2) - 100,
		height = 100,
		width  = 150,
	}

	back_button := rl.Rectangle {
		x      = 10,
		y      = 10,
		height = 50,
		width  = 75,
	}


	if rl.GuiButton(quit_button_rect, "QUIT") {
		quit_game = true
	}

	if rl.GuiButton(back_button, "<-") {
		screen_manager.current_screen = screen_manager.previous_screen

	}

}


draw_game_over :: proc() { 
    rl.DrawText("GAME OVER", i32(screen_width / 2), i32(screen_height / 2), 60, rl.RAYWHITE)

}
