extends Sprite

func ready()
	pc = PhysicsComponent()
	physicscomponent = pc
	pc.set_as_box(false)
	pc.set_as_sensor(true)
	pos = vec2(200, -325)

sound = ContentManager.Load("Sounds/coin.wav")

func update()
	if (physicscomponent.get_colliding_objects())
		free()
	sound.Play()
