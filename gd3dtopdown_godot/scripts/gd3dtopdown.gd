extends GD3Dtd_character

@onready var anim_tree = $AnimationTree
var SPEED = 5.0
var MOVE_LERP = 0.25

#If the _enter_tree function is declared it overrides the _ready function
#should this be needed, dont forget to initialize the character with character_init()

#_enter _tree():
#	character_init()	

#Camera movements can be locked when aiming by using the *_aimlock functions
#if the function name includes *_mouse_* it handles the mouse movement
#if it doesn't a Vector2 can be passed as an argument for the displacement of the camera
func _unhandled_input(event):
	rotate_camera_mouse_aimlock(event)
#here the character_move function is used, it takes the delta, a Vector2 as movement
#wether the character is aiming to the mouse position, the movement speed and a
#smoothing value
func _physics_process(delta):
	var movement = Input.get_vector("move_left","move_right","move_forward","move_back")
	
	character_move(delta,movement,Input.is_action_pressed("aim"),SPEED,MOVE_LERP)
	
	anim_tree.set("parameters/BlendSpace1D/blend_position",get_real_velocity().length()/SPEED)
