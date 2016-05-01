#version 330
precision mediump float;

in vec4 oColor;

out vec4 fragmentColor;

void main() {
	fragmentColor = oColor;
}