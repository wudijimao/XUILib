#version 300 es
precision mediump float;

in vec4 Color;
in vec2 texCoord;
out vec4 fragmentColor;

uniform bool useTexture;
uniform sampler2D s_texture;

void main() {
if(useTexture) {
        fragmentColor = texture(s_texture, texCoord);
        //alpha blend  fragmentColor = vec4(fragmentColor.r * fragmentColor.a + Color.r * Color.a * (1.0f - fragmentColor.a), fragmentColor.g * fragmentColor.a + Color.g * Color.a * (1.0f - fragmentColor.a), fragmentColor.b * fragmentColor.a + Color.b * Color.a * (1.0f - fragmentColor.a), 1.0f - (1.0f - fragmentColor.a) * (1.0f - Color.a));
   } else {
       fragmentColor = Color;
   }
}