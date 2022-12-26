#pragma once
#include <string>
#include <GL/glew.h>

namespace Gengine {
	class GLSLProgram
	{
	public:
		GLSLProgram();
		~GLSLProgram();

		void CompileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
		void LinkShaders();
		void AddAttribute(const std::string& attributeName);

		GLint GetUniformLocation(const std::string& uniformName);

		void Use();
		void Unuse();

	private:
		void CompileShader(const std::string& filePath, GLuint& id);

		int _numAttributes;

		GLuint _programID;
		GLuint _vertexShaderID;
		GLuint _fragmentShaderID;
	};
}
