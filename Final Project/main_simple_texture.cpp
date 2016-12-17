//
//  main_spotlight.cpp
//  HCI 557 Spotlight example
//
//  Created by Rafael Radkowski on 5/28/15.
//  Copyright (c) 2015 -. All rights reserved.
//

// stl include
#include <iostream>
#include <string>
#include <map>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>





// glfw includes
#include <GLFW/glfw3.h>


// include local files
#include "controls.h"
#include "HCI557Common.h"
#include "CoordSystem.h"
#include "Plane3D.h"
#include "Texture.h"



using namespace std;
using namespace glm;



// The handle to the window object
GLFWwindow*         window;

// Define some of the global variables we're using for this sample
GLuint program;

/* A trackball to move and rotate the camera view */
extern Trackball trackball;


// this is a helper variable to allow us to change the texture blend model
extern int g_change_texture_blend;
extern int animationvariable1;
extern int animationvariable2;
extern int animationvariable3;
extern int animationvariable4;


/*!
A struct to define keyframe
*/
typedef struct _keyframe
{
	float               _t; // the time fraction
	glm::vec3           _p; // the position
	glm::quat           _q; // the orientation

							/*
							Constructor
							*/
	_keyframe(float t, glm::vec3 p, glm::quat q)
	{
		_t = t;
		_p = p;
		_q = q;
	}

	/*
	Default constructor
	*/
	_keyframe()
	{
		_t = -1.0;
		_p = glm::vec3(0.0, 0.0, 0.0);
		_q = glm::quat(0.0, 0.0, 0.0, 0.0);
	}

	// prints the data into a terminal
	void print(void)
	{
		cout << "t: " << _t << "\tp: " << _p.x << ", " << _p.y << ", " << _p.z << "\tq: " << _q.x << ", " << _q.y << ", " << _q.z << ", " << _q.w << endl;
	}

}Keyframe;

/*
Type for the keyframe animation
*/
typedef map<double, Keyframe> KeyframeAnimation;

// Variable to store the keyframes
KeyframeAnimation myKeyframes;




/*!
@brief returns the time fraction for a given time and animation duration
@param time - the current animation time, application runtime, etc. in seconds
@param duration - the duration of the animation in seconds
@return the time fraction in an interval between 0 and 1.
*/
float getTimeFraction(const float time, const float duration)
{
	// we cast to an int. this results in the number of
	float interval = floor(time / duration);

	// return the current interval time
	float current_interval = time - interval*duration;

	// return the fraction / position in our current timeline
	float fraction = current_interval / duration;

	return fraction;
}



