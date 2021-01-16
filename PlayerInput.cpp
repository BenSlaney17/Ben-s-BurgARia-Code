#include "PlayerInput.h"
#include <system/platform.h>
#include <input/input_manager.h>
#include <input/sony_controller_input_manager.h>
#include "input\touch_input_manager.h"
#include <input/keyboard.h>
#include "maths\vector2.h"
#include "graphics\sprite.h"

namespace Ben
{
	PlayerInput::PlayerInput(gef::Platform& platform) :
		input_manager_(gef::InputManager::Create(platform)),
		keyboard_(NULL),
		controller_manager_(NULL),
		active_touch_id_(-1)
	{
		keyboard_ = input_manager_->keyboard();
		controller_manager_ = input_manager_->controller_input();
		input_manager_->touch_manager()->EnablePanel(0);
	}

	PlayerInput::~PlayerInput()
	{
		if (input_manager_)
			delete input_manager_;

		input_manager_ = NULL;
		keyboard_ = NULL;
		controller_manager_ = NULL;
	}

	void PlayerInput::Update()
	{
		input_manager_->Update();
	}

	const bool PlayerInput::IsInputDown(const std::string& input_name)
	{
		if (!input_manager_) return false;

		const FlexibleInput* input = &GetInputOfName(input_name);

		// try adding pre processor directives
		if (keyboard_)
			return keyboard_->IsKeyDown(input->keyboard_keycode);

		if (controller_manager_)
		{
			const gef::SonyController* controller = controller_manager_->GetController(0);
			return  controller->buttons_down() & input->vita_input;
		}

		return false;
	}

	const bool PlayerInput::IsInputPressed(const std::string& input_name)
	{
		// Guard clause in case there is no input manager created
		if (!input_manager_) return false;

		const FlexibleInput* input = &GetInputOfName(input_name);

		if (keyboard_)
			return  keyboard_->IsKeyPressed(input->keyboard_keycode);

		const gef::SonyController* controller = controller_manager_->GetController(0);
		if (controller_manager_)
			return controller->buttons_pressed() & input->vita_input;
	}

	const bool PlayerInput::IsInputReleased(const std::string& input_name)
	{
		if (!input_manager_) return false;

		const FlexibleInput* input = &GetInputOfName(input_name);

		if (keyboard_)
			return keyboard_->IsKeyReleased(input->keyboard_keycode);

		const gef::SonyController* controller = controller_manager_->GetController(0);
		if (controller_manager_)
			return controller->buttons_released() & input->vita_input;

		return false;
	}

	const bool PlayerInput::IsAnyPressed()
	{
		// check if anything has been pressed
		if(!input_manager_)
			return false;
	
		const gef::SonyController* controller = controller_manager_->GetController(0);
		if (controller_manager_)
			return controller->buttons_pressed();

		return false;
	}

	const bool PlayerInput::IsNewTouch( gef::Vector2 & touchPos)
	{
		gef::TouchInputManager* touch_input_manager = input_manager_->touch_manager();
		// check for touch input
		if (touch_input_manager->max_num_panels() > 0)
		{
			// get the active touches for this panel
			const gef::TouchContainer& panel_touches = touch_input_manager->touches(0);

			// go through the touches
			for (gef::ConstTouchIterator touch = panel_touches.begin(); touch != panel_touches.end(); ++touch)
			{
				// if active touch id is -1, then we are not currently processing a touch
				//if (active_touch_id_ == -1)
				//{
					// check for the start of a new touch
					if (touch->type == gef::TT_NEW)
					{
						active_touch_id_ = touch->id;
						touchPos = touch->position;
						return true;
					}
				//}
				/*else
				{
					active_touch_id_ = -1;
				}*/
			}
		}

		return false;
	}

	const FlexibleInput& PlayerInput::GetInputOfName(const std::string& name)
	{
		for (size_t i = 0; i < input_table.size(); i++)
		{
			if (input_table[i].input_name == name)
				return input_table[i];
		}
	}

	bool TouchInput::IsInside(const gef::Sprite & sprite, const gef::Vector2 & point)
	{
		//check if outwith sprite height
		if (point.y < (sprite.position().y() - (0.5 * sprite.height())) || point.y >(sprite.position().y() + (0.5*sprite.height()))) {
			return false;
		}
		//check if outwith sprite width
		if (point.x <(sprite.position().x() - (0.5 * sprite.width())) || point.x >(sprite.position().x() + (0.5*sprite.width()))) {
			return false;
		}
		// if not outwidth sprite area return true
		return true;
	}
}