//
//  main.cpp
//  OpenGL4Test
//
//  Created by Rafael Radkowski on 5/28/15.
//  Copyright (c) 2015 -. All rights reserved.
//

// stl include
#include <iostream>
#include <string>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


// glfw includes
#include <GLFW/glfw3.h>


// include local files
#include "controls.h"
#include "HCI557Common.h"
#include "CoordSystem.h"

// this line tells the compiler to use the namespace std.
// Each object, command without a namespace is assumed to be part of std. 
using namespace std;




static const string vs_string =
"#version 410 core                                                 \n"
"                                                                   \n"
"uniform mat4 projectionMatrix;                                    \n"
"uniform mat4 viewMatrix;                                           \n"
"uniform mat4 modelMatrix;                                          \n"
"in vec3 in_Position;                                               \n"
"                                                                   \n"
"in vec3 in_Color;                                                  \n"
"out vec3 pass_Color;                                               \n"
"                                                                  \n"
"void main(void)                                                   \n"
"{                                                                 \n"
"    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);  \n"
"    pass_Color = in_Color;                                         \n"
"}                                                                 \n";

// Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
static const string fs_string  =
"#version 410 core                                                 \n"
"                                                                  \n"
"in vec3 pass_Color;                                                 \n"
"out vec4 color;                                                    \n"
"void main(void)                                                   \n"
"{                                                                 \n"
"    color = vec4(pass_Color, 1.0);                               \n"
"}                                                                 \n";




/// Camera control matrices
glm::mat4 projectionMatrix; // Store the projection matrix
glm::mat4 viewMatrix; // Store the view matrix
glm::mat4 modelMatrix; // Store the model matrix




// The handle to the window object
GLFWwindow*         window;


// Define some of the global variables we're using for this sample
GLuint program;


// USE THESE vertex array objects to define your objects
unsigned int vaoID[2];
unsigned int vboID[4];


/*!
 ADD YOUR CODE TO CREATE THE TRIANGLE STRIP MODEL TO THIS FUNCTION
 */

