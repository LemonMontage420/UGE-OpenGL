#pragma once

#include<glad/glad.h>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

std::string get_file_contents(const char* filename);

class Shader 
{
	public:
		GLuint ID;
		Shader(const char* vertexFile, const char* fragmentFile);
		void Activate();
		void Deactivate();
		void Delete();

		void setBool(const std::string& name, bool value);

		void setInt(const std::string& name, int value);
		void setVec2Int(const std::string& name, glm::ivec2);
		void setVec3Int(const std::string& name, glm::ivec3);
		void setVec4Int(const std::string& name, glm::ivec4);

		void setUInt(const std::string& name, unsigned int value);
		void setVec2UInt(const std::string& name, glm::uvec2);
		void setVec3UInt(const std::string& name, glm::uvec3);
		void setVec4UInt(const std::string& name, glm::uvec4);

		void setFloat(const std::string& name, float value);
		void setVec2(const std::string& name, glm::vec2 value);
		void setVec3(const std::string& name, glm::vec3 value);
		void setVec4(const std::string& name, glm::vec4 value);

		void setMatrix2x2(const std::string& name, glm::mat2x2 value);
		void setMatrix2x3(const std::string& name, glm::mat2x3 value);
		void setMatrix2x4(const std::string& name, glm::mat2x4 value);

		void setMatrix3x2(const std::string& name, glm::mat3x2 value);
		void setMatrix3x3(const std::string& name, glm::mat3x3 value);
		void setMatrix3x4(const std::string& name, glm::mat3x4 value);

		void setMatrix4x2(const std::string& name, glm::mat4x2 value);
		void setMatrix4x3(const std::string& name, glm::mat4x3 value);
		void setMatrix4x4(const std::string& name, glm::mat4x4 value);
};
