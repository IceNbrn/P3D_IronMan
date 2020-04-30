#include "SceneTexture.h"

#include <GL/glew.h>
#include "Renderer.h"
#include "imgui/imgui.h"
#include "Random.h"

namespace scene
{
	SceneTexture2D::SceneTexture2D(GLFWwindow* window)
		:
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
		m_TranslationA(200, 200, 0),
		m_TranslationB(400, 200, 0),
		m_Window(window),
		m_Rotation(1.0f)
	{
		int width, height;
		glfwGetWindowSize(m_Window, &width, &height);

		m_ScreenSize.x = width;
		m_ScreenSize.y = height;

		m_Proj = glm::ortho(0.0f, m_ScreenSize.x, 0.0f, m_ScreenSize.y, -1.0f, 1.0f);

		m_Speed.x = Random::Float(-1.0f, 1.0f);
		m_Speed.y = m_Speed.x;

		float positions[] = {
			-50.0f, -50.0f, 0.0f, 0.0f,  //0
			 50.0f, -50.0f, 1.0f, 0.0f,  //1
			 50.0f,  50.0f, 1.0f, 1.0f,  //2
			-50.0f,  50.0f, 0.0f, 1.0f   //3
		};

		//Index buffer
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		//Vertex array object
		m_VAO = std::make_unique<VertexArray>();

		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);

		m_VAO->AddBuffer(*m_VertexBuffer, layout);
		m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);

		//m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");
		m_Shader = std::make_unique<Shader>("res/shaders/basic.vs", "res/shaders/basic.fs");
		m_Shader->Bind();
		m_Shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

		m_Texture = std::make_unique<Texture>("res/textures/BoxGL.png");
		m_Shader->SetUniform1i("u_Texture", 0);

	}

	SceneTexture2D::~SceneTexture2D()
	{
	}

	void SceneTexture2D::OnUpdate(float deltaTime)
	{
		m_Rotation = glm::rotate(m_Rotation, glm::radians(10.0f) * deltaTime, glm::vec3(0.0f, 0.0f, 1.0f));

		m_TranslationA.x += m_Speed.x;
		m_TranslationA.y += m_Speed.y;

		CheckCollision();
	}

	void SceneTexture2D::OnRender()
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;
		m_Texture->Bind();

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA) * m_Rotation;
			glm::mat4 mvp = m_Proj * m_View * model;

			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}

		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
			glm::mat4 mvp = m_Proj * m_View * model;

			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
	}

	void SceneTexture2D::OnImGuiRender()
	{
		float randomValue = Random::Float();
		int max = 10.0f;
		int min = -10.0f;
		ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 960.0f);
		ImGui::SliderFloat2("Speed", &m_Speed.x, 0.0f, 10.0f);

		ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_FirstUseEver);
		ImGui::Begin("Stats");
		ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
		ImGui::Text("Application average %.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

	void SceneTexture2D::CheckCollision()
	{
		if (m_TranslationA.y > m_ScreenSize.y) m_Speed.y = -m_Speed.y;
		else if (m_TranslationA.y <= 0) m_Speed.y = -m_Speed.y;

		if (m_TranslationA.x > m_ScreenSize.x) m_Speed.x = -m_Speed.x;
		else if (m_TranslationA.x <= 0) m_Speed.x = -m_Speed.x;
	}
}