/*!
@brief returns the two keyframes for a given time.
@param keyframes - a map with all keyframes of type KeyframeAnimation
@param time - the time fraction between 0 and 1.
@param k0, reference to the first keyframe
@param k2, reference to the second keyframe
@return the number of keyframes. 1 if the time is equal to a keyframe, otherwise 2.
*/
int getKeyframes(KeyframeAnimation& keyframes, const double time, Keyframe& k0, Keyframe& k1)
{
	int num_keyframes = 0;

	// get a keyframe iterator
	KeyframeAnimation::iterator k_itr = keyframes.lower_bound(time);

	Keyframe k0_temp, k1_temp;

	// Obtain the first keyframe
	k1 = (*k_itr).second; num_keyframes++;


	// Check whether we are not at the beginning of this map
	if (k_itr != keyframes.begin())
	{
		k_itr--;  // decrement
		k0 = (*k_itr).second; // obtain the second keyframe
		num_keyframes++;
	}

	// write the first keyframe into k0 if we only have one
	if (num_keyframes == 1)
	{
		k0 = k1;
	}

	return num_keyframes;
}


	/*!
	@brief Interpolate between two keyframes
	@param fraction - the time fraction for the interpolation / the location between two keyframes.
	The value must be between 0 and 1.
	@param k0, the start keyframe
	@param k1, the end keyframe,
	@param res, reference to a variable for the result.
	*/
	bool interpolateKeyframe(const float fraction, const Keyframe& k0, const Keyframe& k1, Keyframe& res)
	{
		/////////////////////////////////////////////////////////////////////////
		// 1. Check the time delta

		// delta time
		float delta_t = k1._t - k0._t;

		// Check whether we have a delta time. Otherwise, we are at the location of exactly one keyframe
		if (delta_t == 0.0f) {
			res = k0;
			return true;
		}

		/////////////////////////////////////////////////////////////////////////
		// 2. Interpolat the position

		// get the delta
		glm::vec3 delta_p = k1._p - k0._p;

		// position interpolation
		glm::vec3 p_int = k0._p + delta_p * (fraction - k0._t) / (delta_t);


		/////////////////////////////////////////////////////////////////////////
		// 3. Rotation interpolation

		// Calculate the distance between the target angle and the current angle.
		float delta_angle = sqrt((k1._q.x - k0._q.x)*(k1._q.x - k0._q.x) +
			(k1._q.y - k0._q.y)*(k1._q.y - k0._q.y) +
			(k1._q.z - k0._q.z)*(k1._q.z - k0._q.z) +
			(k1._q.w - k0._q.w)*(k1._q.w - k0._q.w));


		// Linear interpolation of the rotation using slerp
		glm::quat r_int = glm::slerp(k0._q, k1._q, (fraction - k0._t) / (delta_t));


		/////////////////////////////////////////////////////////////////////////
		// 4. Write the result
		res = Keyframe(fraction, p_int, r_int);

		return true;
	}


	/*!
	This initializes the keyframes.
	*/
	void initKeyframeAnimation(void){
		myKeyframes[0.0] = Keyframe(0.0, glm::vec3(-50.0, 1.0, 10.0), angleAxis(0.0f, glm::vec3(0.0, 0.0, 1.0)));
		myKeyframes[0.5] = Keyframe(0.5, glm::vec3(-10.0, 1.0, 30.0), angleAxis(0.57f, glm::vec3(0.0, 0.0, 1.0)));
		myKeyframes[0.7] = Keyframe(0.7, glm::vec3(-10.0, 1.0, 50.0), angleAxis(1.28f, glm::vec3(0.0, 0.0, 1.0)));
		myKeyframes[0.8] = Keyframe(0.8, glm::vec3(-10.0, 1.0, 70.0), angleAxis(1.53f, glm::vec3(0.0, 0.0, 1.0)));
		myKeyframes[1.0] = Keyframe(1.0, glm::vec3(-10.0, 1.0, 90.0), angleAxis(1.98f, glm::vec3(0.0, 0.1, 1.0)));

	}


