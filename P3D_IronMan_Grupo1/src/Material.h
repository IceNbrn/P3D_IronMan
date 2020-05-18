#include <glm/glm.hpp>
#include <string>

class Material
{
public:
	glm::vec3	ks; // Specular Color
	glm::vec3	ka; // Ambient Color
	glm::vec3	kd; // Diffuse Color
	
	float		ns; // Shininess
	float		ni; // Index of Refraction

	std::string textureMap; // Texture Map
	Material(glm::vec3 ks, glm::vec3 ka, glm::vec3 kd, float ns, float ni, const std::string& textureMap)
		: ks(ks), ka(ka), kd(kd), ns(ns), ni(ni), textureMap(textureMap)
	{
	}

	inline const std::string& GetTextureFilepath() const { return textureMap; }
};