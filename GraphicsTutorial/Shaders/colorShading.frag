#version 130

in vec2 fragmentPosition;
in vec4 fragmentColor;

out vec4 color;

uniform float time;

void main(){
	color = vec4(fragmentColor.r * (cos(fragmentPosition.x * 16.0 + time) + 1.0),
				 fragmentColor.g * (cos(fragmentPosition.y * 8.0 + time) + 1.0),
				 fragmentColor.b * (cos(fragmentPosition.x * 2.0 + time) + 1.0),
				 fragmentColor.a);
}