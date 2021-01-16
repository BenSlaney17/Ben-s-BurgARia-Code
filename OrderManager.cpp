#include "OrderManager.h"

#include "graphics\sprite_renderer.h"
#include "graphics\sprite.h"
#include "graphics\texture.h"
#include "graphics\font.h"
#include "sony_sample_framework.h"
#include "input/input_manager.h"
#include "input\touch_input_manager.h"
#include "audio\audio_manager.h"

#include "PlayerInput.h"
#include "load_texture.h"
#include "OrderGenerator.h"
#include "GameSettings.h"
#include "ScoreManager.h"
#include "LoadSprite.h"
#include "Button.h"

OrderManager::OrderManager(Ben::PlayerInput * player_input, gef::Platform& platform, ScoreManager* score_manager, gef::AudioManager* audio_manager)
	: player_input_(player_input),
	audio_manager_(audio_manager),
	order_texture_(NULL),
	order_sprite_(NULL),
	order_counter_texture_(NULL),
	order_counter_sprite_(NULL),
	top_bun_texture_(NULL),
	top_bun_sprite_(NULL),
	bottom_bun_texture_(NULL),
	bottom_bun_sprite_(NULL),
	patty_texture_(NULL),
	patty_sprite_(NULL),
	tomato_texture_(NULL),
	tomato_sprite_(NULL),
	onion_texture_(NULL),
	onion_sprite_(NULL),
	send_order_button_(NULL),
	score_manager_(score_manager),
	order_timer_(0)
{
	isRightHandedControls = GameSettings::Instance().GetAreControlsRightSided();

	InitOrderSprite(platform);
	LoadSprite::InitSprite(platform, top_bun_texture_, "burgerBunTop.png", top_bun_sprite_);
	LoadSprite::InitSprite(platform, bottom_bun_texture_, "burgerBunBottom.png", bottom_bun_sprite_);
	LoadSprite::InitSprite(platform, patty_texture_, "burgerPatty.png", patty_sprite_);
	LoadSprite::InitSprite(platform, onion_texture_, "onionSlice.png", onion_sprite_);
	LoadSprite::InitSprite(platform, tomato_texture_, "tomatoSlice.png", tomato_sprite_);

	const float spriteWidth = 90.0f;
	const float spriteHeight = 33.0f;

	top_bun_sprite_->set_width(spriteWidth);
	top_bun_sprite_->set_height(spriteHeight);
	bottom_bun_sprite_->set_width(spriteWidth);
	bottom_bun_sprite_->set_height(spriteHeight);
	patty_sprite_->set_width(spriteWidth);
	patty_sprite_->set_height(spriteHeight);
	onion_sprite_->set_width(spriteWidth);
	onion_sprite_->set_height(spriteHeight);
	tomato_sprite_->set_width(spriteWidth);
	tomato_sprite_->set_height(spriteHeight);

	UInt32 button_sample_ = audio_manager_->LoadSample("ButtonSelect.wav", platform);
	send_order_button_ = new Button(platform, player_input, "button2.png", "Send", audio_manager_);

	// set the new order audio sample
	new_order_sound_ = audio_manager_->LoadSample("NewOrderClick.wav", platform);

	SetTimeUntilNextOrder();
	orders = std::deque<Order>();
}

void OrderManager::SetTimeUntilNextOrder()
{
	float minTime = GameSettings::Instance().GetDifficultySettings().minTimeBetweenOrders;
	float maxTime = GameSettings::Instance().GetDifficultySettings().maxTimeBetweenOrders;
	time_until_next_order_ = rand() % (int)(maxTime - minTime + 1) + (int)minTime;
}

void OrderManager::InitOrderSprite(gef::Platform & platform)
{
	//setup order sprite
	order_texture_ = CreateTextureFromPNG("order_template2.png", platform);
	order_sprite_ = new gef::Sprite();
	order_sprite_->set_texture(order_texture_);
	order_sprite_->set_height(400.0f);
	order_sprite_->set_width(300.0f);
	startingXPosition = isRightHandedControls ? 150 : platform.width() - 150;
	order_sprite_->set_position
	( startingXPosition, 200, 0);

	order_counter_texture_ = CreateTextureFromPNG("Target.png", platform);
	order_counter_sprite_ = new gef::Sprite();
	order_counter_sprite_->set_texture(order_counter_texture_);
	order_counter_sprite_->set_height(50.0f);
	order_counter_sprite_->set_width(50.0f);
	order_counter_sprite_->set_position(startingXPosition + (isRightHandedControls ? 185.f : -185.f)
		, 25, 0);

	short_time_texture_ = CreateTextureFromPNG("shortTime.png", platform);
	mid_time_texture_ = CreateTextureFromPNG("medTime.png", platform);
	long_time_texture_ = CreateTextureFromPNG("longTime.png", platform);
	time_sprite_ = new gef::Sprite();
	time_sprite_->set_width(50.0f);
	time_sprite_->set_height(50.0f);
	time_sprite_->set_position(startingXPosition + (isRightHandedControls ? 185.f : -185.f)
		, 75, 0);
}

