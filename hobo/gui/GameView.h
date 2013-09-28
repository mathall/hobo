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

#ifndef HOBO_GUI_GAMEVIEW_H
#define HOBO_GUI_GAMEVIEW_H

#include <functional>
#include <memory>

#include "nanaka/gui/Button.h"
#include "nanaka/gui/DPad.h"
#include "nanaka/gui/Panel.h"

#include "gui/ComboHUD.h"
#include "gui/TriggerHUD.h"
#include "gui/PlayerHUD.h"

class Font;
class GUI;
class RenderTargetPanel;

class GameViewClient
{
public:

	virtual ~GameViewClient(){}

	virtual void ScreenTouched() = 0;
	virtual void Pause() = 0;
	virtual void Unpause() = 0;
	virtual void Restart() = 0;
	virtual void KickOff(float angle, float strength) = 0;
};

enum GameViewState
{
	GameViewStateStart,
	GameViewStatePlaying,
	GameViewStatePaused,
	GameViewStateStoped,
};

class GameView final
	: public PanelListener
	, public ButtonListener
	, public DPadListener
{
public:

	GameView(std::function<void(void)> backButtonCallback, const Font& font);

	void SetClient(GameViewClient* client);

	bool StateTransition(GameViewState viewState);

	std::shared_ptr<RenderTargetPanel> GetRenderPanel() const;
	std::shared_ptr<Panel> GetViewPanel() const;
	ComboHUD& GetComboHUD() const;
	TriggerHUD& GetTriggerHUD() const;
	PlayerHUD& GetPlayerHUD() const;

	/**
	 * PanelListener implementation.
	 */
	void PanelInput(UUID panelId, const InputEvent& event) override;

	/**
	 * ButtonListener implementation.
	 */
	void ButtonPushed(UUID buttonId) override;

	/**
	 * DPadListener implementation.
	 */
	void DPadInput(UUID DPadId, DPadAction action, Vec2f position) override;

private:

	void SetState(GameViewState viewState);

	GameViewClient* m_client;

	GameViewState m_viewState;

	std::function<void(void)> m_backButtonCallback;

	std::shared_ptr<RenderTargetPanel> m_renderPanel;
	std::shared_ptr<Panel> m_viewPanel;
	std::shared_ptr<Button> m_backButton;
	std::shared_ptr<Button> m_restartButton;
	std::shared_ptr<Panel> m_pausePanel;
	std::shared_ptr<DPad> m_DPad;

	std::unique_ptr<ComboHUD> m_comboHUD;
	std::unique_ptr<TriggerHUD> m_triggerHUD;
	std::unique_ptr<PlayerHUD> m_playerHUD;
};

inline void GameView::SetClient(GameViewClient* client)
{
	m_client = client;
}

inline std::shared_ptr<RenderTargetPanel> GameView::GetRenderPanel() const
{
	return m_renderPanel;
}

inline std::shared_ptr<Panel> GameView::GetViewPanel() const
{
	return m_viewPanel;
}

inline ComboHUD& GameView::GetComboHUD() const
{
	return *m_comboHUD;
}

inline TriggerHUD& GameView::GetTriggerHUD() const
{
	return *m_triggerHUD;
}

inline PlayerHUD& GameView::GetPlayerHUD() const
{
	return *m_playerHUD;
}

#endif // HOBO_GUI_GAMEVIEW_H
