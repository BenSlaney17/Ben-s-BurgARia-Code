#pragma once

namespace gef
{
	class Texture;
	class Sprite;
	class Platform;
}
static class LoadSprite
{
public:
	static void InitSprite(gef::Platform& platform, gef::Texture* texture, const  char* filename, gef::Sprite*& sprite);
	static void InitSpriteToFillScreen(gef::Platform& platform, gef::Texture* texture, const  char* filename, gef::Sprite*& sprite);
};

