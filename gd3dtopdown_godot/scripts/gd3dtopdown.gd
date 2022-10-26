extends GD3Dtd_character
func _ready():
	character_init()
func _input(event):
	rotate_camera_mouse_aimlock(event)
	#rotate_camera_mouse(event)
func _physics_process(delta):
	var movement = Input.get_vector("move_left","move_right","move_forward","move_back")
	character_move(delta,movement,Input.is_action_pressed("aim"),
		Input.is_action_pressed("sprint"))
	
