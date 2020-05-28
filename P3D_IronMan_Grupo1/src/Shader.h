#pragma once
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <GL/glew.h>


struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

enum class ShaderType
{
	NONE = -1,
	VERTEX = 0,
	FRAGMENT = 1
};

class Shader
{
private:
	std::string m_FilePath;
	unsigned int m_RendererId;
	mutable std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& filePath);
	Shader(const std::string& vsFilePath, const std::string& fsFilePath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	// Set Uniforms
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniformVec3f(const std::string& name, const glm::vec3 vec);
	void SetUniformMat3f(const std::string& name, const glm::mat3 matrix);
	void SetUniformMat3fv(const std::string& name, const GLfloat* value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string name, const glm::mat4 matrix);

private:
	ShaderProgramSource ParseShader(const std::string& filePath);
	ShaderProgramSource ParseShader(const std::string& vsFilePath, const std::string& fsFilePath);
	
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	
	// Stores the uniform location into a unordered map, so next time when it's needed is faster.
	// Works like a uniform cache.
	GLint GetUniformLocation(const std::string& name) const;
};
