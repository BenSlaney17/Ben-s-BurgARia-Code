#include "MainMenuState.h"
#include "LevelState.h"
#include "graphics\sprite.h"
#include <graphics\sprite_renderer.h>
#include "LoadSprite.h"
#include "load_texture.h"
#include <graphics/font.h>
#include "system\platform.h"
#include "PlayerInput.h"
#include "Maths.h"
#include "Button.h"
#include "GameSettings.h"
#include "audio\audio_manager.h"

MainMenuState::MainMenuState()
	: cur_state_(MMS_Main),
	background_texture_(NULL),
	background_sprite_(NULL),
	logo_texture_(NULL),
	logo_sprite_(NULL),
	start_game_button_(NULL),
	settings_button_(NULL),
	easy_button_(NULL),
	medium_button_(NULL),
	hard_button_(NULL),
	left_hand_button_(NULL),
	right_hand_button_(NULL),
	highlight_sprite_(NULL),
	highlight_texture_(NULL),
	loading_texture_(NULL),
	loading_sprite_(NULL)
{
}

MainMenuState::~MainMenuState()
{
	delete background_texture_;
	background_texture_ = NULL;

	delete background_sprite_;
	background_sprite_ = NULL;

	delete logo_texture_;
	logo_texture_ = NULL;

	delete logo_sprite_;
	logo_sprite_ = NULL;

	delete start_game_button_;
	start_game_button_ = NULL;

	delete settings_button_;
	settings_button_ = NULL;

	delete easy_button_;
	easy_button_ = NULL;
	delete medium_button_;
	medium_button_ = NULL;
	delete hard_button_;
	hard_button_ = NULL;
	delete left_hand_button_;
	left_hand_button_ = NULL;
	delete right_hand_button_;
	right_hand_button_ = NULL;

	delete highlight_sprite_;
	highlight_sprite_ = NULL;
	delete highlight_texture_;
	highlight_texture_ = NULL;

	delete loading_texture_;
	loading_texture_ = NULL;
	delete loading_sprite_;
	loading_sprite_ = NULL;
}


void MainMenuState::OnEnter(gef::Platform& platform, gef::AudioManager* audio_manager,  Ben::PlayerInput * player_input, 
	gef::Renderer3D * renderer3D, gef::SpriteRenderer * sprite_renderer, gef::Font* font)
{
	GameState::OnEnter(platform, audio_manager,  player_input, renderer3D, sprite_renderer, font);

	LoadSprite::InitSpriteToFillScreen(platform, background_texture_, "titleBackground.png", background_sprite_);
	
	InitLogo(platform);

	button_press_sample_id_ = audio_manager_->LoadSample("ButtonSelect.wav", platform);
	InitMainMenuButtons(platform, player_input);

	InitSettings(platform, player_input);

	LoadSprite::InitSpriteToFillScreen(platform, loading_texture_, "loading.png", loading_sprite_);

	audio_manager_->LoadMusic("menuMusic.wav", *platform_);
	gef::VolumeInfo musicInfo;
	musicInfo.volume = 0.75f;
	audio_manager_->SetMusicVolumeInfo(musicInfo);
	audio_manager_->PlayMusic();

	is_first_animation_complete_ = false;
	is_second_animation_complete_ = false;

	cur_state_ = MMS_Main;

	animation_timer_ = 0.0f;
}

void MainMenuState::InitSettings(gef::Platform& platform, Ben::PlayerInput* player_input)
{
	float xCentre = platform.width() / 2;
	easy_button_ = new Button(platform, player_input, "button2.png", "Commis Chef", audio_manager_, button_press_sample_id_);
	easy_button_->SetFontSize(0.9f);
	easy_button_->SetPosition(xCentre - 250, 200);
	medium_button_ = new Button(platform, player_input, "button2.png", "Sous Chef", audio_manager_, button_press_sample_id_);
	medium_button_->SetPosition(xCentre, 200);
	hard_button_ = new Button(platform, player_input, "button2.png", "Chef de Cuisine", audio_manager_, button_press_sample_id_);
	hard_button_->SetPosition(xCentre + 250, 200);
	hard_button_->SetFontSize(0.8f);

	left_hand_button_ = new Button(platform, player_input, "button2.png", "left", audio_manager_, button_press_sample_id_);
	left_hand_button_->SetPosition(xCentre - 150, 400);
	right_hand_button_ = new Button(platform, player_input, "button2.png", "right", audio_manager_, button_press_sample_id_);
	right_hand_button_->SetPosition(xCentre + 150, 400);

	LoadSprite::InitSprite(platform, highlight_texture_, "selectedButtonOutline.png", highlight_sprite_);
	highlight_sprite_->set_width(BUTTON_WIDTH_DEFAULT + 10);
	highlight_sprite_->set_height(BUTTON_HEIGHT_DEFAULT + 3);
}

