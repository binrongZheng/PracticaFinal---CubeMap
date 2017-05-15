#version 330 core
#define NUM_MAX_PLIGHTS 2
#define NUM_MAX_SLIGHTS 2

struct Material {
	sampler2D diffuse;
    sampler2D specular;
    float     shininess;
};  

struct PLight {
	vec3 lightPos;
	vec3 Lambient;
	vec3 Ldiffuse;
	vec3 Lspecular;	
	float Lconstant;
	float Llinear;
	float Lquadratic;
};

struct DLight {
	vec3 Lambient;
	vec3 Ldiffuse;
	vec3 Lspecular;	
	vec3 Ldirection;
};

struct SLight {
	vec3 lightPos;
	vec3 Lambient;
	vec3 Ldiffuse;
	vec3 Lspecular;	
	vec3 Ldirection;
	float Lconstant;
	float Llinear;
	float Lquadratic;
	float LcutOff;
	float LouterCutOff;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
out vec4 color;
  
uniform vec3 viewPos;
uniform Material material;
uniform DLight dlight;
uniform PLight plight[NUM_MAX_PLIGHTS];
uniform SLight slight[NUM_MAX_SLIGHTS];

vec3 DirectionalLight(DLight light, vec3 Normal, vec3 viewDirection);
vec3 PointLight(PLight light, vec3 Normal, vec3 viewDirection);
vec3 SpotLight(SLight light, vec3 Normal, vec3 viewDirection);

void main(){
	vec3 viewDir = normalize(viewPos - FragPos);
	color += vec4(DirectionalLight(dlight,Normal,viewDir),1);
	for (int i=0;i<NUM_MAX_PLIGHTS;i++){
		color += vec4(PointLight(plight[i],Normal,viewDir),1);
	}
	for (int i=0;i<NUM_MAX_SLIGHTS;i++){
		color += vec4(SpotLight(slight[i],Normal,viewDir),1);
	}
	
} 

vec3 DirectionalLight(DLight light, vec3 Normal, vec3 viewDirection){
	// Ambient
	vec3 ambient = light.Lambient* vec3(texture(material.diffuse, TexCoords));
   	vec3 lightDir = normalize(-light.Ldirection);
 
	// Diffuse 
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.Ldiffuse * diff * vec3(texture(material.diffuse, TexCoords));
    
    // Specular
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDirection, reflectDir), 0.0), material.shininess);
    vec3 specular = light.Lspecular * spec * vec3(texture(material.specular, TexCoords));  
    
	// Finally    
    vec3 colorD = (ambient + diffuse + specular);
	return colorD;
}

vec3 PointLight(PLight light, vec3 Normal, vec3 viewDirection){
 // Ambient
	vec3 ambient = light.Lambient* vec3(texture(material.diffuse, TexCoords));
   	vec3 lightDir = normalize(light.lightPos-FragPos);
 
 // Diffuse 
	vec3 norm=normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.Ldiffuse * diff * vec3(texture(material.diffuse, TexCoords));
    
    // Specular
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDirection, reflectDir), 0.0), material.shininess);
    vec3 specular = light.Lspecular * spec * vec3(texture(material.specular, TexCoords));  
    
	//Distance
	float distance    = length(light.lightPos - FragPos);
    float attenuation = 1.0f / (light.Lconstant + light.Llinear * distance + light.Lquadratic * (distance * distance));    

    ambient  *= attenuation;  
    diffuse  *= attenuation;
    specular *= attenuation;   
            
	// Finally    
    vec3 colorP = (ambient + diffuse + specular);
	return colorP;
}

vec3 SpotLight(SLight light, vec3 Normal, vec3 viewDirection){
	vec3 lightDir = normalize(light.lightPos-FragPos);
	float theta = dot(lightDir, normalize(-light.Ldirection));
	
	if(theta > light.LouterCutOff){
	 // Ambient
		vec3 ambient = light.Lambient* vec3(texture(material.diffuse, TexCoords));	
	
	// Diffuse 
		vec3 norm=normalize(Normal);
		
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = light.Ldiffuse * diff * vec3(texture(material.diffuse, TexCoords));

	// Specular
		vec3 reflectDir = reflect(-lightDir, norm);  
		float spec = pow(max(dot(viewDirection, reflectDir), 0.0), material.shininess);
		vec3 specular = light.Lspecular * spec * vec3(texture(material.specular, TexCoords));  
		
	//Distance
		
		float epsilon = light.LcutOff-light.LouterCutOff;
		float intensity =clamp((theta-light.LouterCutOff)/epsilon,0,1);

		diffuse  *= intensity;
		specular *= intensity;   
		
		float distance    = length(light.lightPos - FragPos);
		float attenuation = 1.0f / (light.Lconstant + light.Llinear * distance + light.Lquadratic * (distance * distance));    
		ambient  *= attenuation;
		diffuse  *= attenuation;
		specular *= attenuation;   
		
	// Finally    
		vec3 colorS = (ambient+diffuse+specular);
		return colorS;
   }
   else {
		vec3 ambient = light.Lambient* vec3(texture(material.diffuse, TexCoords));	
		float distance    = length(light.lightPos - FragPos);
		float attenuation = 1.0f / (light.Lconstant + light.Llinear * distance + light.Lquadratic * (distance * distance)); 
		ambient  *= attenuation;		
		vec3 colorS = (ambient);
		return colorS;
	}
	
}