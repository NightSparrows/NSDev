#pragma once

#include <map>

#include "../Model.h"
#include "armature/ModelBone.h"

namespace ns {

	class NS_API AnimatedModel : public Model {
	public:
		~AnimatedModel();

		// The bone offset relative to mesh
		void addBoneOffset(Uint32 offset, Uint32 count);

		const std::map<Uint32, Uint32> *getBoneOffsets();

		void addBone(ModelBone *bone);

		const std::vector<ModelBone*> *getBones();

		// return NULL when not found
		ModelBone *findBoneByName(std::string &name);

		void setRootBone(ModelBone *rootBone);

		ModelBone *getRootBone();

	protected:
		ModelBone *m_rootBone;
		std::vector<ModelBone*> m_bones;
		// offset, count
		std::map<Uint32, Uint32> m_boneOffsets;
	};


}
