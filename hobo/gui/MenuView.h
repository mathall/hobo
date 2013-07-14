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

#ifndef HOBO_GUI_MENUVIEW_H
#define HOBO_GUI_MENUVIEW_H

#include <functional>
#include <memory>
#include <string>

#include "nanaka/gui/Button.h"

class GUI;
class Panel;
class RenderTargetPanel;
class Font;

class MenuViewClient
{
public:

	virtual void Shutdown() = 0;
};

enum MenuViewState
{
	MenuViewStateNonstarted,
	MenuViewStateStarted,
	MenuViewStateExitNonstarted,
	MenuViewStateExitStarted,
};

class MenuView : public ButtonListener
{
public:

	MenuView(std::function<void(void)> playButtonCallback, const Font& font);

	void SetClient(MenuViewClient* client);

	bool StateTransition(MenuViewState viewState);

	std::shared_ptr<RenderTargetPanel> GetRenderPanel();
	std::shared_ptr<Panel> GetViewPanel();

	/**
	 * ButtonListener implementation.
	 */
	void ButtonPushed(UUID buttonId) override;

private:

	void SetState(MenuViewState viewState);

	std::shared_ptr<Button> CreateButton(
		std::string buttonText,
		bool flipH,
		const Font& font);

	MenuViewState m_viewState;
	MenuViewClient* m_client;
	std::function<void(void)> m_playButtonCallback;

	std::shared_ptr<RenderTargetPanel> m_renderPanel;
	std::shared_ptr<Panel> m_viewPanel;

	std::shared_ptr<Button> m_playButton;
	std::shared_ptr<Button> m_resumeButton;
	std::shared_ptr<Button> m_howtoButton;
	std::shared_ptr<Button> m_highscoreButton;

	std::shared_ptr<Button> m_optionsButton;
	std::shared_ptr<Button> m_aboutButton;
	std::shared_ptr<Button> m_exitButton;

	std::shared_ptr<Panel> m_exitDialogue;
	std::shared_ptr<Button> m_exitYetButton;
	std::shared_ptr<Button> m_exitNoButton;
};

inline void MenuView::SetClient(MenuViewClient* client)
{
	m_client = client;
}

inline std::shared_ptr<RenderTargetPanel> MenuView::GetRenderPanel()
{
	return m_renderPanel;
}

inline std::shared_ptr<Panel> MenuView::GetViewPanel()
{
	return m_viewPanel;
}

#endif // HOBO_GUI_MENUVIEW_H
