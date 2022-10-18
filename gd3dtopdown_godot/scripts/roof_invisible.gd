extends GD3Dvisual_obstacle


func _ready():
	var vis = Callable(self,"_visible_roof")
	var invis = Callable(self,"_invisible_roof")
	
	connect("visual_appear_signal",vis)
	connect("visual_disappear_signal",invis)

func _invisible_roof(_rid,_object):
	visible = false
func _visible_roof(_rid, _object):
	visible = true
