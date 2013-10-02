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

#include "main/Game.h"

#include "nanaka/entity/EntitySystem.h"
#include "nanaka/entity/EntityTree.h"
#include "nanaka/game/GameEM.h"
#include "nanaka/gui/RenderTargetPanel.h"
#include "nanaka/main/INanaka.h"

#include "main/EntityTypes.h"
#include "main/Player.h"
#include "main/PlayerET.h"

Game::Game(INanaka* nanaka, std::unique_ptr<GameView> gameView)
	: m_nanaka(nanaka)
	, m_world(std::make_shared<World>())
	, m_gameView(std::move(gameView))
	, m_sound("Sounds/loop.ogg")
{
	m_gameView->SetClient(this);
	m_gameView->GetRenderPanel()->SetRenderTargetPanelListener(this);

	m_world->GetPhysicsMgr().SetHandleResponse(
		m_nanaka->GetEntitySystem().GetEntityTypeId(TriggerEntityType),
		m_nanaka->GetEntitySystem().GetEntityTypeId(PlayerEntityType), true);

	m_timeController = std::unique_ptr<TimeController>(
		new TimeController(m_world->GetTime()));

	m_comboController = std::unique_ptr<ComboController>(
		new ComboController(*m_timeController));

	PlayerEMParams playerParams(*m_comboController, *m_timeController);
	auto playerEntity = nanaka->GetEntitySystem().SpawnEntity(
		PlayerEntityType, &playerParams, &m_world->GetEntityTree());
	m_player = &playerEntity->GetModule<GameEM>()->GetGameObject<Player>();

	auto cameraEntity =
		nanaka->GetEntitySystem().SpawnEntity(CameraEntityType, NULL);
	m_camera =
		&cameraEntity->GetModule<GameEM>()->GetGameObject<CameraObject>();
	m_camera->SetRenderTarget(m_gameView->GetRenderPanel());
	cameraEntity->SetPosition(Vec3f(0.0f, -10.0f, 0.0f));

	playerEntity->AttachEntity(cameraEntity);

	m_triggerManager = std::unique_ptr<TriggerManager>(new TriggerManager(
		m_nanaka, *m_player, *m_timeController, *m_comboController, m_world));

	std::vector<LayerType> layerTypes = {
		GrassLayerType,
		Hills1LayerType,
		Hills2LayerType,
		SkyLayerType,
		CloudsLayerType,
	};

	for (auto layerType : layerTypes)
	{
		m_layers.push_back(std::unique_ptr<Layer>(
			new Layer(m_nanaka, *m_camera, m_world, layerType)));
	}

	Restart();
}

void Game::Update()
{
	switch (m_gameState)
	{
	case GameStateRunning:
		m_world->Tick();
		UpdateLayers();
		m_triggerManager->Update(
			m_player->GetEntity()->GetPosition().x, !m_player->HasPass());
		m_triggerManager->UpdateTriggerHUD(m_gameView->GetTriggerHUD());
		m_comboController->Update(m_world->GetTime().GetRealTick());
		m_comboController->UpdateComboHUD(m_gameView->GetComboHUD());
		m_timeController->Update();
		break;

	case GameStatePaused:
		break;
	}

	if (m_player->IsStopped())
	{
		m_sound.Stop();
		m_gameView->StateTransition(GameViewStateStoped);
	}

	m_player->UpdatePlayerHUD(m_gameView->GetPlayerHUD());
}

void Game::KickOff(float angle, float strength)
{
	m_sound.Repeat();
	m_player->KickOff(angle, strength);
	m_triggerManager->KickOff();
}

void Game::OnNewSize(UUID panelId)
{
	for (auto& layer : m_layers)
	{
		layer->UpdateViewSize();
	}
}

void Game::UpdateLayers()
{
	const float cameraX = m_camera->GetEntity()->GetPosition().x;
	const float viewWidth = m_camera->GetMaxViewWidth();
	const float layerScroll = cameraX - viewWidth * 0.5f;
	for (auto& layer : m_layers)
	{
		layer->SetScroll(layerScroll);
	}
}

void Game::ScreenTouched()
{
	if (m_comboController->IsComboActivated())
	{
		if (m_player->AddComboScore(m_comboController->ExecuteCombo()))
		{
			// ascend?
		}
	}
	else
	{
		m_player->Soar();
	}
}

void Game::Pause()
{
	m_sound.Pause();
	m_gameState = GameStatePaused;
	m_gameView->StateTransition(GameViewStatePaused);
}

void Game::Restart()
{
	m_sound.Stop();
	m_triggerManager->Reset(0.0f);
	m_player->Reset(0.0f);
	m_comboController->Reset();
	Unpause();
}
