extends Sprite

define K_KEY Key("K")
define L_KEY Key("L")
define SPACE_KEY Key("J")
define speed 5

func ready()
	body = get_child("RigidBody2D")
	body.set_as_box(true)

func update()
	if (Keyboard.is_key_pressed(K_KEY))
		pos = pos - vec3(speed, 0, 0)
	
	if (Keyboard.is_key_pressed(L_KEY))
		pos = pos + vec3(speed, 0, 0)
	
	if (Keyboard.is_key_pressed(SPACE_KEY))
		body.apply_force(vec2(0.0, 5000.0))
