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

#include "main/Main.h"

#include "nanaka/main/INanaka.h"
#include "nanaka/gui/GUI.h"
#include "nanaka/entity/EntitySystem.h"

#include "gui/MenuView.h"
#include "gui/GameView.h"
#include "main/BillboardET.h"
#include "main/CameraObjectET.h"
#include "main/EntityTypes.h"
#include "main/PlayerET.h"
#include "main/TriggerET.h"

IGame* CreateGame()
{
	return new Main();
}

void Main::Initialize(INanaka* nanaka)
{
	m_nanaka = nanaka;

	auto gotoGameView = [&]()
	{
		m_nanaka->GetGUI().SetActiveView(m_gameViewId);
		m_currentView = m_gameViewId;
	};

	auto gotoMenuView = [&]()
	{
		m_nanaka->GetGUI().SetActiveView(m_menuViewId);
		m_currentView = m_menuViewId;
	};

	auto fontId = m_nanaka->GetGUI().RegisterFont("Fonts/Default.nft");
	const Font* font = m_nanaka->GetGUI().GetFont(fontId);

	m_menuViewId = m_nanaka->GetGUI().CreateView();
	auto menuView = std::unique_ptr<MenuView>(
		new MenuView(gotoGameView, *font));
	m_nanaka->GetGUI().SetViewContent(m_menuViewId, menuView->GetViewPanel());

	m_gameViewId = m_nanaka->GetGUI().CreateView();
	auto gameView = std::unique_ptr<GameView>(
		new GameView(gotoMenuView, *font));
	m_nanaka->GetGUI().SetViewContent(m_gameViewId, gameView->GetViewPanel());

	auto& entitySystem = m_nanaka->GetEntitySystem();
	entitySystem.RegisterEntityType<PlayerET>(PlayerEntityType);
	entitySystem.RegisterEntityType<TriggerET>(TriggerEntityType);
	entitySystem.RegisterEntityType<BillboardET>(BillboardEntityType);
	entitySystem.RegisterEntityType<CameraObjectET>(CameraEntityType);

	m_game = std::unique_ptr<Game>(new Game(m_nanaka, std::move(gameView)));
	m_menu = std::unique_ptr<Menu>(new Menu(m_nanaka, std::move(menuView)));

	gotoMenuView();
}

void Main::GameLoop()
{
	if (m_currentView == m_gameViewId)
	{
		m_game->Update();
		m_game->Draw();
	}
	else
	{
		m_menu->Update();
		m_menu->Draw();
	}
}

void Main::OnPause()
{
	if (m_currentView == m_gameViewId)
	{
		m_game->Pause();
	}
}
