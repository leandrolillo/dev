#version 120

attribute vec2 vertex;
attribute vec2 textureCoordinate;
attribute vec2 normal;
attribute vec3 color;


void main() {
	/* first transform the normal into eye space and normalize the result */

	vec3 normal = normalize(gl_NormalMatrix * gl_Normal);
	/* now normalize the light's direction. Note that according to the

	OpenGL specification, the light is stored in eye space. Also since
	we're talking about a directional light, the position field is actually
	direction */
	vec3 lightDir = normalize(vec3(gl_LightSource[0].position));
	/* compute the cos of the angle between the normal and lights direction.

	The light is directional so the direction is constant for every vertex.
	Since these two are normalized the cosine is the dot product. We also
	need to clamp the result to the [0,1] range. */
	float NdotL = max(dot(normal, lightDir), 0.0);
	/* Compute the diffuse term */

	vec4 diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
	
	//output variables
	gl_FrontColor =  vec4(color, 1.0) * (NdotL * diffuse);
	gl_Position = ftransform();
    gl_TexCoord[0] = vec4(textureCoordinate, 1.0, 1.0);
}

// void main()
// {
   // gl_FrontColor = vec4(color, 1.0);
   // gl_Position = ftransform();
   //gl_TexCoord[0] = vec4(textureCoordinate, 1.0, 1.0);
// }