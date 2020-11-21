/*
 * PA_4.cpp
 *
 *  Created on: Nov 17, 2020
 *      Author: 1509742_snhu
 */


#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

using namespace std;

#define WINDOW_TITLE "2D Right-Triangles" //window title name

//Vertex and fragment shader source macro
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version "\n" #Source
#endif

//Variables for window width and height
int WindowWidth = 800, WindowHeight = 600;

/* User-defined function prototypes to:
 * initialize the program, set window size,
 * redraw graphics on the window when resized,
 * and render graphics on the screen
 */
void UInitialize(int, char*[]);
void UInitWindow(int, char*[]);
void UResizeWindow(int, int);
void URenderGraphics(void);
void UCreateVBO(void);
void UCreateShaders(void);

//Vertex Shader Program Source Code
/*Vertex Shader Program Source Code*/
const GLchar* VertexShader =
{
  "#version 400\n" // Declare version number

  "layout(location=0) in vec4 in_Position;\n" // Receive vertex coordinates from attribute 0. i.e. 2 floats per vertex.

   /*Get the vertex colors from the Vertex Buffer Object*/
  "layout(location=1) in vec4 in_Color;\n" // for attribute 1 expect vec(4) floats passed into the Vertex shader.
  "out vec4 ex_Color;\n" // declare a vec 4 variable that will reference the vertex colors passed into the Vertex shader from the buffer.

  "void main(void)\n"
  "{\n"
  "  gl_Position = in_Position;\n" // Sends vertex positions to gl_position vec 4
  "  ex_Color = in_Color;\n" // references vertex colors sent from the buffer
  "}\n"
};


/*Fragment Shader Program Source Code*/
const GLchar* FragmentShader =
{
  "#version 400\n"

  "in vec4 ex_Color;\n" // Vertex colors from the vertex shader
  "out vec4 out_Color;\n" // vec4 variable that will reference the vertex colors passed into the fragment shader the vertex shader

  "void main(void)\n"
  "{\n"
  //"  out_Color = vec4(0.0, 1.0, 0.0, 1.0);\n" // Sets the pixels / fragments of the triangle to green
  "  out_Color = ex_Color;" // Send the vertex colors to the GPU
  "}\n"
};

//main function. Entry point to the OPenGl Program
int main (int argc, char* argv[])
{

   UInitialize(argc, argv); //Init OpenGL program
   glutMainLoop(); //starts the OpenGL loop in background
   exit(EXIT_SUCCESS); // terminates the program
}

