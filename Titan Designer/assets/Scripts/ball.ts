extends Sprite

reset_flag = false
acc_flag = false

func ready()
	body = get_child("RigidBody2D")
	body.set_as_circle(true)
	reset()
	
func reset()
	pos = vec3(0.0, 0.0, 10.0)
	body.velocity = vec2(-50, 0)
	
func update()
	if (reset_flag)
		reset()
		reset_flag = false
		
	if (acc_flag)
		body.velocity = body.velocity * vec2(1.1)
		acc_flag = false
	

func handle_event(event)
	if (get_type(event) == "CollisionEvent")
		object = event.get_object().get_name()
	
		if (object == "Wall3")
			get_parent().get_parent().get_child("Label").text = "Player lost"
			reset_flag = true
		if (object == "Wall4")
			get_parent().get_parent().get_child("Label").text = "Player won"
			reset_flag = true
		if (object == "Player" || object == "Opponent")
			acc_flag = true
