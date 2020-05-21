#define _CRT_SECURE_NO_WARNINGS

#include "Model.h"
#include <iostream>
#include <fstream>
#include "Utils.h"


Model::Model(const std::string& filePath)
	: m_Material(nullptr), m_FilePath(filePath)
{
}

Model::~Model()
{

}

bool Model::LoadModel()
{
	std::ifstream stream;
	std::string line;

	stream.open(m_FilePath, std::ifstream::in);

	if (stream.is_open())
	{
		stream.seekg(7);
		stream >> line;
		std::string materialPath;

		vector<string> splitString = Utils::Split(m_FilePath, '/');
		for (int i = 0; i < splitString.size() - 1; i++)
		{
			materialPath += splitString[i];
			materialPath += "/";
		}
		materialPath += line;

		m_Material = LoadMaterial(materialPath);
		while (getline(stream, line))
		{
			stream >> line;
			if (line == "v")
			{
				glm::vec3 vertex;
				stream >> vertex.x >> vertex.y >> vertex.z;
				m_OutPositions.push_back(vertex);
			}
			else if (line == "vt")
			{
				glm::vec2 uv;
				stream >> uv.x >> uv.y;
				m_OutUvs.push_back(uv);
			}
			else if (line == "vn")
			{
				glm::vec3 normal;
				stream >> normal.x >> normal.y >> normal.z;
				m_OutNormals.push_back(normal);
			}
		}
	}
	else 
	{
		std::cout << "[ModelLoader]: " << "Failed to open the file " << m_FilePath << std::endl;
	}

	m_Vertices.resize(m_OutPositions.size());
	
	for (int i = 0; i < m_Vertices.size(); ++i)
	{
		m_Vertices[i].position	= m_OutPositions[i];
		m_Vertices[i].texcoord	= m_OutUvs[i];
		m_Vertices[i].normal	= m_OutNormals[i];
	}
	
	return true;
}

Material* Model::LoadMaterial(const std::string& filePath)
{
	glm::vec3 ka, ks, kd;
	float ns, ni;
	char textureName[50];
	std::string texturePath;
	
	std::ifstream stream;
	std::string line;

	stream.open(filePath, std::ifstream::in);

	if (stream.is_open())
	{
		stream >> line;
		while (getline(stream, line))
		{
			stream >> line;
			if (line == "Ka")
			{
				stream >> ka.x >> ka.y >> ka.z;
			}
			else if (line == "Ks")
			{
				stream >> ks.x >> ks.y >> ks.z;
			}
			else if (line == "Kd")
			{
				stream >> kd.x >> kd.y >> kd.z;
			}
			else if (line == "Ns")
			{
				stream >> ns;
			}
			else if (line == "Ni")
			{
				stream >> ni;
			}
			else if (line == "map")
			{
				stream >> textureName;
				vector<string> splitString = Utils::Split(m_FilePath, '/');
				for (int i = 0; i < splitString.size() - 1; i++)
				{
					texturePath += splitString[i];
					texturePath += "/";
				}
				texturePath += textureName;
			}
		}
	}
	else
	{
		std::cout << "[ModelLoader-Texture]: " << "Failed to open the file " << filePath << std::endl;
	}
	Material* material = new Material(ks, ka, kd, ns, ni, texturePath);

	return material;
}

std::vector<Vertex>& Model::GetVertices()
{
	return m_Vertices;
}

Material* Model::GetMaterial() const
{
	return m_Material;
}

