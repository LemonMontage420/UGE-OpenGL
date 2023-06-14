#include"Mesh.h"

Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices)
{
	Mesh::vertices = vertices;
	Mesh::indices = indices;
	//Mesh::textures = textures;

	//Construct And Bind The VAO Before Constructing The VBO And EBO (VERY IMPORTANT)
	VAO.Bind();

	//Construct And Configure The VBO And EBO
	VBO VBO(vertices, GL_STATIC_DRAW);
	EBO EBO(indices, GL_STATIC_DRAW);

	//Enable And Configure The Vertex Attributes We're Using In The Vertex Shader
	VAO.LinkAttribute(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0); //Vertex Position Attribute
	VAO.LinkAttribute(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float))); //Vertex Normal Attribute
	VAO.LinkAttribute(VBO, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float))); //Vertex Texture Coordinates Attributes
	VAO.LinkAttribute(VBO, 3, 4, GL_FLOAT, sizeof(Vertex), (void*)(8 * sizeof(float))); //Vertex Color Attribute

	//Unbind The VAO, VBO, And EBO (MAKE SURE THE VAO GETS UNBOUND FIRST) (Not Strictly Necessary Here As We Only Have One Set Of Vertex Data,
	//But This Step Will Need To Be Taken When Modifying Multiple Objects For Example, So It's Good Practice)
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();

	//Delete The Now Useless VBO And EBO, As We'll Be Constructing Them Again The Next Time This Function Is Called
	VBO.Delete();
	EBO.Delete();
}

