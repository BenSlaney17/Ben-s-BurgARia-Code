#include "LevelTimer.h"
#include "graphics\sprite.h"
#include "graphics\sprite_renderer.h"
#include "graphics\font.h"
#include "graphics\texture.h"
#include "system\platform.h"
#include "GameSettings.h"

LevelTimer::LevelTimer()
	: timer_(0.0f)
{
}


LevelTimer::~LevelTimer()
{
}

void LevelTimer::Update(float frame_time)
{
	timer_ += frame_time;
}

void LevelTimer::Render(gef::SpriteRenderer * renderer, gef::Font * font)
{
	float xPos = GameSettings::Instance().GetAreControlsRightSided() ? 750.f : 100.f;
	gef::Vector4 pos(xPos, 510, 0.0);
	int inGameTime = 1 + (timer_ / seconds_per_hour_);
	font->RenderText(renderer, pos, 1.0f, 0xffffffff, gef::TJ_CENTRE, "Hour: %i", inGameTime);
}
