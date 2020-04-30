#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <GLFW/glfw3.h>
#include <memory>
#include "Scene.h"

namespace scene
{
	class SceneTexture2D : public scene::Scene
	{
	public:
		SceneTexture2D(GLFWwindow* window);
		~SceneTexture2D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void CheckCollision();
	private:
		GLFWwindow* m_Window;
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;

		glm::mat4 m_Proj, m_View;
		glm::vec3 m_TranslationA, m_TranslationB;
		glm::vec2 m_Speed;
		glm::mat4 m_Rotation;
		glm::vec2 m_ScreenSize;
	};
}
