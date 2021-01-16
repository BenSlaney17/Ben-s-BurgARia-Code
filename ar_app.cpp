#include "ar_app.h"
#include <system/platform.h>
#include <graphics/sprite_renderer.h>
#include <graphics/texture.h>
#include <graphics/mesh.h>
#include <graphics/primitive.h>
#include <assets/png_loader.h>
#include <graphics/image_data.h>
#include <graphics/font.h>
#include <input/touch_input_manager.h>
#include <maths/vector2.h>
#include <input/input_manager.h>
#include <maths/math_utils.h>
#include <graphics/renderer_3d.h>
#include <graphics/render_target.h>
#include <audio\audio_manager.h>

#include "MainMenuState.h"
#include "LevelState.h"

#include "PlayerInput.h"

ARApp::ARApp(gef::Platform& platform) :
	Application(platform),
	input_manager_(NULL),
	audio_manager_(NULL),
	sprite_renderer_(NULL),
	font_(NULL),
	renderer_3d_(NULL),	
	player_input_(NULL),
	cur_state_(NULL)
{
}

void ARApp::Init()
{
	player_input_ = new Ben::PlayerInput(platform_);
	audio_manager_ = gef::AudioManager::Create();
	input_manager_ = gef::InputManager::Create(platform_);

	sprite_renderer_ = gef::SpriteRenderer::Create(platform_);
	renderer_3d_ = gef::Renderer3D::Create(platform_);

	InitFont();
	SetupLights();

	cur_state_ = new MainMenuState();
	cur_state_->OnEnter(platform_, audio_manager_,  player_input_, renderer_3d_, sprite_renderer_, font_);
}

void ARApp::CleanUp()
{
	CleanUpFont();

	delete audio_manager_;
	audio_manager_ = NULL;
	
	delete sprite_renderer_;
	sprite_renderer_ = NULL;

	delete renderer_3d_;
	renderer_3d_ = NULL;

	delete input_manager_;
	input_manager_ = NULL;

	delete player_input_;
	player_input_ = NULL;

}

bool ARApp::Update(float frame_time)
{
	fps_ = 1.0f / frame_time;

	player_input_->Update();
	
	GameState* state = cur_state_->Update(frame_time);
	// if it's a different state
	if (state != NULL)
	{
		cur_state_->OnExit();

		delete cur_state_;
		cur_state_ = state;

		cur_state_->OnEnter(platform_, audio_manager_, player_input_, renderer_3d_, sprite_renderer_, font_);
	}

	return true;
}

void ARApp::Render()
{
	cur_state_->Render();

	sprite_renderer_->Begin(false);
	DrawHUD();
	sprite_renderer_->End();
}

void ARApp::InitFont()
{
	font_ = new gef::Font(platform_);
	font_->Load("newFont");
}

void ARApp::CleanUpFont()
{
	delete font_;
	font_ = NULL;
}

void ARApp::DrawHUD()
{
	if(font_)
	{
		// NO FPS 
		//font_->RenderText(sprite_renderer_, gef::Vector4(850.0f, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "FPS: %.1f", fps_);
	}
}

void ARApp::SetupLights()
{
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(-300.0f, -500.0f, 100.0f));

	gef::Default3DShaderData& default_shader_data = renderer_3d_->default_shader_data();
	default_shader_data.set_ambient_light_colour(gef::Colour(0.5f, 0.5f, 0.5f, 1.0f));
	default_shader_data.AddPointLight(default_point_light);
}
