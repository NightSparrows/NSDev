

#include "TestStaticRenderer.h"

namespace ns {

	TestStaticRenderer::TestStaticRenderer() : Renderer("testStatic") {

	}

	TestStaticRenderer::~TestStaticRenderer() {

	}

	void TestStaticRenderer::addEntity(Ref<Model> model, Ref<Entity>entity) {
		
		auto it = this->m_entities.find(model);
		Ref <std::list<Transform*>> list;
		if (it == this->m_entities.end()) {
			list = CreateRef<std::list<Transform*>>();
			this->m_entities.emplace(std::pair<Ref<Model>, Ref<std::list<Transform*>>>(model, list));
		}
		else {
			list = it->second;
		}

		list->push_back(&entity->transform);

	}
	void TestStaticRenderer::init() {

		this->m_shader = ns::Shader::Create("static");


		// Uniform buffer
		this->m_instancesUniformBuffer = Buffer::Create(Buffer::UniformBuffer);
		this->m_instancesUniformBuffer->bind();
		this->m_instancesUniformBuffer->emptyData(10000 * sizeof(glm::mat4), Buffer::DynamicDraw);
		this->m_instancesUniformBuffer->unbind();
	}

	void TestStaticRenderer::render() {
		RenderAPI::DepthTest(true);
		this->m_shader->bind();

		for (auto it = this->m_entities.begin(); it != this->m_entities.end(); it++) {
			
			// Per Model
			Ref<Model> model = it->first;
			Ref<std::list<Transform*>> list = it->second;

			this->m_instancesUniformBuffer->bind();
			void *mapPtr = this->m_instancesUniformBuffer->startMapping();
			for (Transform *trans : *list) {
				memcpy(mapPtr, &trans->matrix(), sizeof(glm::mat4));
				mapPtr = (char*)mapPtr + sizeof(glm::mat4);
			}
			this->m_instancesUniformBuffer->stopMapping();
			this->m_instancesUniformBuffer->unbind();

			for (auto meshIt = model->getMeshes().begin(); meshIt != model->getMeshes().end(); meshIt++) {

				ns::TexturedMesh *mesh = *meshIt;
				mesh->getMaterial()->getDiffues()->bind(0);
				ns::RenderAPI::DrawIndexInstanced(mesh->getIndicesCount(), mesh->getOffset(), static_cast<uint32_t>(list->size()));
			}
		}

		this->m_shader->unbind();
	}

	void TestStaticRenderer::clearBuffer() {
		this->m_entities.clear();
		this->m_instancesUniformBuffer.reset();
	}

	void TestStaticRenderer::cleanUp() {
		this->m_shader.reset();
	}



}
