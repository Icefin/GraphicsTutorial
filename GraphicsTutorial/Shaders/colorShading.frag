#version 130

in vec2 fragmentPosition;
in vec4 fragmentColor;
in vec2 fragmentUV;

out vec4 color;

uniform sampler2D mySampler;

void main(){
	vec4 textureColor = texture(mySampler, fragmentUV);

	color = fragmentColor * textureColor;
}