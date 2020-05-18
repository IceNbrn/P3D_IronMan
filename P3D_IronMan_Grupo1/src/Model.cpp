#define _CRT_SECURE_NO_WARNINGS

#include "Model.h"
#include <iostream>
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
	FILE* file = fopen(m_FilePath.c_str(), "r");

	if (file == NULL)
	{
		std::cout << "[ModelLoader]: " << "Failed to open the file!" << std::endl;
		return false;
	}

	while(true)
	{
		char lineHeader[128];
		int result = fscanf(file, "%s", lineHeader);
		if (result == EOF)
			break;
		if(strcmp(lineHeader, "mtllib") == 0)
		{
			char fileName[60];
			fscanf(file, "%s", fileName);

			std::string materialPath;

			vector<string> splitString = Utils::Split(m_FilePath, '/');
			for (int i = 0; i < splitString.size() - 1; i++)
			{
				materialPath += splitString[i];
				materialPath += "/";
			}
			materialPath += fileName;
			
			m_Material = LoadMaterial(materialPath);
		}
		else if(strcmp(lineHeader, "v") == 0)
		{
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			m_OutPositions.push_back(vertex);
		}
		else if(strcmp(lineHeader, "vt") == 0)
		{
			glm::vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			m_OutUvs.push_back(uv);
		}
		else if(strcmp(lineHeader, "vn") == 0)
		{
			glm::vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			m_OutNormals.push_back(normal);
		}
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
	
	FILE* file = fopen(filePath.c_str(), "r");

	if (file == NULL)
	{
		std::cout << "[MaterialLoader]: " << "Failed to open the file!" << std::endl;
	}
	
	glm::vec3 ka, ks, kd;
	float ns, ni;
	char textureName[50];
	std::string texturePath;
	
	while (true)
	{
		char lineHeader[128];
		int result = fscanf(file, "%s", lineHeader);
		if (result == EOF)
			break;

		if (strcmp(lineHeader, "Ka") == 0)
		{
			
			fscanf(file, "%f %f %f\n", &ka.x, &ka.y, &ka.z);
			//m_Material->ka = ka;
		}
		else if (strcmp(lineHeader, "Ks") == 0)
		{
			//glm::vec3 ks;
			fscanf(file, "%f %f %f\n", &ks.x, &ks.y, &ks.z);
			//m_Material->ks = ks;
		}
		else if (strcmp(lineHeader, "Kd") == 0)
		{
			//glm::vec3 kd;
			fscanf(file, "%f %f %f\n", &kd.x, &kd.y, &kd.z);
			//m_Material->kd = kd;
		}
		else if (strcmp(lineHeader, "Ns") == 0)
		{
			fscanf(file, "%f\n", &ns);
		}
		else if (strcmp(lineHeader, "Ni") == 0)
		{
			fscanf(file, "%f\n", &ni);
		}
		else if (strcmp(lineHeader, "map") == 0)
		{
			fscanf(file, "%s", textureName);
			

			vector<string> splitString = Utils::Split(m_FilePath, '/');
			for (int i = 0; i < splitString.size() - 1; i++)
			{
				texturePath += splitString[i];
				texturePath += "/";
			}
			texturePath += textureName;
		}
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

