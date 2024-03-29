#pragma once

#include "Scene.h"

namespace scene
{
	class SceneClearColor : public Scene
	{
	public:
		SceneClearColor();
		~SceneClearColor();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		float m_Color[4];
	};
}
