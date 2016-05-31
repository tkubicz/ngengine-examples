#include "ColoredTriangleExample.h"
#include "NGE/Media/MediaManager.hpp"
#include "NGE/Media/Shaders/GLSLProgram.hpp"
#include "NGE/Events/EventManager.hpp"
#include "NGE/Core/Delegate.hpp"
#include "NGE/Tools/GLError.hpp"

ColoredTriangleExample::ColoredTriangleExample() {
}

ColoredTriangleExample::~ColoredTriangleExample() {
}

bool ColoredTriangleExample::Init() {
	check_gl_error();

	log_debug("Initialising MediaManager");
	Media::MediaManager::GetInstance().initialize();

	log_debug("Loading shaders");
	Media::Shaders::ShaderManager& shaderManager = Media::Shaders::ShaderManager::GetInstance();
	shaderManager.LoadProgram("shader", "shader.xml");
	shader = shaderManager.GetProgram("shader");

	vertices[0].position = Math::vec3f(-1, -1, 0);
	vertices[1].position = Math::vec3f(0, 1, 0);
	vertices[2].position = Math::vec3f(1, -1, 0);

	vertices[0].color = Math::vec3f(1, 0, 0);
	vertices[1].color = Math::vec3f(0, 1, 0);
	vertices[2].color = Math::vec3f(0, 0, 1);

	indices[0] = 0;
	indices[1] = 1;
	indices[2] = 2;

	Math::vec3f testVerts[3];

	testVerts[0] = Math::vec3f(1, 1, 0);
	testVerts[1] = Math::vec3f(1, 0, 1);
	testVerts[2] = Math::vec3f(0, 0, 1);

	glGenVertexArrays(1, &vaoID);
	glGenBuffers(1, &vboVerticesID);
	glGenBuffers(1, &vboIndicesID);
	GLsizei stride = sizeof (Vertex);

	if (vaoID == 0 || vboVerticesID == 0 || vboIndicesID == 0) {
		log_error("Could not generate buffers");
		return false;
	}

	glBindVertexArray(vaoID);
	glBindBuffer(GL_ARRAY_BUFFER, vboVerticesID);
	glBufferData(GL_ARRAY_BUFFER, sizeof (vertices), &vertices[0], GL_STATIC_DRAW);
	check_gl_error();

	glEnableVertexAttribArray(shader->getAttribLocation("vVertex"));
	glVertexAttribPointer(shader->getAttribLocation("vVertex"), 3, GL_FLOAT, GL_FALSE, stride, 0);
	check_gl_error();

	glEnableVertexAttribArray(shader->getAttribLocation("vColor"));
	glVertexAttribPointer(shader->getAttribLocation("vColor"), 3, GL_FLOAT, GL_FALSE, stride, (const GLfloat*) offsetof(Vertex, color));
	check_gl_error();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (indices), &indices[0], GL_STATIC_DRAW);
	check_gl_error();

	return true;
}

void ColoredTriangleExample::Render() {
	Rendering::Renderer::GetInstance().ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	Rendering::Renderer::GetInstance().ClearBuffers();
	Rendering::Renderer::GetInstance().GetMatrixStack().Identity();

	shader->bindShader();
	shader->sendUniform4x4("MVP", &(projectionMatrix * modelViewMatrix)[0], false);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);
	shader->unbindShader();
}

void ColoredTriangleExample::Shutdown() {
	Media::MediaManager::GetInstance().GetShaderManager().Deinitialise();
}

void ColoredTriangleExample::OnResize(int width, int height) {
	Application::OnResize(width, height);
	projectionMatrix.SetOrthographicProjection(-1, 1, -1, 1, -1, 1);
}