#version 300 es
precision mediump float;

in vec4 Color;
in vec2 texCoord;
out vec4 fragmentColor;

uniform bool useTexture;
uniform sampler2D s_texture;

void main() {
    fragmentColor = vec4(1,1,0,1);
}