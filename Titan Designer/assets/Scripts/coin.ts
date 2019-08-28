extends Sprite

define K_KEY Key("K")
define L_KEY Key("L")

func ready()
	body = get_child("RigidBody2D")
	body.set_as_box(true)

func update()
	if (Keyboard.is_key_pressed(K_KEY))
		pos = pos - vec3(0.1, 0, 0)
	if (Keyboard.is_key_pressed(L_KEY))
		pos = pos + vec3(0.1, 0, 0)
