extends GD3Dselectable_node


# Called when the node enters the scene tree for the first time.
func _ready():
	self.selected_signal.connect(selected)
	unselected_signal.connect(unselected)

# Called every frame. 'delta' is the elapsed time since the previous frame.
func selected():
	print("selected")
func unselected():
	print("unselected")
