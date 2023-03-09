#pragma once

#include <NS/SparrowEngine.h>

class Skybox {
	friend class SkyboxRenderer;
public:
	Skybox();
	~Skybox();

	void loadTexture(std::vector<std::string>* texturesFilePath);

private:
	ns::Ref<ns::Texture> m_texture;
};
