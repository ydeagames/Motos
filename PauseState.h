#pragma once


#include "GameState.h"


class PauseState: public GameState
{
	public:
		PauseState();

	public:
		virtual ~PauseState();


	public:
		void Initialize() override;
		void Update(float elapsedTime) override;
		void Render() override;
		void Finalize() override;
};
