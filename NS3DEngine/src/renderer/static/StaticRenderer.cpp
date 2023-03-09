
#include <NS/renderer/static/StaticRenderer.h>
#include <NS/core/graphics/RenderAPI.h>

namespace ns {


	StaticRenderer::StaticRenderer(Ref<Shader> shader) : Renderer("static") {
		this->m_shader = shader;
	}

	void StaticRenderer::addModel(const Transform& transform, const Ref<Model> model) {
		auto it = m_entities.find(model);

		std::vector<glm::mat4> *list;
		if (it == m_entities.end()) {
			list = new std::vector<glm::mat4>();
			m_entities.emplace(std::pair<Ref<Model>, std::vector<glm::mat4>*>(model, list));
		}
		else {
			list = it->second;
		}
		list->push_back(transform.matrix());
	}

	void StaticRenderer::init() {
		this->m_transMatricesBuffer = Buffer::Create(Buffer::UniformBuffer);
		this->m_transMatricesBuffer->bind();
		this->m_transMatricesBuffer->emptyData(MAX_INSTANCES * 64, Buffer::StreamDraw);
		this->m_transMatricesBuffer->unbind();
	}

	void StaticRenderer::update(float delta) {

	}

	void StaticRenderer::render() {

		this->m_shader->bind();

		for (auto it = this->m_entities.begin(); it != this->m_entities.end(); it++) {

			// entities
			std::vector<glm::mat4>* list = it->second;
			void *mapPtr = this->m_transMatricesBuffer->startMapping();
			for (auto matPtr = list->begin(); matPtr != list->end(); matPtr++) {
				glm::mat4 matrix = *matPtr;
				memcpy(mapPtr, &matrix[0][0], sizeof(glm::mat4));
				mapPtr = (char*)mapPtr + sizeof(glm::mat4);
			}
			this->m_transMatricesBuffer->stopMapping();
			this->m_transMatricesBuffer->unbind();

			// 1 is used by MasterRenderer
			this->m_transMatricesBuffer->bindBase(2);

			Ref<Model> model = it->first;

			model->bind();
			RenderAPI::EnableVertexAttributes(3);

			for (auto meshPtr = model->getMeshes().begin(); meshPtr != model->getMeshes().end(); meshPtr++) {
				TexturedMesh* mesh = *meshPtr;
				Material* material = mesh->getMaterial();
				material->getDiffues()->bind(0);
				if (material->hasNormalTexture()) {
					material->getNormalTexture()->bind(1);
				}
				RenderAPI::DrawIndexInstanced(mesh->getIndicesCount(), mesh->getOffset(), (int)list->size());
			}

			RenderAPI::DisableVertexAttributes(3);
			model->unbind();
		}

		this->m_shader->unbind();
	}

	void StaticRenderer::clearBuffer() {
		for (auto it = this->m_entities.begin(); it != this->m_entities.end(); it++) {
			delete it->second;
		}
		this->m_entities.clear();
	}

	void StaticRenderer::cleanUp() {
		this->m_transMatricesBuffer.reset();
	}


}
