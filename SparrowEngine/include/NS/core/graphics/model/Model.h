#pragma once

#include <vector>

#include "../objects/VertexArray.h"

#include "TexturedMesh.h"

namespace ns {

	// TexturedMesh is owned by Model and I think use raw new/delete is a good way to manage TexturedMeshes.
	// So as Material
	class NS_API Model {
	public:
		Model();
		~Model();

		void bind();
		void unbind();

		Ref<Buffer> getIndicesBuffer();
		Ref<Buffer> getDataBuffer();

		const std::vector<TexturedMesh*>& getMeshes() { return this->m_Meshes; }
		void addMesh(TexturedMesh* mesh);

		const std::vector<Material*>& getMaterials() { return this->m_Materials; }
		void addMaterial(Material *material);

	private:
		Ref<VertexArray> vao;
		std::vector<TexturedMesh*> m_Meshes;
		std::vector<Material*> m_Materials;
	};

}
