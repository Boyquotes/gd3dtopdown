extends GD3Dtopdown

@onready var cam = $Camera3d
func _ready():
	set_camera_transform(cam.transform)
	
func _input(_event):
	_input_handle(_event)
func _physics_process(_delta):
	move_and_slide()