unsigned int createTriangleStripModel(void)
{

	float *vertices = new float[111];
	float *colors = new float[111];

	//FRONT FACE
	vertices[0] = 0.0; vertices[1] = 0.0; vertices[2] = 0.0;
	colors[0] = 0.3; colors[2] = 0.3; colors[3] = 0.3;
	vertices[3] = 0.0; vertices[4] = 0.0; vertices[5] = 1.0;
	colors[3] = 0.3; colors[2] = 0.3; colors[3] = 0.3;
	vertices[6] = 1.0; vertices[7] = 0.0; vertices[8] = 0.0;
	colors[6] = 0.3; colors[7] = 0.3; colors[8] = 0.3;
	vertices[9] = 1.0; vertices[10] = 0.0; vertices[11] = 1.0;
	colors[9] = 0.3; colors[10] = 0.3; colors[11] = 0.3;

	// right side face
	vertices[12] = 1.0; vertices[13] = 2.0; vertices[14] = 0.0;
	colors[12] = 0.3; colors[13] = 0.3; colors[14] = 0.3;
	vertices[15] = 1.0; vertices[16] = 2.0; vertices[17] = 1.0;
	colors[15] = 0.3; colors[16] = 0.3; colors[17] = 0.3;

	//right sided front face
	vertices[18] = 3.0; vertices[19] = 2.0; vertices[20] = 0.0;
	colors[18] = 0.3; colors[19] = 0.3; colors[20] = 0.3;
	vertices[21] = 3.0; vertices[22] = 2.0; vertices[23] = 1.0;
	colors[21] = 0.3; colors[22] = 0.3; colors[23] = 0.3;

	  //right sided right face
	vertices[24] = 3.0; vertices[25] = 3.0; vertices[26] = 0.0;
	colors[24] = 0.3; colors[25] = 0.3; colors[26] = 0.3;
	vertices[27] = 3.0; vertices[28] = 3.0; vertices[29] = 1.0;
	colors[21] = 0.3; colors[22] = 0.3; colors[23] = 0.3;

	//back side bottom face
	vertices[30] = 0.0; vertices[31] = 3.0; vertices[32] = 0.0;
	colors[30] = 0.3; colors[31] = 0.3; colors[32] = 0.3;
	vertices[33] = 0.0; vertices[34] = 3.0; vertices[35] = 1.0;
	colors[33] = 0.3; colors[34] = 0.3; colors[35] = 0.3;

	//left side bottom face
	vertices[36] = 0.0; vertices[37] = 0.0; vertices[38] = 0.0;
	colors[36] = 0.3; colors[37] = 0.3; colors[38] = 0.3;

	vertices[39] = 0.0; vertices[40] = 0.0; vertices[41] = 1.0;
	colors[39] = 0.3; colors[40] = 0.3; colors[41] = 0.3;

	vertices[42] = 0.0; vertices[43] = 0.0; vertices[44] = 1.0;
	colors[42] = 0.3; colors[43] = 0.3; colors[44] = 0.3;

	vertices[45] = 1.0; vertices[46] = 0.0; vertices[47] = 1.0;
	colors[45] = 0.3; colors[46] = 0.3; colors[47] = 0.3;

	vertices[48] = 0.0; vertices[49] = 2.0; vertices[50] = 1.0;
	colors[48] = 0.3; colors[49] = 0.3; colors[50] = 0.3;

	vertices[48] = 1.0; vertices[49] = 2.0; vertices[50] = 1.0;
	colors[48] = 0.3; colors[49] = 0.3; colors[50] = 0.3;

	vertices[51] = 0.0; vertices[52] = 2.0; vertices[53] = 2.0;
	colors[51] = 0.3; colors[52] = 0.3; colors[53] = 0.3;

	vertices[54] = 1.0; vertices[55] = 2.0; vertices[56] = 2.0;
	colors[54] = 0.3; colors[55] = 0.3; colors[56] = 0.3;

	vertices[57] = 0.0; vertices[58] = 3.0; vertices[59] = 2.0;
	colors[57] = 0.3; colors[58] = 0.3; colors[59] = 0.3;

	vertices[60] = 1.0; vertices[61] = 3.0; vertices[62] = 2.0;
	colors[60] = 0.3; colors[61] = 0.3; colors[62] = 0.3;

	vertices[63] = 1.0; vertices[64] = 3.0; vertices[65] = 2.0;
	colors[63] = 0.3; colors[64] = 0.3; colors[65] = 0.3;

	vertices[66] = 1.0; vertices[67] = 2.0; vertices[68] = 2.0;
	colors[66] = 0.3; colors[67] = 0.3; colors[68] = 0.3;

	vertices[69] = 3.0; vertices[70] = 3.0; vertices[71] = 1.0;
	colors[69] = 0.3; colors[70] = 0.3; colors[71] = 0.3;

	vertices[72] = 3.0; vertices[73] = 2.0; vertices[74] = 1.0;
	colors[72] = 0.3; colors[73] = 0.3; colors[74] = 0.3;

	vertices[75] = 3.0; vertices[76] = 2.0; vertices[77] = 1.0;
	colors[75] = 0.3; colors[76] = 0.3; colors[77] = 0.3;

	vertices[78] = 3.0; vertices[79] = 2.0; vertices[80] = 0.0;
	colors[78] = 0.3; colors[79] = 0.3; colors[80] = 0.3;

	vertices[81] = 3.0; vertices[82] = 2.0; vertices[83] = 0.0;
	colors[81] = 0.3; colors[82] = 0.3; colors[83] = 0.3;

	vertices[84] = 3.0; vertices[85] = 3.0; vertices[86] = 0.0;
	colors[84] = 0.3; colors[85] = 0.3; colors[86] = 0.3;

	vertices[87] = 1.0; vertices[88] = 2.0; vertices[89] = 0.0;
	colors[87] = 0.3; colors[88] = 0.3; colors[89] = 0.3;


	vertices[90] = 1.0; vertices[91] = 3.0; vertices[92] = 0.0;
	colors[90] = 0.3; colors[91] = 0.3; colors[92] = 0.3;

	vertices[93] = 0.0; vertices[94] = 2.0; vertices[95] = 0.0;
	colors[93] = 0.3; colors[94] = 0.3; colors[95] = 0.3;

	vertices[96] = 0.0; vertices[97] = 3.0; vertices[98] = 0.0;
	colors[96] = 0.3; colors[97] = 0.3; colors[98] = 0.3;


	vertices[99] = 0.0; vertices[100] = 2.0; vertices[101] = 0.0;
	colors[99] = 0.3; colors[100] = 0.3; colors[101] = 0.3;


	vertices[102] = 0.0; vertices[103] = 0.0; vertices[104] = 0.0;
	colors[102] = 0.3; colors[103] = 0.3; colors[104] = 0.3;


	vertices[105] = 1.0; vertices[106] = 2.0; vertices[107] = 0.0;
	colors[105] = 0.3; colors[106] = 0.3; colors[107] = 0.3;


	vertices[108] = 1.0; vertices[109] = 0.0; vertices[110] = 0.0;
	colors[108] = 0.3; colors[109] = 0.3; colors[110] = 0.3;
	
	glGenVertexArrays(2, &vaoID[0]);
	glBindVertexArray(vaoID[0]);

	glGenBuffers(2, vboID);

	// vertices
	glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);
	glBufferData(GL_ARRAY_BUFFER,111 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	

	// color
	glBindBuffer(GL_ARRAY_BUFFER, vboID[1]);
	glBufferData(GL_ARRAY_BUFFER, 111 * sizeof(GLfloat), colors, GL_STATIC_DRAW);

	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	delete[] vertices;
	return 1;
}

