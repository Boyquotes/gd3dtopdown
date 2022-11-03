extends GD3Dtd_character

@onready var anim_tree = $AnimationTree
var SPEED = 5.0
var MOVE_LERP = 0.25

func _input(event):
	rotate_camera_mouse_aimlock(event)
func _physics_process(delta):
	var movement = Input.get_vector("move_left","move_right","move_forward","move_back")
	
	character_move(delta,movement,Input.is_action_pressed("aim"),SPEED,MOVE_LERP)
	
	anim_tree.set("parameters/BlendSpace1D/blend_position",get_real_velocity().length()/SPEED)
