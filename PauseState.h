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
		void Update() override;
		void Render() override;
		void Finalize() override;
};
