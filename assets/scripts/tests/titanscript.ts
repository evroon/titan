func arithmetic_add()
	return 42 + 3

func arithmetic_subtract()
	return 42 - 3

func conditional_if()
	if (5 > 3)
		return "success"
	
	return false

func conditional_negate_if()
	if (5 < 3)
		return false
	
	return "success"

func conditional_if_else()
	if (5 < 3)
		return false
	else
		return "success"
	
	return false

func conditional_if_elseif()
	if (5 < 3)
		return false
	else if (5 > 3)
		return "success"
	
	return false

func conditional_if_elseifelse()
	if (5 < 3)
		return false
	elseif (6 < 3)
		return false
	else
		return "success"
	
	return false