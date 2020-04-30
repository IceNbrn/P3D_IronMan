#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
#include "scenes/Scene.h"
#include "scenes/SceneClearColor.h"
#include "scenes/SceneTexture.h"
#include "scenes/Scene3D.h"

void ProcessInput(GLFWwindow* window, bool* isMyToolActive)
{
	//TODO: this function needs refactoring
	// If key 'escape' is pressed exit the application
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || *isMyToolActive)
	{
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        glfwTerminate();
        exit(0);
	}
}

int main(void)
{
    GLFWwindow* window;
    
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    bool isMyToolActive = false;

    const char* glslVersion = "#version 130";

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1280, 720, "IPCA - P3D | Grupo 1 | IronMan", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //Sync with vsync
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error !" << std::endl;
	
	std::cout << glGetString(GL_VERSION) << std::endl;

	{
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		Renderer renderer;
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glslVersion);

		scene::Scene* currentScene = nullptr;
		scene::SceneMenu* sceneMenu = new scene::SceneMenu(currentScene);
		currentScene = sceneMenu;

		sceneMenu->RegisterScene<scene::SceneClearColor>("Clear Color");
		sceneMenu->RegisterScene<scene::SceneTexture2D, GLFWwindow*>("2D Texture", window);
		sceneMenu->RegisterScene<scene::Scene3D, GLFWwindow*>("3D", window);


		while (!glfwWindowShouldClose(window))
		{
			ProcessInput(window, &isMyToolActive);
			GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
			renderer.Clear();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			//Calculate the delta time
			const float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			if (currentScene)
			{
				currentScene->OnUpdate(deltaTime);
				currentScene->OnRender();
				ImGui::Begin("Test", &isMyToolActive, ImGuiWindowFlags_MenuBar);
				if (ImGui::BeginMenuBar())
				{
					if (ImGui::BeginMenu("File"))
					{
						if (ImGui::MenuItem("Open..", nullptr)) { /* Do stuff */ }
						if (ImGui::MenuItem("Save", nullptr)) { /* Do stuff */ }
						if (ImGui::MenuItem("Close", nullptr)) { isMyToolActive = true; }
						ImGui::EndMenu();
					}
					ImGui::EndMenuBar();
				}
				if (currentScene != sceneMenu && ImGui::Button("<-"))
				{
					delete currentScene;
					currentScene = sceneMenu;
				}
				currentScene->OnImGuiRender();
				ImGui::End();
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
		delete currentScene;
		if (currentScene != sceneMenu)
			delete sceneMenu;
	}

    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
