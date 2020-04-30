#define _USE_MATH_DEFINES

#include "Scene3D.h"
#include "imgui/imgui.h"
#include <glm/gtx/string_cast.hpp>
#include "float.h"
#include <math.h>

namespace scene
{
	Scene3D::Scene3D(GLFWwindow* window)
	{
		int width, height;
		glfwGetWindowSize(window, &width, &height);

		float fov = 60.0f;
		float nearPlane = 0.1f;
		float farPlane = 100.0f;
		float aspectRatio = (float)width / (float)height;

		m_Model = glm::mat4(1.0f);
		m_View = glm::mat4(1.0f);
		m_Proj = glm::mat4(1.0f);

		m_Model = glm::rotate(m_Model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		m_View = glm::translate(m_View, glm::vec3(0.0f, 0.0f, -3.0f));
		m_Proj = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);

		float positions[] = {
			// positions		// texture coords
			// Back Face
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // A 0
			 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // B 1
			 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // C 2
			-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // D 3
			// Front Face
			-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, // E 4
			 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // F 5
			-0.5f,  0.5f, 0.5f, 0.0f, 1.0f, // G 6
			 0.5f,  0.5f, 0.5f, 1.0f, 1.0f, // H 7
			// Top Face
			 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // c 8
			 0.5f,  0.5f, 0.5f, 1.0f, 0.0f,	 // h 9
			-0.5f,  0.5f, 0.5f, 0.0f, 0.0f,  // g 10
			-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // d 11
			// Right Face
			 0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // f 12
			 0.5f,  0.5f, 0.5f, 1.0f, 1.0f, // h 13
			 0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // c 14
			 0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // b 15
			// Bottom Face
			 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f, // 16
			 0.5f,  -0.5f, 0.5f, 1.0f, 0.0f, // 17
			-0.5f,  -0.5f, 0.5f, 0.0f, 0.0f, // 18
			-0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, //19
			// Left Face
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // 20
			-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, // 21
			-0.5f, 0.5f, 0.5, 1.0f, 1.0f, // 22
			-0.5f, 0.5f, -0.5, 0.0f, 1.0f, //23
			
			
		};
		unsigned int indices[] = {
			// Back Face
			0, 1, 3, // first triangle
			1, 2, 3,  // second triangle
			// Front Face
			4, 5, 6,
			5, 7, 6,
			// Top Face
			8, 9, 10,
			10, 11, 8,
			// Right Face
			12, 13, 14,
			14, 15, 12,
			// Bottom Face
			16, 19, 18,
			18, 17, 16,
			// Left Face
			20, 21, 22,
			22, 23, 20
		};
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		//Vertex array object
		m_VAO = std::make_unique<VertexArray>();

		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 24 * 5 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);

		m_VAO->AddBuffer(*m_VertexBuffer, layout);
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 36);

		m_Shader = std::make_unique<Shader>("res/shaders/3dTest.vs", "res/shaders/3dTest.fs");
		m_Shader->Bind();

		m_Texture = std::make_unique<Texture>("res/textures/BoxGL.png");
		m_Shader->SetUniform1i("u_Texture", 0);
	}

	Scene3D::~Scene3D()
	{
		m_Texture->Unbind();
		m_Shader->Unbind();
		GLCall(glDisable(GL_DEPTH_TEST));
	}

	void Scene3D::OnUpdate(float deltaTime)
	{
		double sinPitch, cosPitch, sinRoll, cosRoll, sinYaw, cosYaw;

		sinPitch = -m_Model[2][0];
		cosPitch = sqrt(1 - sinPitch * sinPitch);

		if (abs(cosPitch) > DBL_EPSILON)
		{
			sinRoll = m_Model[2][1] / cosPitch;
			cosRoll = m_Model[2][2] / cosPitch;
			sinYaw = m_Model[1][0] / cosPitch;
			cosYaw = m_Model[0][0] / cosPitch;
		}
		else
		{
			sinRoll = -m_Model[1][2];
			cosRoll = m_Model[1][1];
			sinYaw = 0;
			cosYaw = 1;
		}

		//m_RotationValue = atan2(sinYaw, cosYaw) * 180 / M_PI;
		
		//m_RotationValue = atan2(sinPitch, cosPitch) * 180 / M_PI;
		m_RotationValue = atan2(sinRoll, cosRoll) * 180 / M_PI;
	}

	void Scene3D::OnRender()
	{

		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		Renderer renderer;
		m_Texture->Bind();

		//m_Model = glm::rotate(m_Model, (float)glfwGetTime() * glm::radians(2.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//m_Model = glm::rotate(m_Model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

		m_Model = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotate), glm::vec3(0.0f, 1.0f, 0.0f));

		m_Shader->Bind();
		m_Shader->SetUniformMat4f("u_Projection", m_Proj);
		m_Shader->SetUniformMat4f("u_View", m_View);
		m_Shader->SetUniformMat4f("u_Model", m_Model);
		renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
	}

	void Scene3D::OnImGuiRender()
	{
		ImGui::Text("Rotation Value: %.3f", m_RotationValue);
		ImGui::SliderFloat("Rotate", &m_Rotate, -360.0f, 360);
		ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_FirstUseEver);
		ImGui::Begin("Stats");
		ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
		ImGui::Text("Application average %.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

}
