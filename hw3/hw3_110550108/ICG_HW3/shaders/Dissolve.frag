#version 330 core

// Advanced:
// Implement Dissolve effect

uniform sampler2D ourTexture;

in vec2 texCoord;
in vec3 position;

uniform float dissolvePos;

out vec4 FragColor;

void main()
{
    vec4 textureColor = texture(ourTexture, texCoord);

    float dissolve = 1.0f;
    if (position.x < dissolvePos)
        dissolve = 0;

    FragColor = vec4(textureColor.rgb, textureColor.a * dissolve);
}