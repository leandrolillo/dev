#version 120
uniform sampler2D color_texture;

varying vec3 normalVector;

void main(void)
{
	vec4 ambient = gl_FrontMaterial.ambient * gl_LightModel.ambient + gl_LightSource[0].ambient * gl_LightModel.ambient;
	vec4 emission = gl_FrontMaterial.emission;
	vec4 diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
	vec4 specular = gl_FrontMaterial.specular * gl_LightSource[0].specular;
	float shininess = gl_FrontMaterial.shininess;
	
    vec3 lightDir = normalize(vec3(gl_LightSource[0].position));
	vec3 halfVector = normalize(vec3(gl_LightSource[0].halfVector));
    vec3 normal = normalVector;
 
	float NdotL = max(dot(normal, lightDir), 0.0);
	float NdotHV = max(dot(normal, halfVector),0.0);
	vec4 color =  	ambient 
					+ emission 
					+ diffuse * NdotL
					+ specular * pow(NdotHV, shininess);

	gl_FragColor =  color;
    //gl_FragColor =  texture2D(color_texture, gl_TexCoord[0].st) * color;
}