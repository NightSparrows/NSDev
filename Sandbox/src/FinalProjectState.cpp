
#include "FinalProjectState.h"
#include "PlayableCamera.h"
#include "testAnimatedRenderer/TestAnimatedRenderer.h"
#include "renderer/terrain/TerrainRenderer.h"
#include "renderer/skybox/SkyboxRenderer.h"
#include "renderer/shadow/ShadowRenderer.h"
#include "renderer/static/StaticRenderer.h"
#include "renderer/toonShading/ToonRenderer.h"
#include "renderer/envMapRenderer/EnvMapRenderer.h"


// Test import
#include <NS/ModelLoader.h>
#include <BasicRenderer.h>
#include <TestRenderer.h>
#include <DeferredShadingRenderer.h>
#include "Player.h"
#include "PlayerCamera.h"



extern ns::Ref<ns::Scene> scene;
extern ns::Ref<ns::Entity> entity;
extern ns::Ref<ns::Entity> entity2;
//extern ns::Ref<ns::AnimatedModel> model;
extern ns::Ref<ns::AnimatedModel> testModel;
extern ns::Ref<TestAnimatedRenderer> testAnimatedRenderer;
extern ns::Ref<ns::ModelAnimator> animator;
//extern ns::Ref<ns::ModelAnimator> animator2;

ns::Ref<Terrain> terrain;
ns::Ref<TerrainRenderer> terrainRenderer;

ns::Ref<Skybox> skybox;
ns::Ref<SkyboxRenderer> skyboxRenderer;

ns::Ref<StaticRenderer> staticRenderer;
ns::Ref<ns::Entity> barrelEntity;
ns::Ref<ns::Model> barrelModel;

ns::Ref<Player> player;
ns::Ref<PlayerCamera> playerCamera;

// for MSAA
ns::Ref<ns::FrameBuffer> msaaFbo;

// shadow
ns::Ref<ShadowRenderer> shadowRenderer;
ns::Ref<ns::Light> lightSun;

ns::Ref<EnvMapRenderer> envMapRenderer;

// shiny mario
ns::Ref<ns::Model> marioModel;
ns::Ref<ns::Entity> marioEntity;

// shakti
ns::Ref<ns::AnimatedModel> shaktiModel;
ns::Ref<ns::Entity> shaktiEntity;
ns::Ref<ns::ModelAnimator> shaktiAnimator;
ns::Ref<ToonRenderer> toonRenderer;

/*
// Testing UUID
uuids::uuid id = uuids::uuid_system_generator{}();

using namespace std::string_literals;
auto str = "47183823-2574-4bfd-b411-99ed177d3e43"s;
auto testID = uuids::uuid::from_string(str);

NS_CORE_INFO("Resource generated ID:{0}", id);
NS_CORE_INFO("Test ID:{0}", testID.value());

std::string str1 = uuids::to_string(id);

NS_CORE_INFO("str: {0}", str1);
*/

