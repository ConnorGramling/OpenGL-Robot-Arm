#version 120

attribute vec4 vPosition;
attribute vec4 vColor;
attribute vec4 vNormal;

varying vec4 color;
varying vec4 N, V, L;
varying vec2 texCoord;

uniform mat4 ctm, model_view, projection;

uniform vec4 light_position;

void main()
{
	N = normalize(model_view * ctm * vNormal);
	L = normalize(model_view * (light_position - ctm * vPosition));
	V = normalize(vec4(0, 0, 0, 1) - model_view * ctm * vPosition);

	color = vColor;
	gl_Position = projection * model_view * ctm * vPosition;
}
