extends Sprite

func ready()
	pos = vec2(70, -400)
	size = vec2(300, 50)
	bounds = vec4(0.0, 6.0, 0.0, 1.0)
	
	pc = PhysicsComponent()
	physicscomponent = pc
	pc.set_as_box(false)
	