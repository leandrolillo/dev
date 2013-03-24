#version 120

attribute vec3 vertex;
attribute vec2 textureCoordinate;
attribute vec3 normal;
attribute vec3 color;


varying vec4 diffuse, ambient, emission;
varying vec3 normalVector,halfVector;

void main() {
	normalVector = normalize(gl_NormalMatrix * gl_Normal);
	halfVector = gl_LightSource[0].halfVector.xyz;
	
	ambient = gl_FrontMaterial.ambient * gl_LightModel.ambient + gl_LightSource[0].ambient * gl_LightModel.ambient;
	emission = gl_FrontMaterial.emission;
	diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
	
//	vec4 specular;
	// if (NdotL > 0.0) {
		// // normalize the half-vector, and then compute the cosine (dot product) with the normal
		// float NdotHV = max(dot(normalVector, normalize(gl_LightSource[0].halfVector.xyz)),0.0);
		// specular = gl_FrontMaterial.specular * gl_LightSource[0].specular * pow(NdotHV,gl_FrontMaterial.shininess);
		// diffuse = NdotL * gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
	// }
	
	//output variables
	//gl_FrontColor =  ambient + emission + diffuse + specular;
	gl_Position = ftransform();
    gl_TexCoord[0] = vec4(textureCoordinate, 1.0, 1.0);
}