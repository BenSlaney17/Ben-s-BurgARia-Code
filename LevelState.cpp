#include "LevelState.h"
#include <system/platform.h>
#include <graphics/sprite.h>
#include <graphics\renderer_3d.h>
#include <graphics\sprite_renderer.h>
#include <sony_tracking.h>
#include <sony_sample_framework.h>
#include <graphics/font.h>
#include <graphics/render_target.h>

#include "GameObject.h"
#include "World.h"
#include "InteractionManager.h"
#include "BurgerAssembly.h"
#include "ScoreManager.h"
#include "OrderManager.h"
#include "GameSettings.h"
#include "load_texture.h"
#include "LoadSprite.h"
#include "PlayerInput.h"
#include "MainMenuState.h"
#include "audio\audio_manager.h"
#include "LevelTimer.h"
#include "Button.h"

LevelState::LevelState()
	:primitive_builder_(NULL),
	cur_state_(LS_Preparing),
	preparing_texture_(NULL),
	preparing_sprite_(NULL),
	pause_texture_(NULL),
	pause_sprite_(NULL),
	main_menu_button_(NULL),
	camera_texture_(NULL),
	camera_display_(NULL),
	world_(NULL),
	interaction_manager_(NULL),
	burger_assembly_(NULL),
	score_manager_(NULL),
	level_timer_(NULL),
	order_manager_(NULL),
	score_texture_(NULL),
	score_sprite_(NULL)
{
}

LevelState::~LevelState()
{
	delete primitive_builder_;
	primitive_builder_ = NULL;

	camera_texture_ = NULL;

	delete preparing_texture_;
	preparing_texture_ = NULL;

	delete preparing_sprite_;
	preparing_sprite_ = NULL;

	delete pause_texture_;
	pause_texture_ = NULL;

	delete pause_sprite_;
	pause_sprite_ = NULL;

	delete main_menu_button_;
	main_menu_button_ = NULL;

	delete world_;
	world_ = NULL;

	delete level_timer_;
	level_timer_ = NULL;

	delete interaction_manager_;
	interaction_manager_ = NULL;

	burger_assembly_ = NULL;

	delete score_manager_;
	score_manager_ = NULL;

	delete order_manager_;
	order_manager_ = NULL;

	delete score_texture_;
	score_texture_ = NULL;

	delete score_sprite_;
	score_sprite_ = NULL;
}

void LevelState::OnEnter(gef::Platform& platform, gef::AudioManager* audio_manager,  Ben::PlayerInput * player_input, gef::Renderer3D * renderer3D, gef::SpriteRenderer * sprite_renderer, gef::Font* font)
{
	GameState::OnEnter(platform, audio_manager, player_input, renderer3D, sprite_renderer, font);

	// Extremely hacky way of making the frying pan the first audio sample in the audio manager.
	// For some reason audio manager won't actually change the volume of a sample if it isn't sample[0],
	// even though the VolumeInfo of the sample does change
	UInt32 test_sample = audio_manager_->LoadSample("frying.wav", platform);

	PreparingScreenInit(platform);

	PauseScreenInit(platform);

	LoadSprite::InitSpriteToFillScreen(platform, score_texture_, "scoreBackground.png", score_sprite_);

	primitive_builder_ = new PrimitiveBuilder(platform);
	score_manager_ = new ScoreManager();
	order_manager_ = new OrderManager(player_input_, platform, score_manager_, audio_manager_);
	world_ = new World(platform, audio_manager, primitive_builder_, player_input_, renderer_3d_, order_manager_);
	InitCamera();
	interaction_manager_ = new InteractionManager(platform, world_->GetCollisionEngine(), player_input_);
	// we need a reference to burger assembly
	burger_assembly_ = world_->GetBurgerAssembly();

	level_timer_ = new LevelTimer();

	// initialise sony framework
	sampleInitialize();
	smartInitialize();

	// reset marker tracking
	AppData* dat = sampleUpdateBegin();
	smartTrackingReset();
	sampleUpdateEnd(dat);

	// stop main menu music, don't currently have music for the level itself
	// want this to be the last thing to happen in the OnEnter
	audio_manager_->StopMusic();
}

