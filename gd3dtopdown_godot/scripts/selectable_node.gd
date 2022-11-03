extends GD3Dselectable_node


func _ready():
	selected_signal.connect(selected)
	unselected_signal.connect(unselected)

func selected():
	print("selected")
func unselected():
	print("unselected")
