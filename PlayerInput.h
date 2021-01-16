#pragma once
#include <system/application.h>
#include "input/keyboard.h"
#include "input/sony_controller_input_manager.h"
#include <string>
#include <vector>

namespace gef
{
	class InputManager;
	class Platform;
	class Vector2;
	class Sprite;
}

// this is meant to mimic Unitys Input system, but with Vita related inputs in mind
struct FlexibleInput
{
	// name of the input
	std::string input_name;
	// keyboard input, could expand to be both positive and negative (like Unity)
	gef::Keyboard::KeyCode keyboard_keycode;
	// sony controller input
	Int32 vita_input;
};

std::vector<FlexibleInput> const input_table = {
	{"Up",				gef::Keyboard::KC_W,		gef_SONY_CTRL_UP},
	{"Down",			gef::Keyboard::KC_S,		gef_SONY_CTRL_DOWN},
	{"Left",			gef::Keyboard::KC_A,		gef_SONY_CTRL_LEFT},
	{"Right",			gef::Keyboard::KC_D,		gef_SONY_CTRL_RIGHT},
	{"LeftBumper",		gef::Keyboard::KC_Q,		gef_SONY_CTRL_L2},
	{"RightBumper",		gef::Keyboard::KC_E,		gef_SONY_CTRL_R2},
	{"Circle",			gef::Keyboard::KC_G,		gef_SONY_CTRL_CIRCLE},
	{"Square",			gef::Keyboard::KC_F,		gef_SONY_CTRL_SQUARE},
	{"Cross",			gef::Keyboard::KC_H,		gef_SONY_CTRL_CROSS},
	{"Pause",			gef::Keyboard::KC_ESCAPE,	gef_SONY_CTRL_START}
};


namespace Ben
{
	class PlayerInput
	{

	public:
		PlayerInput(gef::Platform& platform);
		~PlayerInput();

		void Update();

		const bool IsInputDown(const std::string& input_name);
		const bool IsInputPressed(const std::string& input_name);
		const bool IsInputReleased(const std::string& input_name);

		const bool IsAnyPressed();
		const bool IsUpPressed() { return IsInputPressed("Up"); }
		const bool IsDownPressed() { return IsInputPressed("Down"); }
		const bool IsLeftPressed() { return IsInputPressed("Left"); }
		const bool IsRightPressed() { return IsInputPressed("Right"); }
		
		const bool IsNewTouch( gef::Vector2& OUT_touchPos);
	private:

		const FlexibleInput& GetInputOfName(const std::string& name);

		gef::InputManager* input_manager_;
		gef::Keyboard* keyboard_;
		gef::SonyControllerInputManager* controller_manager_;

		Int32 active_touch_id_;
	};

	static class TouchInput
	{
	public:
		static bool IsInside(const gef::Sprite& sprite, const gef::Vector2& point);
	};
		
}
