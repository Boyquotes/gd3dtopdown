shader_type spatial;
uniform float ammount : hint_range(0.0, 1.0);
uniform sampler2D noise;
uniform vec4 current_color : source_color;

void fragment() {
	float noise_sample = texture(noise,UV).r;
	ALBEDO = current_color.xyz;
	ALPHA = step(noise_sample,1.0-ammount);
	ALPHA_SCISSOR_THRESHOLD = 0.1;
}