Mesh::Mesh(Primitive& primitiveType, unsigned int detailLevel) 
{
	if (primitiveType == Primitive::Triangle) //Complete
	{
		Vertex vertices[] =
		{
			//				  Positions						Normals						TexCoords				Colors
			Vertex{glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)}, //Bottom Left
			Vertex{glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)}, //Bottom Right
			Vertex{glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.5f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)} //Top
		};
		GLuint indices[] =
		{
			0, 1, 2
		};

		std::vector<Vertex> triangleVertices(vertices, vertices + (sizeof(vertices) / sizeof(Vertex)));
		std::vector<GLuint> triangleIndices(indices, indices + (sizeof(indices) / sizeof(GLuint)));

		Mesh::vertices = triangleVertices;
		Mesh::indices = triangleIndices;
	}
	if (primitiveType == Primitive::Quad) //Complete
	{
		Vertex vertices[] =
		{
			//				  Positions						Normals						TexCoords				Colors
			Vertex{glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)}, //Bottom Left
			Vertex{glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)}, //Top Left
			Vertex{glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)}, //Bottom Right
			Vertex{glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)}, //Top Right
		};
		GLuint indices[] =
		{
			0, 3, 1,
			0, 2, 3
		};

		std::vector<Vertex> quadVertices(vertices, vertices + (sizeof(vertices) / sizeof(Vertex)));
		std::vector<GLuint> quadIndices(indices, indices + (sizeof(indices) / sizeof(GLuint)));

		Mesh::vertices = quadVertices;
		Mesh::indices = quadIndices;
	}
	if (primitiveType == Primitive::CircleMesh) //Complete
	{
		float radius = 1.0f;

		unsigned int numVertices = detailLevel + (uint64_t)1;
		std::vector<Vertex> circleVertices(numVertices);

		circleVertices[0].position = glm::vec3(0.0f, 0.0f, 0.0f);
		circleVertices[0].normals = glm::vec3(0.0f, 0.0f, 1.0f);
		circleVertices[0].texCoords = glm::vec2(0.5f, 0.5f);
		circleVertices[0].color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		for (unsigned int i = 1; i < numVertices; i++)
		{
			float sin = glm::sin(((float)i / (float)detailLevel) * (glm::pi<float>() * 2));
			float cos = glm::cos(((float)i / (float)detailLevel) * (glm::pi<float>() * 2));

			circleVertices[i].position = glm::vec3(cos * (radius / 2.0f), sin * (radius / 2.0f), 0.0f);
			circleVertices[i].normals = glm::vec3(0.0f, 0.0f, 1.0f);
			circleVertices[i].texCoords = glm::vec2((cos * 0.5f) + 0.5f, (-sin * -0.5f) + 0.5f);
			circleVertices[i].color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		}

		unsigned int numTriangles = detailLevel;
		std::vector<GLuint> circleIndices(numTriangles * (uint64_t)3);

		//Bottom Face Of The Cone That's Connected Through A Triangle Fan With A Vertex In The Center
		unsigned int v = 0;
		for (unsigned int i = 0; i < numTriangles; i++)
		{
			if (i != numTriangles - 1)
			{
				circleIndices[v++] = 0;
				circleIndices[v++] = i + 1;
				circleIndices[v++] = i + 2;
			}
			else
			{
				circleIndices[v++] = 0;
				circleIndices[v++] = i + 1;
				circleIndices[v++] = 1;
			}
		}

		Mesh::vertices = circleVertices;
		Mesh::indices = circleIndices;
	}
	if (primitiveType == Primitive::Pyramid) //Flat Shaded, Missing UVs
	{
		Vertex vertices[] =
		{
//				  Positions						Normals									TexCoords				Colors
			Vertex{glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0f, 0.447214f, 0.894427f), glm::vec2(0.5f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
			Vertex{glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0f, 0.447214f, -0.894427f), glm::vec2(0.5f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
			Vertex{glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.894427f, 0.447214f, 0.0f), glm::vec2(0.5f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
			Vertex{glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(-0.894427f, 0.447214f, 0.0f), glm::vec2(0.5f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},

			Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.447214f, -0.894427f), glm::vec2(1.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
			Vertex{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.447214f, -0.894427f), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},

			Vertex{glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.894427f, 0.447214f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
			Vertex{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.894427f, 0.447214f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},

			Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-0.894427f, 0.447214f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
			Vertex{glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-0.894427f, 0.447214f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},

			Vertex{glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.447214f, 0.894427f), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
			Vertex{glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.447214f, 0.894427f), glm::vec2(1.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},

			Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
			Vertex{glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
			Vertex{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
			Vertex{glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)}
			
		};
		GLuint indices[] =
		{
			0, 10, 11,
			1, 5, 4,
			2, 6, 7,
			3, 8, 9,
			12, 14, 13,
			13, 14, 15
		};

		std::vector<Vertex> pyramidVertices(vertices, vertices + (sizeof(vertices) / sizeof(Vertex)));
		std::vector<GLuint> pyramidIndices(indices, indices + (sizeof(indices) / sizeof(GLuint)));

		Mesh::vertices = pyramidVertices;
		Mesh::indices = pyramidIndices;
	}
	if (primitiveType == Primitive::Cube) //Flat Shaded, Missing UVs
	{
		Vertex vertices[] =
		{
			//				  Positions						Normals						TexCoords				Colors
			Vertex{glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
			Vertex{glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
			Vertex{glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},

			Vertex{glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
			Vertex{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
			Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},

			Vertex{glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
			Vertex{glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
			Vertex{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},

			Vertex{glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
			Vertex{glm::vec3(0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
			Vertex{glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},

			Vertex{glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
			Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
			Vertex{glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},

			Vertex{glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
			Vertex{glm::vec3(-0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
			Vertex{glm::vec3(0.5f, -0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(1.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},

			Vertex{glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec2(1.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
			Vertex{glm::vec3(0.5f, 0.5f, -0.5f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
			Vertex{glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
			Vertex{glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
			Vertex{glm::vec3(-0.5f, 0.5f, -0.5f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec2(0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)},
			Vertex{glm::vec3(-0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec2(0.0f, 1.0f), glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)}
		};
		GLuint indices[] =
		{
			0, 1, 2,
			3, 4, 5,
			6, 7, 8,
			9, 10, 11,
			12, 13, 14,
			15, 16, 17,
			0, 18, 1,
			3, 19, 4,
			6, 20, 7,
			9, 21, 10,
			12, 22, 13,
			15, 23, 16
		};

		std::vector<Vertex> cubeVertices(vertices, vertices + (sizeof(vertices) / sizeof(Vertex)));
		std::vector<GLuint> cubeIndices(indices, indices + (sizeof(indices) / sizeof(GLuint)));

		Mesh::vertices = cubeVertices;
		Mesh::indices = cubeIndices;
	}
	if (primitiveType == Primitive::UVSphere) //Smooth Shaded, Complete
	{
		// One vertex at every latitude-longitude intersection,
		// plus one for the north pole and one for the south.
		// One meridian serves as a UV seam, so we double the vertices there.
		unsigned int numLatitudeLines = (unsigned int)glm::floor(detailLevel / 2);
		unsigned int numLongitudeLines = detailLevel;
		float radius = 1.0f;
		unsigned int numVertices = numLatitudeLines * (numLongitudeLines + 1) + 2;


		std::vector<Vertex> uvSphereVertices(numVertices);
		// North pole.
		uvSphereVertices[0].position = glm::vec3(0.0f, radius, 0.0f);
		uvSphereVertices[0].normals = uvSphereVertices[0].position * (1.0f / radius);
		uvSphereVertices[0].texCoords = glm::vec2(0.5f, 1.0f);
		uvSphereVertices[0].color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		// South pole.
		uvSphereVertices[numVertices - 1].position = glm::vec3(0.0f, -radius, 0.0f);
		uvSphereVertices[numVertices - 1].normals = uvSphereVertices[numVertices - 1].position * (1.0f / radius);
		uvSphereVertices[numVertices - 1].texCoords = glm::vec2(0.5f, 0.0f);
		uvSphereVertices[numVertices - 1].color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		// +1.0f because there's a gap between the poles and the first parallel.
		float latitudeSpacing = 1.0f / (numLatitudeLines + 1.0f);
		float longitudeSpacing = 1.0f / (numLongitudeLines);

		// start writing new vertices at position 1
		unsigned int v = 1;
		for (unsigned int latitude = 0; latitude < numLatitudeLines; latitude++) 
		{
			for (unsigned int longitude = 0; longitude <= numLongitudeLines; longitude++) 
			{

				// Scale coordinates into the 0...1 texture coordinate range,
				// with north at the top (y = 1).
				uvSphereVertices[v].texCoords = glm::vec2(longitude * longitudeSpacing, 1.0f - (latitude + 1) * latitudeSpacing);

				// Convert to spherical coordinates:
				// theta is a longitude angle (around the equator) in radians.
				// phi is a latitude angle (north or south of the equator).
				float theta = uvSphereVertices[v].texCoords.x * 2.0f * glm::pi<float>() - glm::radians(90.0f);
				float phi = (uvSphereVertices[v].texCoords.y - 0.5f) * glm::pi<float>();

				// This determines the radius of the ring of this line of latitude.
				// It's widest at the equator, and narrows as phi increases/decreases.
				float c = glm::cos(phi);

				// Usual formula for a vector in spherical coordinates.
				// You can exchange x & z to wind the opposite way around the sphere.
				uvSphereVertices[v].position = glm::vec3(c * glm::cos(theta), glm::sin(phi), c * glm::sin(theta)) * radius;

				uvSphereVertices[v].normals = uvSphereVertices[v].position * (1.0f / radius);
				uvSphereVertices[v].color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

				// Proceed to the next vertex.
				v++;
			}
		}


		unsigned int numTriangles = numLatitudeLines * numLongitudeLines * 2;
		std::vector<GLuint> uvSphereIndices(numTriangles * (uint64_t)3);
		unsigned int index = 0;
		for (unsigned int i = 0; i < numLongitudeLines; i++) 
		{
			uvSphereIndices[index++] = 0;
			uvSphereIndices[index++] = i + 2;
			uvSphereIndices[index++] = i + 1;
		}

		// Each row has one more unique vertex than there are lines of longitude,
		// since we double a vertex at the texture seam.
		int rowLength = numLongitudeLines + 1;

		for (unsigned int latitude = 0; latitude < numLatitudeLines - 1; latitude++) {
			// Plus one for the pole.
			int rowStart = latitude * rowLength + 1;
			for (unsigned int longitude = 0; longitude < numLongitudeLines; longitude++)
			{
				int firstCorner = rowStart + longitude;

				// First triangle of quad: Top-Left, Bottom-Left, Bottom-Right
				uvSphereIndices[index++] = firstCorner;
				uvSphereIndices[index++] = firstCorner + rowLength + 1;
				uvSphereIndices[index++] = firstCorner + rowLength;

				// Second triangle of quad: Top-Left, Bottom-Right, Top-Right
				uvSphereIndices[index++] = firstCorner;
				uvSphereIndices[index++] = firstCorner + 1;
				uvSphereIndices[index++] = firstCorner + rowLength + 1;
			}
		}
		int pole = numVertices - 1;
		int bottomRow = (numLatitudeLines - 1) * rowLength + 1;

		for (unsigned int i = 0; i < numLongitudeLines; i++) {
			uvSphereIndices[index++] = pole;
			uvSphereIndices[index++] = bottomRow + i;
			uvSphereIndices[index++] = bottomRow + i + 1;
		}

		Mesh::vertices = uvSphereVertices;
		Mesh::indices = uvSphereIndices;
	}
	if (primitiveType == Primitive::IcoSphere) //Smooth Shaded, Incomplete
	{
		const float X = 0.525731112119133606f;
		const float Z = 0.850650808352039932f;
		const float N = 0.0f;

		glm::vec3 pos[12] =
		{
			glm::vec3(-X, N, Z),
			glm::vec3(X, N, Z),
			glm::vec3(-X, N, -Z),
			glm::vec3(X, N, -Z),
			glm::vec3(N, Z, X),
			glm::vec3(N, Z, -X),
			glm::vec3(N, -Z, X),
			glm::vec3(N, -Z, -X),
			glm::vec3(Z, X, N),
			glm::vec3(-Z, X, N),
			glm::vec3(Z, -X, N),
			glm::vec3(-Z, -X, N)
		};
		std::vector<Vertex> icoSphereVertices(12);
		for (unsigned int i = 0; i < icoSphereVertices.size(); i++)
		{
			icoSphereVertices[i].position = pos[i];
			icoSphereVertices[i].normals = glm::normalize(icoSphereVertices[i].position);
			icoSphereVertices[i].texCoords = glm::vec2(0.0f, 0.0f);
			icoSphereVertices[i].color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		}

		GLuint inds[] =
		{
			0,4,1,
			0,9,4,
			9,5,4,
			4,5,8,
			4,8,1,
			8,10,1,
			8,3,10,
			5,3,8,
			5,2,3,
			2,7,3,
			7,10,3,
			7,6,10,
			7,11,6,
			11,0,6,
			0,1,6,
			6,1,10,
			9,0,11,
			9,11,2,
			9,2,5,
			7,2,11
		};
		unsigned int numTriangles = 20;
		std::vector<GLuint> icoSphereIndices(numTriangles * (uint64_t)3);
		for (unsigned int i = 0; i < icoSphereIndices.size(); i++)
		{
			icoSphereIndices[i] = inds[i];
		}
		
		Mesh::vertices = icoSphereVertices;
		Mesh::indices = icoSphereIndices;
	}
	if (primitiveType == Primitive::CubeSphere) //Smooth/Flat Shaded Hybrid, Complete
	{
		float spherizeAmount = 1.0f;

		std::vector<Vertex> cubeSphereVertices;

		for (unsigned k = 0; k < 6; k++)
		{
			for (unsigned j = 0; j < detailLevel; j++)
			{
				for (unsigned i = 0; i < detailLevel; i++)
				{
					Vertex vert;
					glm::vec3 pointOnUnitCube;
					glm::vec3 normalVec;
					if (k == 0) 
					{
						pointOnUnitCube = glm::vec3((float)i / ((float)detailLevel - 1.0f), (float)j / ((float)detailLevel - 1.0f), 1.0f);
						normalVec = glm::vec3(0.0f, 0.0f, 1.0f);
					}
					if (k == 1)
					{
						pointOnUnitCube = glm::vec3((float)i / ((float)detailLevel - 1.0f), (float)j / ((float)detailLevel - 1.0f), 0.0f);
						normalVec = glm::vec3(0.0f, 0.0f, -1.0f);
					}
					if (k == 2)
					{
						pointOnUnitCube = glm::vec3((float)i / ((float)detailLevel - 1.0f), 1.0f, (float)j / ((float)detailLevel - 1.0f));
						normalVec = glm::vec3(0.0f, 1.0f, 0.0f);
					}
					if (k == 3)
					{
						pointOnUnitCube = glm::vec3((float)i / ((float)detailLevel - 1.0f), 0.0f, (float)j / ((float)detailLevel - 1.0f));
						normalVec = glm::vec3(0.0f, -1.0f, 0.0f);
					}
					if (k == 4)
					{
						pointOnUnitCube = glm::vec3(1.0f, (float)j / ((float)detailLevel - 1.0f), (float)i / ((float)detailLevel - 1.0f));
						normalVec = glm::vec3(1.0f, 0.0f, 0.0f);
					}
					if (k == 5)
					{
						pointOnUnitCube = glm::vec3(0.0f, (float)j / ((float)detailLevel - 1.0f), (float)i / ((float)detailLevel - 1.0f));
						normalVec = glm::vec3(-1.0f, 0.0f, 0.0f);
					}

					pointOnUnitCube -= glm::vec3(0.5f, 0.5f, 0.5f);
					glm::vec3 pointOnUnitSphere = glm::normalize(pointOnUnitCube);

					//Not Working :(
					//float x2 = pointOnUnitCube.x * pointOnUnitCube.x;
					//float y2 = pointOnUnitCube.y * pointOnUnitCube.y;
					//float z2 = pointOnUnitCube.z * pointOnUnitCube.z;
					//float x = pointOnUnitCube.x * glm::sqrt(1.0f - 0.5f * (y2 + z2) + y2 * z2 / 3.0f);
					//float y = pointOnUnitCube.y * glm::sqrt(1.0f - 0.5f * (z2 + x2) + z2 * x2 / 3.0f);
					//float z = pointOnUnitCube.z * glm::sqrt(1.0f - 0.5f * (x2 + y2) + x2 * y2 / 3.0f);
					//glm::vec3 betterPointOnUnitSphere = glm::vec3(x, y, z);

					vert.position = glm::mix(pointOnUnitCube, pointOnUnitSphere, spherizeAmount);
					vert.normals = glm::mix(normalVec, glm::normalize(vert.position), spherizeAmount);
					if (k == 1 or k == 2 or k == 4) 
					{
						vert.texCoords = glm::vec2(glm::abs(1.0f - (float)i / ((float)detailLevel - 1.0f)), (float)j / ((float)detailLevel - 1.0f));
					}
					else
					{
						vert.texCoords = glm::vec2((float)i / ((float)detailLevel - 1.0f), (float)j / ((float)detailLevel - 1.0f));
					}
					vert.color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

					cubeSphereVertices.push_back(vert);
				}
			}
		}

		unsigned int numTriangles = 12;
		for (unsigned int i = 0; i < detailLevel - 2; i++)
		{
			numTriangles += ((i + 2) * 12) + ((i + 1) * 12);
		}
		std::vector<GLuint> cubeSphereIndices(numTriangles * (uint64_t)3);
		unsigned int v = 0;
		for (unsigned k = 0; k < 6; k++)
		{
			for (unsigned j = 0; j < detailLevel - 1; j++)
			{
				for (unsigned int i = 0; i < detailLevel - 1; i++)
				{
					if (k == 1 or k == 2 or k == 4) 
					{
						cubeSphereIndices[v++] = i + (j * detailLevel) + (k * detailLevel * detailLevel);
						cubeSphereIndices[v++] = i + detailLevel + (j * detailLevel) + (k * detailLevel * detailLevel);
						cubeSphereIndices[v++] = i + 1 + (j * detailLevel) + (k * detailLevel * detailLevel);

						cubeSphereIndices[v++] = i + 1 + (j * detailLevel) + (k * detailLevel * detailLevel);
						cubeSphereIndices[v++] = i + detailLevel + (j * detailLevel) + (k * detailLevel * detailLevel);
						cubeSphereIndices[v++] = i + detailLevel + 1 + (j * detailLevel) + (k * detailLevel * detailLevel);
					}
					else
					{
						cubeSphereIndices[v++] = i + (j * detailLevel) + (k * detailLevel * detailLevel);
						cubeSphereIndices[v++] = i + 1 + (j * detailLevel) + (k * detailLevel * detailLevel);
						cubeSphereIndices[v++] = i + detailLevel + (j * detailLevel) + (k * detailLevel * detailLevel);

						cubeSphereIndices[v++] = i + 1 + (j * detailLevel) + (k * detailLevel * detailLevel);
						cubeSphereIndices[v++] = i + detailLevel + 1 + (j * detailLevel) + (k * detailLevel * detailLevel);
						cubeSphereIndices[v++] = i + detailLevel + (j * detailLevel) + (k * detailLevel * detailLevel);
					}
				}
			}
		}

		Mesh::vertices = cubeSphereVertices;
		Mesh::indices = cubeSphereIndices;
	}
	if (primitiveType == Primitive::Capsule) //Smooth Shaded, Complete
	{
		int longitudes = 24;
		int latitudes = 12;
		int rings = 5;
		float depth = 1.0f;
		float radius = 0.5f;
		std::string profile = "Aspect";

		bool calcMiddle = rings > 0;
		int halfLats = latitudes / 2;
		int halfLatsn1 = halfLats - 1;
		int halfLatsn2 = halfLats - 2;
		int ringsp1 = rings + 1;
		int lonsp1 = longitudes + 1;
		float halfDepth = depth * 0.5f;
		float summit = halfDepth + radius;

		// Vertex index offsets.
		int vertOffsetNorthHemi = longitudes;
		int vertOffsetNorthEquator = vertOffsetNorthHemi + lonsp1 * halfLatsn1;
		int vertOffsetCylinder = vertOffsetNorthEquator + lonsp1;
		int vertOffsetSouthEquator;
		if (calcMiddle) 
		{
			vertOffsetSouthEquator = vertOffsetCylinder + lonsp1 * rings;
		}
		else
		{
			vertOffsetCylinder;
		}
		int vertOffsetSouthHemi = vertOffsetSouthEquator + lonsp1;
		int vertOffsetSouthPolar = vertOffsetSouthHemi + lonsp1 * halfLatsn2;
		int vertOffsetSouthCap = vertOffsetSouthPolar + lonsp1;

		// Initialize arrays.
		int vertLen = vertOffsetSouthCap + longitudes;
		std::vector<glm::vec3> vs(vertLen);
		std::vector<glm::vec2> vts(vertLen);
		std::vector<glm::vec3> vns(vertLen);

		float toTheta = 2.0f * glm::pi<float>() / longitudes;
		float toPhi = glm::pi<float>() / latitudes;
		float toTexHorizontal = 1.0f / longitudes;
		float toTexVertical = 1.0f / halfLats;

		// Calculate positions for texture coordinates vertical.
		float vtAspectRatio = 1.0f;

		if (profile == "Aspect") 
		{
			vtAspectRatio = radius / (depth + radius + radius);
		}
		else if (profile == "Uniform") 
		{
			vtAspectRatio = (float)halfLats / (ringsp1 + latitudes);
		}
		else if (profile == "Fixed")
		{
			vtAspectRatio = 1.0f / 3.0f;
		}
		else
		{
			vtAspectRatio = 1.0f / 3.0f;
		}

		float vtAspectNorth = 1.0f - vtAspectRatio;
		float vtAspectSouth = vtAspectRatio;

		std::vector<glm::vec2> thetaCartesian(longitudes);
		std::vector<glm::vec2> rhoThetaCartesian(longitudes);
		std::vector<float> sTextureCache(lonsp1);

		// Polar vertices.
		for (int j = 0; j < longitudes; ++j)
		{
			float jf = (float)j;
			float sTexturePolar = 1.0f - ((jf + 0.5f) * toTexHorizontal);
			float theta = (jf * toTheta) + glm::radians(90.0f);

			float cosTheta = glm::cos(theta);
			float sinTheta = glm::sin(theta);

			thetaCartesian[j] = glm::vec2(cosTheta, sinTheta);
			rhoThetaCartesian[j] = glm::vec2(radius * cosTheta, radius * sinTheta);

			// North.
			vs[j] = glm::vec3(0.0f, summit, 0.0f);
			vts[j] = glm::vec2(glm::abs(1.0f - sTexturePolar), 1.0f);
			vns[j] = glm::vec3(0.0f, 1.0f, 0.0f);

			// South.
			int idx = vertOffsetSouthCap + j;
			vs[idx] = glm::vec3(0.0f, -summit, 0.0f);
			vts[idx] = glm::vec2(glm::abs(1.0f - sTexturePolar), 0.0f);
			vns[idx] = glm::vec3(0.0f, -1.0f, 0.0f);
		}

		// Equatorial vertices.
		for (int j = 0; j < lonsp1; ++j)
		{
			float sTexture = 1.0f - j * toTexHorizontal;
			sTextureCache[j] = sTexture;

			// Wrap to first element upon reaching last.
			int jMod = j % longitudes;
			glm::vec2 tc = thetaCartesian[jMod];
			glm::vec2 rtc = rhoThetaCartesian[jMod];

			// North equator.
			int idxn = vertOffsetNorthEquator + j;
			vs[idxn] = glm::vec3(rtc.x, halfDepth, -rtc.y);
			vts[idxn] = glm::vec2(glm::abs(1.0f - sTexture), vtAspectNorth);
			vns[idxn] = glm::vec3(tc.x, 0.0f, -tc.y);

			// South equator.
			int idxs = vertOffsetSouthEquator + j;
			vs[idxs] = glm::vec3(rtc.x, -halfDepth, -rtc.y);
			vts[idxs] = glm::vec2(glm::abs(1.0f - sTexture), vtAspectSouth);
			vns[idxs] = glm::vec3(tc.x, 0.0f, -tc.y);
		}

		// Hemisphere vertices.
		for (int i = 0; i < halfLatsn1; ++i)
		{
			float ip1f = i + 1.0f;
			float phi = ip1f * toPhi;

			// For coordinates.
			float cosPhiSouth = glm::cos(phi);
			float sinPhiSouth = glm::sin(phi);

			// Symmetrical hemispheres mean cosine and sine only needs
			// to be calculated once.
			float cosPhiNorth = sinPhiSouth;
			float sinPhiNorth = -cosPhiSouth;

			float rhoCosPhiNorth = radius * cosPhiNorth;
			float rhoSinPhiNorth = radius * sinPhiNorth;
			float zOffsetNorth = halfDepth - rhoSinPhiNorth;

			float rhoCosPhiSouth = radius * cosPhiSouth;
			float rhoSinPhiSouth = radius * sinPhiSouth;
			float zOffsetSouth = -halfDepth - rhoSinPhiSouth;

			// For texture coordinates.
			float tTexFac = ip1f * toTexVertical;
			float cmplTexFac = 1.0f - tTexFac;
			float tTexNorth = cmplTexFac + vtAspectNorth * tTexFac;
			float tTexSouth = cmplTexFac * vtAspectSouth;

			int iLonsp1 = i * lonsp1;
			int vertCurrLatNorth = vertOffsetNorthHemi + iLonsp1;
			int vertCurrLatSouth = vertOffsetSouthHemi + iLonsp1;

			for (int j = 0; j < lonsp1; ++j)
			{
				int jMod = j % longitudes;

				float sTexture = sTextureCache[j];
				glm::vec2 tc = thetaCartesian[jMod];

				// North hemisphere.
				int idxn = vertCurrLatNorth + j;
				vs[idxn] = glm::vec3(rhoCosPhiNorth * tc.x, zOffsetNorth, -rhoCosPhiNorth * tc.y);
				vts[idxn] = glm::vec2(glm::abs(1.0f - sTexture), tTexNorth);
				vns[idxn] = glm::vec3(cosPhiNorth * tc.x, -sinPhiNorth, -cosPhiNorth * tc.y);

				// South hemisphere.
				int idxs = vertCurrLatSouth + j;
				vs[idxs] = glm::vec3(rhoCosPhiSouth * tc.x, zOffsetSouth, -rhoCosPhiSouth * tc.y);
				vts[idxs] = glm::vec2(glm::abs(1.0f - sTexture), tTexSouth);
				vns[idxs] = glm::vec3(cosPhiSouth * tc.x, -sinPhiSouth, -cosPhiSouth * tc.y);
			}
		}

		// Cylinder vertices.
		if (calcMiddle)
		{
			// Exclude both origin and destination edges
			// (North and South equators) from the interpolation.
			float toFac = 1.0f / ringsp1;
			int idxCylLat = vertOffsetCylinder;

			for (int h = 1; h < ringsp1; ++h)
			{
				float fac = h * toFac;
				float cmplFac = 1.0f - fac;
				float tTexture = cmplFac * vtAspectNorth + fac * vtAspectSouth;
				float z = halfDepth - depth * fac;

				for (int j = 0; j < lonsp1; ++j)
				{
					int jMod = j % longitudes;
					glm::vec2 tc = thetaCartesian[jMod];
					glm::vec2 rtc = rhoThetaCartesian[jMod];
					float sTexture = sTextureCache[j];

					vs[idxCylLat] = glm::vec3(rtc.x, z, -rtc.y);
					vts[idxCylLat] = glm::vec2(glm::abs(1.0f - sTexture), tTexture);
					vns[idxCylLat] = glm::vec3(tc.x, 0.0f, -tc.y);

					++idxCylLat;
				}
			}
		}

		// Triangle indices.
		// Stride is 3 for polar triangles;
		// stride is 6 for two triangles forming a quad.
		int lons3 = longitudes * 3;
		int lons6 = longitudes * 6;
		int hemiLons = halfLatsn1 * lons6;

		int triOffsetNorthHemi = lons3;
		int triOffsetCylinder = triOffsetNorthHemi + hemiLons;
		int triOffsetSouthHemi = triOffsetCylinder + ringsp1 * lons6;
		int triOffsetSouthCap = triOffsetSouthHemi + hemiLons;

		int fsLen = triOffsetSouthCap + lons3;
		std::vector<int> tris(fsLen);

		// Polar caps.
		for (int i = 0, k = 0, m = triOffsetSouthCap; i < longitudes; ++i, k += 3, m += 3)
		{
			// North.
			tris[k] = i;
			tris[k + (uint64_t)1] = vertOffsetNorthHemi + i;
			tris[k + (uint64_t)2] = vertOffsetNorthHemi + i + 1;

			// South.
			tris[m] = vertOffsetSouthCap + i;
			tris[m + (uint64_t)1] = vertOffsetSouthPolar + i + 1;
			tris[m + (uint64_t)2] = vertOffsetSouthPolar + i;
		}

		// Hemispheres.
		for (int i = 0, k = triOffsetNorthHemi, m = triOffsetSouthHemi; i < halfLatsn1; ++i)
		{
			int iLonsp1 = i * lonsp1;

			int vertCurrLatNorth = vertOffsetNorthHemi + iLonsp1;
			int vertNextLatNorth = vertCurrLatNorth + lonsp1;

			int vertCurrLatSouth = vertOffsetSouthEquator + iLonsp1;
			int vertNextLatSouth = vertCurrLatSouth + lonsp1;

			for (int j = 0; j < longitudes; ++j, k += 6, m += 6)
			{
				// North.
				int north00 = vertCurrLatNorth + j;
				int north01 = vertNextLatNorth + j;
				int north11 = vertNextLatNorth + j + 1;
				int north10 = vertCurrLatNorth + j + 1;

				tris[k] = north00;
				tris[(uint64_t)k + (uint64_t)1] = north11;
				tris[(uint64_t)k + (uint64_t)2] = north10;

				tris[(uint64_t)k + (uint64_t)3] = north00;
				tris[(uint64_t)k + (uint64_t)4] = north01;
				tris[(uint64_t)k + (uint64_t)5] = north11;

				// South.
				int south00 = vertCurrLatSouth + j;
				int south01 = vertNextLatSouth + j;
				int south11 = vertNextLatSouth + j + 1;
				int south10 = vertCurrLatSouth + j + 1;

				tris[m] = south00;
				tris[(uint64_t)m + (uint64_t)1] = south11;
				tris[(uint64_t)m + (uint64_t)2] = south10;

				tris[(uint64_t)m + (uint64_t)3] = south00;
				tris[(uint64_t)m + (uint64_t)4] = south01;
				tris[(uint64_t)m + (uint64_t)5] = south11;
			}
		}

		// Cylinder.
		for (int i = 0, k = triOffsetCylinder; i < ringsp1; ++i)
		{
			int vertCurrLat = vertOffsetNorthEquator + i * lonsp1;
			int vertNextLat = vertCurrLat + lonsp1;

			for (int j = 0; j < longitudes; ++j, k += 6)
			{
				int cy00 = vertCurrLat + j;
				int cy01 = vertNextLat + j;
				int cy11 = vertNextLat + j + 1;
				int cy10 = vertCurrLat + j + 1;

				tris[k] = cy00;
				tris[k + (uint64_t)1] = cy11;
				tris[k + (uint64_t)2] = cy10;

				tris[k + (uint64_t)3] = cy00;
				tris[k + (uint64_t)4] = cy01;
				tris[k + (uint64_t)5] = cy11;
			}
		}


		std::vector<Vertex> capsuleVertices(vertLen);
		std::vector<GLuint> capsuleIndices(fsLen);

		for (int i = 0; i < vertLen; i++)
		{
			capsuleVertices[i].position = vs[i];
			capsuleVertices[i].normals = vns[i];
			capsuleVertices[i].texCoords = vts[i];
			capsuleVertices[i].color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		}
		for (int i = 0; i < fsLen; i++)
		{
			capsuleIndices[i] = (GLuint)tris[i];
		}

		Mesh::vertices = capsuleVertices;
		Mesh::indices = capsuleIndices;
	}
	if (primitiveType == Primitive::Cone) //Smooth Shaded With Flat Bottom, Make It So I Can Adjust The Cone's Angle Through The Light Class When I Instantiate This Mesh For The Spotlight To Reflect The Physical Light's Angle Coverage
	{
		float radius = 1.0f;
		//Initialize The Number Of Vertices And Vertex Array Accordingly
		unsigned int numVertices = (detailLevel * 2) + 2;
		std::vector<Vertex> coneVertices(numVertices);

		//Top Vertex
		coneVertices[0].position = glm::vec3(0.0f, 0.5f, 0.0f);
		coneVertices[0].normals = glm::vec3(0.0f, 1.0f, 0.0f);
		coneVertices[0].texCoords = glm::vec2(0.0f, 1.0f);
		coneVertices[0].color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		//Bottom Vertex
		coneVertices[numVertices - 1].position = glm::vec3(0.0f, -0.5f, 0.0f);
		coneVertices[numVertices - 1].normals = glm::vec3(0.0f, -1.0f, 0.0f);
		coneVertices[numVertices - 1].texCoords = glm::vec2(0.5f, 0.5f);
		coneVertices[numVertices - 1].color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		//Vertex Ring, 1st j Iteration Is For The Normals That Point Outwards, 2nd Iteration Is For The Normals That Point Downwards (Making The Bottom Side Flat)
		for (unsigned int j = 0; j < 2; j++)
		{
			for (unsigned i = 0; i < detailLevel; i++)
			{
				int index = i + (j * detailLevel);

				float sin = glm::sin(((float)i / (float)detailLevel) * (glm::pi<float>() * 2));
				float cos = glm::cos(((float)i / (float)detailLevel) * (glm::pi<float>() * 2));

				coneVertices[index + (uint64_t)1].position = glm::vec3(sin * (radius / 2.0f), -0.5f, cos * (radius / 2.0f));
				if (j == 0) 
				{
					coneVertices[index + (uint64_t)1].normals = glm::normalize(glm::vec3(sin, (radius / 2.0f), cos)); //Outward Normals
					coneVertices[index + (uint64_t)1].texCoords = glm::vec2(0.5f, 0.0f); //Current Texture Coordinates Are Set To Be A Single Strip Down The Middle Of The Texture From The Base Of The Cone To The Top, I Can Get Proper TexCoords Like With The Cylinder If I Have As Many Vertices At The Base Of The Cone On The Top
				}
				else
				{
					coneVertices[index + (uint64_t)1].normals = glm::vec3(0.0f, -1.0f, 0.0f); //Downward Normals For Flat Bottom
					coneVertices[index + (uint64_t)1].texCoords = glm::vec2((sin * 0.5f) + 0.5f, glm::abs(1.0f - ((cos * -0.5f) + 0.5f)));
				}
				coneVertices[index + (uint64_t)1].color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
			}
		}

		//Initialize The Number Of Triangles We Want And The Index Array Accordingly
		unsigned int numTriangles = detailLevel * 2;
		std::vector<GLuint> coneIndices(numTriangles * (uint64_t)3);

		int v = 0;
		//"Hat" Part Of The Cone
		for (unsigned int i = 0; i < numTriangles / 2; i++)
		{
			if (i != (numTriangles / 2) - 1) 
			{
				coneIndices[v++] = 0;
				coneIndices[v++] = i + 1;
				coneIndices[v++] = i + 2;
			}
			else
			{
				coneIndices[v++] = 0;
				coneIndices[v++] = i + 1;
				coneIndices[v++] = 1;
			}
		}
		//Bottom Face Of The Cone That's Connected Through A Triangle Fan With A Vertex In The Center
		for (unsigned int i = 0; i < numTriangles / 2; i++)
		{
			if (i != (numTriangles / 2) - 1)
			{
				coneIndices[v++] = numVertices - 1;
				coneIndices[v++] = i + detailLevel + 2;
				coneIndices[v++] = i + detailLevel + 1;
			}
			else
			{
				coneIndices[v++] = numVertices - 1;
				coneIndices[v++] = detailLevel + 1;
				coneIndices[v++] = i + detailLevel + 1;
			}
		}

		//Link The Mesh's Mesh Data With The Cone Data
		Mesh::vertices = coneVertices;
		Mesh::indices = coneIndices;
	}
	if (primitiveType == Primitive::Cylinder) //Smoothed With Flat Bottoms, Complete, Just Reverse The U Texcoord On The Cylinder Wall On The Back
	{
		float radius = 1.0f;
		unsigned int numVertices = (detailLevel * 4) + 2;
		std::vector<Vertex> cylinderVertices(numVertices);

		//Cylinder Wall
		for (unsigned int i = 0; i < detailLevel * 2; i++)
		{
			float sin = glm::sin(((float)i / (float)detailLevel) * (glm::pi<float>() * 2));
			float cos = glm::cos(((float)i / (float)detailLevel) * (glm::pi<float>() * 2));

			if (i < detailLevel)
			{
				//Top Circle
				cylinderVertices[i].position = glm::vec3(sin * (radius / 2.0f), 0.5f, cos * (radius / 2.0f));
				cylinderVertices[i].texCoords = glm::vec2((sin * 0.5f) + 0.5f, 1.0f);
			}
			else
			{
				//Bottom Circle
				cylinderVertices[i].position = glm::vec3(sin * (radius / 2.0f), -0.5f, cos * (radius / 2.0f));
				cylinderVertices[i].texCoords = glm::vec2((sin * 0.5f) + 0.5f, 0.0f);
			}
			cylinderVertices[i].normals = glm::normalize(glm::vec3(sin, 0.0f, cos));
			cylinderVertices[i].color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		}

		cylinderVertices[detailLevel * (uint64_t)2].position = glm::vec3(0.0f, 0.5f, 0.0f);
		cylinderVertices[detailLevel * (uint64_t)2].normals = glm::vec3(0.0f, 1.0f, 0.0f);
		cylinderVertices[detailLevel * (uint64_t)2].texCoords = glm::vec2(0.5f, 0.5f);
		cylinderVertices[detailLevel * (uint64_t)2].color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		//Top Face
		for (unsigned int i = (detailLevel * 2) + 1; i < (detailLevel * 3) + 1; i++)
		{
			float sin = glm::sin(((float)i / (float)detailLevel) * (glm::pi<float>() * 2));
			float cos = glm::cos(((float)i / (float)detailLevel) * (glm::pi<float>() * 2));

			cylinderVertices[i].position = glm::vec3(sin * (radius / 2.0f), 0.5f, cos * (radius / 2.0f));
			cylinderVertices[i].normals = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
			cylinderVertices[i].texCoords = glm::vec2(glm::abs(1.0f - ((sin * 0.5f) + 0.5f)), (cos * 0.5f) + 0.5f);
			cylinderVertices[i].color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		}

		cylinderVertices[detailLevel * (uint64_t)3 + (uint64_t)1].position = glm::vec3(0.0f, -0.5f, 0.0f);
		cylinderVertices[detailLevel * (uint64_t)3 + (uint64_t)1].normals = glm::vec3(0.0f, -1.0f, 0.0f);
		cylinderVertices[detailLevel * (uint64_t)3 + (uint64_t)1].texCoords = glm::vec2(0.5f, 0.5f);
		cylinderVertices[detailLevel * (uint64_t)3 + (uint64_t)1].color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		//Bottom Face
		for (unsigned int i = (detailLevel * 3) + 2; i < (detailLevel * 4) + 2; i++)
		{
			float sin = glm::sin(((float)i / (float)detailLevel) * (glm::pi<float>() * 2));
			float cos = glm::cos(((float)i / (float)detailLevel) * (glm::pi<float>() * 2));

			cylinderVertices[i].position = glm::vec3(sin * (radius / 2.0f), -0.5f, cos * (radius / 2.0f));
			cylinderVertices[i].normals = glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f));
			cylinderVertices[i].texCoords = glm::vec2((sin * 0.5f) + 0.5f, glm::abs(1.0f - ((cos * -0.5f) + 0.5f)));
			cylinderVertices[i].color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		}

		unsigned int numTriangles = detailLevel * 4;
		std::vector<GLuint> cylinderIndices(numTriangles * (uint64_t)3);

		unsigned int v = 0;
		//Cylinder Wall Triangles
		for (unsigned int i = 0; i < detailLevel; i++)
		{
			if (i != detailLevel - 1)
			{
				cylinderIndices[v++] = i;
				cylinderIndices[v++] = i + detailLevel;
				cylinderIndices[v++] = i + 1;
			}
			else
			{
				cylinderIndices[v++] = 0;
				cylinderIndices[v++] = i;
				cylinderIndices[v++] = i + detailLevel;
			}

			if (i != detailLevel - 1)
			{
				cylinderIndices[v++] = i + 1;
				cylinderIndices[v++] = i + detailLevel;
				cylinderIndices[v++] = i + detailLevel + 1;
			}
			else
			{
				cylinderIndices[v++] = 0;
				cylinderIndices[v++] = i + detailLevel;
				cylinderIndices[v++] = detailLevel;
			}
		}
		//Top Triangle Fan
		for (unsigned int i = detailLevel * 2; i < detailLevel * 3; i++)
		{
			if (i != (detailLevel * 3) - 1)
			{
				cylinderIndices[v++] = detailLevel * 2;
				cylinderIndices[v++] = i + 1;
				cylinderIndices[v++] = i + 2;
			}
			else
			{
				cylinderIndices[v++] = detailLevel * 2;
				cylinderIndices[v++] = i + 1;
				cylinderIndices[v++] = detailLevel * 2 + 1;
			}
		}
		//Bottom Triangle Fan
		for (unsigned int i = (detailLevel * 3) + 1; i < (detailLevel * 4) + 1; i++)
		{
			if (i != detailLevel * 4)
			{
				cylinderIndices[v++] = (detailLevel * 3) + 1;
				cylinderIndices[v++] = i + 2;
				cylinderIndices[v++] = i + 1;
			}
			else
			{
				cylinderIndices[v++] = (detailLevel * 3) + 1;
				cylinderIndices[v++] = (detailLevel * 3) + 2;
				cylinderIndices[v++] = i + 1;
			}
		}

		Mesh::vertices = cylinderVertices;
		Mesh::indices = cylinderIndices;
	}
	if (primitiveType == Primitive::Torus) //Smooth Shaded, Complete, Just Do One Final Verification
	{
		float radius = 0.5f;
		float tube = 0.5f;
		tube *= radius; //Make Sure The Tube Variable Is A Percentage Of The Total Torus Radius

		unsigned int radialSegments = (unsigned int)glm::floor(detailLevel / 2.0f);
		unsigned int tubularSegments = detailLevel;

		std::vector<Vertex> torusVertices;
		for (unsigned int j = 0; j <= radialSegments; j++)
		{
			for (unsigned int i = 0; i <= tubularSegments; i++)
			{
				Vertex vertex;

				float u = (float)i / (float)tubularSegments * (glm::pi<float>() * 2);
				float v = (float)j / (float)radialSegments * (glm::pi<float>() * 2);

				//Positions
				vertex.position.x = (radius + tube * glm::cos(v)) * glm::cos(u);
				vertex.position.y = (radius + tube * glm::cos(v)) * glm::sin(u);
				vertex.position.z = tube * glm::sin(v);

				//Normals
				glm::vec3 center;

				center.x = radius * glm::cos(u);
				center.y = radius * glm::sin(u);
				center.z = 0;
				vertex.normals = glm::normalize(vertex.position - center);

				//TexCoords
				vertex.texCoords.x = (vertex.position.x + 1.0f) / 2.0f;
				vertex.texCoords.y = (vertex.position.y + 1.0f) / 2.0f;

				//Vertex Colors
				vertex.color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

				torusVertices.push_back(vertex);
			}
		}

		std::vector<GLuint> torusIndices;
		for (unsigned int j = 1; j <= radialSegments; j++) {

			for (unsigned int i = 1; i <= tubularSegments; i++) {

				//Indices
				unsigned int a = (tubularSegments + 1) * j + i - 1;
				unsigned int b = (tubularSegments + 1) * (j - 1) + i - 1;
				unsigned int c = (tubularSegments + 1) * (j - 1) + i;
				unsigned int d = (tubularSegments + 1) * j + i;

				//Triangles
				torusIndices.push_back(a);
				torusIndices.push_back(b);
				torusIndices.push_back(d);

				torusIndices.push_back(b);
				torusIndices.push_back(c);
				torusIndices.push_back(d);
			}
		}

		Mesh::vertices = torusVertices;
		Mesh::indices = torusIndices;
	}

	//Construct And Bind The VAO Before Constructing The VBO And EBO (VERY IMPORTANT)
	VAO.Bind();

	//Construct And Configure The VBO And EBO
	VBO VBO(vertices, GL_STATIC_DRAW);
	EBO EBO(indices, GL_STATIC_DRAW);

	//Enable And Configure The Vertex Attributes We're Using In The Vertex Shader
	VAO.LinkAttribute(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0); //Vertex Position Attribute
	VAO.LinkAttribute(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float))); //Vertex Normal Attribute
	VAO.LinkAttribute(VBO, 2, 2, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float))); //Vertex Texture Coordinates Attributes
	VAO.LinkAttribute(VBO, 3, 4, GL_FLOAT, sizeof(Vertex), (void*)(8 * sizeof(float))); //Vertex Color Attribute

	//Unbind The VAO, VBO, And EBO (MAKE SURE THE VAO GETS UNBOUND FIRST) (Not Strictly Necessary Here As We Only Have One Set Of Vertex Data,
	//But This Step Will Need To Be Taken When Modifying Multiple Objects For Example, So It's Good Practice)
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();

	//Delete The Now Useless VBO And EBO, As We'll Be Constructing Them Again The Next Time This Function Is Called
	VBO.Delete();
	EBO.Delete();

	renderViaIndices = true;
}

void Mesh::Draw(Shader& shader, Camera& camera, Transform& transform)
{
	//Bind The VAO We Wanna Use And Activate The Shader Program We Wanna Use
	shader.Activate();
	VAO.Bind();

	//Set The Camera Matrix (projection * view) That's Already Been Updated By The Camera Class
	camera.SetMatrix(shader, "camMatrix");

	//Initialize The Model Matrix (A Mesh Should Be Responsible For Updating Its Own Model Matrix, Might Move The Model Matrix Into A New 
	//"Game Object" Class, In Which The Mesh Class Is Merely A Part Of It)
	glm::mat4 model = glm::mat4(1.0f);

	//Update The Model Matrix In The TRS Order (Translation, Rotation, Scale)
	model = glm::translate(model, transform.position); //Translation Is Global
	model = glm::rotate(model, glm::radians(transform.rotation.x), glm::vec3(1.0f, 0.0f, 0.0f)); 
	model = glm::rotate(model, glm::radians(transform.rotation.y), glm::vec3(0.0f, 1.0f, 0.0f)); //Rotation Is A Bit Complicated
	model = glm::rotate(model, glm::radians(transform.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, transform.scale); //Scale Is Local

	//Create A More Elegant Solution To Setting Matrix Uniforms (Should Be Able To Set Uniforms By 
	//Calling Them Once The Shader Has Been Initialized, Aswell As Support Universal And Non-Universal Shader Uniforms)

	//Set The Now Updated Model Class
	shader.setMatrix4x4("modelMatrix", model);

	//Draw Our Triangles Onto The Back Buffer
	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, 0); //Casted indices.size() to a GLsizei to prevent warning about possible loss of data from converting from "size_t" to "GLsizei"
	
	//Unbind The VAO And Deactivate The Shader Program We Just Used To Make Way For The Next Potential Draw Call
	VAO.Unbind();
	shader.Deactivate();
}

void Mesh::Delete() 
{
	//Delete The Remaining Mesh Data (VBO And EBO Get Deleted After We're Done Constructing The Mesh And The Shader Deletes Itself As It's Not
	//Inherently Part Of Any *One* Mesh)
	VAO.Delete();
}