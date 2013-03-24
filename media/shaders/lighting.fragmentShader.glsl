#version 120
uniform sampler2D color_texture;

varying vec4 diffuse, ambient, emission;
varying vec3 normalVector, halfVector;

void main(void)
{
    vec3 lightDir = vec3(gl_LightSource[0].position);
 
    vec4 color = ambient + emission;
    vec3 normal = normalize(normalVector);
 
	float NdotL = max(-dot(normal, lightDir), 0.0);
	
	if (NdotL > 0.0) {
        color += diffuse * NdotL;
        vec3 halfV = normalize(halfVector);
        float NdotHV = max(-dot(normal,halfV),0.0);
        color += gl_FrontMaterial.specular *
                gl_LightSource[0].specular *
                pow(NdotHV, gl_FrontMaterial.shininess);
    }
	
	gl_FragColor =  color;
   //gl_FragColor =  texture2D(color_texture, gl_TexCoord[0].st) * gl_Color;
}