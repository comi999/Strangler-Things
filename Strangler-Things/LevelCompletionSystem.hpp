#pragma once

#include "Delegate.hpp"


class LevelCompletionSystem
{
public:
	// Bool is true if this is the final level
	inline static Delegate< void, bool > OnLevelComplete;

	void InitForNewLevel();
	void Update();

private:
	int m_PlayersRequired;
	int m_PlayersComplete;
};