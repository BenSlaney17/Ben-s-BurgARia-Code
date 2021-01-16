#pragma once
#include "maths\vector4.h"
#include "graphics\sprite.h"
namespace gef
{
	class Platform;
	class Texture;
	class SpriteRenderer;
	class Font;
	class AudioManager;
}
namespace Ben
{
	class PlayerInput;
}

#define BUTTON_HEIGHT_DEFAULT 80
#define BUTTON_WIDTH_DEFAULT 200
class Button
{
public:
	Button( gef::Platform& platform, Ben::PlayerInput* player_input, const char* texture_filename, const char* text, gef::AudioManager* audio_manager);
	Button( gef::Platform& platform, Ben::PlayerInput* player_input, const char* texture_filename, const char* text, gef::AudioManager* audio_manager, UInt32 button_press_sample_id);
	
	~Button();

	void SetButtonPressSampleID(UInt32 id) { button_press_ = id; }
	void Update(float frame_time);
	void Render(gef::SpriteRenderer* renderer, gef::Font* font);

	const gef::Vector4 GetPosition() { return sprite_->position(); }
	void SetPosition(float x, float y){	sprite_->set_position(x, y, 0);}
	void SetPosition(gef::Vector4 pos) { sprite_->set_position(pos); }

	void Scale(float s);
	void SetWidth(float w);
	void SetHeight(float h);

	bool HasBeenPressed() { return has_been_pressed_; }
	
	void SetFontSize(float size) { font_size_ = size; }

	private:
	Ben::PlayerInput* player_input_;
	gef::AudioManager* audio_manager_;
	UInt32 button_press_;

	gef::Sprite* sprite_;
	gef::Texture* texture_;

	const char* text_;

	bool has_been_pressed_;

	float font_size_;
};

