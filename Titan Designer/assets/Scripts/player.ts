extends Sprite

define W_KEY Key("W")
define S_KEY Key("S")
define D_KEY Key("D")
define A_KEY Key("A")

define up_vector vec2(0, 1)
define down_vector vec2(0, -1)
define right_vector vec2(1, 0)
define left_vector vec2(-1, 0)

define SPEED 2.0

func ready()
	pos = vec2(0, 400)
	
	pc = PhysicsComponent()
	physicscomponent = pc
	pc.set_as_box(true)
	pc.fixed_rotation = true

func update()
	print("test22")