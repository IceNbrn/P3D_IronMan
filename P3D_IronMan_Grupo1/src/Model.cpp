#define _CRT_SECURE_NO_WARNINGS

#include "Model.h"
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

std::vector<Vertex>& Model::GetVertices()
{
	return m_Vertices;
}

