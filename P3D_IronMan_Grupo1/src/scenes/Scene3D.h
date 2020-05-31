#pragma once

#include "Scene.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <GLFW/glfw3.h>
#include "Camera.h"
#include "Model.h"

class Shader;

namespace scene
{
	class Scene3D : public Scene
	{
	public:
		Scene3D(GLFWwindow* window, Camera* camera);
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
		std::unique_ptr<Model> m_ModelIronMan;

		glm::mat4 m_Proj, m_View, m_Model;
		glm::vec2 m_ScreenSize;
		float m_Rotate;
		float m_RotationValue;
		float m_Radius;

		float m_Fov;
		float m_NearPlane;
		float m_FarPlane;
		float m_AspectRatio;

		bool m_Ambient;
		bool m_Directional;
		bool m_Point;
		bool m_Spot;
		bool m_DeformModel;

		glm::vec3 m_AmbientLight;
		
		glm::vec3 m_CameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		float m_CameraSpeed;
		Camera* m_Camera;

		void ProcessInput(GLFWwindow* window, float dt);
	};
}
