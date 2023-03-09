
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <NS/core/Log.h>
#include <NS/loader/TextureLoader.h>
#include <stb_image.h>

namespace ns {

	bool Loader::loadTexture2D(Ref<Texture> &texture, const std::string& filePath, const TextureConfig config) {

		// check whether texture is texture2d
		if (texture->getType() != Texture::texture2d) {
			NS_CORE_WARN("This texture is not texture2d!");
			return false;
		}

		int width, height, channels;
		stbi_uc *data = stbi_load(filePath.c_str(), &width, &height, &channels, 0/* desired channel(?) */);
		if (!data) {
			NS_CORE_WARN("Failed to load texture:{0}\n{1}", filePath, stbi_failure_reason());
		}
		else {
			GLenum internalFormat = 0, dataFormat = 0;

			if (channels == 4) {
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else {
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}

			texture->bind();
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);

			if (config.mipMapping) {
				// 如果先Generate在載入會有錯誤
				glGenerateMipmap(GL_TEXTURE_2D);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				// TODO: config manually
				if (glfwExtensionSupported("GL_EXT_texture_filter_anisotropic") && config.anisotropicFiltering) {
					float amount;
					glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &amount);
					amount = std::min(4.f, amount);
					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 0);
					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, amount);
				}
				else {
					glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, -0.4f);
#ifdef NS_DEBUG
					NS_CORE_INFO("Anisotropic not supported.");
#endif // NS_DEBUG
				}
			}


			texture->unbind();

			stbi_image_free(data);
		}

		return true;
	}

	bool Loader::loadTextureCube(Ref<Texture> texture, const std::vector<std::string>* filesPath, const TextureConfig config) {

		// check whether texture is textureCube
		if (texture->getType() != Texture::textureCube) {
			NS_CORE_WARN("This texture is not texture2d!");
			return false;
		}

		// variables
		int width, height, channels;

		// bind the texture object
		texture->bind(0);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		// load textures
		for (int i = 0; i < 6; i++) {
			if (i >= filesPath->size()) {
				NS_CORE_WARN("The texture has not enough!");
				break;
			}
			std::string filePath = filesPath->at(i);
			stbi_uc *data = stbi_load(filePath.c_str(), &width, &height, &channels, 0/* desired channel(?) */);
			if (!data) {
				NS_CORE_WARN("Failed to load texture:{0}\n{1}", filePath, stbi_failure_reason());
			}
			else {
				stbi_set_flip_vertically_on_load(false);
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			}
			stbi_image_free(data);
		}

		texture->unbind();

		return true;
	}

	void Loader::loadEmptyCube(Ref<Texture> texture, Uint32 size) {
		texture->bind();
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		for (short i = 0; i < 6; i++) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		}
		texture->unbind();
	}
}