void HWState::init() {

	// Model
	//model = ns::CreateRef<ns::AnimatedModel>();

	testModel = ns::CreateRef<ns::AnimatedModel>();
	//ns::ModelLoader::loadAnimated(testModel, "res/Miku.pmx", "res/TDASakuraMiku");
	ns::ModelLoader::loadAnimated(testModel, "res/archer/dae/archer_walk.dae", "res/archer/textures");
	ns::Material *material;
	// archer
	material = new ns::Material();
	ns::Ref<ns::Texture> archerTexture = ns::Texture::Create(ns::Texture::texture2d);
	ns::Loader::loadTexture2D(archerTexture, "res/archer/textures/Archer.png");
	material->setDiffuse(archerTexture);
	testModel->addMaterial(material);
	testModel->getMeshes()[0]->setMaterial(material);
	// end archer
	//ns::ModelLoader::loadAnimated(testModel, "res/thinMatrixModel/model.dae", "res/thinMatrixModel");
	//ns::ModelLoader::loadAnimated(testModel, "res/Izayoi.pmx", "res/Izayoi");
	//ns::ModelLoader::loadAnimated(testModel, "res/pmxModels/TdaMikukimonolongHair.pmx", "res/pmxModels/TdaMikukimonolongHair");
	//ns::ModelLoader::loadStatic(testModel, "res/Izayoi.pmx", "res/Izayoi");
	//ns::ModelLoader::loadStatic(model, "res/SiriusBlue.pmx", "res");

	// Scene
	scene = ns::CreateRef<ns::Scene>();

	lightSun = ns::CreateRef<ns::Light>();
	lightSun->color = glm::vec3(1.2f, 1.2f, 1.2f);
	lightSun->position = glm::vec3(25000, 50000, 25000);
	lightSun->attenuation = glm::vec3(1, 0, 0);
	scene->lights.push_back(lightSun);

	ns::Ref<ns::Light> light = ns::CreateRef<ns::Light>();
	light->color = glm::vec3(1.f, 0, 0);
	light->position = glm::vec3(-210, 10, -215);
	light->attenuation = glm::vec3(1, 0.01, 0.002);
	scene->lights.push_back(light);
	
	light = ns::CreateRef<ns::Light>();
	light->color = glm::vec3(1.f, 1.f, 0);
	light->position = glm::vec3(-230, 10, -215);
	light->attenuation = glm::vec3(1, 0.01, 0.002);
	scene->lights.push_back(light);
	/*
	light = ns::CreateRef<ns::Light>();
	light->color = glm::vec3(1, 0, 0);
	light->position = glm::vec3(0, 0, 0);
	light->attenuation = glm::vec3(1, 0, 0);
	scene->lights.push_back(light);
	*/

	// Entity
	entity = ns::CreateRef<ns::Entity>();
	entity->transform.position.z = -225;
	entity->transform.position.x = -180;
	entity->transform.position.y = 10.f;
	entity->transform.rotation.x = -90.f;
	//entity->transform.rotation.x = glm::radians(-90.0f);

	entity2 = ns::CreateRef<ns::Entity>();
	entity2->transform.position.z = -25;
	entity2->transform.position.x = -20;

	// testAnimatedRenderer
	testAnimatedRenderer = ns::CreateRef<TestAnimatedRenderer>();
	ns::MasterRenderer::Add(testAnimatedRenderer);

	// Animator
	animator = ns::CreateRef<ns::ModelAnimator>(testModel);
	ns::Ref<ns::ModelAnimation> animation = ns::CreateRef<ns::ModelAnimation>();
	ns::ModelLoader::loadModelAnimation(animation, "res/archer/dae/archer_hit.dae");
	animator->addAnimation("walk", animation);
	animator->play("walk");
	
	// terrain
	terrain = ns::CreateRef<Terrain>(-1, -1);
	ns::Loader::loadTexture2D(terrain->getBlendMap(), "res/terrain/blendMap.png");
	ns::Loader::loadTexture2D(terrain->getrTexture(), "res/terrain/r.png");
	ns::Loader::loadTexture2D(terrain->getgTexture(), "res/terrain/g.png");
	ns::Loader::loadTexture2D(terrain->getbTexture(), "res/terrain/b.png");
	ns::Loader::loadTexture2D(terrain->getbgTexture(), "res/terrain/back.png");
	terrainRenderer = ns::CreateRef<TerrainRenderer>();
	ns::MasterRenderer::Add(terrainRenderer);

	// skybox
	skybox = ns::CreateRef<Skybox>();
	std::vector<std::string> skyboxTexturePath;
	skyboxTexturePath.push_back("res/skybox/right.png");
	skyboxTexturePath.push_back("res/skybox/left.png");
	skyboxTexturePath.push_back("res/skybox/top.jpg");
	skyboxTexturePath.push_back("res/skybox/bottom.png");
	skyboxTexturePath.push_back("res/skybox/back.png");
	skyboxTexturePath.push_back("res/skybox/front.png");
	skybox->loadTexture(&skyboxTexturePath);
	skyboxRenderer = ns::CreateRef<SkyboxRenderer>();
	skyboxRenderer->setSkybox(skybox);
	ns::MasterRenderer::Add(skyboxRenderer);


	// Player
	player = ns::CreateRef<Player>();

	playerCamera = ns::CreateRef<PlayerCamera>(glm::ivec2(ns::GameEngine::GetWindow()->getWidth(), ns::GameEngine::GetWindow()->getHeight()), player.get());
	
	// MSAA Frame buffer
	msaaFbo = ns::FrameBuffer::Create(ns::FrameBuffer::Type::MultiSamples, ns::GameEngine::GetWindow()->getWidth(), ns::GameEngine::GetWindow()->getHeight(), 1);

	// shadow
	shadowRenderer = ns::CreateRef<ShadowRenderer>(playerCamera.get(), 8192);
	shadowRenderer->init();

	// static (with normal mapping
	staticRenderer = ns::CreateRef<StaticRenderer>();
	ns::MasterRenderer::Add(staticRenderer);

	// barrel with normal mapping
	barrelModel = ns::CreateRef<ns::Model>();
	ns::ModelLoader::loadStatic(barrelModel, "res/barrel.obj");
	material = new ns::Material();
	ns::Ref<ns::Texture> barrelTexture = ns::Texture::Create(ns::Texture::texture2d);
	ns::Loader::loadTexture2D(barrelTexture, "res/barrel.png");
	material->setDiffuse(barrelTexture);
	barrelTexture = ns::Texture::Create(ns::Texture::texture2d);
	ns::Loader::loadTexture2D(barrelTexture, "res/barrelNormal.png");
	material->setNormalTexture(barrelTexture);
	barrelTexture = ns::Texture::Create(ns::Texture::texture2d);
	ns::Loader::loadTexture2D(barrelTexture, "res/barrelS.png");
	material->setExtraInfoTexture(barrelTexture);
	barrelModel->addMaterial(material);
	barrelModel->getMeshes()[0]->setMaterial(material);
	// barrel entity
	barrelEntity = ns::CreateRef<ns::Entity>();
	barrelEntity->transform.position.x = -220.f;
	barrelEntity->transform.position.z = -225.f;
	barrelEntity->transform.position.y = 5.f;
	
	// env map renderer
	// it should not in the scene process or maybe other method
	envMapRenderer = ns::CreateRef<EnvMapRenderer>();
	envMapRenderer->init();


	// shiny mario
	marioModel = ns::CreateRef<ns::Model>();
	marioEntity = ns::CreateRef<ns::Entity>();
	marioEntity->transform.position.x = -200;
	marioEntity->transform.position.z = -225;
	// cube
	//marioEntity->transform.position.y = 10.f;
	//marioEntity->transform.scale *= 10.f;
	// sonic
	marioEntity->transform.scale *= 0.8f;
	ns::ModelLoader::loadStatic(marioModel, "res/sonic.obj");

	// shakti
	shaktiModel = ns::CreateRef<ns::AnimatedModel>();
	ns::ModelLoader::loadAnimated(shaktiModel, "res/shakti/shakti.pmx", "res/shakti");
	shaktiEntity = ns::CreateRef<ns::Entity>();
	shaktiEntity->transform.position.x = -50;
	shaktiEntity->transform.position.z = -50;
	shaktiAnimator = ns::CreateRef<ns::ModelAnimator>(shaktiModel);
	toonRenderer = ns::CreateRef<ToonRenderer>();
	ns::MasterRenderer::Add(toonRenderer);
}

