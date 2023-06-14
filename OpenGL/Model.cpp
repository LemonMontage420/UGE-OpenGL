#include"Model.h"

Model::Model(std::string modelPath)
{
    LoadModel(modelPath);
}

void Model::LoadModel(std::string path) 
{
    // read file via ASSIMP
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }

    // retrieve the directory path of the filepath
    directory = path.substr(0, path.find_last_of('/'));

    // process ASSIMP's root node recursively
    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene) 
{
    // process each mesh located at the current node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        // the node object only contains indices to index the actual objects in the scene. 
        // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh, scene));
    }
    // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) 
{
    //Data To Fill
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    //Go Through The Mesh's Vertices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        //Position Data
        if (mesh->HasPositions()) 
        {
            glm::vec3 position;
            position.x = mesh->mVertices[i].x;
            position.y = mesh->mVertices[i].y;
            position.z = mesh->mVertices[i].z;
            vertex.position = position;
        }
        else 
        {
            vertex.position = glm::vec3(0.0f, 0.0f, 0.0f);
        }

        //Normal Data
        if (mesh->HasNormals())
        {
            glm::vec3 normals;
            normals.x = mesh->mNormals[i].x;
            normals.y = mesh->mNormals[i].y;
            normals.z = mesh->mNormals[i].z;
            vertex.normals = normals;
        }
        else
        {
            vertex.normals = glm::vec3(0.0f, 0.0f, 0.0f);
        }

        //Texture Coordinates Data
        glm::vec2 texCoords[AI_MAX_NUMBER_OF_TEXTURECOORDS];
        for (unsigned int j = 0; j < AI_MAX_NUMBER_OF_TEXTURECOORDS; j++)
        {
            if (mesh->HasTextureCoords(j)) 
            {
                texCoords[j].x = mesh->mTextureCoords[j][i].x;
                texCoords[j].y = mesh->mTextureCoords[j][i].y;
            }
            else
            {
                texCoords[j] = glm::vec2(0.0f, 0.0f);
            }
        }
        //Only Pass The First Texture Coordinate Channel For Now (There Can Be Up To 8 Texture Coordinate Channels)
        vertex.texCoords = texCoords[0];

        ////Tangent And Bitangent Data (Disabled For Now Until I Get Normal Maps In Place)
        //if (mesh->HasTangentsAndBitangents())
        //{
        //    glm::vec3 tanAndBi[2];

        //    //Tangents
        //    tanAndBi[0].x = mesh->mTangents[i].x;
        //    tanAndBi[0].y = mesh->mTangents[i].y;
        //    tanAndBi[0].z = mesh->mTangents[i].z;
        //    vertex.tangent = tanAndBi[0];

        //    //Bitangents
        //    tanAndBi[1].x = mesh->mBitangents[i].x;
        //    tanAndBi[1].y = mesh->mBitangents[i].y;
        //    tanAndBi[1].z = mesh->mBitangents[i].z;
        //    vertex.bitangent = tanAndBi[1];
        //}

        //TODO: Fix Not Being Able To Grab Vertex Colors From Mesh
        //Vertex Color Data
        glm::vec4 vertCol[AI_MAX_NUMBER_OF_COLOR_SETS];
        for (unsigned int j = 0; j < AI_MAX_NUMBER_OF_COLOR_SETS; j++)
        {
            if (mesh->HasVertexColors(j)) 
            {
                vertCol[j].x = mesh->mColors[j][i].r;
                vertCol[j].y = mesh->mColors[j][i].g;
                vertCol[j].z = mesh->mColors[j][i].b;
                vertCol[j].w = mesh->mColors[j][i].a;
            }
            else
            {
                vertCol[j] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
            }
        }
        //Only Pass The First Vertex Color Channel For Now (There Can Be Up To 8 Vertex Color Channels)
        vertex.color = vertCol[0];

        vertices.push_back(vertex);
    }

    //Index Data
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        //Retrieve All Indices Of The Face And Store Them In The Indices Vector
        for (unsigned int j = 0; j < face.mNumIndices; j++) 
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    //Return A Mesh Object Created From The Extracted Mesh Data
    return Mesh(vertices, indices);
}