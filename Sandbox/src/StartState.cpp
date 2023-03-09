
#include <NS/SparrowEngine.h>
#include "StartState.h"
#include "PlayableCamera.h"
#include "testAnimatedRenderer/TestAnimatedRenderer.h"

// Test import
#include <NS/ModelLoader.h>
#include <BasicRenderer.h>
#include <TestRenderer.h>
#include <DeferredShadingRenderer.h>


ns::Ref<ns::PlayableCamera> camera;
ns::Ref<ns::Scene> scene;
ns::Ref<ns::Entity> entity;
ns::Ref<ns::Entity> entity2;
ns::Ref<ns::AnimatedModel> model;
ns::Ref<ns::AnimatedModel> testModel;
ns::Ref<TestAnimatedRenderer> testAnimatedRenderer;
ns::Ref<ns::ModelAnimator> animator;
ns::Ref<ns::ModelAnimator> animator2;

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

void StartState::init() {

	// Model
	model = ns::CreateRef<ns::AnimatedModel>();

	ns::ModelLoader::loadAnimated(model, "res/Miku.pmx", "res/TDASakuraMiku");

	testModel = ns::CreateRef<ns::AnimatedModel>();
	ns::ModelLoader::loadAnimated(testModel, "res/thinMatrixModel/model.dae", "res/thinMatrixModel");
	//ns::ModelLoader::loadAnimated(testModel, "res/Izayoi.pmx", "res/Izayoi");
	//ns::ModelLoader::loadAnimated(testModel, "res/pmxModels/TdaMikukimonolongHair.pmx", "res/pmxModels/TdaMikukimonolongHair");
	//ns::ModelLoader::loadStatic(testModel, "res/Izayoi.pmx", "res/Izayoi");
	//ns::ModelLoader::loadStatic(model, "res/SiriusBlue.pmx", "res");

	// Camera
	camera = ns::CreateRef<ns::PlayableCamera>(glm::ivec2(ns::GameEngine::GetWindow()->getWidth(), ns::GameEngine::GetWindow()->getHeight()));
	
	// Scene
	scene = ns::CreateRef<ns::Scene>();

	ns::Ref<ns::Light> light = ns::CreateRef<ns::Light>();
	light->color = glm::vec3(1.8f, 1.8f, 1.8f);
	light->position = glm::vec3(0, 100, -25);
	light->attenuation = glm::vec3(1, 0, 0);
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
	entity->transform.position.z = -25;
	entity->transform.rotation.x = glm::radians(-90.f);
	//entity->transform.rotation.x = glm::radians(-90.0f);

	entity2 = ns::CreateRef<ns::Entity>();
	entity2->transform.position.z = -25;
	entity2->transform.position.x = -20;

	// testAnimatedRenderer
	testAnimatedRenderer = ns::CreateRef<TestAnimatedRenderer>();
	//ns::MasterRenderer::Add(testAnimatedRenderer);
	ns::MasterRenderer::Add(testAnimatedRenderer);

	// Animator
	animator = ns::CreateRef<ns::ModelAnimator>(testModel);
	ns::Ref<ns::ModelAnimation> animation = ns::CreateRef<ns::ModelAnimation>();
	ns::ModelLoader::loadModelAnimation(animation, "res/thinMatrixModel/model.dae");
	//ns::ModelLoader::loadModelAnimation(animation, "res/walk.vmd");
	animator->addAnimation("walk", animation);
	animator->play("walk");

	animator2 = ns::CreateRef<ns::ModelAnimator>(model);
	
}

void StartState::onUpdate(ns::Time delta) {

	camera->update(delta);

	// animator
	animator->update(delta);
	entity->transform.rotation.y += glm::radians(20.f) * delta;
}

void StartState::onEvent(ns::Event & event) {
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
}

void StartState::onRender() {

	// camera render calc projection matrix
	camera->render();

	// batching
	//renderer->addEntity(entity, model);
	/*
	renderer->addEntity(entity, testModel);
	renderer->addEntity(entity2, model);
	*/
	//testAnimatedRenderer->addEntity(entity, animator);
	//testAnimatedRenderer->addEntity(entity2, animator2);

	ns::MasterRenderer::PrepareScene(camera, scene);
	ns::MasterRenderer::RenderScene();
	
	
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

void StartState::cleanUp() {

	model.reset();
	testModel.reset();
	// Reset 試紙手動釋放記憶體，非count - 1
	camera.reset();
	entity.reset();
	scene.reset();
	
	// Test animated renderer
	testAnimatedRenderer = nullptr;

	animator.reset();

}
