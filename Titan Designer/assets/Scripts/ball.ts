extends Sprite

func ready()
	body = get_child("RigidBody2D")
	body.set_as_circle(true)
	body.velocity = vec2(-50, 0)