#define _CRT_SECURE_NO_WARNINGS

#include "ModelLoader.h"
#include <iostream>

Model::Model(const std::string& filePath)
	: m_FilePath(filePath)
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

		if(strcmp(lineHeader, "v") == 0)
		{
			glm::vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			m_OutVertices.push_back(vertex);
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
}

std::vector<glm::vec3>& Model::GetVertices()
{
	return m_OutVertices;
}

std::vector<glm::vec2>& Model::GetUvs()
{
	return m_OutUvs;
}

std::vector<glm::vec3>& Model::GetNormals()
{
	return m_OutNormals;
}
