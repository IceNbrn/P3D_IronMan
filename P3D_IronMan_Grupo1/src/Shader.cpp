#include "Shader.h"
#include "Renderer.h"

#include <sstream>
#include <fstream>
#include <iostream>

Shader::Shader(const std::string& filePath)
	: m_FilePath(filePath), m_RendererId(0)
{
	ShaderProgramSource source = ParseShader(filePath);
	m_RendererId = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::Shader(const std::string& vsFilePath, const std::string& fsFilePath)
{
	ShaderProgramSource source = ParseShader(vsFilePath, fsFilePath);
	m_RendererId = CreateShader(source.VertexSource, source.FragmentSource);
}

Shader::~Shader()
{
	// Deletes a program object
	GLCall(glDeleteProgram(m_RendererId));
}

void Shader::Bind() const
{
	// Installs a program object as part of current rendering state
	GLCall(glUseProgram(m_RendererId));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4 matrix)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]))
}

ShaderProgramSource Shader::ParseShader(const std::string& filePath)
{
	std::ifstream stream(filePath);

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while(getline(stream, line))
	{
		if(line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else
		{
			// Cast the enum to a int
			ss[(int)type] << line << std::endl;
		}
	}
	return { ss[0].str(), ss[1].str() };
}

ShaderProgramSource Shader::ParseShader(const std::string& vsFilePath, const std::string& fsFilePath)
{
	std::ifstream vsStream(vsFilePath);
	std::ifstream fsStream(fsFilePath);

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while(getline(vsStream, line))
	{
		ss[(int)ShaderType::VERTEX] << line << std::endl;
	}
	while (getline(fsStream, line))
	{
		ss[(int)ShaderType::FRAGMENT] << line << std::endl;
	}
	
	return { ss[0].str(), ss[1].str() };
}

unsigned Shader::CompileShader(unsigned type, const std::string& source)
{
	// Creates an empty shader object and returns non-zero value by which it can be referenced.
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	// Replaces the source code in a shader object
	glShaderSource(id, 1, &src, nullptr);
	// Compiles a shader object
	glCompileShader(id);

	int result;
	// Get the last compile operation was successful or not (return GL_TRUE = Successful or GL_FALSE = Error). 
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if(result == GL_FALSE)
	{
		int length;
		// Get the number of characters in the information log.
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		// Returns the information log for a shader object.
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "[Shader]: Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader." << std::endl;
		std::cout << "[Shader]: " << message << std::endl;
		return 0;
	}
	
	return id;
}

unsigned Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	// Creates an empty program object and returns non-zero value by which it can be referenced.
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	// Attaches a shader object to a program object.
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	// Links the program object specified by program.
	// If any shader object is attached they will be used to create an executable.
	glLinkProgram(program);
	// Validates a program object to see whether the executables contained in program can execute given the current OpenGL state.
	glValidateProgram(program);
	// Deletes a shader object, frees the memory
	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

GLint Shader::GetUniformLocation(const std::string& name) const
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];

	GLint location;
	// Returns an integer that represents the location of a specific uniform variable within a program object
	GLCall(location = glGetUniformLocation(m_RendererId, name.c_str()));
	if(location == -1)
		std::cout << "[Shader] : " << "(Warning) Uniform '" << name << "' doesn't exist!" << std::endl;

	m_UniformLocationCache[name] = location;

	return location;
}