OrderManager::~OrderManager()
{
	player_input_ = NULL;

	delete order_texture_;
	order_texture_ = NULL;

	delete order_sprite_;
	order_sprite_ = NULL;

	delete onion_texture_;
	onion_texture_ = NULL;

	delete tomato_texture_;
	tomato_texture_ = NULL;

	delete patty_texture_;
	patty_texture_ = NULL;

	delete send_order_button_;
	send_order_button_ = NULL;

	delete short_time_texture_;
	short_time_texture_ = NULL;

	delete mid_time_texture_;
	mid_time_texture_ = NULL;

	delete long_time_texture_;
	long_time_texture_ = NULL;

	delete time_sprite_;
	time_sprite_ = NULL;
}

bool OrderManager::ShouldGenerateNewOrder()
{
	int maxNumOfOrders = GameSettings::Instance().GetDifficultySettings().maxNumOfOrders;
	return timeSinceLastOrder >= time_until_next_order_ && orders.size() < maxNumOfOrders;
}
void OrderManager::Update(float frame_time)
{
	order_timer_ += frame_time;

	HideOrShowOrder(frame_time);

	// order generation
	timeSinceLastOrder += frame_time;

	if (ShouldGenerateNewOrder())
	{
		GenerateNewOrder();
	}

	// hide all sprites before rendering, not at all the best method, but it works and isn't too expensive to become a problem 
	HideSprite(top_bun_sprite_);
	HideSprite(bottom_bun_sprite_);
	HideSprite(patty_sprite_);
	HideSprite(tomato_sprite_);
	HideSprite(onion_sprite_);

	// check for touch input on send button if first order is complete
	if (orders.size() > 0 &&  orders[0].order_complete_)
	{
		send_order_button_->Update(frame_time);
		CheckForTouchInput();
	}
}

void OrderManager::GenerateNewOrder()
{
	orders.push_back(OrderGenerator::GenerateOrder(order_timer_));
	audio_manager_->PlaySample(new_order_sound_);
	timeSinceLastOrder = 0;
	SetTimeUntilNextOrder();
}

void OrderManager::CheckForTouchInput()
{
	if(send_order_button_->HasBeenPressed())
	{
		// send order
		CompleteFirstOrder();		
	}
}

struct SpritesToRender
{
	SpritesToRender()
	{
	}
	SpritesToRender(bool sR, gef::Sprite* sp)
	{
		shouldRender = sR;
		sprite = sp;
	}
	bool shouldRender;
	gef::Sprite* sprite;
};


void OrderManager::Render(gef::SpriteRenderer* sprite_renderer, gef::Font* font)
{
	sprite_renderer->DrawSprite(*order_sprite_);
	sprite_renderer->DrawSprite(*order_counter_sprite_);
	font->RenderText(sprite_renderer, order_counter_sprite_->position() + gef::Vector4(0.f, -18.f, 0.f), 1.3f, 0xff000000, gef::TJ_CENTRE, "%i", orders.size());

	// don't continue if there are no orders
	if (orders.size() == 0) return;
	

	// for each burger in the first order
	int numOfBurgers = orders[0].burgers_.size();
	const float xOffset = 45.0f;
	float startingX = xOffset - (xOffset* numOfBurgers);

	for (size_t i = 0; i < numOfBurgers; i++)
	{
		BurgerComponents burger = orders[0].burgers_[i];
		std::vector<gef::Sprite*> spritesToRender = std::vector<gef::Sprite*>();

		// add pointers to the relevant sprites to the spritesToRender vector
		GetSpritesToRender(spritesToRender, burger);
	
		DisplayOrder(sprite_renderer, spritesToRender, startingX+ (i * xOffset * 2), orders[0].burgers_[i].is_complete_);
	}

	// if order is complete then draw button
	if (orders[0].order_complete_)
	{
		send_order_button_->SetPosition(order_counter_sprite_->position() + gef::Vector4(-190.f, 250.f, 0.f));
		//send_order_sprite_->set_position(order_counter_sprite_->position() + gef::Vector4(-170.f, 250.f, 0.f));
		send_order_button_->Render(sprite_renderer, font);
	}
}

