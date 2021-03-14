#version 330

uniform mat4 pvm;
uniform mat4 normalMatrix;

in vec3 vertex;
in vec3 normal;
in vec2 textureCoordinate;
in vec3 color;

out vec4 normal_output;
out vec3 color_output;
out vec2 textureCoordinate_output;
void main()
{
    gl_Position = pvm * vec4(vertex, 1.0f) ;
    textureCoordinate_output =  textureCoordinate; //vec4(textureCoordinate, 1.0, 1.0);
    normal_output = normalize(normalMatrix  * vec4(normal, 0.0f));
    color_output = color;
}
