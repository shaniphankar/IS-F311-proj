#include "shader.h"
#include <string.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

/*! This function reads the textfile given. In our case, these are the vertex and fragment shaders. 
\param filename Contains name of the image to be loaded
\param width Width of the image
\param height Height of the Image
\return The texture which the input image has been bound.
*/
static char* textFileRead(const char *fileName) {
	char* text;
    
	if (fileName != NULL) {
        FILE *file = fopen(fileName, "rt");
        
		if (file != NULL) {
            fseek(file, 0, SEEK_END);
            int count = ftell(file);
            rewind(file);
            
			if (count > 0) {
				text = (char*)malloc(sizeof(char) * (count + 1));
				count = fread(text, sizeof(char), count, file);
				text[count] = '\0';
			}
			fclose(file);
		}
	}
	return text;
}

/*! Constructor for our Shader object. Doesn't do anything since we never made use of empty call to shader.
*/
Shader::Shader() {
    
}

/*! Constructor for our Shader Object.
\param vsFile Contains path to vertex shader
\param fsFile Contains path to fragment shader
\return The texture which the input image has been bound.
*/
Shader::Shader(const char *vsFile, const char *fsFile) {
    init(vsFile, fsFile);
}

/*! Initializes our shader using the given vertex and fragment shader.
\param vsFile Contains path to vertex shader
\param fsFile Contains path to fragment shader
*/
void Shader::init(const char *vsFile, const char *fsFile) {
	shader_vp = glCreateShader(GL_VERTEX_SHADER);
	shader_fp = glCreateShader(GL_FRAGMENT_SHADER);
    
	const char* vsText = textFileRead(vsFile);
	const char* fsText = textFileRead(fsFile);	
    
    if (vsText == NULL || fsText == NULL) {
        cerr << "Either vertex shader or fragment shader file not found." << endl;
        return;
    }
    
	glShaderSource(shader_vp, 1, &vsText, 0);
	glShaderSource(shader_fp, 1, &fsText, 0);
    
	glCompileShader(shader_vp);
	glCompileShader(shader_fp);

	shader_id = glCreateProgram();
	glAttachShader(shader_id, shader_fp);
	glAttachShader(shader_id, shader_vp);
	glLinkProgram(shader_id);
}

/*! Destructor for our Shader Object. 
*/
Shader::~Shader() {
	glDetachShader(shader_id, shader_fp);
	glDetachShader(shader_id, shader_vp);
	glDeleteShader(shader_fp);
	glDeleteShader(shader_vp);
	glDeleteProgram(shader_id);
}

/*! Getter function that returns id of Shader object currently in use.
*/
unsigned int Shader::id() {
	return shader_id;
}

/*! On calling this, the shader takes effect in our scene.
*/
void Shader::bind() {
	glUseProgram(shader_id);
}
/*! On calling this, the shader loses effect in our scene.
*/
void Shader::unbind() {
	glUseProgram(0);
}
