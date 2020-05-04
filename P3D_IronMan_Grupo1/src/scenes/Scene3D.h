#pragma once

#include "Scene.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <GLFW/glfw3.h>

class Shader;

namespace scene
{
	class Scene3D : public Scene
	{
	public:
		Scene3D(GLFWwindow* window);
		~Scene3D();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		GLFWwindow* m_Window;
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;

		glm::mat4 m_Proj, m_View, m_Model;
		glm::vec2 m_ScreenSize;
		float m_Rotate;
		float m_RotationValue;
		float m_Radius;
	};
}