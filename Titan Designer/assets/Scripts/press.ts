extends TextButton

func ready()
	print("777")
	
func clicked()
	get_parent().get_child("TextBox").set_text("787")