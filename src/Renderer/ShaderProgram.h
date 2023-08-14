#pragma once

#include<iostream>
#include<glad/glad.h>
#include<string>

namespace Renderer {
	class ShaderProgram {
	public:
		ShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
		~ShaderProgram();
		bool isCompiled() const { return m_isCompiled; }
		void use() const;

		ShaderProgram() = delete; //запретили конструктор без параметров
		ShaderProgram(ShaderProgram&) = delete; //запрещаяем шейдер на основе другого шейдера, т к при удалении будут проблемы, поскольку у них одни и те же идентификаторы 
		ShaderProgram& operator=(const ShaderProgram&) = delete; //чтобы не могли присвоить один шейдер другому, поскольку у них одни и те же идентификаторы 
		ShaderProgram& operator=(ShaderProgram&& shaderProgram) noexcept; //модификатор noexcept применяется к функциям, которые гарантированно не могут генерировать исключения. 
		ShaderProgram(ShaderProgram&& shaderProgram) noexcept;						//noexcept позволяет компиляторам генерировать лучший объектный код.

	private:
		bool createShader(const std::string& source, const GLenum shaderType, GLuint& shaderID);
		bool m_isCompiled = false;
		GLuint m_ID = 0;
	};
}