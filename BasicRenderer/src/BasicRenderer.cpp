
#include <BasicRenderer.h>
#include <NS/core/graphics/RenderAPI.h>
#include <NS/core/Log.h>

#define MAX_INSTANCE 1000

namespace ns {

	BasicRenderer::BasicRenderer(Ref<Shader> shader) : Renderer("basic") {
		this->m_shader = shader;
		this->m_transMatrix = this->m_shader->getUniform("transformationMatrix");
	}

	void BasicRenderer::addEntity(Ref<Entity> entity, Ref<Model> model) {
		auto it = this->m_map->find(model);

		std::list<Ref<Entity>> *batch;
		if (it == this->m_map->end()) {
			batch = new std::list<Ref<Entity>>();
			this->m_map->insert(std::pair<Ref<Model>, std::list<Ref<Entity>>*>(model, batch));
		}
		else {
			batch = it->second;
		}
		batch->push_back(entity);
	}

	void BasicRenderer::init() {
		this->m_map = new std::map<Ref<Model>, std::list<Ref<Entity>>*>();
	}

	void BasicRenderer::update(float delta) {

	}

	void BasicRenderer::render() {

		// initialize
		ns::RenderAPI::DepthTest(true);
		ns::RenderAPI::Blend(true);

		// shader start
		this->m_shader->bind();
		// Draw model
		for (auto it = this->m_map->begin(); it != this->m_map->end(); it++) {

			Ref<Model> model = it->first;

			model->bind();

			RenderAPI::EnableVertexAttributes(3);		// 開3個


			// Store instance Data
			std::list<Ref<Entity>> *list = it->second;

			for (auto listIt = list->begin(); listIt != list->end(); listIt++) {
				Ref<Entity> entity = *listIt;

				this->m_transMatrix->load(entity->transform.matrix());

				for (auto meshIt = model->getMeshes().begin(); meshIt != model->getMeshes().end(); meshIt++) {
					TexturedMesh *mesh = *meshIt;
					mesh->getMaterial()->getDiffues()->bind(0);
					/*
[13:34:15] NS:  2784
[13:34:15] NS:  6912
[13:34:15] NS:  288
[13:34:15] NS:  23568
[13:34:15] NS: body 215904
[13:34:15] NS: sesp 123192
[13:34:15] NS:  432
[13:34:15] NS:  432
[13:34:15] NS:  52416
[13:34:15] NS: sepants 30720
[13:34:15] NS: Apants 32736
[13:34:15] NS: gt 26448
[13:34:15] NS: sepmtl 17568
[13:34:15] NS:  43488
[13:34:15] NS:  43488
[13:34:15] NS:  24000
[13:34:15] NS:  14142
[13:34:15] NS: hair 128040
[13:34:15] NS: hair3 19680
[13:34:15] NS:  480
[13:34:15] NS:  480
[13:34:15] NS:  492
[13:34:15] NS:  492
[13:34:15] NS:  480
[13:34:15] NS:  480
[13:34:15] NS:  480
[13:34:15] NS:  480
*/
					// 123192 cloth (30720, 32736) pant 26448 吊帶
					// NS_CORE_INFO("{0} {1}", mesh->getName(), mesh->getIndicesCount());
					//if (mesh->getIndicesCount() != 30720 && mesh->getIndicesCount() != 123192 && mesh->getIndicesCount() != 32736 && mesh->getIndicesCount() != 17568) {
						RenderAPI::DrawIndexed(mesh->getIndicesCount(), mesh->getOffset());
					//}
				}
			}

			/*
			for (auto meshIt = model->getMeshes().begin(); meshIt != model->getMeshes().end(); meshIt++) {
				TexturedMesh *mesh = *meshIt;
				mesh->getMaterial()->getDiffues()->bind(0);
				RenderAPI::DrawIndexInstanced(mesh->getIndicesCount(), mesh->getOffset(), list->size());
				// RenderAPI::DrawIndexed(mesh->getIndicesCount(), mesh->getOffset());
				//NS_CORE_INFO("Draw: {0}, count:{1}, offset:{2}", mesh->getName(), mesh->getIndicesCount(), mesh->getOffset());
			}
			*/
			RenderAPI::DisableVertexAttributes(3);		// 關3個
			model->unbind();
		}

		// 我是覺得不需要做這件事
		/*
		ns::RenderAPI::DepthTest(false);
		ns::RenderAPI::Blend(false);
		*/
		// shader stop
		this->m_shader->unbind();
	}

	void BasicRenderer::clearBuffer() {
		for (auto it = this->m_map->begin(); it != this->m_map->end(); it++) {
			delete it->second;
		}
		this->m_map->clear();
	}

	void BasicRenderer::cleanUp() {
		delete this->m_map;
	}


}