void LevelState::PreparingScreenInit(gef::Platform & platform)
{
	LoadSprite::InitSprite(platform, preparing_texture_, "prepScreen.png", preparing_sprite_);
	preparing_sprite_->set_position(platform.width() / 2, platform.height() / 2, 0);
	preparing_sprite_->set_width(platform.width());
	preparing_sprite_->set_height(platform.height());
}

void LevelState::PauseScreenInit(gef::Platform & platform)
{
	LoadSprite::InitSprite(platform, pause_texture_, "pauseScreen.png", pause_sprite_);
	pause_sprite_->set_position(platform.width() / 2, platform.height() / 2, 0);
	pause_sprite_->set_width(platform.width());
	pause_sprite_->set_height(platform.height());

	main_menu_button_ = new Button(platform, player_input_, "button2.png", "Main Menu", audio_manager_);
	main_menu_button_->SetPosition(gef::Vector4(platform.width() / 2, platform.height() / 2, 0));
}

void LevelState::InitCamera()
{
	orthographic_projection.SetIdentity();
	orthographic_projection.OrthographicFrustumD3D(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	const float widthScale = 200.0f;
	camera_display_ = new gef::Sprite();
	camera_display_->set_position(gef::Vector4(((float)DISPLAY_WIDTH / 2.0f), (float)DISPLAY_HEIGHT / 2.0f, 1.0f));
	camera_display_->set_width((float)DISPLAY_WIDTH);
	camera_display_->set_height((float)DISPLAY_HEIGHT * VIEW_SCALE_V);
	camera_texture_ = new gef::TextureVita();
}


GameState * LevelState::Update(const float & frame_time)
{
	AppData* dat = sampleUpdateBegin();

	// use the tracking library to try and find markers
	smartUpdate(dat->currentImage);

	switch (cur_state_)
	{
	case LS_Preparing:
		PreparingUpdate();
		break;
	case LS_Playing:
		PlayingUpdate(frame_time);
		break;
	case LS_Paused:
		PausedUpdate();
		break;
	case LS_Score:
		ScoreUpdate();
		break;
	case LS_ReturnMainMenu:
	{
		return new MainMenuState();
		break;
	}
	default:
		break;
	}
	sampleUpdateEnd(dat);
	return NULL;
}

void LevelState::ScoreUpdate()
{
	if (player_input_->IsAnyPressed())
	{
		cur_state_ = LS_ReturnMainMenu;
	}
}

void LevelState::PausedUpdate()
{
	main_menu_button_->Update(0);
	if (main_menu_button_->HasBeenPressed())
	{
		cur_state_ = LS_ReturnMainMenu;
	}
	if (player_input_->IsInputPressed("Pause"))
	{
		cur_state_ = LS_Playing;
	}
}

void LevelState::PreparingUpdate()
{
	if (player_input_->IsAnyPressed())
	{
		cur_state_ = LS_Playing;
	}
}

void LevelState::PlayingUpdate(const float & frame_time)
{
	world_->Update(frame_time);

	interaction_manager_->Update();

	order_manager_->Update(frame_time);

	level_timer_->Update(frame_time);

	if (player_input_->IsInputPressed("Pause"))
	{
		cur_state_ = LS_Paused;
		audio_manager_->MuteAllSamples();
	}

	if (level_timer_->IsTimeUp())
	{
		cur_state_ = LS_Score;
		audio_manager_->MuteAllSamples();
	}
}

void LevelState::Render()
{
	AppData* dat = sampleRenderBegin();

	// Render the camera feed
	RenderCameraFeed(dat);

	switch (cur_state_)
	{
	case LS_Preparing:
		PreparingRender();
		break;
	case LS_Playing:
		Render3DScene();
		RenderOverlay();
		break;
	case LS_Paused:
		PauseRender();
		break;
	case LS_Score:
		ScoreRender();
		break;
	default:
		break;
	}
	

	sampleRenderEnd();
}

void LevelState::PreparingRender()
{
	sprite_renderer_->Begin(false);
	sprite_renderer_->DrawSprite(*preparing_sprite_);
	sprite_renderer_->End();
}

void LevelState::PauseRender()
{
	sprite_renderer_->Begin(false);
	sprite_renderer_->DrawSprite(*pause_sprite_);
	main_menu_button_->Render(sprite_renderer_, font_);
	sprite_renderer_->End();
}

void LevelState::ScoreRender()
{
	sprite_renderer_->Begin(false);
	sprite_renderer_->DrawSprite(*score_sprite_);
	font_->RenderText(sprite_renderer_, gef::Vector4(platform_->width()/2, platform_->height()/2, -0.9f), 2.5f, 0xffffffff, gef::TJ_CENTRE, "%i", score_manager_->GetScore());
	sprite_renderer_->End();
}

void LevelState::OnExit()
{
	audio_manager_->UnloadAllSamples();
	smartRelease();
	sampleRelease();
}

void LevelState::RenderCameraFeed(AppData * dat)
{
	renderer_3d_->set_projection_matrix(orthographic_projection);
	sprite_renderer_->Begin(true);

	// draw Camera feed sprite here
	// check if ther is data present for the camera image before trying to draw it
	if (dat->currentImage)
	{
		camera_texture_->set_texture(dat->currentImage->tex_yuv);
		camera_display_->set_texture(camera_texture_);
		sprite_renderer_->DrawSprite(*camera_display_);
	}

	sprite_renderer_->End();
}

void LevelState::Render3DScene()
{
	float near_clip = 0.05f;
	float far_clip = 10.0f;

	gef::Matrix44 scaleMatrix;
	scaleMatrix.Scale(gef::Vector4(1.0f, VIEW_SCALE_V, 1.0f, 0.0f));

	gef::Matrix44 FOVProjectionMatrix = platform_->PerspectiveProjectionFov(
		SCE_SMART_IMAGE_FOV,
		(float)SCE_SMART_IMAGE_WIDTH / (float)SCE_SMART_IMAGE_HEIGHT,
		near_clip,
		far_clip);

	gef::Matrix44 finalProjectionMatrix = FOVProjectionMatrix * scaleMatrix;
	renderer_3d_->set_projection_matrix(finalProjectionMatrix);

	gef::Matrix44 viewMatrix;
	viewMatrix.SetIdentity();
	renderer_3d_->set_view_matrix(viewMatrix);

	sprite_renderer_->Begin(false);

	world_->Render(renderer_3d_);

	sprite_renderer_->End();
}

void LevelState::RenderOverlay()
{
	//
	// render 2d hud on top
	//
	gef::Matrix44 proj_matrix2d;

	proj_matrix2d = platform_->OrthographicFrustum(0.0f, platform_->width(), 0.0f, platform_->height(), -1.0f, 1.0f);
	sprite_renderer_->set_projection_matrix(proj_matrix2d);
	sprite_renderer_->Begin(false);
	interaction_manager_->Render2DOverlay(sprite_renderer_, font_);
	order_manager_->Render(sprite_renderer_, font_);

	// render burger assembly values
	float xStartingPosition = GameSettings::Instance().GetAreControlsRightSided() ? 450.0f : 50.0f;
	font_->RenderText(sprite_renderer_, gef::Vector4(xStartingPosition, 0.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Burgers: %i", burger_assembly_->GetNumOfBurgers());
	font_->RenderText(sprite_renderer_, gef::Vector4(xStartingPosition + 170.f, 0.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Onions: %i", burger_assembly_->GetNumOfOnions());
	font_->RenderText(sprite_renderer_, gef::Vector4(xStartingPosition + 315.0f, 0.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Tomatoes: %i", burger_assembly_->GetNumOfTomatoes());
	
	font_->RenderText(sprite_renderer_, gef::Vector4(xStartingPosition, 25.0f, -0.9f), 1.0f, 0xffffffff, gef::TJ_LEFT, "Score: %i", score_manager_->GetScore());
	
	level_timer_->Render(sprite_renderer_, font_);

	sprite_renderer_->End();
}