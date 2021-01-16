#ifndef _RENDER_TARGET_APP_H
#define _RENDER_TARGET_APP_H

#include <system/application.h>
#include <maths/vector2.h>
#include <vector>
#include <graphics/mesh_instance.h>
#include <platform/vita/graphics/texture_vita.h>
#include "primitive_builder.h"


// FRAMEWORK FORWARD DECLARATIONS
namespace gef
{
	class Platform;
	class SpriteRenderer;
	class Font;
	class Renderer3D;
	class Mesh;
	class RenderTarget;
	class InputManager;
	class Texture;
	class AudioManager;
}

namespace Ben
{
	class PlayerInput;
}

class GameState;

class ARApp : public gef::Application
{
public:
	ARApp(gef::Platform& platform);
	void Init();
	
	void CleanUp();
	bool Update(float frame_time);
	void Render();

private:
	void InitFont();
	void CleanUpFont();
	void DrawHUD();
	void SetupLights();

	gef::InputManager* input_manager_;
	gef::AudioManager* audio_manager_;
	gef::SpriteRenderer* sprite_renderer_;
	gef::Font* font_;

	float fps_;

	Ben::PlayerInput* player_input_;
	gef::Renderer3D* renderer_3d_;

	GameState* cur_state_;
	
};




#endif // _RENDER_TARGET_APP_H