#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
/*
int main(void)
{
	int width = 800, height = 600;

	glm::vec3 vertex[] = {
		glm::vec3(0.5f, -0.5f, 0.0f),
		glm::vec3(0.0f, 0.5f, 0.0f),
		glm::vec3(-0.5f, -0.5f, 0.0f)
	};

	// Projection Matrix
	glm::mat4 Projection = glm::frustum(-2.0f, 2.0f, -1.5f, 1.5f, 5.0f, 20.0f);

	// View Matrix
	glm::mat4 View = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	);

	// Model Matrix
	glm::mat4 Model = glm::mat4(1.0f);
	// Translate 10 units in the -z axis
	Model = glm::translate(Model, glm::vec3(0.0f, 0.0f, -10.0f));

	// Model View Projection Matrix (MVP)
	glm::mat4 MVP = Projection * View * Model;

	// Print coordinates
	for (int i = 0; i < 3; ++i)
	{
		glm::vec4 position = MVP * glm::vec4((glm::vec3) vertex[i], 1.0f);

		std::cout << "Coordenadas de Recorte do Vertex" << i << ":\n";
		std::cout << 
			" Xclip=" << position.x <<
			" Yclip=" << position.y <<
			" Zclip=" << position.z <<
			" Wclip=" << position.w << std::endl;
	}


	std::cin.get();
	return 0;
	
}*/