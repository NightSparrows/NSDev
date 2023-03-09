#pragma once

#include <map>

#include "AnimatedModel.h"
#include "ModelAnimation.h"

#define MAX_BONES 50;

namespace ns {

	// run and store the model pose transforms matrices
	// NOT store in model!
	class NS_API ModelAnimator {
	public:
		ModelAnimator(Ref<AnimatedModel> model);
		~ModelAnimator();

		// The update method to update the bone transform
		void update(float delta);

		const Ref<AnimatedModel> getModel();

		const glm::mat4 *getBoneTransformMatrix();

		void addAnimation(std::string name,Ref<ns::ModelAnimation> animation);

		void play();

		void play(std::string animationName);

		void pause();

		bool isPlaying() { this->m_isPlaying; }

		void setBacktrack(bool backtrack);

	protected:

		// °j°é­pºâworld transform
		void applyPoseToBones(std::map<std::string, glm::mat4> *currentPose, ModelBone *bone, glm::mat4 parentTransform);

	private:
		Ref<AnimatedModel> m_model;
		std::map<std::string, Ref<ModelAnimation>> m_animations;
		glm::mat4 *m_boneTransformationMatrix;				// matrix array

		Ref<ModelAnimation> m_currentAnimation;
		float m_time;			// the time that is playing at
		bool m_isPlaying;
		bool m_looping;
		bool m_backtrack;

	};

}
