#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;   // The default texture unit (location) for a texture is 0 which is the default active texture unit so we didn't need to assign a location with glUniform

void main()
{
    //FragColor = texture(ourTexture, TexCoord);
    FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);  
}