void MainMenuState::InitMainMenuButtons(gef::Platform& platform, Ben::PlayerInput* player_input)
{
	start_game_button_ = new Button(platform, player_input, "button2.png", "Start", audio_manager_, button_press_sample_id_);
	start_game_button_->SetPosition(platform.width() / 2, platform.height() / 2 + 100);
	start_game_button_->Scale(0.0f);
	start_game_button_->SetFontSize(0.f);
	settings_button_ = new Button(platform, player_input, "button2.png", "Settings", audio_manager_, button_press_sample_id_);
	settings_button_->SetPosition(platform.width() / 2, platform.height() / 2 + 200);
	settings_button_->Scale(0.0f);
	settings_button_->SetFontSize(0.f);
}

void MainMenuState::InitLogo(gef::Platform & platform)
{
	logo_texture_ = CreateTextureFromPNG("logo.png", platform);
	logo_sprite_ = new gef::Sprite();
	logo_sprite_->set_texture(logo_texture_);
	logo_sprite_->set_width(0);
	logo_sprite_->set_height(0);
	logo_sprite_->set_position(platform.width() / 2, platform.height(), 0);

	logo_target_width_ = 690.f;
	logo_target_height_ = 438;
}

GameState * MainMenuState::Update(const float& frame_time)
{
	// if start game pressed then return new levelState 
	if (!is_second_animation_complete_)
	{
		if (!is_first_animation_complete_)
			AnimateLogoPart1(frame_time);
		else
			AnimateLogoPart2(frame_time);
	}
	else
	{
		switch (cur_state_)
		{
		case MainMenuState::MMS_Main:
			bool shouldChangeGameState;
			GameState* newGameState = MainUpdate(frame_time, shouldChangeGameState);
			if (shouldChangeGameState)
			{
				// I'm only doing this because the only other GameState that can be loaded from MainMenu is the LevelState
				cur_state_ = MMS_Loading;
				has_just_entered_loading_ = true;
			}
			break;
		case MainMenuState::MMS_Settings:
			if (SettingsUpdate())
			{
				cur_state_ = MMS_Main;
			}
			break;
		case MainMenuState::MMS_Loading:
			bool shouldReturnLevelState;
			GameState* gs = LoadingUpdate(shouldReturnLevelState);
			if (shouldReturnLevelState) return gs;
			break;
		default:
			break;
		}
	}

	return NULL;
}
void MainMenuState::AnimateLogoPart1(const float& frame_time)
{
	float targetY = platform_->height() / 2;
	float newYValue = Maths::Lerp(platform_->height(), targetY, animation_timer_);
	logo_sprite_->set_position(gef::Vector4(logo_sprite_->position().x(), newYValue, logo_sprite_->position().z()));

	int numOfRotations = 3;
	logo_sprite_->set_rotation(Maths::Lerp(0, Maths::TWO_PI() * numOfRotations, animation_timer_));
	logo_sprite_->set_width(Maths::Lerp(0, logo_target_width_, animation_timer_));
	logo_sprite_->set_height(Maths::Lerp(0, logo_target_height_, animation_timer_));
	if (logo_sprite_->position().y() <= targetY)
	{
		is_first_animation_complete_ = true;
		animation_timer_ = 0.0f;
		logo_sprite_->set_rotation(0);
	}

	// animation timer should be in range 0 ... 1
	animation_timer_ += frame_time / time_to_complete_logo_animation_;
}

void MainMenuState::AnimateLogoPart2(const float& frame_time)
{
	start_game_button_->SetHeight(Maths::Lerp(0, BUTTON_HEIGHT_DEFAULT, animation_timer_));
	start_game_button_->SetWidth(Maths::Lerp(0, BUTTON_WIDTH_DEFAULT, animation_timer_));
	start_game_button_->SetFontSize(Maths::Lerp(0, 1.0f, animation_timer_));

	settings_button_->SetHeight(Maths::Lerp(0, BUTTON_HEIGHT_DEFAULT, animation_timer_));
	settings_button_->SetWidth(Maths::Lerp(0, BUTTON_WIDTH_DEFAULT, animation_timer_));
	settings_button_->SetFontSize(Maths::Lerp(0, 1.0f, animation_timer_));

	AnimateLogo();
	animation_timer_ += frame_time;
}

