#include "SceneClearColor.h"

#include <GL/glew.h>
#include "Renderer.h"
#include "imgui/imgui.h"

namespace scene
{
	SceneClearColor::SceneClearColor()
		: m_Color{ 0.2f, 0.3f, 0.8f, 1.0f }
	{
	}

	SceneClearColor::~SceneClearColor()
	{
	}

	void SceneClearColor::OnUpdate(float deltaTime)
	{
	}

	void SceneClearColor::OnRender()
	{
		GLCall(glClearColor(m_Color[0], m_Color[1], m_Color[2], m_Color[3]));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}

	void SceneClearColor::OnImGuiRender()
	{
		ImGui::ColorEdit4("Clear Color", m_Color);
	}
}

