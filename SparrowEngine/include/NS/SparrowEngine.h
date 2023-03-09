#pragma once

/**
	
	To use this GameEngine(framework?)
	you must use GameEngine class first




*/

// Core
#include "core/Base.h"
#include "core/Log.h"
#include "core/GameEngine.h"

// Input / Output
#include "core/io/Window.h"
#include "core/io/Mouse.h"
#include "core/io/Keyboard.h"
// Events
#include "core/io/event/KeyEvent.h"
#include "core/io/event/MouseEvent.h"
#include "core/io/event/WindowEvent.h"

// Graphics
#include "core/graphics/NSGraphics.h"


// Loaders
#include "loader/TextureLoader.h"

// Layers
#include "core/layer/Layer.h"
#include "core/layer/MasterLayer.h"

// ImGUI
#include "ImGui/ImGuiLayer.h"

// Networking
#include "core/network/TcpClient.h"

// Audio
#include "core/audio/Audio.h"

// Scene
#include "core/scene/Scene.h"
#include "core/scene/Camera3D.h"