/*!
 ADD YOUR CODE TO CREATE A MODEL USING PRIMITIVES OF YOUR CHOISE TO THIS FUNCTION
 */
unsigned int createTriangleModel(void)
{
    // use the vertex array object vaoID[1] for this model representation
  
	float *vertices = new float[248];
	float *colors = new float[248];

	//X BOTTOM FACE
	vertices[0] = 0.0; vertices[1] = 0.0; vertices[2] = 0.0;
	colors[0] = 0.3; colors[1] = 0.3; colors[2] = 0.3;
	vertices[3] = 3.0; vertices[4] = 0.0; vertices[5] = 0.0;
	colors[3] = 0.3; colors[4] = 0.3; colors[5] = 0.3;
	vertices[6] = 3.0; vertices[7] = -1.0; vertices[8] = 0.0;
	colors[6] = 0.3; colors[7] = 0.3; colors[8] = 0.3;
	vertices[9] = 3.0; vertices[10] = -1.0; vertices[11] = 0.0;
	colors[9] = 0.3; colors[10] = 0.3; colors[11] = 0.3;
	vertices[12] = 0.0; vertices[13] = -1.0; vertices[14] = 0.0;
	colors[12] = 0.3; colors[13] = 0.3; colors[14] = 0.3;
	vertices[15] = 0.0; vertices[16] = 0.0; vertices[17] = 0.0;
	colors[15] = 0.3; colors[16] = 0.3; colors[17] = 0.3;

	//X front face
	vertices[18] = 0.0; vertices[19] = 0.0; vertices[20] = 0.0;
	colors[18] = 0.3; colors[19] = 0.3; colors[20] = 0.3;
	vertices[21] = 3.0; vertices[22] = 0.0; vertices[23] = 0.0;
	colors[21] = 0.3; colors[22] = 0.3; colors[23] = 0.3;
	vertices[24] = 3.0; vertices[25] = 0.0; vertices[26] = 1.0;
	colors[24] = 0.3; colors[25] = 0.3; colors[26] = 0.3;
	vertices[27] = 3.0; vertices[28] = 0.0; vertices[29] = 1.0;
	colors[21] = 0.3; colors[22] = 0.3; colors[23] = 0.3;
	vertices[30] = 0.0; vertices[31] = 0.0; vertices[32] = 0.0;
	colors[30] = 0.3; colors[31] = 0.3; colors[32] = 0.3;
	vertices[33] = 1.0; vertices[34] = 0.0; vertices[35] = 2.0;
	colors[33] = 0.3; colors[34] = 0.3; colors[35] = 0.3;
	vertices[36] = 0.0; vertices[37] = 0.0; vertices[38] = 0.0;
	colors[36] = 0.3; colors[37] = 0.3; colors[38] = 0.3;
	vertices[39] = 1.0; vertices[40] = 0.0; vertices[41] = 2.0;
	colors[39] = 0.3; colors[40] = 0.3; colors[41] = 0.3;

	vertices[42] = 0.0; vertices[43] = 0.0; vertices[44] = 2.0;
	colors[42] = 0.3; colors[43] = 0.3; colors[44] = 0.3;
	//X TOP face
	vertices[45] = 1.0; vertices[46] = 0.0; vertices[47] = 2.0;
	colors[45] = 0.3; colors[46] = 0.3; colors[47] = 0.3;

	vertices[48] = 0.0; vertices[49] = 0.0; vertices[50] = 2.0;
	colors[48] = 0.3; colors[49] = 0.3; colors[50] = 0.3;

	vertices[48] = 0.0; vertices[49] = -1.0; vertices[50] = 2.0;
	colors[48] = 0.3; colors[49] = 0.3; colors[50] = 0.3;

	vertices[51] = 0.0; vertices[52] = -1.0; vertices[53] = 2.0;
	colors[51] = 0.3; colors[52] = 0.3; colors[53] = 0.3;

	vertices[54] = 1.0; vertices[55] = -1.0; vertices[56] = 2.0;
	colors[54] = 0.3; colors[55] = 0.3; colors[56] = 0.3;

	vertices[57] = 1.0; vertices[58] = 0.0; vertices[59] = 2.0;
	colors[57] = 0.3; colors[58] = 0.3; colors[59] = 0.3;
	//X LEFT face
	vertices[60] = 0.0; vertices[61] = 0.0; vertices[62] = 0.0;
	colors[60] = 0.3; colors[61] = 0.3; colors[62] = 0.3;

	vertices[63] = 0.0; vertices[64] = 0.0; vertices[65] = 2.0;
	colors[63] = 0.3; colors[64] = 0.3; colors[65] = 0.3;

	vertices[66] = 0.0; vertices[67] = -1.0; vertices[68] = 2.0;
	colors[66] = 0.3; colors[67] = 0.3; colors[68] = 0.3;

	vertices[69] = 0.0; vertices[70] = -1.0; vertices[71] = 2.0;
	colors[69] = 0.3; colors[70] = 0.3; colors[71] = 0.3;

	vertices[72] = 0.0; vertices[73] = -1.0; vertices[74] = 0.0;
	colors[72] = 0.3; colors[73] = 0.3; colors[74] = 0.3;

	vertices[75] = 0.0; vertices[76] = 0.0; vertices[77] = 0.0;
	colors[75] = 0.3; colors[76] = 0.3; colors[77] = 0.3;
	//X SLANT face
	vertices[78] = 1.0; vertices[79] = 0.0; vertices[80] = 2.0;
	colors[78] = 0.3; colors[79] = 0.3; colors[80] = 0.3;

	vertices[81] = 1.0; vertices[82] = -1.0; vertices[83] = 2.0;
	colors[81] = 0.3; colors[82] = 0.3; colors[83] = 0.3;

	vertices[84] = 3.0; vertices[85] = 0.0; vertices[86] = 1.0;
	colors[84] = 0.3; colors[85] = 0.3; colors[86] = 0.3;

	vertices[87] = 3.0; vertices[88] = 0.0; vertices[89] = 1.0;
	colors[87] = 0.3; colors[88] = 0.3; colors[89] = 0.3;


	vertices[90] = 3.0; vertices[91] = -1.0; vertices[92] = 1.0;
	colors[90] = 0.3; colors[91] = 0.3; colors[92] = 0.3;

	vertices[93] = 1.0; vertices[94] = -1.0; vertices[95] = 2.0;
	colors[93] = 0.3; colors[94] = 0.3; colors[95] = 0.3;
	//X BACK FACE
	vertices[96] = 0.0; vertices[97] = -1.0; vertices[98] = 2.0;
	colors[96] = 0.3; colors[97] = 0.3; colors[98] = 0.3;


	vertices[99] = 0.0; vertices[100] = -1.0; vertices[101] = 0.0;
	colors[99] = 0.3; colors[100] = 0.3; colors[101] = 0.3;


	vertices[102] = 1.0; vertices[103] = -1.0; vertices[104] = 2.0;
	colors[102] = 0.3; colors[103] = 0.3; colors[104] = 0.3;


	vertices[105] = 0.0; vertices[106] = -1.0; vertices[107] = 0.0;
	colors[105] = 0.3; colors[106] = 0.3; colors[107] = 0.3;


	vertices[108] = 1.0; vertices[109] = -1.0; vertices[110] = 2.0;
	colors[108] = 0.3; colors[109] = 0.3; colors[110] = 0.3;
	vertices[111] = 3.0; vertices[112] = -1.0; vertices[113] = 1.0;
	colors[111] = 0.3; colors[112] = 0.3; colors[113] = 0.3;


	vertices[114] = 3.0; vertices[115] = -1.0; vertices[116] = 1.0;
	colors[114] = 0.3; colors[115] = 0.3; colors[116] = 0.3;


	vertices[117] = 0.0; vertices[118] = -1.0; vertices[119] = 0.0;
	colors[117] = 0.3; colors[118] = 0.3; colors[119] = 0.3;


	vertices[120] = 3.0; vertices[121] = -1.0; vertices[122] = 0.0;
	colors[120] = 0.3; colors[121] = 0.3; colors[122] = 0.3;

	//X right FACE
	vertices[123] = 3.0; vertices[124] = -1.0; vertices[125] = 1.0;
	colors[123] = 0.3; colors[124] = 0.3; colors[125] = 0.3;
	vertices[126] = 3.0; vertices[127] = -1.0; vertices[128] = 0.0;
	colors[126] = 0.3; colors[127] = 0.3; colors[128] = 0.3;


	vertices[129] = 3.0; vertices[130] = 0.0; vertices[131] = 0.0;
	colors[129] = 0.3; colors[130] = 0.3; colors[131] = 0.3;


	vertices[132] = 3.0; vertices[133] = 0.0; vertices[134] = 1.0;
	colors[132] = 0.3; colors[133] = 0.3; colors[134] = 0.3;


	vertices[135] = 3.0; vertices[136] = 0.0; vertices[137] = 0.0;
	colors[135] = 0.3; colors[136] = 0.3; colors[137] = 0.3;


	vertices[138] = 3.0; vertices[139] = -1.0; vertices[140] = 1.0;
	colors[138] = 0.3; colors[139] = 0.3; colors[140] = 0.3;

	//Y right FACE

	vertices[141] = 1.0; vertices[142] = 3.0; vertices[143] = 1.0;
	colors[141] = 0.3; colors[142] = 0.3; colors[143] = 0.3;


	vertices[144] = 1.0; vertices[145] = 3.0; vertices[146] = 0.0;
	colors[144] = 0.3; colors[145] = 0.3; colors[146] = 0.3;


	vertices[147] = 1.0; vertices[148] = 0.0; vertices[149] = 0.0;
	colors[147] = 0.3; colors[148] = 0.3; colors[149] = 0.3;


	vertices[150] = 1.0; vertices[151] = 0.0; vertices[152] = 0.0;
	colors[150] = 0.3; colors[151] = 0.3; colors[152] = 0.3;


	vertices[153] = 1.0; vertices[154] = 0.0; vertices[155] = 1.0;
	colors[153] = 0.3; colors[154] = 0.3; colors[155] = 0.3;

	vertices[156] = 1.0; vertices[157] = 3.0; vertices[158] = 1.0;
	colors[156] = 0.3; colors[157] = 0.3; colors[158] = 0.3;
	//Y LEFT FACE

	vertices[159] = 0.0; vertices[160] = 3.0; vertices[161] = 1.0;
	colors[159] = 0.3; colors[160] = 0.3; colors[161] = 0.3;


	vertices[162] = 0.0; vertices[163] = 3.0; vertices[164] = 0.0;
	colors[162] = 0.3; colors[163] = 0.3; colors[164] = 0.3;


	vertices[165] = 0.0; vertices[166] = 0.0; vertices[167] = 1.0;
	colors[165] = 0.3; colors[166] = 0.3; colors[167] = 0.3;


	vertices[168] = 0.0; vertices[169] = 0.0; vertices[170] = 1.0;
	colors[168] = 0.3; colors[169] = 0.3; colors[170] = 0.3;


	vertices[171] = 0.0; vertices[172] = 3.0; vertices[173] = 0.0;
	colors[171] = 0.3; colors[172] = 0.3; colors[173] = 0.3;
	vertices[174] = 0.0; vertices[175] = 0.0; vertices[176] = 0.0;
	colors[174] = 0.3; colors[175] = 0.3; colors[176] = 0.3;
	//Y TOP FACE
	vertices[177] = 0.0; vertices[178] = 3.0; vertices[180] = 1.0;
	colors[177] = 0.3; colors[178] = 0.3; colors[180] = 0.3;


	vertices[181] = 0.0; vertices[182] = 0.0; vertices[183] = 1.0;
	colors[181] = 0.3; colors[182] = 0.3; colors[183] = 0.3;


	vertices[184] = 1.0; vertices[185] = 0.0; vertices[186] = 1.0;
	colors[184] = 0.3; colors[185] = 0.3; colors[186] = 0.3;


	vertices[187] = 1.0; vertices[188] = 0.0; vertices[189] = 1.0;
	colors[187] = 0.3; colors[188] = 0.3; colors[189] = 0.3;


	vertices[190] = 1.0; vertices[191] = 3.0; vertices[192] = 1.0;
	colors[190] = 0.3; colors[191] = 0.3; colors[192] = 0.3;
	vertices[193] = 0.0; vertices[194] = 0.0; vertices[195] = 1.0;
	colors[193] = 0.3; colors[194] = 0.3; colors[195] = 0.3;


	//Y BOTTOM FACE
	vertices[196] = 0.0; vertices[197] = 0.0; vertices[198] = 0.0;
	colors[196] = 0.3; colors[197] = 0.3; colors[198] = 0.3;


	vertices[199] = 0.0; vertices[200] = 3.0; vertices[201] = 0.0;
	colors[199] = 0.3; colors[200] = 0.3; colors[201] = 0.3;


	vertices[202] = 1.0; vertices[203] = 0.0; vertices[204] = 0.0;
	colors[202] = 0.3; colors[203] = 0.3; colors[204] = 0.3;


	vertices[205] = 1.0; vertices[206] = 0.0; vertices[207] = 0.0;
	colors[205] = 0.3; colors[206] = 0.3; colors[207] = 0.3;


	vertices[208] = 1.0; vertices[209] = 3.0; vertices[210] = 0.0;
	colors[208] = 0.3; colors[209] = 0.3; colors[210] = 0.3;

	vertices[211] = 0.0; vertices[212] = 3.0; vertices[213] = 0.0;
	colors[211] = 0.3; colors[212] = 0.3; colors[213] = 0.3;

	//Y FRONT FACE
	vertices[214] = 0.0; vertices[215] = 3.0; vertices[216] = 1.0;
	colors[214] = 0.3; colors[215] = 0.3; colors[216] = 0.3;


	vertices[217] = 1.0; vertices[218] = 3.0; vertices[219] = 1.0;
	colors[217] = 0.3; colors[218] = 0.3; colors[219] = 0.3;


	vertices[220] = 0.0; vertices[221] = 3.0; vertices[222] = 0.0;
	colors[220] = 0.3; colors[221] = 0.3; colors[222] = 0.3;


	vertices[223] = 0.0; vertices[224] = 3.0; vertices[225] = 0.0;
	colors[223] = 0.3; colors[224] = 0.3; colors[225] = 0.3;


	vertices[226] = 1.0; vertices[227] = 3.0; vertices[228] = 0.0;
	colors[226] = 0.3; colors[227] = 0.3; colors[228] = 0.3;
	vertices[229] = 1.0; vertices[230] = 3.0; vertices[231] = 1.0;
	colors[229] = 0.3; colors[230] = 0.3; colors[231] = 0.3;

	//Y BACK FACE
	vertices[231] = 0.0; vertices[232] = 0.0; vertices[233] = 1.0;
	colors[231] = 0.3; colors[232] = 0.3; colors[233] = 0.3;


	vertices[234] = 1.0; vertices[235] = .0; vertices[236] = 1.0;
	colors[234] = 0.3; colors[235] = 0.3; colors[236] = 0.3;


	vertices[237] = 1.0; vertices[238] = 0.0; vertices[239] = 0.0;
	colors[237] = 0.3; colors[238] = 0.3; colors[239] = 0.3;


	vertices[240] = 1.0; vertices[241] = 0.0; vertices[242] = 0.0;
	colors[240] = 0.3; colors[241] = 0.3; colors[242] = 0.3;


	vertices[243] = 0.0; vertices[244] = 0.0; vertices[245] = 0.0;
	colors[243] = 0.3; colors[244] = 0.3; colors[245] = 0.3;

	vertices[246] = 0.0; vertices[247] = 0.0; vertices[248] = 1.0;
	colors[246] = 0.3; colors[247] = 0.3; colors[248] = 0.3;







	glGenVertexArrays(2, &vaoID[1]);
	glBindVertexArray(vaoID[1]);

	glGenBuffers(2, vboID);

	// vertices
	glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);
	glBufferData(GL_ARRAY_BUFFER, 248 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);


	// color
	glBindBuffer(GL_ARRAY_BUFFER, vboID[1]);
	glBufferData(GL_ARRAY_BUFFER, 248 * sizeof(GLfloat), colors, GL_STATIC_DRAW);

	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

