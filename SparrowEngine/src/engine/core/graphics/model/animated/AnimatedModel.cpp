
#include <NS/core/Log.h>

#include <NS/core/graphics/model/animated/AnimatedModel.h>

namespace ns {

	AnimatedModel::~AnimatedModel() {
		for (ModelBone *bone : this->m_bones) {
			delete bone;
		}
		this->m_bones.clear();
	}

	void AnimatedModel::addBoneOffset(Uint32 offset, Uint32 count) {
		this->m_boneOffsets.emplace(std::pair<Uint32, Uint32>(offset, count));
	}

	const std::map<Uint32, Uint32> *AnimatedModel::getBoneOffsets() {
		return &this->m_boneOffsets;
	}

	void AnimatedModel::addBone(ModelBone *bone) {
		this->m_bones.push_back(bone);
	}

	const std::vector<ModelBone*> *AnimatedModel::getBones() {
		return &this->m_bones;
	}

	void AnimatedModel::setRootBone(ModelBone *rootBone) {
		this->m_rootBone = rootBone;
	}

	ModelBone *AnimatedModel::getRootBone() {
		return this->m_rootBone;
	}

	ModelBone *AnimatedModel::findBoneByName(std::string &name) {
		for (ModelBone *bone : this->m_bones) {
			if (name.compare(bone->getName()) == 0) {
				return bone;
			}
		}

		return NULL;
	}
}
