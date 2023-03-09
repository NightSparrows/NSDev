#pragma once

#include <NS/core/Base.h>
#include <NS/core/graphics/Renderer.h>
#include <NS/core/graphics/model/Model.h>

namespace ns {

	class NS_API TestRenderer : public Renderer {
	public:
		TestRenderer();

		void renderTest(Ref<Model> model);

		void init();

		/// Updating method that is used on every frame
		void update(float delta);

		/// The rendering method
		void render();

		void clearBuffer();

		void cleanUp();


	};

}
