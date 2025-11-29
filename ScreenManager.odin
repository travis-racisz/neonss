package main

Screens :: enum {
	MainMenu,
	ConnectThree,
	Settings,
	GameOver,
}

ScreenManager :: struct {
	current_screen:  Screens,
	previous_screen: Screens,
	update:          proc(delta: f32),
}

screen_manager: ScreenManager
quit_game := false

init_game_state :: proc() {
	screen_manager.current_screen = .MainMenu
    screen_manager.previous_screen = .MainMenu
	screen_manager.update = update
}


@(private = "file")
update :: proc(delta: f32) {

}
