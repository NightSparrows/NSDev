
#include <map>

#include <NS/core/Log.h>
#include <NS/core/graphics/RenderAPI.h>
#include <NS/loader/TextureLoader.h>
#include <NS/core/graphics/model/animated/armature/VertexWeight.h>
#include <NS/core/deps/glm/gtx/quaternion.hpp>
#include <NS/core/deps/glm/gtx/matrix_decompose.hpp>

#include "AssimpLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace ns {
	
	Ref<Texture> loadTexture(std::map<std::string, Ref<Texture>> *textures, aiMaterial *aMaterial, aiTextureType type, std::string& texturePath) {

		aiString aTexturePath;

		aiGetMaterialTexture(aMaterial, type, 0, &aTexturePath);

		Ref<Texture> texture = Texture::NO_TEXTURE;

		if (aTexturePath.length > 0) {
			auto it = textures->find(aTexturePath.C_Str());
			if (it != textures->end()) {
				texture = it->second;
			}
			else {
				// Texture is not loaded
				texture = Texture::Create(Texture::texture2d);
				std::string diffuseTexturePath = texturePath + "/" + aTexturePath.C_Str();
				Loader::loadTexture2D(texture, diffuseTexturePath);
				textures->emplace(std::pair<std::string, Ref<Texture>>(aTexturePath.C_Str(), texture));
#ifdef NS_DEBUG
				NS_CORE_INFO("texture loaded:{0}", diffuseTexturePath);
#endif // NS_DEBUG

			}
		}

		return texture;
	}

	void processMaterial(const aiScene *aScene, Ref<Model> model, std::string& texturePath) {

		std::map<std::string, Ref<Texture>> textures;

		for (unsigned int i = 0; i < aScene->mNumMaterials; i++) {
			aiMaterial *aMaterial = aScene->mMaterials[i];

			Material *material = new Material();
			model->addMaterial(material);
			material->setDiffuse(loadTexture(&textures, aMaterial, aiTextureType_DIFFUSE, texturePath));
			material->setNormalTexture(loadTexture(&textures, aMaterial, aiTextureType_NORMALS, texturePath));

			// diffuse color
			aiColor4D color;
			aiGetMaterialColor(aMaterial, AI_MATKEY_COLOR_DIFFUSE, &color);

			material->setDiffuseColor(color.r, color.g, color.b, color.a);
		}
	}

	bool processMeshes(const aiScene *aScene, Ref<Model> model) {

		if (!aScene->HasMeshes()) {
			return false;
		}


		// calc total vertices
		Uint32 totalVertices = 0;

		for (Uint32 i = 0; i < aScene->mNumMeshes; i++) {
			totalVertices += aScene->mMeshes[i]->mNumVertices;
		}

		Uint32 sizePerVertexInByte = 11 * sizeof(float);

		Uint32 meshVertexOffset = 0;	// the offset that the mesh start with
		Uint32 totalVertexCount = 0;
		Uint32 meshIndicesOffset = 0;
		Uint32 meshIndicesCount;

		// create buffer to store data
		Scope<std::vector<Uint32>> indicesData = CreateScope<std::vector<Uint32>>();
		// create the mesh that take data
		Scope<std::vector<char>> verticesData = CreateScope<std::vector<char>>(sizePerVertexInByte * totalVertices);

		// prrocess meshes
		for (Uint32 i = 0; i < aScene->mNumMeshes; i++) {
			aiMesh *aMesh = aScene->mMeshes[i];

			// reset the mesh indices count
			meshIndicesCount = 0;

			// process indices
			for (Uint32 j = 0; j < aMesh->mNumFaces; j++) {
				for (Uint32 k = 0; k < aMesh->mFaces[j].mNumIndices; k++) {
					indicesData->push_back(meshVertexOffset + aMesh->mFaces[j].mIndices[k]);
					meshIndicesCount++;
				}
			}

			// process vertices
			for (Uint32 j = 0; j < aMesh->mNumVertices; j++) {

				memcpy(&(*verticesData)[totalVertexCount * sizePerVertexInByte + 0], &aMesh->mVertices[j].x, sizeof(float));
				memcpy(&(*verticesData)[totalVertexCount * sizePerVertexInByte + 4], &aMesh->mVertices[j].y, sizeof(float));
				memcpy(&(*verticesData)[totalVertexCount * sizePerVertexInByte + 8], &aMesh->mVertices[j].z, sizeof(float));

				float u = 0, v = 0;
				if (aMesh->mTextureCoords[0] != nullptr) {
					u = aMesh->mTextureCoords[0][j].x;
					v = aMesh->mTextureCoords[0][j].y;
				}
				memcpy(&(*verticesData)[totalVertexCount * sizePerVertexInByte + 12], &u, sizeof(float));
				memcpy(&(*verticesData)[totalVertexCount * sizePerVertexInByte + 16], &v, sizeof(float));

				memcpy(&(*verticesData)[totalVertexCount * sizePerVertexInByte + 20], &aMesh->mNormals[j].x, sizeof(float));
				memcpy(&(*verticesData)[totalVertexCount * sizePerVertexInByte + 24], &aMesh->mNormals[j].y, sizeof(float));
				memcpy(&(*verticesData)[totalVertexCount * sizePerVertexInByte + 28], &aMesh->mNormals[j].z, sizeof(float));

				if (aMesh->mTangents != nullptr) {
					memcpy(&(*verticesData)[totalVertexCount * sizePerVertexInByte + 32], &aMesh->mTangents[j].x, sizeof(float));
					memcpy(&(*verticesData)[totalVertexCount * sizePerVertexInByte + 36], &aMesh->mTangents[j].y, sizeof(float));
					memcpy(&(*verticesData)[totalVertexCount * sizePerVertexInByte + 40], &aMesh->mTangents[j].z, sizeof(float));
				}

				totalVertexCount++;
			}

			// textured mesh for this model
			TexturedMesh *texturedMesh = new TexturedMesh(std::string(aMesh->mName.C_Str()), meshIndicesCount, meshIndicesOffset);
			model->addMesh(texturedMesh);

			// generate the texturedmesh for model
			Uint32 materialIndex = aMesh->mMaterialIndex;
			// has material 
			if (materialIndex < model->getMaterials().size()) {
				texturedMesh->setMaterial(model->getMaterials()[materialIndex]);
			}
			else {
				NS_CORE_WARN("no material for this mesh!");
			}

			// add the offset to the mesh
			meshVertexOffset = totalVertexCount;
			meshIndicesOffset += meshIndicesCount;
		}

		// load to model
		model->bind();
		model->getIndicesBuffer()->bind();
		model->getIndicesBuffer()->storeData(&(*indicesData)[0], (uint32_t)indicesData->size() * sizeof(Uint32), Buffer::StaticDraw);

		model->getDataBuffer()->bind();
		model->getDataBuffer()->storeData(&(*verticesData)[0], (uint32_t)verticesData->size(), Buffer::StaticDraw);
		RenderAPI::AttributeFloatPointer(0, 3, false, sizePerVertexInByte, 0);
		RenderAPI::AttributeFloatPointer(1, 2, false, sizePerVertexInByte, (void*)(3 * sizeof(float)));
		RenderAPI::AttributeFloatPointer(2, 3, false, sizePerVertexInByte, (void*)(5 * sizeof(float)));
		RenderAPI::AttributeFloatPointer(3, 3, false, sizePerVertexInByte, (void*)(8 * sizeof(float)));				// tangent
		model->getDataBuffer()->unbind();


		model->unbind();


		return true;
	}

	bool AssimpLoader::loadStatic(Ref<Model> model, std::string& filePath, std::string& texturePath) {

		Assimp::Importer importer;

		const aiScene *aScene = importer.ReadFile(filePath, aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

		if (!aScene) {
			NS_CORE_WARN("Failed to load model using ASSIMP!:{0}", filePath);
			return false;
		}

		processMaterial(aScene, model, texturePath);

		if (!processMeshes(aScene, model)) {
			return false;
		}

		return true;
	}

	glm::mat4 toMatrix(aiMatrix4x4& mOffsetMatrix) {

		glm::mat4 matrix = glm::mat4();
		matrix[0][0] = mOffsetMatrix.a1;
		matrix[1][0] = mOffsetMatrix.a2;
		matrix[2][0] = mOffsetMatrix.a3;
		matrix[3][0] = mOffsetMatrix.a4;
		matrix[0][1] = mOffsetMatrix.b1;
		matrix[1][1] = mOffsetMatrix.b2;
		matrix[2][1] = mOffsetMatrix.b3;
		matrix[3][1] = mOffsetMatrix.b4;
		matrix[0][2] = mOffsetMatrix.c1;
		matrix[1][2] = mOffsetMatrix.c2;
		matrix[2][2] = mOffsetMatrix.c3;
		matrix[3][2] = mOffsetMatrix.c4;
		matrix[0][3] = mOffsetMatrix.d1;
		matrix[1][3] = mOffsetMatrix.d2;
		matrix[2][3] = mOffsetMatrix.d3;
		matrix[3][3] = mOffsetMatrix.d4;
		return matrix;
	}

	bool processAnimatedMeshes(const aiScene *aScene, Ref<AnimatedModel> model) {
		if (!aScene->HasMeshes())
			return false;

		// calc total vertices
		Uint32 totalVertices = 0;

		for (Uint32 i = 0; i < aScene->mNumMeshes; i++) {
			totalVertices += aScene->mMeshes[i]->mNumVertices;
		}

		// no need this transform
		// model->setGlobalInverseTransform(glm::inverse(toMatrix(aScene->mRootNode->mTransformation)));

		// position(3f), texture coordinate(2f), normal(3f), tangent(3f), bone id(4i), weight(4f)
		Uint32 sizePerVertexInByte = ((15 * sizeof(float)) + (4 * sizeof(Int32)));

		Uint32 meshVertexOffset = 0;	// the offset that the mesh start with
		Uint32 totalVertexCount = 0;
		Uint32 meshIndicesOffset = 0;
		Uint32 meshIndicesCount;
		Uint32 boneCount = 0;
		Uint32 boneOffset = 0;

		// create buffer to store data
		Scope<std::vector<Uint32>> indicesData = CreateScope<std::vector<Uint32>>();
		// create the mesh that take data
		Scope<std::vector<char>> verticesData = CreateScope<std::vector<char>>(sizePerVertexInByte * totalVertices);

		// process meshes
		for (Uint32 i = 0; i < aScene->mNumMeshes; i++) {
			aiMesh *aMesh = aScene->mMeshes[i];

			// vertex id, list of vertexweight
			Scope<std::map<Uint32, Ref<std::vector<Ref<VertexWeight>>>>> vwList = CreateScope<std::map<Uint32, Ref<std::vector<Ref<VertexWeight>>>>>();

			for (Uint32 j = 0; j < aMesh->mNumBones; j++) {
				aiBone *aBone = aMesh->mBones[j];

				Uint32 id = boneCount;

				aiVector3D aScale;
				aiVector3D aPosition;
				aiQuaternion aRotation;
				aBone->mOffsetMatrix.Decompose(aScale, aRotation, aPosition);

				glm::vec3 position = { aPosition.x, aPosition.y, aPosition.z };
				glm::quat rotation = glm::quat(aRotation.w, aRotation.x, aRotation.y, aRotation.z);
				rotation = glm::normalize(rotation);
				glm::vec3 scale = { aScale.x, aScale.y, aScale.z };

				// create the bone for model
				ModelBone *bone = new ModelBone(id, std::string(aBone->mName.C_Str()), position, rotation, scale);
				bone->inMesh();
				model->addBone(bone);
				
				for (Uint32 k = 0; k < aBone->mNumWeights; k++) {
					aiVertexWeight aVertexWeight = aBone->mWeights[k];
					
					Ref<VertexWeight> vw = CreateRef<VertexWeight>(id, meshVertexOffset + aVertexWeight.mVertexId, aVertexWeight.mWeight);
					
					auto it = vwList->find(vw->getVertexId());
					Ref<std::vector<Ref<VertexWeight>>> list;
					if (it == vwList->end()) {
						list = CreateRef<std::vector<Ref<VertexWeight>>>();
						vwList->insert(std::pair<Uint32, Ref<std::vector<Ref<VertexWeight>>>>(vw->getVertexId(), list));
					}
					else {
						list = it->second;
					}
					list->push_back(vw);
				}

				boneCount++;
			}

			// process indices
			// reset the indices count
			meshIndicesCount = 0;
			for (Uint32 j = 0; j < aMesh->mNumFaces; j++) {
				for (Uint32 k = 0; k < aMesh->mFaces[j].mNumIndices; k++) {
					indicesData->push_back(meshVertexOffset + aMesh->mFaces[j].mIndices[k]);
					meshIndicesCount++;
				}
			}

			// process vertices
			for (Uint32 j = 0; j < aMesh->mNumVertices; j++) {

				// 也可以執行，但我覺得不明確
				// memcpy(&(*verticesData)[totalVertexCount * sizePerVertexInByte + 0], &aMesh->mVertices[j], (3 * sizeof(float))); 
				memcpy(&(*verticesData)[totalVertexCount * sizePerVertexInByte + 0], &aMesh->mVertices[j].x, sizeof(float));
				memcpy(&(*verticesData)[totalVertexCount * sizePerVertexInByte + 4], &aMesh->mVertices[j].y, sizeof(float));
				memcpy(&(*verticesData)[totalVertexCount * sizePerVertexInByte + 8], &aMesh->mVertices[j].z, sizeof(float));

				memcpy(&(*verticesData)[totalVertexCount * sizePerVertexInByte + 12], &aMesh->mTextureCoords[0][j].x, sizeof(float));
				memcpy(&(*verticesData)[totalVertexCount * sizePerVertexInByte + 16], &aMesh->mTextureCoords[0][j].y, sizeof(float));

				memcpy(&(*verticesData)[totalVertexCount * sizePerVertexInByte + 20], &aMesh->mNormals[j].x, sizeof(float));
				memcpy(&(*verticesData)[totalVertexCount * sizePerVertexInByte + 24], &aMesh->mNormals[j].y, sizeof(float));
				memcpy(&(*verticesData)[totalVertexCount * sizePerVertexInByte + 28], &aMesh->mNormals[j].z, sizeof(float));

				memcpy(&(*verticesData)[totalVertexCount * sizePerVertexInByte + 32], &aMesh->mTangents[j].x, sizeof(float));
				memcpy(&(*verticesData)[totalVertexCount * sizePerVertexInByte + 36], &aMesh->mTangents[j].y, sizeof(float));
				memcpy(&(*verticesData)[totalVertexCount * sizePerVertexInByte + 40], &aMesh->mTangents[j].z, sizeof(float));

				// get the bones list
				auto it = vwList->find(totalVertexCount);

				// no bone for this vertices
				if (it == vwList->end()) {
					memset(&(*verticesData)[totalVertexCount * sizePerVertexInByte + 44], 0, (4 * sizeof(int32_t)));
					memset(&(*verticesData)[totalVertexCount * sizePerVertexInByte + 60], 0, (4 * sizeof(float)));
				}
				else {
					Ref<std::vector<Ref<VertexWeight>>> list = it->second;

					float totalWeights = 0;
					for (Uint32 k = 0; k < list->size(); k++) {
						Ref<VertexWeight> vw = list->at(k);
						totalWeights += vw->getWeight();
					}

					for (Uint32 k = 0; k < 4/* MAX_WEIGHT */; k++) {
						if (k < list->size()) {
							Ref<VertexWeight> vw = list->at(k);
							int boneId = vw->getBoneId();
							float weight = vw->getWeight() / totalWeights;
							memcpy(&(*verticesData)[totalVertexCount * sizePerVertexInByte + 44 + (k * sizeof(int32_t))], &boneId, sizeof(int32_t));
							memcpy(&(*verticesData)[totalVertexCount * sizePerVertexInByte + 60 + (k * sizeof(float))], &weight, sizeof(float));
						}
						else {
							memset(&(*verticesData)[totalVertexCount * sizePerVertexInByte + 44 + (k * sizeof(int32_t))], 0, sizeof(int32_t));
							memset(&(*verticesData)[totalVertexCount * sizePerVertexInByte + 60 + (k * sizeof(float))], 0, sizeof(float));
						}

					}
				}

				totalVertexCount++;
			}

			// textured mesh for this model
			TexturedMesh *texturedMesh = new TexturedMesh(std::string(aMesh->mName.C_Str()), meshIndicesCount, meshIndicesOffset);
			model->addMesh(texturedMesh);
			model->addBoneOffset(boneOffset, boneCount - boneOffset);

			// generate the texturedmesh for model
			Uint32 materialIndex = aMesh->mMaterialIndex;
			// has material 
			if (materialIndex < model->getMaterials().size()) {
				texturedMesh->setMaterial(model->getMaterials()[materialIndex]);
			}
			else {
				NS_CORE_WARN("no material for this mesh!");
			}

			// add the offset to the mesh
			meshVertexOffset = totalVertexCount;
			meshIndicesOffset += meshIndicesCount;
			boneOffset = boneCount;
		}

		// load to model
		model->bind();
		model->getIndicesBuffer()->bind();
		model->getIndicesBuffer()->storeData(&(*indicesData)[0], (Uint32)indicesData->size() * sizeof(Uint32), Buffer::StaticDraw);

		model->getDataBuffer()->bind();
		model->getDataBuffer()->storeData(&(*verticesData)[0], (Uint32)verticesData->size(), Buffer::StaticDraw);
		RenderAPI::AttributeFloatPointer(0, 3, false, sizePerVertexInByte, 0);
		RenderAPI::AttributeFloatPointer(1, 2, false, sizePerVertexInByte, (void*)(3 * sizeof(float)));
		RenderAPI::AttributeFloatPointer(2, 3, false, sizePerVertexInByte, (void*)(5 * sizeof(float)));
		RenderAPI::AttributeFloatPointer(3, 3, false, sizePerVertexInByte, (void*)(8 * sizeof(float)));				// tangent
		RenderAPI::AttributeIntegerPointer(4, 4, sizePerVertexInByte, (void*)(11 * sizeof(float)));
		RenderAPI::AttributeFloatPointer(5, 4, false, sizePerVertexInByte, (void*)(11 * sizeof(float) + (4 * sizeof(Uint32))));
		model->getDataBuffer()->unbind();

		model->unbind();

		return true;
	}

	void processBone(const aiScene *aScene, Ref<AnimatedModel> model, ModelBone *bone) {
		aiNode *aNode = aScene->mRootNode->FindNode(bone->getName());
		if (aNode == nullptr) {
			NS_CORE_WARN("Failed to find bone in the armature.");
			return;
		}

		for (Uint32 i = 0; i < aNode->mNumChildren; i++) {
			aiNode *aChildNode = aNode->mChildren[i];

			ModelBone *childBone = model->findBoneByName(std::string(aChildNode->mName.C_Str()));
			if (childBone == NULL) {
				aiVector3D aScale;
				aiVector3D aPosition;
				aiQuaternion aRotation;
				aChildNode->mTransformation.Decompose(aScale, aRotation, aPosition);

				glm::vec3 position = { aPosition.x, aPosition.y, aPosition.z };
				glm::quat rotation = glm::quat(aRotation.w, aRotation.x, aRotation.y, aRotation.z);
				rotation = glm::normalize(rotation);
				glm::vec3 scale = { aScale.x, aScale.y, aScale.z };

				std::string boneName = aChildNode->mName.C_Str();

				childBone = new ModelBone((Uint32)model->getBones()->size(), boneName, position, rotation, scale);
				model->addBone(childBone);

			}
			bone->addChild(childBone);
			childBone->setParent(bone);
			processBone(aScene, model, childBone);
			//NS_CORE_INFO("Bone {0} has child {1}", bone->getName(), childBone->getName());
		}
	}

	void processBoneRelation(const aiScene *aScene, Ref<AnimatedModel> model) {

		// Ingrone the model data armature transformation
		ModelBone *armature = new ModelBone((Uint32)model->getBones()->size(), "Armature", glm::vec3(0), glm::quat(), glm::vec3(1));
		model->addBone(armature);
		processBone(aScene, model, armature);
		model->setRootBone(armature);
#ifdef NS_DEBUG
		NS_CORE_INFO("model root bone:{0}", model->getRootBone()->getName());
#endif
		/*
		// Old method

		size_t originCountOfBone = model->getBones()->size();
		for (size_t i = 0; i < originCountOfBone; i++) {
			ModelBone *bone = model->getBones()->at(i);
			processBone(aScene, model, bone);
		}

		// Determine the root bone
		for (auto it = model->getBones()->begin(); it != model->getBones()->end(); it++) {
			ModelBone *bone = *it;
			if (bone->hasParent()) {
				if (!bone->getParent()->isInMesh()) {
					model->setRootBone(bone->getParent());
					NS_CORE_INFO("Root node find: {0}", bone->getParent()->getName());
					break;
				}
			}
		}
		*/
	}

	bool AssimpLoader::loadAnimated(Ref<AnimatedModel> model, std::string& filePath, std::string& texturePath) {

		Assimp::Importer importer;

		const aiScene *aScene = importer.ReadFile(filePath, aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_LimitBoneWeights);

		if (!aScene) {
			NS_CORE_WARN("Failed to load model using ASSIMP!:{0}", filePath);
			return false;
		}

		processMaterial(aScene, model, texturePath);

		if (!processAnimatedMeshes(aScene, model)) {
			return false;
		}

		processBoneRelation(aScene, model);

		return true;
	}

	bool AssimpLoader::loadModelAnimation(Ref<ModelAnimation> animation, const char *filePath) {

		Assimp::Importer importer;

		const aiScene *aScene = importer.ReadFile(filePath, 0);

		if (!aScene) {
			NS_CORE_WARN("Failed to load model using ASSIMP!:{0}", filePath);
			return false;
		}

		// Just read the first animation of this file
		aiAnimation *aAnimation = aScene->mAnimations[0];

		float tickPerSecond = aAnimation->mTicksPerSecond == 0 ? 1.f : (float)aAnimation->mTicksPerSecond;


		animation->setDuration((float)aAnimation->mDuration / tickPerSecond);
#ifdef NS_DEBUG
		NS_CORE_TRACE("Loading animation: {0}", aAnimation->mName.C_Str());
		NS_CORE_TRACE("animation time: {0}s", animation->getDuration());
		NS_CORE_TRACE("channels: {0}", aAnimation->mNumChannels);
#endif // NS_DEBUG

		for (Uint32 i = 0; i < aAnimation->mNumChannels; i++) {
			aiNodeAnim *aNodeAnim = aAnimation->mChannels[i];

			std::string boneName = aNodeAnim->mNodeName.C_Str();

			for (Uint32 j = 0; j < aNodeAnim->mNumPositionKeys; j++) {
				aiVectorKey aPositionKey = aNodeAnim->mPositionKeys[j];	// use position key to represent a frame
				aiQuatKey aRotationKey = aNodeAnim->mRotationKeys[j];
				aiVectorKey aScaleKey = aNodeAnim->mScalingKeys[j];
				Ref<KeyFrame> frame;
				if (animation->getFrames()->size() < j + 1) {
					float timeStamp = (float)aPositionKey.mTime / tickPerSecond;
					frame = CreateRef<KeyFrame>(timeStamp);
					animation->addFrame(frame);
				}
				else {
					frame = (*animation->getFrames())[j];
				}
				glm::vec3 position = { aPositionKey.mValue.x, aPositionKey.mValue.y, aPositionKey.mValue.z };
				glm::quat rotation = glm::quat(aRotationKey.mValue.w, aRotationKey.mValue.x, aRotationKey.mValue.y, aRotationKey.mValue.z);
				rotation = glm::normalize(rotation);
				glm::vec3 scale = { aScaleKey.mValue.x, aScaleKey.mValue.y, aScaleKey.mValue.z };

				frame->addBoneTransform(boneName, position, rotation, scale);

			}

		}
#ifdef NS_DEBUG
		NS_CORE_INFO("Total frame: {0}", animation->getFrames()->size());
#endif // NS_DEBUG

		return true;
	}

}
