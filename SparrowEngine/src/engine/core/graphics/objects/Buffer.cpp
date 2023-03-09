
#include <NS/core/graphics/objects/Buffer.h>
#include <platform/OpenGL/objects/OpenGLBuffer.h>

namespace ns {



	Ref<Buffer> Buffer::Create(Type type)
	{
		return CreateRef<OpenGLBuffer>(type);
	}

}
