#pragma once
#include "GameState.h"
#include "graphics\sprite.h"

namespace gef
{
	class Platform;
	class Sprite;
	class Texture;
}

class Button;

class MainMenuState : public GameState
{
public:
	MainMenuState();
	~MainMenuState();

	void OnEnter(gef::Platform& platform, gef::AudioManager* audio_manager,  Ben::PlayerInput* player_input, gef::Renderer3D* renderer3D, gef::SpriteRenderer* sprite_renderer, gef::Font* font) override;
	void InitSettings(gef::Platform& platform, Ben::PlayerInput* player_input);
	void InitMainMenuButtons(gef::Platform& platform, Ben::PlayerInput* player_input);
	void InitLogo(gef::Platform & platform);
	GameState* Update(const float& frame_time) override;

	GameState* LoadingUpdate(bool& retflag);
	
	void Render() override;
	
	void OnExit() override;

private:
	void AnimateLogoPart1(const float & frame_time);
	void AnimateLogoPart2(const float & frame_time);

	void AnimateLogo();

	bool SettingsUpdate();
	void HandSettingUpdate();
	void DifficultySettingsUpdate();
	GameState* MainUpdate(const float& frame_time, bool& retflag);

	void SettingsRender();
	void MainMenuRender();

	enum MainMenuStates
	{
		MMS_Main, MMS_Settings, MMS_Loading
	};

	MainMenuStates cur_state_;

	gef::Texture* background_texture_;
	gef::Sprite* background_sprite_;

	gef::Texture* logo_texture_;
	gef::Sprite* logo_sprite_;
	float logo_target_width_;
	float logo_target_height_;

	float time_to_complete_logo_animation_ = 3.f;
	bool is_first_animation_complete_;
	bool is_second_animation_complete_;
	float animation_timer_;
	
	Button* start_game_button_;
	Button* settings_button_;

#pragma region Settings Variables
	gef::Sprite* highlight_sprite_;
	gef::Texture* highlight_texture_;
	Button* easy_button_;
	Button* medium_button_;
	Button* hard_button_;

	Button* left_hand_button_;
	Button* right_hand_button_;
#pragma endregion

	gef::Texture* loading_texture_;
	gef::Sprite* loading_sprite_;
	bool has_just_entered_loading_;

	UInt32 button_press_sample_id_;
};

