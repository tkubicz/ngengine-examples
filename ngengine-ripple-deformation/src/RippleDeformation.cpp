#include "RippleDeformation.h"
#include <NGE/Core/Core.hpp>
#include <NGE/Rendering/Rendering.hpp>
#include <NGE/Media/Shaders/ShaderManager.hpp>
#include <NGE/Tools/GLError.hpp>

bool RippleDeformation::Init() {
	Media::Shaders::ShaderManager& shaderManager = Media::Shaders::ShaderManager::GetInstance();

	shaderManager.LoadProgram("shader", "shader.xml");
	shader = shaderManager.GetProgram("shader");
	if (shader == nullptr) {
		log_error("Could not load shader");
		return false;
	}

	check_gl_error();

	CreateTopology();
	CreateBufferObjects();

	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	return true;
}

void RippleDeformation::Prepare(float dt) {
	time = dt * 1000;
}

void RippleDeformation::Render() {
	Rendering::Renderer::GetInstance().ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	Rendering::Renderer::GetInstance().ClearBuffers();
	Rendering::Renderer::GetInstance().GetMatrixStack().Identity();

	Math::mat4f T, Rx, MV, MVP;
	T.Translate(0, 0, dist);
	Rx = T;
	Rx.RotateX(rX);
	MV = Rx;
	MV.RotateY(rY);
	MVP = P * MV;

	shader->bindShader();
	shader->sendUniform4x4("MVP", &MVP[0], false);
	shader->sendUniform("time", time);
	glDrawElements(GL_TRIANGLES, TOTAL_INDICES, GL_UNSIGNED_SHORT, 0);
	shader->unbindShader();
}

void RippleDeformation::Shutdown() {
	shader->unload();

	glDeleteBuffers(1, &vboVerticesID);
	glDeleteBuffers(1, &vboIndicesID);
	glDeleteVertexArrays(1, &vaoID);
}

void RippleDeformation::OnResize(int width, int height) {
	Application::OnResize(width, height);

	P.SetPerspectiveProjection(45.f, float(width) / float(height), 1.f, 1000.f);
}

void RippleDeformation::OnMouse(NGE::Events::MouseEvent& event) {
}


void RippleDeformation::CreateTopology() {
	// Setup plane geometry.
	int count = 0, i = 0, j = 0;
	for (j = 0; j < NUM_Z; ++j) {
		for (i = 0; i < NUM_X; ++i) {
			vertices[count++] = Math::vec3f(((float(i) / (NUM_X - 1)) * 2 - 1) * HALF_SIZE_X, 0, ((float(j) / (NUM_Z - 1)) * 2 - 1) * HALF_SIZE_Z);
		}
	}

	// Fill plane indices array.
	GLushort* id = &indices[0];
	for (i = 0; i < NUM_Z; ++i) {
		for (j = 0; j < NUM_X; ++j) {
			int i0 = i * (NUM_X + 1) + j;
			int i1 = i0 + 1;
			int i2 = i0 + (NUM_X + 1);
			int i3 = i2 + 1;
			if ((j + i) % 2) {
				*id++ = i0;
				*id++ = i2;
				*id++ = i1;
				*id++ = i1;
				*id++ = i2;
				*id++ = i3;
			} else {
				*id++ = i0;
				*id++ = i2;
				*id++ = i3;
				*id++ = i0;
				*id++ = i3;
				*id++ = i1;
			}
		}
	}
}

void RippleDeformation::CreateBufferObjects() {
	glGenVertexArrays(1, &vaoID);
	glGenBuffers(1, &vboVerticesID);
	glGenBuffers(1, &vboIndicesID);

	glBindVertexArray(vaoID);
	{
		glBindBuffer(GL_ARRAY_BUFFER, vboVerticesID);
		glBufferData(GL_ARRAY_BUFFER, sizeof (vertices), &vertices[0], GL_STATIC_DRAW);

		check_gl_error();

		glEnableVertexAttribArray(shader->getAttribLocation("vVertex"));
		glVertexAttribPointer(shader->getAttribLocation("vVertex"), 3, GL_FLOAT, GL_FALSE, 0, 0);

		check_gl_error();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (indices), &indices[0], GL_STATIC_DRAW);

		check_gl_error();
	}
}
