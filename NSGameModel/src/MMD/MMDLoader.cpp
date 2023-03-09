
#include "MMDLoader.h"

#include <string>

#include <NS/core/Log.h>
#include <NS/loader/TextureLoader.h>
#include <NS/core/graphics/RenderAPI.h>

#include <Pmx.h>
#include <Vmd.h>

namespace ns {

	bool MMDLoader::load(Ref<AnimatedModel> model, std::string &filePath, std::string& texturePath) {

		pmx::PmxModel modelData;
		std::ifstream stream = std::ifstream(filePath.c_str(), std::ios_base::binary);
		modelData.Read(&stream);
		stream.close();

		// Start loading to OpenGL

		// bind the vao
		model->bind();

		// Load indices : the eaziest one
		model->getIndicesBuffer()->bind();
		std::vector<unsigned int> indicesData;
		for (size_t i = 0; i < modelData.index_count; i++) {
			indicesData.push_back((unsigned int)modelData.indices[i]);
		}
		model->getIndicesBuffer()->storeData(&indicesData[0], modelData.index_count * sizeof(unsigned int), Buffer::StaticDraw);
		// no need to unbind indices buffer
		// model->getIndicesBuffer()->unbind();
		// End Load indices : the eaziest one

		// load vertices
		// each vertex contain
		//		3 float position 2 float textureCoords 3 float normals 4 float weight 4 int bone
		int sizePerVertexInByte = ((12 * sizeof(float)) + (4 * sizeof(int)));

		std::vector<char> verticesData(modelData.vertex_count * sizePerVertexInByte);

		for (int i = 0; i < modelData.bone_count; i++) {
			pmx::PmxBone* pBone = &modelData.bones[i];

			const char *boneName = (char*)pBone->bone_name.c_str();

			// Ref<ModelBone> bone = CreateRef<ModelBone>(i, pBone->bone_english_name, pBone->);



		}

		for (size_t i = 0; i < modelData.vertex_count; i++) {
			
			// position
			memcpy(&verticesData[i * sizePerVertexInByte + 0], &modelData.vertices[i].positon[0], sizeof(modelData.vertices[i].positon));

			// textureCoords
			memcpy(&verticesData[i * sizePerVertexInByte + 12], &modelData.vertices[i].uv[0], sizeof(modelData.vertices[i].uv));

			// normals
			memcpy(&verticesData[i * sizePerVertexInByte + 20], &modelData.vertices[i].normal[0], sizeof(modelData.vertices[i].normal));

			// bone & it's weight
			float weight;
			float weight2;
			switch (modelData.vertices[i].skinning_type)
			{
			case pmx::PmxVertexSkinningType::BDEF1:
				memcpy(&verticesData[i * sizePerVertexInByte + 32],
					&((pmx::PmxVertexSkinningBDEF1*)(modelData.vertices[i].skinning.get()))->bone_index, sizeof(int));
				memset(&verticesData[i * sizePerVertexInByte + 36], 0, 12);
				weight = 1.f;
				memcpy(&verticesData[i * sizePerVertexInByte + 48], &weight, sizeof(float));
				memset(&verticesData[i * sizePerVertexInByte + 52], 0, 12);
				break;
			case pmx::PmxVertexSkinningType::BDEF2:
				memcpy(&verticesData[i * sizePerVertexInByte + 32],
					&((pmx::PmxVertexSkinningBDEF2*)(modelData.vertices[i].skinning.get()))->bone_index1, sizeof(int));
				memcpy(&verticesData[i * sizePerVertexInByte + 36],
					&((pmx::PmxVertexSkinningBDEF2*)(modelData.vertices[i].skinning.get()))->bone_index2, sizeof(int));
				memset(&verticesData[i * sizePerVertexInByte + 40], 0, 8);
				weight = ((pmx::PmxVertexSkinningBDEF2*)(modelData.vertices[i].skinning.get()))->bone_weight;
				weight2 = 1.f - weight;
				memcpy(&verticesData[i * sizePerVertexInByte + 48], &weight, sizeof(float));
				memcpy(&verticesData[i * sizePerVertexInByte + 52], &weight2, sizeof(float));
				memset(&verticesData[i * sizePerVertexInByte + 56], 0, 8);
				break;
			case pmx::PmxVertexSkinningType::BDEF4:		// 常用
				memcpy(&verticesData[i * sizePerVertexInByte + 32],
					&((pmx::PmxVertexSkinningBDEF4*)(modelData.vertices[i].skinning.get()))->bone_index1, sizeof(int));
				memcpy(&verticesData[i * sizePerVertexInByte + 36],
					&((pmx::PmxVertexSkinningBDEF4*)(modelData.vertices[i].skinning.get()))->bone_index2, sizeof(int));
				memcpy(&verticesData[i * sizePerVertexInByte + 40],
					&((pmx::PmxVertexSkinningBDEF4*)(modelData.vertices[i].skinning.get()))->bone_index3, sizeof(int));
				memcpy(&verticesData[i * sizePerVertexInByte + 44],
					&((pmx::PmxVertexSkinningBDEF4*)(modelData.vertices[i].skinning.get()))->bone_index4, sizeof(int));
				memcpy(&verticesData[i * sizePerVertexInByte + 48],
					&((pmx::PmxVertexSkinningBDEF4*)(modelData.vertices[i].skinning.get()))->bone_weight1, sizeof(float));
				memcpy(&verticesData[i * sizePerVertexInByte + 52],
					&((pmx::PmxVertexSkinningBDEF4*)(modelData.vertices[i].skinning.get()))->bone_weight2, sizeof(float));
				memcpy(&verticesData[i * sizePerVertexInByte + 56],
					&((pmx::PmxVertexSkinningBDEF4*)(modelData.vertices[i].skinning.get()))->bone_weight3, sizeof(float));
				memcpy(&verticesData[i * sizePerVertexInByte + 60],
					&((pmx::PmxVertexSkinningBDEF4*)(modelData.vertices[i].skinning.get()))->bone_weight4, sizeof(float));
				break;
				// TODO SDEF, QDEF  沒研究
			default:
				break;
			}

		}

		// load to data buffer
		model->getDataBuffer()->bind();
		model->getDataBuffer()->storeData(&verticesData[0], (uint32_t)verticesData.size(), Buffer::StaticDraw);
		RenderAPI::AttributeFloatPointer(0, 3, false, sizePerVertexInByte, 0);
		RenderAPI::AttributeFloatPointer(1, 2, false, sizePerVertexInByte, (void*)(3 * sizeof(float)));
		RenderAPI::AttributeFloatPointer(2, 3, false, sizePerVertexInByte, (void*)(5 * sizeof(float)));
		RenderAPI::AttributeIntegerPointer(3, 4, sizePerVertexInByte, (void*)(8 * sizeof(float)));
		RenderAPI::AttributeFloatPointer(4, 4, false, sizePerVertexInByte, (void*)48);
		model->getDataBuffer()->unbind();
		// end load to data buffer

		// End load vertices

		// load material
	
		// load textures
		std::vector<Ref<Texture>> textures;
		for (size_t i = 0; i != modelData.texture_count; i++) {
			Ref<Texture> texture = Texture::Create(Texture::Type::texture2d);	// TODO Resource manager
			std::string texPath;
			texPath = texturePath + "/" + std::string(modelData.textures[i].begin(), modelData.textures[i].end());
			Loader::loadTexture2D(texture, texPath);
			textures.push_back(texture);
		}
		// end load textures

		int indicesOffset = 0;
		for (size_t i = 0; i != modelData.material_count; i++) {
			Material *material = new Material();
			// texture
			material->setDiffuse(textures.at(modelData.materials[i].diffuse_texture_index));
			// ambient
			material->setAmbient(modelData.materials[i].ambient[0], modelData.materials[i].ambient[1], modelData.materials[i].ambient[2]);
			model->addMaterial(material);
			// diffuse

			// MMD material is contain by mesh
			TexturedMesh *mesh = new TexturedMesh(std::string(modelData.materials[i].material_name.begin(), modelData.materials[i].material_name.end()), modelData.materials[i].index_count, indicesOffset);
			mesh->setMaterial(material);
			indicesOffset += modelData.materials[i].index_count;
			model->addMesh(mesh);
			// diffuse
		}

		// end load material

		// load bones
		// End load bone

		model->unbind();
		// End load model

		return true;
	}

	bool MMDLoader::loadAnimation(Ref<ModelAnimation> animation, const char *filePath) {
		vmd::VmdMotion motion;
		motion.LoadFromFile(filePath);

		NS_CORE_INFO("bone frame size: {0}", motion.bone_frames.size());
		NS_CORE_INFO("face frame size: {0}", motion.face_frames.size());
		NS_CORE_INFO("model name:{0}", motion.model_name);

		for (size_t i = 0; i < motion.bone_frames.size(); i++) {
			vmd::VmdBoneFrame boneFrame = motion.bone_frames[i];
			boneFrame.frame;	// which frame
			boneFrame.name; // bone name

			glm::vec3 position = glm::vec3(boneFrame.position[0], boneFrame.position[1], boneFrame.position[2]);
			glm::quat rotation = glm::quat(boneFrame.orientation[0], boneFrame.orientation[1], boneFrame.orientation[2], boneFrame.orientation[3]);
			
			NS_CORE_INFO("frame: {0} bone name:{1}", boneFrame.frame, boneFrame.name);
		}

		return true;
	}
}
