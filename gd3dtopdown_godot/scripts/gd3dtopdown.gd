extends GD3Dtd_character

@onready var anim_tree = $AnimationTree
func _ready():
	character_init()
func _input(event):
	rotate_camera_mouse_aimlock(event)
	#rotate_camera_mouse(event)
func _physics_process(delta):
	var movement = Input.get_vector("move_left","move_right","move_forward","move_back")
	character_move(delta,movement,Input.is_action_pressed("aim"),
		Input.is_action_pressed("sprint"))
	#anim_tree.set("parameters/BlendSpace1D/blend_position",get_real_velocity().length()/get_last_speed())
