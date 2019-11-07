#pragma once

#include <memory>

#include "GameState.h"


class DebugCamera;
class GridFloor;


class PlayState: public GameState
{
	public:
		PlayState();

	public:
		virtual ~PlayState();


	public:
		void Initialize() override;
		void Update() override;
		void Render() override;
		void Finalize() override;
};
