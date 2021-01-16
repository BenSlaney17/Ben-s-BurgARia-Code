#pragma once
#include "GameState.h"
#include <maths/vector2.h>
#include <graphics/mesh_instance.h>
#include <platform/vita/graphics/texture_vita.h>
#include "primitive_builder.h"

// Vita AR includes
#include <camera.h>
#include <gxm.h>
#include <motion.h>
#include <libdbg.h>
#include <libsmart.h>

namespace gef
{
	class Texture;
	class TextureVita;
	class Sprite;
}

struct AppData;

class World;
class InteractionManager;
class BurgerAssembly;
class OrderManager;
class ScoreManager;
class LevelTimer;
class Button;

enum LevelStates
{
	LS_Preparing, LS_Playing, LS_Paused, LS_Score, LS_ReturnMainMenu
};

class LevelState :
	public GameState
{
public:
	LevelState();
	~LevelState();

	void OnEnter(gef::Platform& platform_, gef::AudioManager* audio_manager, Ben::PlayerInput* player_input, gef::Renderer3D* renderer3D, gef::SpriteRenderer* sprite_renderer, gef::Font* font) override;

	GameState* Update(const float& frame_time) override;

	void Render() override;

	void OnExit() override;

private:
	void InitCamera();
	void PauseScreenInit(gef::Platform & platform);
	void PreparingScreenInit(gef::Platform & platform);

	void ScoreUpdate();
	void PausedUpdate();
	void PreparingUpdate();
	void PlayingUpdate(const float & frame_time);

	void RenderCameraFeed(AppData * dat);
	void PreparingRender();
	void PauseRender();
	void ScoreRender();
	void Render3DScene();
	void RenderOverlay();
	
	LevelStates cur_state_;

#pragma region Preparing Variables
	gef::Texture* preparing_texture_;
	gef::Sprite* preparing_sprite_;
#pragma endregion

#pragma region Pause Variables
	gef::Texture* pause_texture_;
	gef::Sprite* pause_sprite_;

	Button* main_menu_button_;
#pragma endregion

#pragma region Playing Variables
	World * world_;
	PrimitiveBuilder * primitive_builder_;

	gef::Sprite* camera_display_;
	gef::Matrix44 orthographic_projection;

	InteractionManager* interaction_manager_;
	BurgerAssembly* burger_assembly_;
	ScoreManager* score_manager_;
	OrderManager* order_manager_;
	gef::TextureVita* camera_texture_;
	LevelTimer* level_timer_;
#pragma endregion

#pragma region Score Variables
	gef::Texture* score_texture_;
	gef::Sprite* score_sprite_;
#pragma endregion



	

	


	
	
};