//	delete[] vertices;
	return 1;
}



/*!
 ADD YOUR CODE TO RENDER THE TRIANGLE STRIP MODEL TO THIS FUNCTION
 */
void renderTriangleStripModel(void)
{
    // Bind the buffer and switch it to an active buffer
    glBindVertexArray(vaoID[0]);
        
	// HERE: THIS CAUSES AN ERROR BECAUSE I DO NOT KNOW HOW MANY TRIANGLES / VERTICES YOU HAVE.
	// COMPLETE THE LINE
    // Draw the triangles
    glDrawArrays(GL_TRIANGLE_STRIP,0, 90 );


    // Unbind our Vertex Array Object
    glBindVertexArray(0);
}



/*!
 ADD YOUR CODE TO RENDER THE TRIANGLE STRIP MODEL TO THIS FUNCTION
 */
void renderTriangleModel(void)
{

	// Bind the buffer and switch it to an active buffer
	glBindVertexArray(vaoID[1]);

	// HERE: THIS CAUSES AN ERROR BECAUSE I DO NOT KNOW HOW MANY TRIANGLES / VERTICES YOU HAVE.
	// COMPLETE THE LINE
	// Draw the triangles
	glDrawArrays(GL_TRIANGLES, 0, 248);


	// Unbind our Vertex Array Object
	glBindVertexArray(0);
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*!
 This function creates the two models
 */
void setupScene(void) {
    
    //createTriangleStripModel();
    //renderTriangleStripModel();

	createTriangleModel();
	//renderTriangleModel();

    
}




int main(int argc, const char * argv[])
{
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Init glfw, create a window, and init glew
    
    // Init the GLFW Window
    window = initWindow();
    
    
    // Init the glew api
    initGlew();
    
	// Prepares some defaults
	CoordSystemRenderer* coordinate_system_renderer = new CoordSystemRenderer(10.0);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// The Shader Program starts here
    
    // Vertex shader source code. This draws the vertices in our window. We have 3 vertices since we're drawing an triangle.
    // Each vertex is represented by a vector of size 4 (x, y, z, w) coordinates.
    static const string vertex_code = vs_string;
    static const char * vs_source = vertex_code.c_str();
    
    // Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
    static const string fragment_code = fs_string;
    static const char * fs_source = fragment_code.c_str();
    
    // This next section we'll generate the OpenGL program and attach the shaders to it so that we can render our triangle.
    program = glCreateProgram();
    
    // We create a shader with our fragment shader source code and compile it.
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_source, NULL);
    glCompileShader(fs);
    
    // We create a shader with our vertex shader source code and compile it.
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vs_source, NULL);
    glCompileShader(vs);
    
    // We'll attach our two compiled shaders to the OpenGL program.
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    
    glLinkProgram(program);
    
    // We'll specify that we want to use this program that we've attached the shaders to.
    glUseProgram(program);
    
    //// The Shader Program ends here
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    
    /// IGNORE THE NEXT PART OF THIS CODE
    /// IGNORE THE NEXT PART OF THIS CODE
    /// IGNORE THE NEXT PART OF THIS CODE
    // It controls the virtual camera
    
    // Set up our green background color
    static const GLfloat clear_color[] = { 0.6f, 0.7f, 1.0f, 1.0f };
    static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    
    projectionMatrix = glm::perspective(1.1f, (float)800 / (float)600, 0.1f, 100.f);
    modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // Create our model matrix which will halve the size of our model
    viewMatrix = glm::lookAt(glm::vec3(1.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    int projectionMatrixLocation = glGetUniformLocation(program, "projectionMatrix"); // Get the location of our projection matrix in the shader
    int viewMatrixLocation = glGetUniformLocation(program, "viewMatrix"); // Get the location of our view matrix in the shader
    int modelMatrixLocation = glGetUniformLocation(program, "modelMatrix"); // Get the location of our model matrix in the shader
    
    
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]); // Send our projection matrix to the shader
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]); // Send our view matrix to the shader
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader
    
    
	 //// The Shader Program ends here
    //// START TO READ AGAIN
    //// START TO READ AGAIN
    //// START TO READ AGAIN
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    


    glBindAttribLocation(program, 0, "in_Position");
    glBindAttribLocation(program, 1, "in_Color");
    

    
    // this creates the scene
    setupScene();
    
    int i=0;

    // Enable depth test
    // ignore this line, it allows us to keep the distance value after we proejct each object to a 2d canvas.
    glEnable(GL_DEPTH_TEST);
    
    // This is our render loop. As long as our window remains open (ESC is not pressed), we'll continue to render things.
    while(!glfwWindowShouldClose(window))
    {
        
        // Clear the entire buffer with our green color (sets the background to be green).
        glClearBufferfv(GL_COLOR , 0, clear_color);
        glClearBufferfv(GL_DEPTH , 0, clear_depth);
        
        // this draws the coordinate system
		coordinate_system_renderer->draw();
        
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //// This generate the object
        // Enable the shader program
        glUseProgram(program);
        
        // this changes the camera location
        glm::mat4 rotated_view = viewMatrix * GetRotationMatrix();
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &rotated_view[0][0]); // send the view matrix to our shader
        

        // This moves the model to the right
      modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.0f, 0.0f));
      glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader
        

		// This line renders your triangle strip model
        renderTriangleStripModel();
        

        // This moves the model to the left
       modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, -0.0f, 0.0f));
       glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader
        
		// This line renders your Ppolygon model
	   renderTriangleModel();

        
		// disable the shader program
        glUseProgram(0);


        //// This generate the object
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        
        
        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();
        
    }
    
	// delete the coordinate system object
	delete coordinate_system_renderer;

    // Program clean up when the window gets closed.
    glDeleteVertexArrays(2, vaoID);
    glDeleteProgram(program);
}

