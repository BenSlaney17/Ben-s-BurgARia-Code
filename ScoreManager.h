#pragma once
class ScoreManager
{
public:
	ScoreManager() { ResetScore(); }
	~ScoreManager();

	int GetScore() { return score_; }
	void ResetScore() { score_ = 0; }
	void AddToScore(int val) { score_ += val; }

private:
	int score_;
};

