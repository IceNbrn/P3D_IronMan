#include <string>
#include <vector>
#include <glm/glm.hpp>

class Model
{
public:
	Model(const std::string& filePath);
	~Model();

	bool LoadModel();

	std::vector<glm::vec3>& GetVertices();
	std::vector<glm::vec2>& GetUvs();
	std::vector<glm::vec3>& GetNormals();
	
private:
	std::string m_FilePath;
	std::vector<glm::vec3> m_OutVertices;
	std::vector<glm::vec2> m_OutUvs;
	std::vector<glm::vec3> m_OutNormals;
};