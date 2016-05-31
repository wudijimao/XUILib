#version 300 es

precision mediump float;

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 vTexCoord;
layout(location = 2) in vec4 inColor;

out vec4 Color;
out vec2 texCoord;
out vec4 pos;

uniform mat4 canvasMat;
uniform mat4 viewMat;


void main() {
    vec4 tempPos = vec4(inPos, 1.0);
//    if (uIsClipsToBounds) {
//        if (tempPos.x < uClipsBounds.x) {
//            vTexCoord.u = 
//            tempPos.x = uClipsBounds.x;
//        } else if(tempPos.x > uClipsBounds.y) {
//            tempPos.x = uClipsBounds.y;
//        }
//        if (tempPos.y < uClipsBounds.z) {
//            tempPos.y = uClipsBounds.z;
//        } else if(tempPos.y > uClipsBounds.w) {
//            tempPos.y = uClipsBounds.w;
//        }
//    }
    pos = tempPos * viewMat;
    gl_Position = pos * canvasMat;
    texCoord = vTexCoord;
    Color = inColor;
}