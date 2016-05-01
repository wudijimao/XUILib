#version 330
precision mediump float;

in vec4 inColor;
in vec3 inPos;

out vec4 oColor;

void main() {
	oColor = inColor;
    gl_Position = vec4(inPos,1);
}