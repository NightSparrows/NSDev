
#include <NS/core/graphics/model/animated/armature/ModelBone.h>
#include <NS/core/deps/glm/gtx/quaternion.hpp>

namespace ns {

	ModelBone::ModelBone(Uint32 id, const std::string &name, glm::vec3 position, glm::quat rotation, glm::vec3 scale) {
		this->m_id = id;
		this->m_name = name;
		this->m_position = position;
		this->m_rotation = rotation;
		this->m_scale = scale;
		this->m_hasParent = false;
		this->m_inMesh = false;
	}

	glm::mat4 ModelBone::getOffsetMatrix() {
		glm::mat4 matrix = glm::mat4(1);
		matrix = glm::translate(matrix, this->m_position);
		matrix = matrix * glm::toMat4(this->m_rotation);
		matrix = glm::scale(matrix, this->m_scale);
		return matrix;
	}

	Uint32 ModelBone::getId() {
		return this->m_id;
	}

	const char *ModelBone::getName() {
		return this->m_name.c_str();
	}

	glm::vec3 ModelBone::getPosition() {
		return this->m_position;
	}

	glm::quat ModelBone::getRotation() {
		return this->m_rotation;
	}

	glm::vec3 ModelBone::getScale() {
		return this->m_scale;
	}

	void ModelBone::setPosition(glm::vec3 position) {
		this->m_position = position;
	}

	void ModelBone::setRotation(glm::quat rotation) {
		this->m_rotation = rotation;
	}
	void ModelBone::setScale(glm::vec3 scale) {
		this->m_scale = scale;
	}

	ModelBone *ModelBone::getParent() {
		return this->m_parent;
	}

	const std::vector <ModelBone*> *ModelBone::getChildren() {
		return &this->m_children;
	}

	void ModelBone::addChild(ModelBone *child) {
		this->m_children.push_back(child);
	}

	void ModelBone::setParent(ModelBone *parent) {
		this->m_hasParent = true;
		this->m_parent = parent;
	}

	bool ModelBone::hasParent() {
		return this->m_hasParent;
	}

	void ModelBone::inMesh() {
		this->m_inMesh = true;
	}

	bool ModelBone::isInMesh() {
		return this->m_inMesh;
	}
}
