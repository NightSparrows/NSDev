#pragma once

#include <NS/SparrowEngine.h>

class Terrain {

public:
	Terrain(int x, int z);
	~Terrain();

	void bind();

	ns::Ref<ns::Texture> getBlendMap();
	ns::Ref<ns::Texture> getrTexture();
	ns::Ref<ns::Texture> getgTexture();
	ns::Ref<ns::Texture> getbTexture();
	ns::Ref<ns::Texture> getbgTexture();

	int getX();
	int getZ();

private:
	int m_x, m_z;

	ns::Ref<ns::Texture> m_blendTexture;
	ns::Ref<ns::Texture> m_rTexture;
	ns::Ref<ns::Texture> m_gTexture;
	ns::Ref<ns::Texture> m_bTexture;
	ns::Ref<ns::Texture> m_bgTexture;

};
