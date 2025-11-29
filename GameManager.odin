package main 

GameManager :: struct { 
    heart: i32, 
    points: i32, 
    update: proc(delta: f32)
}

game_manager: GameManager

init_game_manager :: proc() { 
    game_manager.heart = 3
    game_manager.points = 0 
    game_manager.update = update
}


@(private = "file")
update :: proc(delta: f32){ 
    if game_manager.heart <= 0 { 
        screen_manager.current_screen = .GameOver
    }

}