//implements the UInitialize function
void UInitialize (int argc, char* argv[])
{
	//glew status variable

	GLenum GlewInitResult;

	UInitWindow (argc, argv); //creates the window

	//checks glew status
	GlewInitResult = glewInit();


	if (GLEW_OK != GlewInitResult)
	{
		fprintf(stderr, "ERROR: %s\n", glewGetErrorString(GlewInitResult));
		exit(EXIT_FAILURE);
	}

	//Displays GPU OpenGL version
	fprintf(stdout, "INFO: OpenGL version: %s\n", glGetString(GL_VERSION));

	UCreateVBO(); // calls the function to create the vertex buffer object

	UCreateShaders(); // calls the function to create the shader program

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

//Implements the UInitWindow Function
void UInitWindow(int argc, char* argv[])
{
	//Initializes freeglut
	glutInit(&argc, argv);

	// sets the window size
   glutInitWindowSize(WindowWidth, WindowHeight);

   //Memory Buffer setup for display
   glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

   // Creates a window with the macro placeholder title
   glutCreateWindow(WINDOW_TITLE);

   glutReshapeFunc(UResizeWindow); //called whrn the windows is resized
   glutDisplayFunc(URenderGraphics); //renders graphics on the screen
}

//Implements the UResizeWindow function
void UResizeWindow(int Width, int Height)
{
	glViewport(0, 0, Width, Height);
}

//Implements the URenderGraphics Function
void URenderGraphics(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clears screen

	//Creates the triangle
	GLuint totalVerticies = 6;
	//glDrawArrays(GL_TRIANGLES, 0, totalVerticies); //Draws the triangle

	// Draw triangle using indicies
	glDrawElements(GL_TRIANGLES, totalVerticies, GL_UNSIGNED_SHORT, NULL);

	glutSwapBuffers(); //flips the back buffer with front buffer every frame
}

//Implements the CreateVBO Function
void UCreateVBO(void)
{
	//Specifies coordinates for triangle verticies on x and y
	GLfloat verts[] =
	{
			//indices will be 0,1,2,3,4,1
			//index 0 will be shared

			/*index 0*/
			-1.0f, 1.0f, // top-center of the screen
			1.0f, 0.0f, 0.0f, 1.0f, // Red vertex

			/*index 1*/
			-1.0f, 0.0f, // bottom-left of the screen
			0.0f, 0.0f, 1.0f, 1.0f, // Blue vertex

			/*index 2*/
			-0.5f, 0.0f, // bottom-right of the screen
			0.0f, 1.0f, 0.0f, 1.0f, // Green vertex

			//-0.5f, 0.0f, // top-center of the screen
			//1.0f, 0.0f, 0.0f, 1.0f, // Red vertex

			/*index 3*/
			0.0f, 0.0f, // bottom-left of the screen
			1.0f, 0.0f, 0.0f, 1.0f, // Red vertex

			/*index 4*/
			0.0f, -1.0f, // bottom-right of the screen
			0.0f, 1.0f, 0.0f, 1.0f, // Green vertex
	};

	//stores the size of the verts array / number of coordinates needed for the triangle
	float numVerticies = sizeof(verts);

	GLuint myBufferID; //variable for vertex buffer object id
	glGenBuffers(1, &myBufferID); //creates 1 buffer
	glBindBuffer(GL_ARRAY_BUFFER, myBufferID); //activates buffer
	glBufferData(GL_ARRAY_BUFFER, numVerticies, verts, GL_STATIC_DRAW); //sends vertex data to GPU

	//Create the vertex Attribute pointer
	GLuint floatsPerVertex = 2; //number of coordinates per vertex
	glEnableVertexAttribArray(0);//specifies the inital position of the coordinates in the buffer

	//Strides between vertex coordinates is 6 (x,y,r,g,b,a)
	GLint vertexStride = sizeof(float) * 6;

	/*instructs the GPU on how to handle the vertex buffer object data.
	 * parameters: atrribPointerPosition | coordinates per vertex|data type| deavtivate normaliztions | 0 strides | 0 offset
	 */
	glVertexAttribPointer(0, floatsPerVertex, GL_FLOAT, GL_FALSE, vertexStride, 0);

	// sets an attribute pointer position for the vertex colors
	glEnableVertexAttribArray(1);

	GLint colorStride = sizeof(float) * 6;

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, colorStride, (char*)(sizeof(float) * 2));

	//creates buffer object for indices
	GLushort indicies[] = {0, 1, 2, 2, 3, 4};
	float numIndicies = sizeof(indicies);
	GLuint indexBufferID;
	glGenBuffers(1, &indexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndicies, indicies, GL_STATIC_DRAW);
}

//Implements the UCreateShaders function
void UCreateShaders(void)
{
	//Create a shader program object
	GLuint ProgramId = glCreateProgram();

	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER); //create a vertex shader object
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER); //create a fragment shader object

	glShaderSource(vertexShaderId, 1, &VertexShader, NULL); //retrieves the vertex shader source
	glShaderSource(fragmentShaderId, 1, &FragmentShader, NULL); //retrieves the fragment shader source

	glCompileShader(vertexShaderId); //compile the vertex shader
	glCompileShader(fragmentShaderId); //compile the fragment shader

	//attaches the shaders to the shader program
	glAttachShader(ProgramId, vertexShaderId);
	glAttachShader(ProgramId, fragmentShaderId);

	glLinkProgram(ProgramId); //links the shader program
	glUseProgram(ProgramId); //uses the shader program
}





