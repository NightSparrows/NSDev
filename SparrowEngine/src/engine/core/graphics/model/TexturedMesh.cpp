


#include <NS/core/graphics/model/TexturedMesh.h>

namespace ns {

	TexturedMesh::TexturedMesh(std::string& name, Uint32 indicesCount, Uint32 offset) :
		m_Name(name), m_IndicesCount(indicesCount), m_Offset(offset)
	{
		this->m_Material = &Material::NO_MATERIAL;
	}

	TexturedMesh::~TexturedMesh() {

	}

	Uint32 TexturedMesh::getIndicesCount() {
		return this->m_IndicesCount;
	}

	Uint32 TexturedMesh::getOffset() {
		return this->m_Offset;
	}

	Material* TexturedMesh::getMaterial() {
		return this->m_Material;
	}

	void TexturedMesh::setMaterial(Material* material) {
		this->m_Material = material;
	}

	std::string TexturedMesh::getName() {
		return this->m_Name;
	}

}
