#include "Scene.h"
#include "imgui/imgui.h"

namespace scene
{
	SceneMenu::SceneMenu(Scene*& currentScenePointer)
		: m_CurrentScene(currentScenePointer)
	{
		
	}

	void SceneMenu::OnImGuiRender()
	{
		for (auto& test : m_Scenes)
		{
			if (ImGui::Button(test.first.c_str()))
				m_CurrentScene = test.second();
		}
	}

}