int main(int argc, const char * argv[])
{
    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Init glfw, create a window, and init glew
    
    // Init the GLFW Window
    window = initWindow();
	GLObjectObj* loadedModel1 = NULL; // the basic / normal model
	GLObjectObj* loadedModel2 = NULL; // the basic / normal model
	GLObjectObj* loadedModel3 = NULL; // the basic / normal model
	GLObjectObj* loadedModel4 = NULL; // the basic / normal model
    
    // Init the glew api
    initGlew();
    
    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Create some models
    
    // coordinate system
    CoordSystem* cs = new CoordSystem(40.0);
    
    
    // create an apperance object.
	GLAppearance* apperance_0 = new GLAppearance("../../data/shaders/single_texture.vs", "../../data/shaders/single_texture.fs");
	GLDirectLightSource  light_source;
    light_source._lightPos = glm::vec4(00.0,20.0,20.0, 0.0);
    light_source._ambient_intensity = 0.2;
    light_source._specular_intensity = 5.5;
    light_source._diffuse_intensity = 2.0;
    light_source._attenuation_coeff = 0.0;
    // add the light to this apperance object
	apperance_0->addLightSource(light_source);

	GLSpotLightSource spotlight_source;
    spotlight_source._lightPos = glm::vec4(0.0,00.0,50.0, 1.0);
    spotlight_source._ambient_intensity = 0.2;
    spotlight_source._specular_intensity = 30.5;
    spotlight_source._diffuse_intensity = 10.0;
    spotlight_source._attenuation_coeff = 0.0002;
    spotlight_source._cone_direction = glm::vec3(-1.0, -1.0,-1.0);
    spotlight_source._cone_angle = 20.0;
    apperance_0->addLightSource(spotlight_source);
    
    // Create a material object
    GLMaterial material_0;
    material_0._diffuse_material = glm::vec3(0.2, 0.0, 1.0);
    material_0._ambient_material = glm::vec3(0.2, 0.0, 1.0);
    material_0._specular_material = glm::vec3(1.0, 1.0, 1.0);
    material_0._shininess = 12.0;
    material_0._transparency = 0.5;
    
	// Add the material to the apperance object
    apperance_0->setMaterial(material_0);

	//Loading an object
	// create the sphere geometry
	GLPlane3D* plane_0 = new GLPlane3D(0.0, 0.0, 0.0, 100.0, 100.0);
	plane_0->setApperance(*apperance_0);
	plane_0->init();
	// Add a texture
	GLTexture* texture0 = new GLTexture();
	texture0->loadAndCreateTexture("road.bmp", 1);
	apperance_0->setTexture(texture0, 1);

	//************************************************************************************************
	// Finalize the appearance object
	apperance_0->finalize();
	apperance_0->updateLightSources();

	GLAppearance* apperance_1 = new GLAppearance("../../data/shaders/single_texture1.vs", "../../data/shaders/single_texture1.fs");
	GLAppearance* apperance_2 = new GLAppearance("../../data/shaders/single_texture2.vs", "../../data/shaders/single_texture2.fs");
	GLAppearance* apperance_3 = new GLAppearance("../../data/shaders/single_texture3.vs", "../../data/shaders/single_texture3.fs");
	GLAppearance* apperance_4 = new GLAppearance("../../data/shaders/multi_texture.vs", "../../data/shaders/multi_texture.fs");

	apperance_1->addLightSource(light_source);
	apperance_1->addLightSource(spotlight_source);
	apperance_1->setMaterial(material_0);
	

	//sahiti 

	
	
	loadedModel1 = new GLObjectObj("../data/car22.obj");
	loadedModel1->setApperance(*apperance_1);

	glm::mat4 rotate1 = glm::rotate(glm::mat4(1.0), 1.57f, glm::vec3(0.0f, 1.0f, 0.0f));
	loadedModel1->init();
	loadedModel1->setMatrix(rotate1);


	glm::mat4 translate1 = glm::translate(glm::vec3(-20.0f, 1.0f, -20.0f));
	//loadedModel1->init();
    

	glm::mat4 rot1 = rotate1*translate1;
	loadedModel1->setMatrix(rot1);

	glm::mat4 back = rot1;


	loadedModel2 = new GLObjectObj("../data/auto.obj");
	
	loadedModel2->setApperance(*apperance_3);
	loadedModel2->init();
	glm::mat4 translate3 = glm::translate(glm::vec3(10.0f, 4.0f, 40.0f));
	loadedModel2->setMatrix(translate3);

	glm::mat4 back2 = translate3;

	
	loadedModel3 = new GLObjectObj("../data/trafficlight.obj");
	
	loadedModel3->setApperance(*apperance_4);


	
	loadedModel3->init();
	glm::mat4 translate5 = glm::translate(glm::vec3(-20.0f, 1.0f, -25.0f));
	loadedModel3->setMatrix(translate5);


	loadedModel4 = new GLObjectObj("../data/trafficlight.obj");
	loadedModel4->setApperance(*apperance_4);
	loadedModel4->init();
	glm::mat4 translate6 = glm::translate(glm::vec3(20.0f, 1.0f, 25.0f));
	loadedModel4->setMatrix(translate6);


	

    //************************************************************************************************

//	GLPlane3D* plane_1 = new GLPlane3D(0.0, 0.0, 0.0, 100.0, 100.0);
//	plane_1->setApperance(*apperance_1);
//	plane_1->init();
	GLTexture* texture1 = new GLTexture();
	texture1->loadAndCreateTexture("pink.bmp",2);
	apperance_1->setTexture(texture1, 2);


	
	

	GLTexture* texture3 = new GLTexture();
	texture3->loadAndCreateTexture("yellow.bmp",3);
	apperance_3->setTexture(texture3, 3);

	//GLTexture* texture5 = new GLTexture();
	//texture5->loadAndCreateTexture("green.bmp");
	
	GLMultiTexture* texture4 = new GLMultiTexture();
	int texid = texture4->loadAndCreateTextures("green.bmp", "yellow.bmp", "red.bmp");
	apperance_4->setTexture(texture4);



	    // If you want to change appearance parameters after you init the object, call the update function

	apperance_1->finalize();
	apperance_1->updateLightSources();


	
	// This inits the keyframe animation
	initKeyframeAnimation();


	
    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Main render loop
    
    // Set up our green background color
    static const GLfloat clear_color[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    // This sets the camera to a new location
    // the first parameter is the eye position, the second the center location, and the third the up vector.
    SetViewAsLookAt(glm::vec3(12.0f, 12.0f, 65.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    
    // Enable depth test
    // ignore this line, it allows us to keep the distance value after we proejct each object to a 2d canvas.
    glEnable(GL_DEPTH_TEST);
    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Blending
    
    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // sphere->enableNormalVectorRenderer();
	int j = 0;


   
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// Main render loop

	// This is our render loop. As long as our window remains open (ESC is not pressed), we'll continue to render things.
	while (!glfwWindowShouldClose(window))
	{

		// Clear the entire buffer with our green color (sets the background to be green).
		glClearBufferfv(GL_COLOR, 0, clear_color);
		glClearBufferfv(GL_DEPTH, 0, clear_depth);

		// Interpolate between keyframes
		Keyframe k0, k1, k_res;

		float time = glfwGetTime();

		
		float f = getTimeFraction(time, 1000.0); // we assume that the animation takes 8 seconds
		int num = getKeyframes(myKeyframes, f, k0, k1);

		bool ret = interpolateKeyframe(f, k0, k1, k_res);

		k_res.print();

	    // glm:mat4 translatex = glm::translate(k_res._p);

		//translate1 = translate1*translatex;
		//plane_1->setMatrix(translate1);

        
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //// This renders the objects
		bool ret1 = texture4->setTextureBlendMode(g_change_texture_blend);
		if (ret1)apperance_4->updateTextures();
		
		// Set the trackball locatiom
        SetTrackballLocation(trackball.getRotationMatrix());


        // draw the objects
        cs->draw();
        plane_0->draw();

		loadedModel1->draw();
		loadedModel2->draw();

		
		loadedModel3->draw();
		loadedModel4->draw();


		
        
        // change the texture appearance blend mode
     
		if (animationvariable1 == 1)
		{

			float a = 0.1;
			j++;
			if (j > 500)
			{
				j = 0;
				glm::mat4 translate2 = glm::translate(glm::vec3(0.0f, 0.0f, -50.0f));
				rot1 = rot1*translate2;
				loadedModel1->setMatrix(rot1);
			}

			glm::mat4 translate2 = glm::translate(glm::vec3(0.0f, 0.0f, a));
			rot1 = rot1*translate2;
			loadedModel1->setMatrix(rot1);
		}
		else if (animationvariable3 == 1)
		{

			animationvariable1 = 0;

			loadedModel1->setMatrix(back);

		}

		if (animationvariable2 == 1)
		{

		
			float b = -0.1;
			j++;
			if (j > 500)
			{
				j = 0;
				glm::mat4 translate4 = glm::translate(glm::vec3(0.0f, 0.0f, -10.0f));
				translate3 = translate3*translate4;
				loadedModel2->setMatrix(translate3);
			}

			glm::mat4 translate4 = glm::translate(glm::vec3(0.0f, 0.0f, b ));
			translate3 = translate3*translate4;
			loadedModel2->setMatrix(translate3);
			
		}
		
		else if (animationvariable4 == 1)
		{


			loadedModel2->setMatrix(back2);

		}
		
        //// This renders the objects
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      
        
        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    
   
    }
    
    
    delete cs;
    
    
}

