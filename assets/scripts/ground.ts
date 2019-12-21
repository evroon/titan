extends Sprite

func ready()
	body = get_child("RigidBody2D")
	body.set_as_box(true)
	body.velocity = vec2(-100, 0)