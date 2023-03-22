#pragma once
#include <string>
#include <GL/glew.h>

namespace Gengine {
	class GLSLProgram
	{
	public:
		GLSLProgram();
		~GLSLProgram();

		void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
		void compileShadersFromSource(const char* vertexSource, const char* fragmentSource);
		void linkShaders();
		void addAttribute(const std::string& attributeName);

		GLint getUniformLocation(const std::string& uniformName);

		void use();
		void unuse();

		void destroy();

	private:
		void compileShader(const char* source, const std::string& name, GLuint& id);

		int m_numAttributes;

		GLuint m_programID;
		GLuint m_vertexShaderID;
		GLuint m_fragmentShaderID;
	};
}
