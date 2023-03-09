#pragma once

#include <string>

#include "../../../../Base.h"
#include "../../../../deps/glm/glm.hpp"
#include "../../../../deps/glm/gtc/quaternion.hpp"

namespace ns {

	class NS_API ModelBone {
	public:
		ModelBone(Uint32 id, const std::string &name, glm::vec3 position, glm::quat rotation, glm::vec3 scale);

		///  the offset relative to world and the vector is point to origin, 
		/// in the other word, the transformation is inverse relative to pose transformation matrix
		glm::mat4 getOffsetMatrix();

		Uint32 getId();

		const char *getName();

		glm::vec3 getPosition();
		glm::quat getRotation();
		glm::vec3 getScale();

		void setPosition(glm::vec3 position);
		void setRotation(glm::quat rotation);
		void setScale(glm::vec3 scale);

		ModelBone *getParent();

		const std::vector<ModelBone*> *getChildren();

		void addChild(ModelBone *child);

		void setParent(ModelBone *parent);

		// set the m_inMesh flags to true : my loader need to do the job
		void inMesh();

		bool hasParent();
		// Whether it is effect the vertices
		bool isInMesh();

	private:
		bool m_hasParent;
		ModelBone *m_parent;
		std::vector<ModelBone*> m_children;
		bool m_inMesh;

		Uint32 m_id;
		std::string m_name;
		glm::vec3 m_position;
		glm::quat m_rotation;
		glm::vec3 m_scale;

	};

}
