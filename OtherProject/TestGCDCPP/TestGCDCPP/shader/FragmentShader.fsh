#version 300 es
precision mediump float;
in vec3 Color;
in vec2 texCoord;
out vec4 fragmentColor;

uniform sampler2D s_texture;

void main() {
    fragmentColor = texture(s_texture, texCoord);
}