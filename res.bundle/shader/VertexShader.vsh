#version 300 es

precision mediump float;

in vec3 inPos;
in vec2 vTexCoord;
in vec4 inColor;

out vec4 Color;
out vec2 texCoord;

uniform mat4 myMat;

void main() {
    gl_Position = vec4(inPos, 1.0) * myMat;
    texCoord = vTexCoord;
    Color = inColor;
}