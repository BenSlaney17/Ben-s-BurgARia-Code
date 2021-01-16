#pragma once
#include <unordered_map>

static class GameSettings
{
public:

	static GameSettings& Instance();

	const bool GetAreControlsRightSided() { return are_controls_right_handed_; }
	void SwitchControlSide() { are_controls_right_handed_ = !are_controls_right_handed_; }
	void SetControlSide(bool isRight) { are_controls_right_handed_ = isRight; }

	enum Difficulty
	{
		Easy, Medium, Hard
	};

	const Difficulty GetCurrentDifficulty() { return cur_difficulty_; }
	void SetDifficulty(Difficulty new_difficulty) { cur_difficulty_ = new_difficulty; }

	struct DifficultyGameSettings
	{
		int maxNumOfOrders;
		float minTimeBetweenOrders;
		float maxTimeBetweenOrders;
	};

	DifficultyGameSettings difficultySettings[3] = 
	{
		{ 3, 20.0f, 40.0f },
		{ 4, 15.0f, 30.0f },
		{ 5, 10.0f, 20.0f }
	};

	const DifficultyGameSettings GetDifficultySettings()
	{
		return difficultySettings[cur_difficulty_];
	}

private:
	GameSettings();

	static GameSettings* instance;
	bool are_controls_right_handed_;
	Difficulty cur_difficulty_;

};