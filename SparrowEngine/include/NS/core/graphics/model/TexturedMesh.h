#pragma once

#include <string>

#include "Material.h"

namespace ns {

	/// Will not release Material delete by Model class
	class NS_API TexturedMesh {
	public:
		TexturedMesh(std::string& name, Uint32 indicesCount, Uint32 offset);
		~TexturedMesh();

		Uint32 getIndicesCount();

		Uint32 getOffset();

		Material* getMaterial();

		bool hasMaterial() { return this->m_Material != nullptr; }

		void setMaterial(Material* material);

		std::string getName();

	private:
		std::string m_Name;
		Uint32 m_IndicesCount;
		Uint32 m_Offset;
		Material* m_Material;
	};

}
