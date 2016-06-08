#include "SubdivisionGeometryExample.h"
#include <NGE/Core/Core.hpp>
#include <NGE/Rendering/Rendering.hpp>
#include <NGE/Media/Shaders/ShaderManager.hpp>
#include <NGE/Tools/GLError.hpp>

using namespace NGE;

bool SubdivisionGeometryExample::Init() {
	Media::Shaders::ShaderManager& manager = Media::Shaders::ShaderManager::GetInstance();
	manager.LoadProgram("shader", "shader.xml");

	shader = manager.GetProgram("shader");
	shader->BindShader();
	shader->sendUniform("sub_divisions", sub_divisions);

	CreateGeometry();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	return true;
}

void SubdivisionGeometryExample::Render() {
	Rendering::Renderer::GetInstance().ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	Rendering::Renderer::GetInstance().ClearBuffers();

	Math::mat4f T, Rx, Ry, MV;
	T.Translate(0.f, 0.f, dist);
	Rx.RotateAxis(rX, Math::vec3f(1.f, 0.f, 0.f));
	Ry.RotateAxis(rY, Math::vec3f(0.0f, 1.0f, 0.0f));
	MV.Translate(-5, 0, -5);

	MVP = P * T * Rx * Ry * MV;

	shader->sendUniform("sub_divisions", sub_divisions);
	shader->sendUniform4x4("MVP", &MVP[0]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

	T.Translate(10, 0, 0);
	MVP *= T;
	shader->sendUniform4x4("MVP", &MVP[0]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

	T.Translate(0, 0, 10);
	MVP *= T;
	shader->sendUniform4x4("MVP", &MVP[0]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);

	T.Translate(-10, 0, 0);
	MVP *= T;
	shader->sendUniform4x4("MVP", &MVP[0]);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
}

void SubdivisionGeometryExample::Shutdown() {
	shader->Terminate();

	glDeleteBuffers(1, &vboVerticesId);
	glDeleteBuffers(1, &vboIndicesId);
	glDeleteVertexArrays(1, &vaoId);

	check_gl_error();
}

void SubdivisionGeometryExample::OnMouse(NGE::Events::MouseEvent& event) {
	if (event.GetButtonId() == BUTTON2) {
		state = 0;
	} else {
		state = 1;
	}

	if (state == 0) {
		dist *= (1 + (event.GetY() - oldY) / 60.0f);
	} else {
		rY += (event.GetX() - oldX) / 22.0f;
		rX += (event.GetY() - oldY) / 22.0f;
	}
	oldX = event.GetX();
	oldY = event.GetY();
}

void SubdivisionGeometryExample::OnResize(int width, int height) {
	Application::OnResize(width, height);
	P.SetPerspectiveProjection(45.f, (float) width / (float) height, 0.01f, 10000.f);
}

void SubdivisionGeometryExample::CreateGeometry() {
	// Setup quad vertices.
	vertices[0] = Math::vec3f(-5, 0, -5);
	vertices[1] = Math::vec3f(-5, 0, 5);
	vertices[2] = Math::vec3f(5, 0, 5);
	vertices[3] = Math::vec3f(5, 0, -5);

	// Setup quad indices.
	GLushort* id = &indices[0];
	*id++ = 0;
	*id++ = 1;
	*id++ = 2;
	*id++ = 0;
	*id++ = 2;
	*id++ = 3;

	glGenVertexArrays(1, &vaoId);
	glGenBuffers(1, &vboVerticesId);
	glGenBuffers(1, &vboIndicesId);
	check_gl_error();

	glBindVertexArray(vaoId);
	{
		glBindBuffer(GL_ARRAY_BUFFER, vboVerticesId);
		glBufferData(GL_ARRAY_BUFFER, sizeof (vertices), &vertices[0], GL_STATIC_DRAW);
		check_gl_error();

		glEnableVertexAttribArray(shader->GetAttribLocation("vVertex"));
		glVertexAttribPointer(shader->GetAttribLocation("vVertex"), 3, GL_FLOAT, GL_FALSE, 0, 0);
		check_gl_error();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndicesId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof (indices), &indices[0], GL_STATIC_DRAW);
		check_gl_error();
	}
}

void SubdivisionGeometryExample::OnKeyPressed(NGE::Events::KeyboardEvent& event) {
	if (event.GetAction() == PRESSED) {

		switch (event.GetKeyId()) {
			case KEY_UP:
			case KEY_W:
				sub_divisions++;
				break;

			case KEY_DOWN:
			case KEY_S:
				sub_divisions--;
				break;
		}
	}
}

