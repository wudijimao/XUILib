#version 300 es

precision mediump float;

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 vTexCoord;
layout(location = 2) in vec4 inColor;

out vec4 Color;
out vec2 texCoord;
out vec4 pos;

uniform mat4 myMat;
uniform vec4 uClipsBounds;
uniform bool uIsClipsToBounds;




void main() {
    vec4 tempPos = vec4(inPos, 1.0);
//    if (uIsClipsToBounds) {
//        if (tempPos.x < uClipsBounds.x) {
//            tempPos.x = uClipsBounds.x;
//        }
//    }
    gl_Position = tempPos * myMat;
    texCoord = vTexCoord;
    Color = inColor;
}