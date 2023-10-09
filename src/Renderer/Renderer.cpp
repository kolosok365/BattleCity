#include "Renderer.h"

namespace RenderEngine {
	void Renderer::draw(const VertexArray& vertexArray, const IndexBuffer& indexBudffer, const ShaderProgram& shader) {
		shader.use();
		vertexArray.bind();
		indexBudffer.bind();

		glDrawElements(GL_TRIANGLES, indexBudffer.getCount(), GL_UNSIGNED_INT, 0);
	}

	void Renderer::setClearColor(float r, float g, float b, float a) {
		glClearColor(r, g, b, a);
	}

	void Renderer::setDepthTest(const bool enable) {
		if (enable) {
			glEnable(GL_DEPTH_TEST);
		}
		else {
			glDisable(GL_DEPTH_TEST);
		}
	}

	void Renderer::clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//очищает буферы до заданных значений
	}

	void Renderer::setViewport(unsigned int width, unsigned int height, unsigned int leftOffset, unsigned int bottomOffset) {
		glViewport(leftOffset, bottomOffset, width, height);
	}

	std::string Renderer::getRendererStr() {
		return (char*)glGetString(GL_RENDERER);
	}

	std::string Renderer::getVersionStr() {
		return (char*)glGetString(GL_VERSION);
	}
}