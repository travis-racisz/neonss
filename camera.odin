package main
import rl "vendor:raylib"


camera2d: rl.Camera2D
camera3d: rl.Camera3D


init_camera :: proc() {
	camera2d.offset = rl.Vector2{80, 30}
	camera2d.target = rl.Vector2{0, 0}
	camera2d.rotation = 0
	camera2d.zoom = 1 // keep pixel-perfect scale to avoid grid distortion
}
