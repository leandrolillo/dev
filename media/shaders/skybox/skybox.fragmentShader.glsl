#version 330

uniform samplerCube textureUnit;

in VertexData {
	vec3 textureCoordinate;
} inputData;

out vec4 fragmentColor;

void main()
{
	fragmentColor = texture(textureUnit, inputData.textureCoordinate);
}
