
// I don't know
#include <imgui.h>
#include <examples/imgui_impl_glfw.h>
#include <examples/imgui_impl_opengl3.h>

// Temporary
#include <GLFW/glfw3.h>

#include <NS/ImGui/ImGuiLayer.h>
#include <NS/core/io/Keyboard.h>
#include <NS/core/GameEngine.h>

namespace ns {

	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {
		this->m_DeltaTime = 0;
	}

	void ImGuiLayer::onAttach() {

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

		ImGui::StyleColorsDark();

		// Setup platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)(GameEngine::GetWindow()->getHandle()), true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::onDetach() {

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::onUpdate(Time delta) {
		this->m_DeltaTime += delta;
	}

	void ImGuiLayer::onEvent(Event& e) {
		ImGuiIO& io = ImGui::GetIO();
		if (e.category == Event::Mouse && io.WantCaptureMouse) { e.handled(); }
		if (e.category == Event::Keyboard && io.WantCaptureKeyboard) { e.handled(); }
	}

	void ImGuiLayer::renderGUI() {
		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Begin("Settings");

		ImGui::Text("Sparrow Engine");
		ImGui::End();

	}

	void ImGuiLayer::BeginFrame() {
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::EndFrame() {
		ImGuiIO& io = ImGui::GetIO();
		io.DeltaTime = this->m_DeltaTime;
		this->m_DeltaTime = 0;
		io.DisplaySize = ImVec2((float)GameEngine::GetWindow()->getWidth(), (float)GameEngine::GetWindow()->getHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			GLFWwindow *backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}
}
