#include "GLSLProgram.h"
#include "Errors.h"

#include <vector>
#include <fstream>

GLSLProgram::GLSLProgram() : _numAttributes(0), _programID(0), _vertexShaderID(0), _fragmentShaderID(0) {
	
}

GLSLProgram::~GLSLProgram() {
	
}

void GLSLProgram::CompileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) {
	//Vertex and fragment shaders are successfully compiled.
	//Now time to link them together into a program.
	//Get a program object.
	_programID = glCreateProgram();
	
	_vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	if (_vertexShaderID == 0) {
		FatalError("Vertex shader failed to be created!");
	}

	_fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	if (_fragmentShaderID == 0) {
		FatalError("Fragment shader failed to be created!");
	}

	CompileShader(vertexShaderFilePath, _vertexShaderID);
	CompileShader(fragmentShaderFilePath, _fragmentShaderID);
}

void GLSLProgram::LinkShaders() {
	//Attach our shaders to our program
	glAttachShader(_programID, _vertexShaderID);
	glAttachShader(_programID, _fragmentShaderID);

	//Link our program
	glLinkProgram(_programID);

	//Note the different functions here : glGetProgram* instead of glGetShader*
	GLint isLinked = 0;
	glGetProgramiv(_programID, GL_LINK_STATUS, (int*)&isLinked);
	if (isLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(_programID, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<char> errorLog(maxLength);
		glGetProgramInfoLog(_programID, maxLength, &maxLength, &errorLog[0]);
		
		//We don't need the program anymore.
		glDeleteProgram(_programID);
		//Don't leak shaders either.
		glDeleteShader(_vertexShaderID);
		glDeleteShader(_fragmentShaderID);

		std::printf("%s\n", &(errorLog[0]));
		FatalError("Shader failed to link");
	}

	//Always detach shaders after a successful link.
	glDetachShader(_programID, _vertexShaderID);
	glDetachShader(_programID, _fragmentShaderID);
	glDeleteShader(_vertexShaderID);
	glDeleteShader(_fragmentShaderID);
}

void GLSLProgram::AddAttribute(const std::string& attributeName) {
	glBindAttribLocation(_programID, _numAttributes++, attributeName.c_str());
}

GLuint GLSLProgram::GetUniformLocation(const std::string& uniformName) {
	GLuint location = glGetUniformLocation(_programID, uniformName.c_str());
	if (location == GL_INVALID_INDEX) {
		FatalError("Uniform " + uniformName + " not found in shader");
	}

	return location;
}

void GLSLProgram::Use() {
	glUseProgram(_programID);
	for (int i = 0; i < _numAttributes; i++) {
		glEnableVertexAttribArray(i);
	}
}

void GLSLProgram::Unuse() {
	glUseProgram(0);
	for (int i = 0; i < _numAttributes; i++) {
		glDisableVertexAttribArray(i);
	}
}

void GLSLProgram::CompileShader(const std::string& filePath, GLuint& id) {
	std::ifstream vertexFile(filePath);
	if (vertexFile.fail()) {
		perror(filePath.c_str());
		FatalError("Failed to open " + filePath);
	}

	std::string fileContents = "";
	std::string line;

	while (std::getline(vertexFile, line)) {
		fileContents += line + "\n";
	}

	vertexFile.close();

	const char* contentsPtr = fileContents.c_str();
	glShaderSource(id, 1, &contentsPtr, nullptr);

	glCompileShader(id);

	GLint isCompiled = 0;
	glGetShaderiv(id, GL_COMPILE_STATUS, &isCompiled);

	if (isCompiled == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		std::vector<char> errorLog(maxLength);
		glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

		//Provide the infolog in whatever manor you deem best.
		//Exitwith failure.
		glDeleteShader(id);

		std::printf("%s\n", &(errorLog[0]));
		FatalError("Shader " + filePath + " failed to compile");
	}
}