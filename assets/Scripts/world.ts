extends World

func ready()
	body = get_child("RigidBody2D")
	body.set_as_box(false)
	
	get_parent().get_child("Label").text = "je moeder"