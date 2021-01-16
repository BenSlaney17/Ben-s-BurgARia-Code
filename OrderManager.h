#pragma once
#include <maths\vector4.h>
#include <deque>
#include "Order.h"
#include <graphics\sprite.h>

namespace gef
{
	class Texture;
	class Sprite;
	class SpriteRenderer;
	class Platform;
	class Font;
	class TouchInputManager;
	class InputManager;
	class AudioManager;
}

namespace Ben
{
	class PlayerInput;
}

class ScoreManager;
class Button;

class OrderManager
{
public:
	OrderManager(Ben::PlayerInput* player_input, gef::Platform& platform, ScoreManager* score_manager, gef::AudioManager* audio_manager);
	~OrderManager();

	void Update(float frame_time);
	void CheckForTouchInput();
	void Render(gef::SpriteRenderer* sprite_renderer, gef::Font* font);

	void CompleteFirstOrder();
	bool HasOrder() { return orders.size() > 0; }
	Order GetFirstOrder() {  return  orders[0]; }
	void SetFirstOrder(Order order) { orders[0] = order; }

private:
	void SetTimeUntilNextOrder();
	void InitOrderSprite(gef::Platform & platform);
	void HideOrShowOrder(float frame_time);
	bool HasOrderSpriteReachedPosition();
	bool ShouldGenerateNewOrder();
	void GenerateNewOrder();

	void HideSprite(gef::Sprite * sprite);
	
	void GetSpritesToRender(std::vector<gef::Sprite *> &spritesToRender, BurgerComponents &burger);
	void DisplayOrder(gef::SpriteRenderer * sprite_renderer, std::vector<gef::Sprite*> sprites, float xOffset, bool isOrderComplete);
	int DetermineScore(float time, float maxScoreTime, float midScoreTime);
	Ben::PlayerInput* player_input_;

	gef::AudioManager* audio_manager_;
	UInt32 new_order_sound_;

	std::deque<Order> orders;
	float timeSinceLastOrder;
	float time_until_next_order_;

#pragma region Sprites
	gef::Texture* order_texture_;
	gef::Sprite* order_sprite_;

	gef::Texture* order_counter_texture_;
	gef::Sprite* order_counter_sprite_;

	gef::Texture* top_bun_texture_;
	gef::Sprite* top_bun_sprite_;
	gef::Texture* bottom_bun_texture_;
	gef::Sprite* bottom_bun_sprite_;

	gef::Texture* patty_texture_;
	gef::Sprite* patty_sprite_;

	gef::Texture* tomato_texture_;
	gef::Sprite* tomato_sprite_;

	gef::Texture* onion_texture_;
	gef::Sprite* onion_sprite_;

	Button* send_order_button_;

	gef::Texture* short_time_texture_;
	gef::Texture* mid_time_texture_;
	gef::Texture* long_time_texture_;
	gef::Sprite* time_sprite_;

#pragma endregion

	ScoreManager* score_manager_;

	enum MovDirections { None, Left, Right };
	float startingXPosition;
	MovDirections movDirection;
	gef::Vector4 targetPosition;

	bool isRightHandedControls;

	float order_timer_;
};