void MainMenuState::AnimateLogo()
{
	float targetY = platform_->height() / 4;
	float newYValue = Maths::Lerp(platform_->height() / 2, targetY, animation_timer_);

	logo_sprite_->set_position(gef::Vector4(logo_sprite_->position().x(), newYValue, logo_sprite_->position().z()));

	float logoTargetScale = 0.7f;
	logo_sprite_->set_width(Maths::Lerp(logo_target_width_, logo_target_width_ * logoTargetScale, animation_timer_));
	logo_sprite_->set_height(Maths::Lerp(logo_target_height_, logo_target_height_ * logoTargetScale, animation_timer_));

	if (logo_sprite_->position().y() <= targetY)
	{
		is_second_animation_complete_ = true;
		animation_timer_ = 0.0f;
	}
}

GameState* MainMenuState::MainUpdate(const float& frame_time, bool& retflag)
{
	retflag = true;
	start_game_button_->Update(frame_time);
	if (start_game_button_->HasBeenPressed())
	{
		// display loading screen
		return new LevelState();
	}

	settings_button_->Update(frame_time);
	if (settings_button_->HasBeenPressed())
	{
		cur_state_ = MMS_Settings;
	}
	retflag = false;
	return NULL;
}

bool MainMenuState::SettingsUpdate()
{
	// difficulty settings
	DifficultySettingsUpdate();

	// dominant hand
	HandSettingUpdate();

	// return true if the player has pressed back
	// might add in a button to do this
	return player_input_->IsInputPressed("Circle");
}

GameState* MainMenuState::LoadingUpdate(bool& retflag)
{
	retflag = true;
	if (!has_just_entered_loading_)
		return new LevelState();

	if (has_just_entered_loading_)
	{
		has_just_entered_loading_ = false;
	}
	retflag = false;
	return {};
}

void MainMenuState::HandSettingUpdate()
{
	left_hand_button_->Update(0);
	if (left_hand_button_->HasBeenPressed())
	{
		GameSettings::Instance().SetControlSide(false);
	}
	right_hand_button_->Update(0);
	if (right_hand_button_->HasBeenPressed())
	{
		GameSettings::Instance().SetControlSide(true);
	}
}

void MainMenuState::DifficultySettingsUpdate()
{
	easy_button_->Update(0);
	if (easy_button_->HasBeenPressed())
	{
		GameSettings::Instance().SetDifficulty(GameSettings::Easy);
	}
	medium_button_->Update(0);
	if (medium_button_->HasBeenPressed())
	{
		GameSettings::Instance().SetDifficulty(GameSettings::Medium);
	}
	hard_button_->Update(0);
	if (hard_button_->HasBeenPressed())
	{
		GameSettings::Instance().SetDifficulty(GameSettings::Hard);
	}
}

void MainMenuState::Render()
{
	sprite_renderer_->Begin(true);
	sprite_renderer_->DrawSprite(*background_sprite_);
	switch (cur_state_)
	{
	case MainMenuState::MMS_Main:
		MainMenuRender();
		break;
	case MainMenuState::MMS_Settings:
		SettingsRender();
		break;
	case MainMenuState::MMS_Loading:
		sprite_renderer_->DrawSprite(*loading_sprite_);
		break;
	default:
		break;
	}
	
	sprite_renderer_->End();
}

void MainMenuState::SettingsRender()
{
	// draw highlight for difficulty
	auto diff = GameSettings::Instance().GetCurrentDifficulty();
	switch (diff)
	{
	case GameSettings::Easy:
		highlight_sprite_->set_position(easy_button_->GetPosition());
		break;
	case GameSettings::Medium:
		highlight_sprite_->set_position(medium_button_->GetPosition());
		break;
	case GameSettings::Hard:
		highlight_sprite_->set_position(hard_button_->GetPosition());
		break;
	default:
		break;
	}
	sprite_renderer_->DrawSprite(*highlight_sprite_);
	easy_button_->Render(sprite_renderer_, font_);
	medium_button_->Render(sprite_renderer_, font_);
	hard_button_->Render(sprite_renderer_, font_);

	// draw highlight for hand
	highlight_sprite_->set_position(GameSettings::Instance().GetAreControlsRightSided() ? right_hand_button_->GetPosition(): left_hand_button_->GetPosition());
	sprite_renderer_->DrawSprite(*highlight_sprite_);
	left_hand_button_->Render(sprite_renderer_, font_);
	right_hand_button_->Render(sprite_renderer_, font_);

	// return to main menu text
	font_->RenderText(sprite_renderer_, gef::Vector4(800, 510.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_CENTRE, "Press 'O' to return");
}

void MainMenuState::MainMenuRender()
{
	sprite_renderer_->DrawSprite(*logo_sprite_);

	if (is_first_animation_complete_)
	{
		start_game_button_->Render(sprite_renderer_, font_);
		settings_button_->Render(sprite_renderer_, font_);
	}
}



void MainMenuState::OnExit()
{
	audio_manager_->UnloadAllSamples();
}
