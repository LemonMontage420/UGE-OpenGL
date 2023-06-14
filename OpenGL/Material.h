#pragma once
#include"Mesh.h"

struct Uniform 
{
	const char* name;
};

class Material
{
	public:
		Shader materialShader;
		Material(Shader& shader);
		void BindTextures(std::vector<Texture>& textures);
		void BindMatrices();
		void Delete();
};
