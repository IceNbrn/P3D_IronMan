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

#define SCR_WIDTH 1280
#define SCR_HEIGHT 720

Camera camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f));
bool firstMouse = true;
float lastX, lastY;

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
	if(glfwGetKey(window, GLFW_KEY_8) == GLFW_PRESS)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

int main(void)
{
    GLFWwindow* window;

	firstMouse = true;
	lastX = SCR_WIDTH / 2.0f;
	lastY = SCR_HEIGHT / 2.0f;
	camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
    
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
    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "IPCA - P3D | Grupo 1 | IronMan", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);
	
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
		sceneMenu->RegisterScene<scene::Scene3D, GLFWwindow*, Camera*>("3D", window, &camera);


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
				ImGui::Begin("Scenes", &isMyToolActive, ImGuiWindowFlags_MenuBar);
				if (ImGui::BeginMenuBar())
				{
					if (ImGui::BeginMenu("Program"))
					{
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
