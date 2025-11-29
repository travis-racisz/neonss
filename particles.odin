package main 
import rl "vendor:raylib"
import "core:math"

@(private = "file")
gravity: f32 = 0.1

Particle :: struct { 
    position: GridCell, 
    velocity: rl.Vector2,
    age: f32, 
    lifetime: f32, 
    color: rl.Color,
}

ParticleSpawner :: struct { 
    particles: [dynamic]Particle, 
    update: proc(delta: f32), 
}

particle_spawner: ParticleSpawner
init_particle_spawner :: proc(){ 
    particle_spawner.particles = make([dynamic]Particle)
    particle_spawner.update = update
}


spawn_particle :: proc(amount:int, spawn_location: GridCell, color: rl.Color){ 
    for i in 0..<amount{ 
        angle := rl.GetRandomValue(-360, 0)
        speed := rl.GetRandomValue(200, 400)
        vel_x := f32(speed) * math.cos_f32(f32(rl.DEG2RAD) * f32(angle))
        vel_y := f32(-speed) * math.sin_f32(f32(rl.DEG2RAD) * f32(angle))

        new_particle := Particle{ 
            position = spawn_location, 
            velocity = {vel_x ,vel_y}, 
            age = 0.0, 
            lifetime = f32(rl.GetRandomValue(0, 3)), 
            color = color
        }
        append(&particle_spawner.particles, new_particle)
    }
}

@(private = "file")
update :: proc(delta:f32){ 
    for &particle in particle_spawner.particles{ 
        particle.velocity.x += gravity * delta
        particle.velocity.y += gravity * delta

        particle.position.x += i32(particle.velocity.x * delta) 
        particle.position.y += i32(particle.velocity.y * delta)

        particle.age += delta 
    }

    for p, i in particle_spawner.particles { 
        if p.age >= p.lifetime{ 
            unordered_remove(&particle_spawner.particles, i)
        }
    }
    draw()

}


@(private = "file")
draw :: proc() { 
    for p in particle_spawner.particles{ 
        rl.DrawCircle(i32(p.position.x), i32(p.position.y), 2, p.color)
    }
}