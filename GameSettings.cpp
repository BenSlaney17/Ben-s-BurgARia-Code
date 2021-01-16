#include "GameSettings.h"

GameSettings::GameSettings()
	: are_controls_right_handed_(true),
	cur_difficulty_(Easy)
{
	
}

GameSettings& GameSettings::Instance()
{
	static GameSettings INSTANCE;
	return INSTANCE;
}
