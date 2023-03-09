
#include <NS/core/Log.h>
#include <NS/core/graphics/model/animated/ModelAnimator.h>

namespace ns {

	ModelAnimator::ModelAnimator(Ref<AnimatedModel> model) {
		this->m_model = model;
		this->m_isPlaying = false;
		this->m_looping = true;
		this->m_backtrack = false;
		this->m_time = 0;
		this->m_currentAnimation = NULL;
		size_t size = model->getBones()->size();
		this->m_boneTransformationMatrix = new glm::mat4[size];
		for (int i = 0; i < size; i++) {
			this->m_boneTransformationMatrix[i] = glm::mat4(1);
		}
	}

	ModelAnimator::~ModelAnimator() {
		delete this->m_boneTransformationMatrix;
		this->m_model = nullptr;
	}

	void ModelAnimator::update(float delta) {
		if (!m_isPlaying)
			return;

		// incrase time
		if (this->m_backtrack) {
			this->m_time -= delta;

			if (this->m_time <= 0) {
				if (!this->m_looping) {
					this->m_isPlaying = false;
					return;
				}
				this->m_time = this->m_currentAnimation->getDuration() + this->m_time;
				
			}
		}
		else {
			this->m_time += delta;

			if (this->m_time >= this->m_currentAnimation->getDuration()) {
				if (!this->m_looping) {
					this->m_isPlaying = false;
					return;
				}
				else {
					this->m_time = this->m_time - ((this->m_time / this->m_currentAnimation->getDuration()) * this->m_currentAnimation->getDuration());
				}
			}
		}

		// calculate Current animation Pose
		const std::vector<Ref<KeyFrame>> *allFrames = this->m_currentAnimation->getFrames();
		Ref<KeyFrame> previousFrame = (*allFrames)[0];
		Ref<KeyFrame> nextFrame = (*allFrames)[0];
		for (Uint32 i = 1; i < allFrames->size(); i++) {
			nextFrame = (*allFrames)[i];
			if (nextFrame->getTimeStamp() > this->m_time)
				break;
			previousFrame = (*allFrames)[i];
		}
		float totalTime = nextFrame->getTimeStamp() - previousFrame->getTimeStamp();
		float currentTime = this->m_time - previousFrame->getTimeStamp();
		float progression = currentTime / totalTime; 

		// current pose in this time
		// bone name, transformation matrix
		std::map<std::string, glm::mat4> currentPose;

		// interpolate pose
		auto pose = previousFrame->getPose();
		for (const auto &it : *pose) {
			std::string boneName = it.first;
			Ref<BoneTransform> previousTransform = previousFrame->getPose()->find(boneName)->second;
			Ref<BoneTransform> nextTransform = nextFrame->getPose()->find(boneName)->second;
			Ref<BoneTransform> currentTransform = BoneTransform::interpolate(previousTransform, nextTransform, progression);
			currentPose.insert(std::pair<std::string, glm::mat4>(boneName, currentTransform->getLocalTransform()));
		}

		// apply to matrices
		/*
		for (ModelBone *bone : *this->m_model->getBones()) {
			if (!bone->hasParent()) {
				applyPoseToBones(&currentPose, bone, glm::mat4(1));
			}
		}
		*/

		// origin method
		applyPoseToBones(&currentPose, this->m_model->getRootBone(), glm::mat4(1));
	}

	void ModelAnimator::applyPoseToBones(std::map<std::string, glm::mat4> *currentPose, ModelBone *bone, glm::mat4 parentTransform) {
		auto it = currentPose->find(bone->getName());
		glm::mat4 currentLocalTransform;
		if (it == currentPose->end()) {
			currentLocalTransform = glm::mat4(1);
		}
		else {
			currentLocalTransform = it->second;
		}
		glm::mat4 globalTransform = parentTransform * currentLocalTransform;
		for (ModelBone *childBone : *bone->getChildren()) {
			applyPoseToBones(currentPose, childBone, globalTransform);
		}
		// Just throw the data into the buffer that will be loaded.
		this->m_boneTransformationMatrix[bone->getId()] = globalTransform * bone->getOffsetMatrix();
	}

	void ModelAnimator::play() {
		if (this->m_currentAnimation == NULL) {
#ifdef NS_DEBUG
			NS_CORE_WARN("No current animation");
#endif
			return;
		}
		this->m_isPlaying = true;
	}

	void ModelAnimator::addAnimation(std::string name, Ref<ns::ModelAnimation> animation) {
		this->m_animations.emplace(std::pair< std::string, Ref<ns::ModelAnimation>>(name, animation));
	}

	void ModelAnimator::play(std::string animationName) {
		auto it = this->m_animations.find(animationName);
		if (it != this->m_animations.end()) {
			this->m_currentAnimation = it->second;
			this->m_isPlaying = true;
		}
		else {
			NS_CORE_WARN("No animation found!");
		}
	}

	void ModelAnimator::pause() {
		this->m_isPlaying = false;
	}

	void ModelAnimator::setBacktrack(bool backtrack) {
		this->m_backtrack = backtrack;
	}

	const glm::mat4 *ModelAnimator::getBoneTransformMatrix() {
		return this->m_boneTransformationMatrix; 
	}

	const Ref<AnimatedModel> ModelAnimator::getModel() {
		return this->m_model;
	}

}
