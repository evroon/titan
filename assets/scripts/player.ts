extends Sprite

define W_KEY Key("W")
define S_KEY Key("S")

define speed 1.0

func ready()
	body = get_child("RigidBody2D")
	body.set_as_box(true)	

func update()
	if (Keyboard.is_key_pressed(W_KEY))
		pos = vec3(pos.x, min(+31.0, pos.y + speed), pos.z)

	if (Keyboard.is_key_pressed(S_KEY))
		pos = vec3(pos.x, max(-31.0, pos.y - speed), pos.z)
