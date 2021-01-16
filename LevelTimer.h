#pragma once

namespace gef
{
	class SpriteRenderer;
	class Sprite;
	class Texture;
	class Font;
}
class LevelTimer
{
public:
	LevelTimer();
	~LevelTimer();

	void Update(float frame_time);
	void Render(gef::SpriteRenderer* renderer, gef::Font* font);

	bool IsTimeUp() { return timer_/seconds_per_hour_ >= hours_limit_; }
private:
	float timer_;
	const float seconds_per_hour_ = 30.f;
	int hours_limit_ = 9;
};