void HWState::onUpdate(ns::Time delta) {

	//camera->update(delta);

	// animator
	animator->update(delta);
	//animator2->update(delta);
	entity->transform.rotation.y += 20.f * delta;

	player->update(delta);
	playerCamera->update(delta);
	barrelEntity->transform.rotation.y += 20.f * delta;
}

void HWState::onEvent(ns::Event & event) {
	switch (event.type)
	{
	case ns::Event::WindowClose:
		ns::GameEngine::Stop();
		break;
	case ns::Event::KeyReleased:
	{
		ns::KeyReleasedEvent *keyEvent = static_cast<ns::KeyReleasedEvent*>(&event);
		if (keyEvent->key == ns::Key::G) {
			ns::Mouse::Grab(ns::Mouse::IsGrab() ? false : true);
		}
		break;
	}
	case ns::Event::WindowResize:
	{
		break;
	}
	default:
		break;
	}
	player->event(&event);
	playerCamera->event(&event);
}

void HWState::onRender() {

	// camera render calc projection matrix
	//camera->render();

	player->render(playerCamera.get());

	// shadow
	shadowRenderer->processAnimatedEntity(player.get(), player->getAnimator());
	shadowRenderer->processAnimatedEntity(entity.get(), animator);
	shadowRenderer->processEntity(barrelEntity.get(), barrelModel.get());
	shadowRenderer->processEntity(marioEntity.get(), marioModel.get());

	// shakti
	shadowRenderer->processAnimatedEntity(shaktiEntity.get(), shaktiAnimator);

	shadowRenderer->render(lightSun.get());
	terrainRenderer->setShadowMap(shadowRenderer->getToShadowMapSpaceMatrix(), shadowRenderer->getShadowMapTexture());
	staticRenderer->setShadowMap(shadowRenderer->getToShadowMapSpaceMatrix(), shadowRenderer->getShadowMapTexture());
	envMapRenderer->setShadowMap(shadowRenderer->getToShadowMapSpaceMatrix(), shadowRenderer->getShadowMapTexture());
	toonRenderer->setShadowMap(shadowRenderer->getToShadowMapSpaceMatrix(), shadowRenderer->getShadowMapTexture());
	// end shadow

	testAnimatedRenderer->addEntity(entity.get(), animator);
	//testAnimatedRenderer->addEntity(entity2, animator2);

	terrainRenderer->processTerrain(terrain);
	playerCamera->render();

	// normal mapping barrel
	staticRenderer->processEntity(barrelModel.get(), barrelEntity.get());

	// shiny mario
	envMapRenderer->processEntity(marioModel.get(), marioEntity.get());
	envMapRenderer->setCurrentCamera(playerCamera->getPosition());
	envMapRenderer->renderMap(scene);
	
	// shakti
	toonRenderer->addEntity(shaktiEntity.get(), shaktiAnimator);


	// for MSAA
	msaaFbo->bind();
	ns::MasterRenderer::PrepareScene(playerCamera, scene);
	ns::MasterRenderer::RenderScene();
	envMapRenderer->render();
	msaaFbo->unbind();

	msaaFbo->resolveToScreen(0);
	
	//defferedRenderer->render(scene);

	/*
	shader->bind();
	shader->getUniform("transformationMatrix")->load(entity->transform.matrix());
	testRenderer->renderTest(model);
	shader->unbind();
	*/

	/*
	guiLayer->BeginFrame();
	guiLayer->renderGUI();
	guiLayer->EndFrame();
	*/
}

void HWState::cleanUp() {

	// shakti
	shaktiModel.reset();
	shaktiEntity.reset();
	shaktiAnimator.reset();


	envMapRenderer->cleanUp();
	envMapRenderer.reset();

	shadowRenderer->cleanUp();
	shadowRenderer.reset();
	msaaFbo.reset();

	//model.reset();
	testModel.reset();
	// Reset count - 1�A���o�ӫK�ū���
	entity.reset();
	scene.reset();

	// Test animated renderer
	testAnimatedRenderer = nullptr;

	animator.reset();

}
