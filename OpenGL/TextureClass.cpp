#include"TextureClass.h"
#include"Camera.h"

Texture::Texture(const char* filename, GLuint textureSlot, GLenum minFilter, GLenum magFilter, GLenum wrapVertical, GLenum wrapHorizontal, glm::vec4 borderColor, GLenum fileFormat, GLenum textureFormat, bool generateMipmaps)
{
	//Image Variables For The Texture
	int imageWidth;
	int imageHeight;
	int imageNumChannels;

	//Make Sure stb_image Flips The Texture So The Texture Isn't Displayed Upside Down
	//(OpenGL Reads Textures From The Bottom Left Corner To The Top Right, While stb_image Reads From Top Left To Bottom Right)
	stbi_set_flip_vertically_on_load(true);

	//Load The Image Data And Store It In A Variable
	unsigned char* imageData = stbi_load(filename, &imageWidth, &imageHeight, &imageNumChannels, 0);

	//Create And Generate The Texture
	GLuint texture;
	glGenTextures(1, &texture);

	//Set The Active Texture Slot To The Slot We Wanna Store Our Texture In And Bind The Texture
	glBindTexture(GL_TEXTURE_2D, texture);

	//Setup Texture Parameters (Min And Mag Filter)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter); //When There's More Texels Than Pixels
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter); //When There's Less Texels Than Pixels
	//Setup Texture Parameters (Texture Wrap Mode) (This Is Per-Axis)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapHorizontal);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapVertical);
	//The Color To Use When Texture Wrap Mode Is Set To GL_CLAMP_TO_BORDER
	float flatColor[] = { borderColor.x, borderColor.y, borderColor.z, borderColor.w };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

	//Give The Texture Its Image Data And Delete The Now Useless Image Data
	glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, imageWidth, imageHeight, 0, fileFormat, GL_UNSIGNED_BYTE, imageData);
	stbi_image_free(imageData);

	if (generateMipmaps)
	{
		//Generate The Mipmaps For The Texture
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	ID = texture;
	texSlot = textureSlot;
}

void Texture::Bind()
{
	//Activate The Texture Slot We Wanna Use And Bind The Texture To The Corresponding OpenGL Texture Buffer Based Off The Texture's Type
	glActiveTexture(GL_TEXTURE0 + texSlot);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind() 
{
	glActiveTexture(GL_TEXTURE0 + texSlot);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete() 
{
	glDeleteTextures(1, &ID);
}