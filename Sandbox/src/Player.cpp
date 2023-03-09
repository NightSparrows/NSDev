
#include "Player.h"
#include <NS/ModelLoader.h>
#include "PlayerCamera.h"

Player::Player() : m_groundVelocity(0), m_isOnGround(true) {
	this->transform.position.x = -200.f;
	this->transform.position.z = -200.f;
	this->m_model = ns::CreateRef<ns::AnimatedModel>();
	ns::ModelLoader::loadAnimated(this->m_model, "res/archer/dae/archer_walk.dae", "res/archer/textures");

	this->transform.position.y += 10.f;

	// archer
	ns::Material *material = new ns::Material();
	ns::Ref<ns::Texture> archerTexture = ns::Texture::Create(ns::Texture::texture2d);
	ns::Loader::loadTexture2D(archerTexture, "res/archer/textures/Archer.png");
	material->setDiffuse(archerTexture);
	this->m_model->addMaterial(material);
	this->m_model->getMeshes()[0]->setMaterial(material);
	this->transform.rotation.x = -90.f;		// ®Õ¥¿
	// renderer getting
	this->m_renderer = std::static_pointer_cast<TestAnimatedRenderer>(ns::MasterRenderer::Get("testAnimated"));
	this->m_animator = ns::CreateRef<ns::ModelAnimator>(this->m_model);
	ns::Ref<ns::ModelAnimation> animation = ns::CreateRef<ns::ModelAnimation>();
	ns::ModelLoader::loadModelAnimation(animation, "res/archer/dae/archer_walk.dae");
	this->m_animator->addAnimation("walk", animation);
	animation = ns::CreateRef<ns::ModelAnimation>();
	ns::ModelLoader::loadModelAnimation(animation, "res/archer/dae/archer_idle.dae");
	this->m_animator->addAnimation("idle", animation);
	this->m_animator->play("idle");

	this->m_status = Status::idle;
}

void Player::event(ns::Event* event) {

	if (event->type == ns::Event::MouseMoved && ns::Mouse::IsGrab()) {
		this->transform.rotation.y -= ns::Mouse::GetDX() * 0.1f;
	}

}

void Player::update(float delta) {

	this->m_currentSpeed = 0;
	this->m_sideSpeed = 0;
	if (ns::Keyboard::isKeyDown(ns::Key::W)) {
		this->m_currentSpeed += this->m_walkSpeed;
		this->m_animator->setBacktrack(false);
	}
	if (ns::Keyboard::isKeyDown(ns::Key::S)) {
		this->m_currentSpeed -= this->m_walkSpeed;
		this->m_animator->setBacktrack(true);
	}
	if (ns::Keyboard::isKeyDown(ns::Key::A)) {
		this->m_sideSpeed += this->m_walkSpeed;
	}
	if (ns::Keyboard::isKeyDown(ns::Key::D)) {
		this->m_sideSpeed -= this->m_walkSpeed;
	}
	if (ns::Keyboard::isKeyDown(ns::Key::Space)) {
		if (this->m_isOnGround) {
			this->m_groundVelocity = 30.f;
			this->m_isOnGround = false;
		}
	}

	this->m_groundVelocity -= 50.f * delta;
	this->transform.position.y += this->m_groundVelocity * delta;
	if (this->transform.position.y < 10.5f) {		// on ground
		this->m_isOnGround = true;
		this->transform.position.y = 10.5f;
	}

	if (this->m_currentSpeed == 0 && this->m_sideSpeed == 0) {
		if (this->m_status != Status::idle) {
			this->m_animator->play("idle");
		}
		this->m_status = Status::idle;
	}
	else {
		if (this->m_status != Status::Walk) {
			this->m_animator->play("walk");
		}
		this->m_status = Status::Walk;
	}

	// modify moving
	float distance = this->m_currentSpeed * delta;
	float dx = distance * glm::sin(glm::radians(this->transform.rotation.y));
	float dz = distance * glm::cos(glm::radians(this->transform.rotation.y));

	distance = this->m_sideSpeed * delta;
	dx -= distance * glm::sin(glm::radians(this->transform.rotation.y - 90.0f));
	dz -= distance * glm::cos(glm::radians(this->transform.rotation.y - 90.0f));

	this->transform.position.x += dx;
	this->transform.position.z += dz;

	// animation
	this->m_animator->update(delta);
}


void Player::render(PlayerCamera *camera) {
	if (!camera->isFirstPersonCamera()) {
		this->m_renderer->addEntity((ns::Entity*)this, this->m_animator);
	}
}

ns::Ref<ns::ModelAnimator> Player::getAnimator() {
	return this->m_animator;
}