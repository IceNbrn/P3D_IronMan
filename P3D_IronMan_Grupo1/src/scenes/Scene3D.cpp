#define _USE_MATH_DEFINES

#include "Scene3D.h"
#include "imgui/imgui.h"
#include <glm/gtx/string_cast.hpp>
#include "float.h"
#include <math.h>
#include <glm\gtc\matrix_inverse.hpp> // glm::inverseTranspose()
#include <vendor\glm\gtc\type_ptr.hpp>

namespace scene
{
	Scene3D::Scene3D(GLFWwindow* window, Camera* camera)
	{		
		m_Window = window;
		int width, height;
		glfwGetWindowSize(window, &width, &height);

		m_Fov = 60.0f;
		m_NearPlane = 0.1f;
		m_FarPlane = 100.0f;
		m_AspectRatio = (float)width / (float)height;

		m_Camera = camera;
		
		m_Model = glm::mat4(1.0f);
		m_View = glm::mat4(1.0f);
		m_Proj = glm::mat4(1.0f);

		m_Model = glm::rotate(m_Model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//m_View = glm::translate(m_View, glm::vec3(0.0f, 0.0f, -3.0f));
		//m_View = glm::lookAt(m_CameraPos, m_CameraPos + m_CameraFront, m_CameraUp);
		//m_Proj = glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
		m_Proj = glm::perspective(glm::radians(m_Camera->Zoom), m_AspectRatio, m_NearPlane, m_FarPlane);
		m_View = m_Camera->GetViewMatrix();
		m_CameraSpeed = 2.5f;
		
		m_Radius = 10.0f;
		

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
		/*
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 24 * 5 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);*/

		m_ModelIronMan = std::make_unique<Model>("res/models/Iron_Man.xyz");
		const bool result = m_ModelIronMan->LoadModel();
		if (!result)
			std::cout << "[Model]: Can't load the model!" << std::endl;
		
		m_VertexBuffer = std::make_unique<VertexBuffer>(&m_ModelIronMan->GetVertices()[0], m_ModelIronMan->GetVertices().size() * sizeof(Vertex));
				
		VertexBufferLayout layout;
		layout.Push<float>(3); // Vertex Position
		layout.Push<float>(2); // Vertex Texture Coordinate
		layout.Push<float>(3); // Vertex Normal

		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		//m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 36);

		m_Shader = std::make_unique<Shader>("res/shaders/3dTest.vs", "res/shaders/3dTest.fs");
		m_Shader->Bind();

		//m_Texture = std::make_unique<Texture>("res/textures/BoxGL.png");
		m_Texture = std::make_unique<Texture>(m_ModelIronMan->GetMaterial()->GetTextureFilepath());
		m_Texture->Bind(0);
		m_Shader->SetUniform1i("u_Texture", 0);
	}

	Scene3D::~Scene3D()
	{
		m_Texture->Unbind();
		m_Shader->Unbind();
		GLCall(glDisable(GL_DEPTH_TEST));
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	void Scene3D::OnUpdate(float deltaTime)
	{
		ProcessInput(m_Window, deltaTime);
		m_View = m_Camera->GetViewMatrix();
		m_Proj = glm::perspective(glm::radians(m_Camera->Zoom), m_AspectRatio, m_NearPlane, m_FarPlane);

	}

	void Scene3D::OnRender()
	{
		GLCall(glClearColor(0.39f, 0.58f, 0.92f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		Renderer renderer;
		m_Shader->SetUniformMat4f("u_Projection", m_Proj);
		m_Shader->SetUniformMat4f("u_View", m_View);
		//m_Shader->SetUniform1f("material", 1.0f);

		{
			m_Model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -4.0f, 0.0f));
			m_Shader->Bind();

			m_Shader->SetUniformMat4f("u_Model", m_Model);

			
			glm::mat4 modelView = m_View * m_Model;
			m_Shader->SetUniformMat4f("u_ModelView", modelView);
			
			
			glm::mat3 normalMatrix;
			normalMatrix = glm::inverseTranspose(glm::mat3(modelView));
			
			m_Shader->SetUniformMat3f("u_NormalMatrix", normalMatrix);

			// Lighting work in progress
			m_Shader->SetUniformVec3f("u_AmbientLight.ambient", glm::vec3(0.1f, 0.1f, 0.1f));
			

			renderer.Draw(*m_VAO, *m_Shader, m_ModelIronMan->GetVertices().size());
		}
		
	}

	void Scene3D::OnImGuiRender()
	{
		if (ImGui::Button("Reset Camera Position")) m_Camera->ResetPosition();
		ImGui::SliderFloat("Camera Speed", m_Camera->GetMovementSpeed(), 0.0f, 15.0f);
		ImGui::SliderFloat("Camera Zoom", &m_Camera->Zoom, 0.0f, 60.0f);
				
		ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_FirstUseEver);
		ImGui::Begin("Stats");
		ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
		ImGui::Text("Application average %.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
		
		ImGui::SetNextWindowSize(ImVec2(0, 50), ImGuiCond_FirstUseEver);
		ImGui::Begin("Help");
		ImGui::Text("Tecla 1: Lock Cursor");
		ImGui::Text("Tecla 2: Unlock Cursor");
		ImGui::Text("WASD: Para mover a camara");
		ImGui::Text("ScrollWheel: Zoom In e Zoom Out");
		ImGui::End();
	}

	void Scene3D::ProcessInput(GLFWwindow* window, float dt)
	{
		float cameraSpeed = m_CameraSpeed * dt;

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			m_Camera->ProcessKeyboard(FORWARD, dt);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			m_Camera->ProcessKeyboard(BACKWARD, dt);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			m_Camera->ProcessKeyboard(LEFT, dt);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			m_Camera->ProcessKeyboard(RIGHT, dt);
		if (glfwGetKey(window, GLFW_KEY_R))
			m_Camera->ResetPosition();
	}

}
