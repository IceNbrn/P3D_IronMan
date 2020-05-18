#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "Texture.h"
#include <memory>

struct Vertex
{
	glm::vec3 position;
	glm::vec2 texcoord;
	glm::vec3 normal;
};

class Model
{
public:
	Model(const std::string& filePath);
	~Model();

	bool LoadModel();

	void Draw();

	std::vector<Vertex>& GetVertices();
	
private:
	std::string m_FilePath;
	std::unique_ptr<Shader> m_Shader;
	std::unique_ptr<Texture> m_Texture;
	std::vector<Vertex> m_Vertices;
		
	std::vector<glm::vec3> m_OutPositions;
	std::vector<glm::vec2> m_OutUvs;
	std::vector<glm::vec3> m_OutNormals;
};
