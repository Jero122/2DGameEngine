#pragma once
#include <glm/glm.hpp>

#include <assimp/mesh.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/cimport.h>

#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "Material.h"
#include "Mesh.h"
#include "Shader.h"
#include "Core/Log.h"
#include "Systems/MaterialSystem.h"
#include "Systems/TextureSystem.h"

class Model
{
public:
	Model() = default;
	Model(std::string const& path)
	{
		loadModel(path);
	}

	void Draw(Shader& shader)
	{
		for (auto& mesh : meshes)
		{
			mesh.Draw(shader);
		}
	}

private:
	std::vector<Mesh> meshes;
	std::string directory;

	void loadModel(std::string path)
	{
		const unsigned int flags = aiProcess_CalcTangentSpace | // calculate tangents and bitangents if possible
			aiProcess_JoinIdenticalVertices | // join identical vertices/ optimize indexing
			//aiProcess_ValidateDataStructure  | // perform a full validation of the loader's output
			aiProcess_Triangulate | // Ensure all verticies are triangulated (each 3 vertices are triangle)
			aiProcess_SortByPType | // ?
			aiProcess_ImproveCacheLocality | // improve the cache locality of the output vertices
			aiProcess_RemoveRedundantMaterials | // remove redundant materials
			aiProcess_FindDegenerates | // remove degenerated polygons from the import
			aiProcess_FindInvalidData | // detect invalid model data, such as invalid normal vectors
			aiProcess_GenUVCoords | // convert spherical, cylindrical, box and planar mapping to proper UVs
			aiProcess_TransformUVCoords | // preprocess UV transformations (scaling, translation ...)
			aiProcess_FindInstances | // search for instanced meshes and remove them by references to one master
			aiProcess_LimitBoneWeights | // limit bone weights to 4 per vertex
			aiProcess_OptimizeMeshes | // join small meshes, if possible;
			aiProcess_SplitByBoneCount | // split meshes with too many bones. Necessary for our (limited) hardware skinning shader
			0;

		const aiScene* scene = aiImportFile(path.c_str(), flags);

		if (!scene || !scene->HasMeshes()) {

			printf("Unable to load '%s'\n", path.c_str());

			return;
		}

		directory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);

		AL_ENGINE_INFO("Model loaded at path:{0}", path);
	}
	void processNode(aiNode* node, const aiScene* scene)
	{
		// process each mesh located at the current node
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			// the node object only contains indices to index the actual objects in the scene. 
			// the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
			AL_ENGINE_TRACE("Processed Mesh:{0}", mesh->mName.C_Str());
		}
		// after we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}
	Mesh processMesh(aiMesh* mesh, const aiScene* scene)
	{
		// data to fill
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<std::shared_ptr<Texture>> textures;

		// walk through each of the mesh's vertices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			Vertex vertex;
			glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
			// positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;
			// normals
			if (mesh->HasNormals())
			{
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.Normal = vector;
			}
			// texture coordinates
			if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				// a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);

			vertices.push_back(vertex);
		}
		// now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		// process materials
		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

		MaterialConfig config;
		aiString matName;
		mat->Get(AI_MATKEY_NAME, matName);
		config.name = std::string(matName.C_Str());

		aiColor4D color(0, 0, 0, 0);

		//Ambient
		if (mat->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS)
		{
			config.emissiveColour = { color.r, color.g, color.b, color.a };
			if (config.emissiveColour.a > 1.0f)
			{
				config.emissiveColour.a = 1.0f;
			}
		}

		//Diffuse/albedo
		if (mat->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
		{
			config.albedoColour = { color.r, color.g, color.b, color.a };
			if (config.albedoColour.a > 1.0f)
			{
				config.albedoColour.a = 1.0f;
			}
		}

		//Emissive
		if (mat->Get(AI_MATKEY_COLOR_EMISSIVE, color) == AI_SUCCESS)
		{
			config.emissiveColour += glm::vec4{ color.r, color.g, color.b, color.a };
		}
		//Opaqueness/Transparency
		const float opaquenessThreshold = 0.05f;
		float Opacity = 1.0f;
		if (mat->Get(AI_MATKEY_OPACITY, Opacity) == AI_SUCCESS) {

			config.transparencyFactor = glm::clamp(1.0f - Opacity, 0.0f, 1.0f);

			if (config.transparencyFactor >= 1.0f - opaquenessThreshold)
			{
				config.transparencyFactor = 0.0f;
			}
		}

		if (mat->Get(AI_MATKEY_COLOR_TRANSPARENT, color) == AI_SUCCESS) {

			const float Opacity = std::max(std::max(color.r, color.g), color.b);

			config.transparencyFactor = glm::clamp(Opacity, 0.0f, 1.0f);

			if (config.transparencyFactor >= 1.0f - opaquenessThreshold)
			{
				config.transparencyFactor = 0.0f;
			}
			config.alphaTest= 0.5f;
		}
		//Metallic Factor
		float tmp = 1.0f;
		if (mat->Get(AI_MATKEY_METALLIC_FACTOR	, tmp) == AI_SUCCESS)
		{
			config.metallic = tmp;
		}
		//Roughness Factor
		if (mat->Get(AI_MATKEY_ROUGHNESS_FACTOR, tmp) == AI_SUCCESS)
		{
			config.roughness = tmp;
		}

		//MAPS
		//albedo map
		config.albedo_map_name = GetMaterialTextureFilePath(mat, aiTextureType_DIFFUSE);
		//normal/height map
		auto normalMapName = GetMaterialTextureFilePath(mat, aiTextureType_NORMALS);
		config.normal_map_name = normalMapName;
		if (normalMapName.empty())
		{
			auto heightMapName = GetMaterialTextureFilePath(mat, aiTextureType_HEIGHT);
			config.normal_map_name = heightMapName;
		}
		aiString metallic, roughness;
		mat->GetTexture(AI_MATKEY_METALLIC_TEXTURE, &metallic);
		mat->GetTexture(AI_MATKEY_ROUGHNESS_TEXTURE, &roughness);
		auto metallicPath = std::string(metallic.C_Str());
		auto roughnessPath = std::string(roughness.C_Str());

		//aoMetallicRoughness map
		if (!metallicPath.empty())
		{
			config.ao_roughness_metallic_name = this->directory + '/' + metallicPath;
		}
		else
		{
			config.ao_roughness_metallic_name = this->directory + '/' + roughnessPath;
		}

		//Emissive map
		config.emissive_map_name = GetMaterialTextureFilePath(mat, aiTextureType_EMISSIVE);
		
		std::shared_ptr<Material> material = MaterialSystem::Accquire(config);

		/*if (textures.empty())
		{
			textures.push_back(TextureSystem::Accquire("assets/Models/Diffuse.jpg",true));
		}
		// return a mesh object created from the extracted mesh data*/
		return Mesh(vertices, indices, material);
	}

	std::string GetMaterialTextureFilePath(aiMaterial* mat, aiTextureType type)
	{
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);

			if (str.length > 0)
			{
				std::string filepath = std::string(str.C_Str());
				filepath = this->directory + '/' + filepath;
				return filepath;
			}
		}
		return "";
	}


	std::vector<std::string> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
		std::vector<std::string> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			// check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			

			std::string filepath = std::string(str.C_Str());
			filepath = this->directory + '/' + filepath;
			textures.push_back(filepath);
		}
		return textures;
	}
};
