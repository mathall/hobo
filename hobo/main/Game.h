/*
 * Copyright (c) 2013, Mathias Hällman. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef HOBO_MAIN_GAME_H
#define HOBO_MAIN_GAME_H

#include <memory>
#include <vector>

#include "nanaka/gui/RenderTargetPanel.h"
#include "nanaka/main/World.h"
#include "nanaka/sound/Sound.h"

#include "gui/GameView.h"
#include "main/CameraObject.h"
#include "main/ComboController.h"
#include "main/Layer.h"
#include "main/TimeController.h"
#include "main/TriggerManager.h"

class INanaka;
class Player;

class Game final : public RenderTargetPanelListener, public GameViewClient
{
public:

	Game(INanaka* nanaka, std::unique_ptr<GameView> gameView);

	void Update();
	void UpdateLayers();
	void Draw() const;

	/**
	 * RenderTargetPanelListener implementation.
	 */
	void OnNewSize(UUID panelId);

	/**
	 * GameViewClient implementation.
	 */
	void ScreenTouched() override;
	void Pause() override;
	void Unpause() override;
	void Restart() override;
	void KickOff(float angle, float strength) override;

private:

	enum GameState
	{
		GameStateRunning,
		GameStatePaused,
	};

	INanaka* m_nanaka;

	std::shared_ptr<World> m_world;

	GameState m_gameState;

	std::unique_ptr<GameView> m_gameView;

	Player* m_player;
	CameraObject* m_camera;

	std::unique_ptr<TimeController> m_timeController;
	std::unique_ptr<ComboController> m_comboController;
	std::unique_ptr<TriggerManager> m_triggerManager;
	std::vector<std::unique_ptr<Layer>> m_layers;

	Sound m_sound;
};

inline void Game::Unpause()
{
	m_sound.Unpause();
	m_gameState = GameStateRunning;
}

inline void Game::Draw() const
{
	m_world->Draw(*m_camera->GetCamera());
}

#endif // HOBO_MAIN_GAME_H
