#include "Light.h"

Light::Light(LightType lightType)
{
	if (lightType == LightType::Directional) 
	{
		Vertex vertices[] =
		{
			Vertex{glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
			Vertex{glm::vec3(-0.5f, 0.0f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
			Vertex{glm::vec3(-0.5f, 0.0f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
			Vertex{glm::vec3(0.5f, 0.0f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
			Vertex{glm::vec3(0.5f, 0.0f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)}
		};
        GLuint indices[] =
        {
			0, 1, 3,
			0, 3, 4,
			0, 4, 2,
			0, 2, 1,

			2, 1, 3,
			2, 3, 4
        };

        std::vector<Vertex> verts(vertices, vertices + (sizeof(vertices) / sizeof(Vertex)));
        std::vector<GLuint> inds(indices, indices + (sizeof(indices) / sizeof(GLuint)));

        lightsVertices = verts;
        lightIndices = inds;
	}
	if (lightType == LightType::Point)
	{
        Vertex vertices[] =
        {
//				  Positions						Normals						TexCoords				Colors
            Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
            Vertex{glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
            Vertex{glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
            Vertex{glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
            Vertex{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
            Vertex{glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
            Vertex{glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
            Vertex{glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)}
        };
        //The Indices In Order To Use Vertices More Than Once (Drawing Elements Instead Of Drawing Arrays)
        GLuint indices[] =
        {
            //Front
            1, 5, 7,
            1, 3, 7,

            //Right
            5, 4, 6,
            5, 7, 6,

            //Back
            0, 2, 6,
            0, 4, 6,

            //Left
            0, 1, 2,
            1, 3, 2,

            //Top
            2, 6, 7,
            2, 3, 7,

            //Bottom
            0, 4, 5,
            0, 1, 5
        };

        std::vector<Vertex> verts(vertices, vertices + (sizeof(vertices) / sizeof(Vertex)));
        std::vector<GLuint> inds(indices, indices + (sizeof(indices) / sizeof(GLuint)));

        lightsVertices = verts;
        lightIndices = inds;
	}
	if (lightType == LightType::Spot)
	{
        Vertex vertices[] =
        {
//				  Positions						Normals						TexCoords				Colors
            Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
            Vertex{glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
            Vertex{glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
            Vertex{glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
            Vertex{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
            Vertex{glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
            Vertex{glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
            Vertex{glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)}
        };
        //The Indices In Order To Use Vertices More Than Once (Drawing Elements Instead Of Drawing Arrays)
        GLuint indices[] =
        {
            //Front
            1, 5, 7,
            1, 3, 7,

            //Right
            5, 4, 6,
            5, 7, 6,

            //Back
            0, 2, 6,
            0, 4, 6,

            //Left
            0, 1, 2,
            1, 3, 2,

            //Top
            2, 6, 7,
            2, 3, 7,

            //Bottom
            0, 4, 5,
            0, 1, 5
        };

        std::vector<Vertex> verts(vertices, vertices + (sizeof(vertices) / sizeof(Vertex)));
        std::vector<GLuint> inds(indices, indices + (sizeof(indices) / sizeof(GLuint)));

        lightsVertices = verts;
        lightIndices = inds;
	}
}

void Light::DrawLight(Shader& lightShader, Camera& camera) 
{
    Mesh lightMesh(lightsVertices, lightIndices);

    lightShader.Activate();
    lightShader.setVec3("lightColor", color);
    lightShader.setFloat("lightPower", power);
    lightShader.Deactivate();

    lightMesh.Draw(lightShader, camera, transform);
}