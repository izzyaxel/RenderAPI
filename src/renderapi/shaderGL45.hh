#pragma once

#include <vector>
#include <string>
#include <cstdio>
#include <unordered_map>

struct Shader
{
	Shader() = delete;
	
	inline Shader(std::vector<uint8_t> const &vertShader, std::vector<uint8_t> const &fragShader) : Shader(std::string{vertShader.begin(), vertShader.end()}, std::string{fragShader.begin(), fragShader.end()}) {}
	
	inline explicit Shader(std::vector<uint8_t> const &compShader) : Shader(std::string{compShader.begin(), compShader.end()}) {}
	
	inline Shader(std::string const &vertShader, std::string const &fragShader)
	{
		uint32_t vertHandle = glCreateShader(GL_VERTEX_SHADER), fragHandle = glCreateShader(GL_FRAGMENT_SHADER);
		this->handle = glCreateProgram();
		char const *vertSource = vertShader.data(), *fragSource = fragShader.data();
		glShaderSource(vertHandle, 1, &vertSource, nullptr);
		glShaderSource(fragHandle, 1, &fragSource, nullptr);
		glCompileShader(vertHandle);
		int32_t success = 0;
		glGetShaderiv(vertHandle, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			int32_t maxLen = 0;
			glGetShaderiv(vertHandle, GL_INFO_LOG_LENGTH, &maxLen);
			std::vector<char> error;
			error.resize(maxLen * sizeof(GLchar));
			glGetShaderInfoLog(vertHandle, maxLen, &maxLen, error.data());
			std::string errorStr{error.begin(), error.end()};
			printf("Shader failed to compile: %s\n", errorStr.data());
			return;
		}
		glCompileShader(fragHandle);
		success = 0;
		glGetShaderiv(fragHandle, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			int32_t maxLen = 0;
			glGetShaderiv(fragHandle, GL_INFO_LOG_LENGTH, &maxLen);
			std::vector<char> error;
			error.resize(maxLen * sizeof(GLchar));
			glGetShaderInfoLog(fragHandle, maxLen, &maxLen, error.data());
			std::string errorStr{error.begin(), error.end()};
			printf("Shader failed to compile: %s\n", errorStr.data());
			return;
		}
		glAttachShader(this->handle, vertHandle);
		glAttachShader(this->handle, fragHandle);
		glLinkProgram(this->handle);
		success = 0;
		glGetProgramiv(this->handle, GL_LINK_STATUS, &success);
		if(!success)
		{
			int32_t maxLen = 0;
			glGetShaderiv(this->handle, GL_INFO_LOG_LENGTH, &maxLen);
			std::vector<char> error;
			error.resize(maxLen * sizeof(GLchar));
			glGetShaderInfoLog(this->handle, maxLen, &maxLen, error.data());
			std::string errorStr{error.begin(), error.end()};
			printf("Shader program %u failed to link\n", this->handle);
			return;
		}
		glDetachShader(this->handle, vertHandle);
		glDetachShader(this->handle, fragHandle);
		glDeleteShader(vertHandle);
		glDeleteShader(fragHandle);
	}
	
	inline explicit Shader(std::string const &compShader)
	{
		uint32_t compHandle = glCreateShader(GL_COMPUTE_SHADER);
		this->handle = glCreateProgram();
		char const *compSource = compShader.data();
		glShaderSource(compHandle, 1, &compSource, nullptr);
		glCompileShader(compHandle);
		int32_t success = 0;
		glGetShaderiv(compHandle, GL_COMPILE_STATUS, &success);
		if(!success)
		{
			int32_t maxLen = 0;
			glGetShaderiv(compHandle, GL_INFO_LOG_LENGTH, &maxLen);
			std::vector<char> error;
			error.resize(maxLen * sizeof(GLchar));
			glGetShaderInfoLog(compHandle, maxLen, &maxLen, error.data());
			std::string errorStr{error.begin(), error.end()};
			printf("Shader failed to compile: %s\n", errorStr.data());
			return;
		}
		
		glAttachShader(this->handle, compHandle);
		glLinkProgram(this->handle);
		success = 0;
		glGetProgramiv(this->handle, GL_LINK_STATUS, &success);
		if(!success)
		{
			int32_t maxLen = 0;
			glGetShaderiv(this->handle, GL_INFO_LOG_LENGTH, &maxLen);
			std::vector<char> error;
			error.resize(maxLen * sizeof(GLchar));
			glGetShaderInfoLog(this->handle, maxLen, &maxLen, error.data());
			std::string errorStr{error.begin(), error.end()};
			printf("Shader program %u failed to link\n", this->handle);
			return;
		}
		glDetachShader(this->handle, compHandle);
		glDeleteShader(compHandle);
	}
	
	inline ~Shader()
	{
		glDeleteProgram(this->handle);
	}
	
	//copy
	inline Shader(Shader &other)
	{
		this->handle = other.handle;
		other.handle = 0;
	}
	
	inline Shader& operator=(Shader other)
	{
		this->handle = other.handle;
		other.handle = 0;
		return *this;
	}
	
	//move
	inline Shader(Shader &&other) noexcept
	{
		this->handle = other.handle;
		other.handle = 0;
	}
	
	inline Shader& operator=(Shader &&other) noexcept
	{
		this->handle = other.handle;
		other.handle = 0;
		return *this;
	}
	
	inline void use()
	{
		glUseProgram(this->handle);
	}
	
	inline int32_t getUniformHandle(std::string const &location)
	{
		auto it = this->uniforms.find(location);
		if(it == this->uniforms.end()) it = this->uniforms.emplace(location, glGetUniformLocation(this->handle, location.data())).first;
		return it->second;
	}
	
	inline void sendFloat(std::string const &location, float val)
	{
		glUniform1f(this->getUniformHandle(location), val);
	}
	
	inline void sendInt(std::string const &location, int32_t val)
	{
		glUniform1i(this->getUniformHandle(location), val);
	}
	
	inline void sendUInt(std::string const &location, uint32_t val)
	{
		glUniform1ui(this->getUniformHandle(location), val);
	}
	
	inline void sendVec2f(std::string const &location, float* val)
	{
		glUniform2fv(this->getUniformHandle(location), 1, val);
	}
	
	inline void sendVec3f(std::string const &location, float* val)
	{
		glUniform3fv(this->getUniformHandle(location), 1, val);
	}
	
	inline void sendVec4f(std::string const &location, float* val)
	{
		glUniform4fv(this->getUniformHandle(location), 1, val);
	}
	
	inline void sendMat3f(std::string const &location, float* val)
	{
		glUniformMatrix3fv(this->getUniformHandle(location), 1, GL_FALSE, val);
	}
	
	inline void sendMat4f(std::string const &location, float* val)
	{
		glUniformMatrix4fv(this->getUniformHandle(location), 1, GL_FALSE, val);
	}
	
	uint32_t handle = 0;
	std::unordered_map<std::string, int32_t> uniforms;
};
