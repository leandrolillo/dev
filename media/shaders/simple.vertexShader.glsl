#version 330

uniform mat4 pvm;

in vec4 vertex;
in vec3 normal;
in vec2 textureCoordinate;
in vec3 color;

out vec3 normal_output;
out vec3 color_output;
out vec2 textureCoordinate_output;
void main()
{
    gl_Position = pvm * vertex ;
    textureCoordinate_output =  textureCoordinate; //vec4(textureCoordinate, 1.0, 1.0);
    normal_output = normal; //normalize(gl_NormalMatrix  * normal);
    color_output = color;
}
