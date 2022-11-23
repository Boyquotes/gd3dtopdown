extends SelectableGD3D


func _ready():
	selected_signal.connect(selected)
	unselected_signal.connect(unselected)

func selected():
	print("selected")
func unselected():
	print("unselected")
