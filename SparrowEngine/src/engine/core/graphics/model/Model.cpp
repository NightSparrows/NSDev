
#include <NS/core/Log.h>
#include <NS/core/graphics/model/Model.h>

namespace ns {

	void Model::bind() {
		this->vao->bind();
	}

	void Model::unbind() {
		this->vao->unbind();
	}

	void Model::addMesh(TexturedMesh *mesh) {
		this->m_Meshes.push_back(mesh);
	}

	void Model::addMaterial(Material *material) {
		this->m_Materials.push_back(material);
	}

	Ref<Buffer> Model::getIndicesBuffer() {
		return this->vao->getIndicesBuffer();
	}

	Ref<Buffer> Model::getDataBuffer() {
		return this->vao->getDataBuffer();
	}


	Model::Model() {
		this->vao = VertexArray::Create();
	}

	Model::~Model() {
		for (TexturedMesh *mesh : this->m_Meshes) {
			delete mesh;
		}

		for (Material *material : this->m_Materials) {
			delete material;
		}

		// Destructed will count down the material use_count!!! <- that what I expect.
#ifdef NS_DEBUG
		if (this->vao.use_count() > 1)
			NS_CORE_WARN("Vertex array is not unused while model will be released.");
#endif // NS_DEBUG
	}

}
