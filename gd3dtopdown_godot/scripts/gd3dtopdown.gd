extends GD3Dtd_character

@onready var anim_tree = $AnimationTree
var SPEED = 5.0
var MOVE_LERP = 0.25
var TURN_SPEED = 1.0
var CAMERA_PREDICT = 1.0
var CAMERA_PREDICT_SPEED = 15.0

var INVERTED_CAMERA = true
var CAMERA_SENSIBILITY = 0.15
#Camera movements can be locked when aiming by using the *_aimlock functions
#if the function name includes *_mouse_* it handles the mouse movement
#if it doesn't a Vector2 can be passed as an argument for the displacement of the camera
func _unhandled_input(event):
	if(not Input.is_action_pressed("aim")):
		rotate_camera_mouse(event,CAMERA_SENSIBILITY,false)
	
func _physics_process(delta):
	var movement = Input.get_vector("move_left","move_right","move_forward","move_back")
	character_move(delta,movement,Input.is_action_pressed("aim"),SPEED,MOVE_LERP,
			TURN_SPEED,CAMERA_PREDICT,CAMERA_PREDICT_SPEED)

	anim_tree.set("parameters/BlendSpace1D/blend_position",get_real_velocity().length()/SPEED)