void OrderManager::GetSpritesToRender(std::vector<gef::Sprite *> &spritesToRender, BurgerComponents &burger)
{
	spritesToRender.push_back(bottom_bun_sprite_);

	std::vector<SpritesToRender>  componentsToRender
	{
		SpritesToRender(burger.num_burgers_ >= 1, patty_sprite_),
		SpritesToRender(burger.num_burgers_ >= 2, patty_sprite_),
		SpritesToRender(burger.num_tomatoes_ >= 1, tomato_sprite_),
		SpritesToRender(burger.num_tomatoes_ >= 2, tomato_sprite_),
		SpritesToRender(burger.num_onions_ >= 1, onion_sprite_),
		SpritesToRender(burger.num_onions_ >= 2, onion_sprite_),
	};

	for (size_t i = 0; i < componentsToRender.size(); i++)
	{
		if (componentsToRender[i].shouldRender)
			spritesToRender.push_back(componentsToRender[i].sprite);
	}

	spritesToRender.push_back(top_bun_sprite_);
}

void OrderManager::HideSprite(gef::Sprite* sprite)
{
	sprite->set_position(-500.0f, 0, 0);
}

// sprites order needs to be: bottom bun, components, top bun
void OrderManager::DisplayOrder(gef::SpriteRenderer* sprite_renderer, std::vector<gef::Sprite*> sprites, float xOffset, bool isOrderComplete)
{
	const float yDifference = isOrderComplete ?  10.0f : 35.0f;
	const float startingPosition = isOrderComplete ? 200.0f - (8 - sprites.size()) * 5.0f : 350.0f - (8 - sprites.size()) * 20.0f;
	for (size_t i = 0; i < sprites.size(); i++)
	{
		sprites[i]->set_position(gef::Vector4(order_sprite_->position().x() + xOffset, startingPosition - (yDifference * i), 0.0f));
		sprite_renderer->DrawSprite(*sprites[i]);
	}

	switch (orders[0].order_time_type_)
	{
	case (Short):
		time_sprite_->set_texture(short_time_texture_);
		break;
	case(Medium):
		time_sprite_->set_texture(mid_time_texture_);
		break;
	case(Long):
		time_sprite_->set_texture(long_time_texture_);
		break;
	default:
		break;
	}
	
	sprite_renderer->DrawSprite(*time_sprite_);

}

void OrderManager::CompleteFirstOrder()
{
	// determine score for the order
	float timeToCompleteOrder = order_timer_ - orders[0].time_of_order_;
	int scoreToAdd = 50; // min score value
	
	OrderTime timeType = orders[0].order_time_type_;
	switch (timeType)
	{
	case Short:
		scoreToAdd += DetermineScore(timeToCompleteOrder, 10, 20);
		break;
	case Medium:
		scoreToAdd += DetermineScore(timeToCompleteOrder, 15, 30);
		break;
	case Long:
		scoreToAdd += DetermineScore(timeToCompleteOrder, 25, 45);
		break;
	default:
		break;
	}

	score_manager_->AddToScore(scoreToAdd);
	orders.pop_front();
}

int OrderManager::DetermineScore(float time, float maxScoreTime, float midScoreTime)
{
	if (time <= maxScoreTime)
		return 50;
	if (time <= midScoreTime)
		return 25;
	return 0;
}

void OrderManager::HideOrShowOrder(float frame_time)
{
	const float moveAmount = 300.0f;
	if (player_input_->IsRightPressed() || player_input_->IsInputPressed("Circle"))
	{
		targetPosition = gef::Vector4(isRightHandedControls ?  startingXPosition : startingXPosition + moveAmount, 200, 0);
		movDirection = Right;
	}
	if (player_input_->IsLeftPressed() || player_input_->IsInputPressed("Square"))
	{
		targetPosition = gef::Vector4(isRightHandedControls ? startingXPosition - moveAmount : startingXPosition , 200, 0);
		movDirection = Left;
	}
	

	if (movDirection == None) return;

	// check if reached position
	if (HasOrderSpriteReachedPosition())
	{
		movDirection = None;
		return;
	}

	static float mov_dist_per_second = 500.0f;

	int dir = movDirection == Left ? -1 : 1;

	order_sprite_->set_position(order_sprite_->position() + gef::Vector4(dir * mov_dist_per_second * frame_time, 0, 0));
	order_counter_sprite_->set_position(order_counter_sprite_->position() + gef::Vector4(dir * mov_dist_per_second * frame_time, 0, 0));
	time_sprite_->set_position(time_sprite_->position() + gef::Vector4(dir * mov_dist_per_second * frame_time, 0, 0));
}

bool OrderManager::HasOrderSpriteReachedPosition()
{
	return movDirection == Left ? order_sprite_->position().x() <= targetPosition.x() : order_sprite_->position().x() >= targetPosition.x();
}
