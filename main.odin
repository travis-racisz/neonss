package main
import rl "vendor:raylib"

screen_height: i32 = 800
screen_width: i32 = 1200
game_target: rl.RenderTexture2D
tint_overlay: rl.Color = rl.Color{30, 20, 50, 0} // subtle dark tint to soften brightness


main :: proc() {

	rl.SetConfigFlags(
		rl.ConfigFlags {
			rl.ConfigFlag.WINDOW_MAXIMIZED,
			rl.ConfigFlag.WINDOW_RESIZABLE,
			rl.ConfigFlag.WINDOW_HIGHDPI,
			rl.ConfigFlag.MSAA_4X_HINT,
			rl.ConfigFlag.VSYNC_HINT,
		},
	)
	rl.InitWindow(screen_width, screen_height, "NEONSS")
    rl.GuiLoadStyle("./assets/theme/theme.rgs")
	screen_width = rl.GetScreenWidth()
	screen_height = rl.GetScreenHeight()

	// Render target used for post-processing
	game_target = rl.LoadRenderTexture(screen_width, screen_height)
	rl.SetTextureFilter(game_target.texture, .POINT)

	bloom_shader := rl.LoadShader("", "shaders/bloom.fs")

	rl.SetTargetFPS(60)
	init_camera()
	make_grid()
	init_snake()
    init_game_manager()
    init_particle_spawner()


	for !quit_game {
		delta := rl.GetFrameTime()
		// Draw the game world into the render texture
		rl.BeginTextureMode(game_target)
		rl.ClearBackground(rl.BLACK)
        // SCREEN MANAGER STUFF 
        switch screen_manager.current_screen{ 
            case .MainMenu: 
                draw_main_menu()
                break
            case .ConnectThree: 
                draw_connect_three_ui()
		        rl.BeginMode2D(camera2d)

		        grid.update(delta)
		        snake.update(delta)
                particle_spawner.update(delta)
                game_manager.update(delta)
                
                

                break
            case .Settings: 
                draw_settings()
                break 
            case .GameOver: 
                break
        }



		rl.EndMode2D()
		rl.EndTextureMode()

		// Render the texture to the screen (flipped vertically)
		rl.BeginDrawing()
		rl.BeginShaderMode(bloom_shader)
		rl.ClearBackground(rl.BLACK)
		src := rl.Rectangle {
			0,
			0,
			f32(game_target.texture.width),
			f32(-game_target.texture.height), // flip vertically
		}
		dst := rl.Rectangle{0, 0, f32(screen_width), f32(screen_height)}
		rl.DrawTexturePro(game_target.texture, src, dst, rl.Vector2{0, 0}, 0, rl.WHITE)
		rl.DrawFPS(10, 10)
		rl.EndShaderMode()
		rl.DrawRectangle(0, 0, screen_width, screen_height, tint_overlay)
		rl.EndDrawing()

	}

	rl.UnloadRenderTexture(game_target)
}
