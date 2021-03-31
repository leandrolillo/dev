#version 330

struct Matrices {
	mat4 pv;
};
uniform Matrices matrices;

uniform float boxSize = 300;

in vec3 position;

out VertexData {
	vec3 textureCoordinate;
} outputData;

void main()
{
    gl_Position = matrices.pv * vec4(position * boxSize, 1.0f);
    outputData.textureCoordinate =  position;
}
