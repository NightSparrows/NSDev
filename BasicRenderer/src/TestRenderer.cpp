
#include <NS/core/graphics/RenderAPI.h>
#include <TestRenderer.h>

namespace ns {


	TestRenderer::TestRenderer() : Renderer("test") {

	}

	void TestRenderer::init() {

	}

	void TestRenderer::renderTest(Ref<Model> model) {
		model->bind();

		RenderAPI::EnableVertexAttributes(3);

		int indicesCount = 0;
		for (size_t i = 0; i < model->getMeshes().size(); i++) {
			model->getMeshes()[i]->getMaterial()->getDiffues()->bind(0);
			RenderAPI::DrawIndexed(model->getMeshes()[i]->getIndicesCount(), indicesCount);
			indicesCount += model->getMeshes()[i]->getIndicesCount();
		}

		RenderAPI::DisableVertexAttributes(3);

		model->unbind();
	}

	void TestRenderer::update(float delta) {

	}

	void TestRenderer::render() {

	}

	void TestRenderer::clearBuffer() {

	}

	void TestRenderer::cleanUp() {

	}



}
