#version 300 es
precision mediump float;

in vec4 Color;
in vec2 texCoord;
out vec4 fragmentColor;

uniform bool useTexture;
uniform sampler2D s_texture;

void main() {
    if(useTexture) {
        fragmentColor = texture(s_texture, texCoord) * Color;
    } else {
        fragmentColor = vec4(1,0,0,1);
    }
}