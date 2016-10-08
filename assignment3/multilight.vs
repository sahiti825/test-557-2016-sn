#version 330 core

// The vertex buffer input                                    
in vec3 in_Color; 
in vec3 in_Position; 
in vec3 in_Normal;  

// Transformations for the projections
uniform mat4 projectionMatrixBox;
uniform mat4 viewMatrixBox;
uniform mat4 modelMatrixBox;
uniform mat4 inverseViewMatrix;

// The material parameters 
uniform vec3 diffuse_color;                                        
uniform vec3 ambient_color;                                         
uniform vec3 specular_color;   

// Position of the light source
uniform vec4 light_position;  
uniform vec4 light_position2; 

// New parameters for the spotlight

uniform float cone_angle2;
uniform vec3 cone_direction2;

// The intensity values for the reflection equations
uniform float diffuse_intensity;                                          
uniform float ambient_intensity;
uniform float specular_intensity;                                       
uniform float shininess;    
uniform float attenuationCoefficient;    

uniform float diffuse_intensity2;                                          
uniform float ambient_intensity2;
uniform float specular_intensity2;                                       
uniform float shininess2;    
uniform float attenuationCoefficient2; 
                                
                                              



// The output color
out vec3 pass_Color;                                            
                                                                 
                                                             
                                                                                                                               
void main(void)                                                 
{                                                               
	vec3 normal = normalize(in_Normal);                                                                   
    vec4 transformedNormal = normalize(transpose(inverse( modelMatrixBox)) * vec4( normal, 1.0 ));
    
    vec4 surfacePostion = modelMatrixBox * vec4(in_Position, 1.0);
                                                                                                       
    vec4 surface_to_light =   normalize( light_position -  surfacePostion );   
    vec4 surface_to_light2 =   normalize( light_position2 -  surfacePostion ); 	
                                                                                                            
    // Diffuse color                                                                                          
    float diffuse_coefficient = max( dot(transformedNormal, surface_to_light), 0.0);
	float diffuse_coefficient2 = max( dot(transformedNormal, surface_to_light2), 0.0);
    vec3 out_diffuse_color = diffuse_color  * diffuse_coefficient * diffuse_intensity;
    vec3 out_diffuse_color2 = diffuse_color  * diffuse_coefficient2 * diffuse_intensity2;	
                                                                                                              
    // Ambient color                                                                                         
    vec3 out_ambient_color = vec3(ambient_color) * ambient_intensity;  
    vec3 out_ambient_color2 = vec3(ambient_color) * ambient_intensity2;	
                                                                                                             
    // Specular color                                                                                        
    vec3 incidenceVector = -surface_to_light.xyz;
    vec3 reflectionVector = reflect(incidenceVector, normal.xyz);
	
	vec3 incidenceVector2 = -surface_to_light2.xyz;
    vec3 reflectionVector2 = reflect(incidenceVector2, normal.xyz);    
    
	vec3 cameraPosition = vec3( inverseViewMatrix[3][0], inverseViewMatrix[3][1], inverseViewMatrix[3][2]);
    vec3 surfaceToCamera = normalize(cameraPosition - surfacePostion.xyz);
    
    float cosAngle = max(0.0, dot(surfaceToCamera, reflectionVector));
    float specular_coefficient = pow(cosAngle, shininess);                                                     

	float cosAngle2 = max(0.0, dot(surfaceToCamera, reflectionVector2));
    float specular_coefficient2 = pow(cosAngle2, shininess);                                                     

	
    vec3 out_specular_color = specular_color * specular_coefficient * specular_intensity; 
    vec3 out_specular_color2 = specular_color * specular_coefficient2 * specular_intensity2;	
  
	
	//attenuation
    float distanceToLight = length(light_position.xyz - surfacePostion.xyz);
    float attenuation = 1.0 / (1.0 + attenuationCoefficient * pow(distanceToLight, 2));
	//attenuation
    float distanceToLight2 = length(light_position2.xyz - surfacePostion.xyz);
    float attenuation2 = 1.0 / (1.0 + attenuationCoefficient2 * pow(distanceToLight, 2));
	
	
	//////////////////////////////////////////////////////////////////////////////////////////////        
    // Spotlight
    // 1. Normalize the cone direction
	vec3 cone_direction_norm2 = normalize(cone_direction2);
    
    // 2. Calculate the ray direction. We already calculated the surface to light direction.
    // 	  All what we need to do is to inverse this value
    vec3 ray_direction2 = -surface_to_light2.xyz;   
    
    // 3. Calculate the angle between light and surface using the dot product again. 
    //    To simplify our understanding, we use the degrees
	float light_to_surface_angle2 = degrees(acos(dot(ray_direction2, cone_direction_norm2))) ;
    
    // 4. Last, we compare the angle with the current direction and 
    //    reduce the attenuation to 0.0 if the light is outside the angle. 
	if(light_to_surface_angle2 > cone_angle2){
		float out_angle=30.0;
		float x = out_angle-cone_angle2;
  		attenuation2 = 1.0 /(5.0 + 3.0*pow(x,2.0)-2*pow(x,3.0));
	}
	
	
	// Calculate the linear color
	vec3 linearColor = out_ambient_color  + attenuation * ( out_diffuse_color + out_specular_color); 
    vec3 linearColor2 = out_ambient_color2  + attenuation2 * ( out_diffuse_color2 + out_specular_color2);	
   
	// Gamma correction	
	vec3 gamma = vec3(1.0/2.2);
	vec3 finalColor = pow(linearColor, gamma);
	vec3 finalColor2 = pow(linearColor2, gamma);
	
	// Pass the color 
	pass_Color =  finalColor+finalColor2;
	// Passes the projected position to the fragment shader / rasterization process. 
    gl_Position = projectionMatrixBox * viewMatrixBox * modelMatrixBox * vec4(in_Position, 1.0);                                                                                                                   
}