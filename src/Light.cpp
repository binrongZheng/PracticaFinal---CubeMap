#include "Light.h"

Light::Light(vec3 pos, vec3 dir, vec3 ambient, vec3 diffuse, vec3 specular, LType lType, int number) {
	Lambient=ambient;
	Ldiffuse=diffuse;
	Lspecular=specular;
	LightType=lType;
	lightNumber = number;
	switch (lType) {
	case DIRECTIONAL:
		Ldirection = dir;
		break;
	case POINT:
		Lpos = pos;
		break;
	case SPOT:
		Ldirection = dir;
		Lpos = pos;
		break;
	default:
		break;
	}
}

void Light::SetAtt(float constant, float lineal, float quadratic) {
	c1 = constant;
	c2 = lineal;
	c3 = quadratic;
}

void Light::SetAperture(float min, float max) {
	MinAperture = min;
	MaxAperture = max;
}

void Light::SetLight(Shader *shad, vec3 CamPos) {
	std::string variable;
	glUniform3f(glGetUniformLocation(shad->Program, "viewPos"), CamPos.x, CamPos.y, CamPos.z);
	switch (LightType){
	case DIRECTIONAL:
		glUniform3f(glGetUniformLocation(shad->Program, "dlight.Ldirection"), Ldirection.x, Ldirection.y, Ldirection.z);
		glUniform3f(glGetUniformLocation(shad->Program, "dlight.Lambient"), Lambient.x, Lambient.y, Lambient.z);
		glUniform3f(glGetUniformLocation(shad->Program, "dlight.Ldiffuse"), Ldiffuse.x, Ldiffuse.y, Ldiffuse.z);
		glUniform3f(glGetUniformLocation(shad->Program, "dlight.Lspecular"), Lspecular.x, Lspecular.y, Lspecular.z);
		break;
	case POINT:		
		variable = "plight[" + std::to_string(lightNumber) + "]";	
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".lightPos").c_str()), Lpos.x, Lpos.y, Lpos.z);
		glUniform3f(glGetUniformLocation(shad->Program, (variable+".Lambient").c_str()),  Lambient.x,Lambient.y,Lambient.z );
		glUniform3f(glGetUniformLocation(shad->Program, (variable+".Ldiffuse").c_str()), Ldiffuse.x,Ldiffuse.y,Ldiffuse.z);
		glUniform3f(glGetUniformLocation(shad->Program, (variable+".Lspecular").c_str()), Lspecular.x,Lspecular.y,Lspecular.z);
		glUniform1f(glGetUniformLocation(shad->Program, (variable+".Lconstant").c_str()), c1);
		glUniform1f(glGetUniformLocation(shad->Program, (variable+".Llinear").c_str()), c2);
		glUniform1f(glGetUniformLocation(shad->Program, (variable+".Lquadratic").c_str()), c3);

		break;
	case SPOT:
		variable = "slight[" + std::to_string(lightNumber) + "]";
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".lightPos").c_str()), Lpos.x, Lpos.y, Lpos.z);
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".Ldirection").c_str()), Ldirection.x, Ldirection.y, Ldirection.z);
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".Lambient").c_str()), Lambient.x, Lambient.y, Lambient.z);
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".Ldiffuse").c_str()), Ldiffuse.x, Ldiffuse.y, Ldiffuse.z);
		glUniform3f(glGetUniformLocation(shad->Program, (variable + ".Lspecular").c_str()), Lspecular.x, Lspecular.y, Lspecular.z);
		glUniform1f(glGetUniformLocation(shad->Program, (variable + ".Lconstant").c_str()), c1);
		glUniform1f(glGetUniformLocation(shad->Program, (variable + ".Llinear").c_str()), c2);
		glUniform1f(glGetUniformLocation(shad->Program, (variable + ".Lquadratic").c_str()), c3);
		glUniform1f(glGetUniformLocation(shad->Program, (variable + ".LcutOff").c_str()), MinAperture);
		glUniform1f(glGetUniformLocation(shad->Program, (variable + ".LouterCutOff").c_str()), MaxAperture);
		break;
	default:
		break;
	}	
}


void Light::Rotate(vec3 rotation) {	
//opcional
}

void Light::SetDirection(vec3 dir) {
	Ldirection = dir;
}

void Light::SetPosition(vec3 pos) {
	Lpos = pos;
}

vec3 Light::GetColor() {
	//media de las luces
	vec3 color;
	color = Ldiffuse + Lspecular + Lambient;
	return color;
}