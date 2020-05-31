#pragma once

#include <functional>
#include <vector>
#include <string>
#include <iostream>

namespace scene
{
	class Scene
	{
	public:
		Scene(){}
		virtual ~Scene(){}

		virtual void OnUpdate(float deltaTime){}
		virtual void OnRender(){}
		virtual void OnImGuiRender(){}
	};

	class SceneMenu : public Scene
	{
	public:
		SceneMenu(Scene*& currentScenePointer);
		void OnImGuiRender() override;

		template<typename T>
		void RegisterScene(const std::string& name)
		{
			std::cout << "[Scene]: " << "Registering scene " << name << std::endl;
			m_Scenes.push_back(std::make_pair(name, []() {return new T(); }));
		}
		template<typename T, typename D>
		void RegisterScene(const std::string& name, D value)
		{
			std::cout << "[Scene]: " << "Registering scene " << name << std::endl;
			// Value = Capture value
			m_Scenes.push_back(std::make_pair(name, [value]() {return new T(value); }));
		}
		template<typename T, typename D, typename F>
		void RegisterScene(const std::string& name, D value, F value1)
		{
			std::cout << "[Scene]: " << "Registering scene " << name << std::endl;
			// Value and Value1 = Capture values
			m_Scenes.push_back(std::make_pair(name, [value, value1]() {return new T(value, value1); }));
		}
	private:
		// Reference to a pointer
		Scene*& m_CurrentScene;
		std::vector<std::pair<std::string, std::function<Scene* ()>>> m_Scenes;
	};
}
