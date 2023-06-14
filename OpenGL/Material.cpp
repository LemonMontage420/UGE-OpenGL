#include"Material.h"

Material::Material(Shader& shader) : materialShader(shader)
{
	materialShader = shader;
}

void Material::BindTextures(std::vector<Texture>& textures)
{
	//Connect Each Texture In The Shader With Its Corresponding Texture Unit Slot (GL_TEXTURE0, GL_TEXTURE1, ...) (This Could Be Done Once On Startup)
	for (int i = 0; i < textures.size(); i++)
	{
		textures[i].Bind();
	}
	materialShader.Activate();

	for (int i = 0; i < textures.size(); i++)
	{
		std::string currentTex = std::to_string(i);
		materialShader.setInt("texture" + currentTex, i);
	}

	//for (int i = 0; i < textures.size(); i++)
	//{
	//	textures[i].Unbind();
	//}
	materialShader.Deactivate();
}

void Material::BindMatrices() 
{

}

void Material::Delete() 
{

}