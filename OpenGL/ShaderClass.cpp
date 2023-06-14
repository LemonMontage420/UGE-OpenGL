#include"ShaderClass.h"

//Reads A Text File And Dumps The Contents Into A String
std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

//Constructor That Builds A Shader Program From Two Seperate Strings
Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	//Debug Variables
	int success;
	char infoLog[512];

	//Read The Vertex And Fragment Shader Files Into A String
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	//Convert The Strings Into Character Arrays For OpenGL
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	//Create The Vertex Shader, Assign The Shader Source Code To It And Compile It
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	//Basic Error Checking To See If The Vertex Shader Compiled Successfully
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Create The Fragment Shader, Assign The Shader Source Code To It And Compile It
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	//Basic Error Checking To See If The Fragment Shader Compiled Successfully
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Create The Shader Program And Link The Vertex And Fragment Shaders To It
	ID = glCreateProgram();
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);

	//Basic Error Checking To See If The Shader Program Linked Successfully
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	//Delete The Now Useless Shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

//Activates the Shader Program
void Shader::Activate()
{
	glUseProgram(ID);
}

//Deactivates the Shader Program
void Shader::Deactivate()
{
	glUseProgram(0);
}

void Shader::setBool(const std::string& name, bool value)
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setVec2Int(const std::string& name, glm::ivec2 value)
{
	glUniform2i(glGetUniformLocation(ID, name.c_str()), value.x, value.y);
}
void Shader::setVec3Int(const std::string& name, glm::ivec3 value)
{
	glUniform3i(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
}
void Shader::setVec4Int(const std::string& name, glm::ivec4 value)
{
	glUniform4i(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::setUInt(const std::string& name, unsigned int value)
{
	glUniform1ui(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setVec2UInt(const std::string& name, glm::uvec2 value)
{
	glUniform2i(glGetUniformLocation(ID, name.c_str()), value.x, value.y);
}
void Shader::setVec3UInt(const std::string& name, glm::uvec3 value)
{
	glUniform3i(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
}
void Shader::setVec4UInt(const std::string& name, glm::uvec4 value)
{
	glUniform4i(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::setFloat(const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setVec2(const std::string& name, glm::vec2 value) 
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), value.x, value.y);
}
void Shader::setVec3(const std::string& name, glm::vec3 value)
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z);
}
void Shader::setVec4(const std::string& name, glm::vec4 value)
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::setMatrix2x2(const std::string& name, glm::mat2x2 value) 
{
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::setMatrix2x3(const std::string& name, glm::mat2x3 value)
{
	glUniformMatrix2x3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::setMatrix2x4(const std::string& name, glm::mat2x4 value)
{
	glUniformMatrix2x4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMatrix3x2(const std::string& name, glm::mat3x2 value)
{
	glUniformMatrix3x2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::setMatrix3x3(const std::string& name, glm::mat3x3 value)
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::setMatrix3x4(const std::string& name, glm::mat3x4 value)
{
	glUniformMatrix3x4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMatrix4x2(const std::string& name, glm::mat4x2 value)
{
	glUniformMatrix4x2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::setMatrix4x3(const std::string& name, glm::mat4x3 value)
{
	glUniformMatrix4x3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::setMatrix4x4(const std::string& name, glm::mat4x4 value)
{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

//Deletes the Shader Program
void Shader::Delete()
{
	glDeleteProgram(ID);
}