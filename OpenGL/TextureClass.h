#pragma once

#include<glad/glad.h>
#include<stb/stb_image.h>
#include<glm/glm.hpp>

class Texture 
{
	public:
		GLuint ID;
		GLuint texSlot;

		Texture(const char* filename, GLuint textureSlot, GLenum minFilter, GLenum magFilter, GLenum wrapVertical, GLenum wrapHorizontal, glm::vec4 borderColor, GLenum fileFormat, GLenum textureFormat, bool generateMipmaps);

		void Bind();
		void Unbind();
		void Delete();

};
