#include "LoadSprite.h"
#include "load_texture.h"
#include "graphics\sprite.h"

// should only be used if the texture of the sprite will not change
void LoadSprite::InitSprite(gef::Platform & platform, gef::Texture * texture, const char * filename, gef::Sprite *& sprite)
{
	texture = CreateTextureFromPNG(filename, platform);
	sprite = new gef::Sprite();
	sprite->set_texture(texture);
}

void LoadSprite::InitSpriteToFillScreen(gef::Platform & platform, gef::Texture * texture, const char * filename, gef::Sprite *& sprite)
{
	texture = CreateTextureFromPNG(filename, platform);
	sprite = new gef::Sprite();
	sprite->set_texture(texture);
	sprite->set_position(platform.width() / 2, platform.height() / 2, 0);
	sprite->set_width(platform.width());
	sprite->set_height(platform.height());

}
