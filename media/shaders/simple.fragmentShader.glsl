#version 330

uniform sampler2D textureUnit;

in vec3 color_output;
in vec2 textureCoordinate_output;

out vec4 outputF;

void main()
{
    outputF = vec4(color_output, 1.0) * texture(textureUnit, textureCoordinate_output);
}
