
#include "TerrainRenderer.h"

TerrainRenderer::TerrainRenderer() : ns::Renderer("terrain") {

}

void TerrainRenderer::init() {
	
	// shader initialization
	this->m_shader = ns::Shader::Create("terrain");
	this->m_shader->addShaderFromFile("res/shader/terrain/shader.vs", ns::Shader::Vertex);
	this->m_shader->addShaderFromFile("res/shader/terrain/shader.fs", ns::Shader::Fragment);
	this->m_shader->bindFragOutput(0, "out_Color");
	this->m_shader->bindAttribute(0, "in_position");
	this->m_shader->bindAttribute(1, "in_texCoord");
	this->m_shader->bindAttribute(2, "in_normal");
	this->m_shader->link();
	this->m_transformationMatrix = this->m_shader->createUniform("u_transformationMatrix");
	this->m_uniformToShadowMapSpace = this->m_shader->createUniform("u_toShadowMapSpace");
	ns::Uniform *shadowMap = this->m_shader->createUniform("shadowMap");
	ns::Uniform* blendMap = this->m_shader->createUniform("blendMap");
	ns::Uniform* rTexture = this->m_shader->createUniform("rTexture");
	ns::Uniform* gTexture = this->m_shader->createUniform("gTexture");
	ns::Uniform* bTexture = this->m_shader->createUniform("bTexture");
	ns::Uniform* bgTexture = this->m_shader->createUniform("bgTexture");
	this->m_shader->validate();
	this->m_shader->bind();
	blendMap->load(0);
	rTexture->load(1);
	gTexture->load(2);
	bTexture->load(3);
	bgTexture->load(4);
	shadowMap->load(5);
	this->m_shader->bindUniformBlock("engineUbo", NSENGINE_UNIFORM_BLOCK_UNIT);	// the engine scene uniform buffer
	this->m_shader->unbind();

	// the standard model
	this->m_model = ns::VertexArray::Create();
	int count = this->m_vertexCount * this->m_vertexCount;
	int sizePerVertexInByte = 8 * sizeof(float);
	ns::Scope<std::vector<char>> verticesData = ns::CreateScope<std::vector<char>>(sizePerVertexInByte * count);
	ns::Scope<std::vector<int>> indices = ns::CreateScope<std::vector<int>>(6 * (this->m_vertexCount - 1)*(this->m_vertexCount - 1));
	int vertexPointer = 0;
	for (int i = 0; i < this->m_vertexCount; i++) {
		for (int j = 0; j < this->m_vertexCount; j++) {
			float terrainVX = (float)j / ((float)this->m_vertexCount - 1) * this->m_size;
			float terrainVY = 0;
			float terrainVZ = (float)i / ((float)this->m_vertexCount - 1) * this->m_size;
			float terrainNX = 0;
			float terrainNY = 1;
			float terrainNZ = 0;
			float terrainTX = (float)j / ((float)this->m_vertexCount - 1);	// U
			float terrainTY = (float)i / ((float)this->m_vertexCount - 1);	// V
			memcpy(&(*verticesData)[vertexPointer * sizePerVertexInByte + 0], &terrainVX, sizeof(float));
			memcpy(&(*verticesData)[vertexPointer * sizePerVertexInByte + 4], &terrainVY, sizeof(float));
			memcpy(&(*verticesData)[vertexPointer * sizePerVertexInByte + 8], &terrainVZ, sizeof(float));

			memcpy(&(*verticesData)[vertexPointer * sizePerVertexInByte + 12], &terrainTX, sizeof(float));
			memcpy(&(*verticesData)[vertexPointer * sizePerVertexInByte + 16], &terrainTY, sizeof(float));

			memcpy(&(*verticesData)[vertexPointer * sizePerVertexInByte + 20], &terrainNX, sizeof(float));
			memcpy(&(*verticesData)[vertexPointer * sizePerVertexInByte + 24], &terrainNY, sizeof(float));
			memcpy(&(*verticesData)[vertexPointer * sizePerVertexInByte + 28], &terrainNZ, sizeof(float));
			vertexPointer++;
		}
	}
	int pointer = 0;
	for (int gz = 0; gz < this->m_vertexCount - 1; gz++) {
		for (int gx = 0; gx < this->m_vertexCount - 1; gx++) {
			int topLeft = (gz*this->m_vertexCount) + gx;
			int topRight = topLeft + 1;
			int bottomLeft = ((gz + 1)*this->m_vertexCount) + gx;
			int bottomRight = bottomLeft + 1;
			(*indices)[pointer++] = topLeft;
			(*indices)[pointer++] = bottomLeft;
			(*indices)[pointer++] = topRight;
			(*indices)[pointer++] = topRight;
			(*indices)[pointer++] = bottomLeft;
			(*indices)[pointer++] = bottomRight;
		}
	}
	this->m_model->bind();
	this->m_model->getIndicesBuffer()->bind();
	this->m_model->getIndicesBuffer()->storeData(&(*indices)[0], (uint32_t)indices->size() * sizeof(unsigned int), ns::Buffer::StaticDraw);

	this->m_model->getDataBuffer()->bind();
	this->m_model->getDataBuffer()->storeData(&(*verticesData)[0], (uint32_t)verticesData->size(), ns::Buffer::StaticDraw);
	ns::RenderAPI::AttributeFloatPointer(0, 3, false, sizePerVertexInByte, 0);
	ns::RenderAPI::AttributeFloatPointer(1, 2, false, sizePerVertexInByte, (void*)(3 * sizeof(float)));
	ns::RenderAPI::AttributeFloatPointer(2, 3, false, sizePerVertexInByte, (void*)(5 * sizeof(float)));
	this->m_model->getDataBuffer()->unbind();
	this->m_model->unbind();

}

void TerrainRenderer::render() {

	ns::RenderAPI::DepthTest(true);
	this->m_shader->bind();

	// shadow matrix
	this->m_uniformToShadowMapSpace->load(this->m_shadowMapMatrix);
	this->m_shadowMapTexture->bind(5);

	this->m_model->bind();
	ns::RenderAPI::EnableVertexAttributes(3);

	// render stuff
	glm::mat4 transformationMatrix;
	for (ns::Ref<Terrain> terrain : this->m_entities) {
		terrain->bind();
		transformationMatrix = glm::mat4(1.0f);
		transformationMatrix = glm::translate(transformationMatrix, glm::vec3(this->m_size * (float)terrain->getX(), 0, this->m_size * (float)terrain->getZ()));
		this->m_transformationMatrix->load(transformationMatrix);
		ns::RenderAPI::DrawIndexed(6 * (this->m_vertexCount - 1)*(this->m_vertexCount - 1), 0);
	}

	ns::RenderAPI::DisableVertexAttributes(3);
	this->m_model->unbind();
	this->m_shader->unbind();
}

void TerrainRenderer::setShadowMap(glm::mat4 shadowMapSpaceMatrix, ns::Ref<ns::Texture> shadowMapTexture) {
	this->m_shadowMapMatrix = shadowMapSpaceMatrix;
	this->m_shadowMapTexture = shadowMapTexture;
}

void TerrainRenderer::processTerrain(ns::Ref<Terrain> terrain) {
	this->m_entities.push_back(terrain);
}

void TerrainRenderer::clearBuffer() {
	this->m_entities.clear();
}

void TerrainRenderer::cleanUp() {
	this->m_model.reset();
	this->m_shader.reset();
}
