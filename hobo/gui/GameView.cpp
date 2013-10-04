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

#include "gui/GameView.h"

#include "nanaka/gui/Counter.h"
#include "nanaka/gui/GUI.h"
#include "nanaka/gui/RenderTargetPanel.h"
#include "nanaka/gui/StaticText.h"
#include "nanaka/input/InputEvent.h"

GameView::GameView(
	std::function<void(void)> backButtonCallback,
	const Font& font)
	: m_backButtonCallback(backButtonCallback)
{
	// Player HUD
	auto FPSPanel = std::make_shared<Panel>();
	auto soarPanel = std::make_shared<Panel>();
	auto ascentPanel = std::make_shared<Panel>();
	auto speedPanel = std::make_shared<Panel>();
	auto scorePanel = std::make_shared<Panel>();
	auto heightPanel = std::make_shared<Panel>();

	m_playerHUD = std::unique_ptr<PlayerHUD>(new PlayerHUD(
		FPSPanel, soarPanel, ascentPanel, speedPanel, scorePanel, heightPanel,
		font));

	// Combo HUD
	auto comboPanel = std::make_shared<Panel>();
	auto comboTimerCounter =
		std::make_shared<Counter>(font, 64.0f, dipGUISizeUnit, 3, 0, ' ');

	m_comboHUD = std::unique_ptr<ComboHUD>(new ComboHUD(
		comboPanel, comboTimerCounter));

	// Trigger HUD
	auto triggerPanel = std::make_shared<Panel>();

	m_triggerHUD = std::unique_ptr<TriggerHUD>(new TriggerHUD(triggerPanel));

	// Render panel
	m_renderPanel = std::make_shared<RenderTargetPanel>();
	m_renderPanel->SetListener(this);

	// HUD panel
	auto HUDPanel = std::make_shared<Panel>();

	LayoutProperties HUDPanelLayoutProps;
	HUDPanelLayoutProps.m_z = 1;
	HUDPanel->SetLayoutProperties(HUDPanelLayoutProps);

	// Pause panel
	m_pausePanel = std::make_shared<Panel>();
	m_pausePanel->SetBackground("", Vec4f(0.0f, 0.0f, 0.0f, 0.4f));
	m_pausePanel->SetListener(this);
	m_pausePanel->SetSwallowsInput(true);

	LayoutProperties pausedPanelLayoutProps;
	pausedPanelLayoutProps.m_z = 2;
	m_pausePanel->SetLayoutProperties(pausedPanelLayoutProps);

	// GUI panel
	auto GUIPanel = std::make_shared<Panel>();

	LayoutProperties GUIPanelLayoutProps;
	GUIPanelLayoutProps.m_z = 3;
	GUIPanel->SetLayoutProperties(GUIPanelLayoutProps);

	// View
	m_viewPanel = std::make_shared<Panel>();
	m_viewPanel->AddWidget(m_renderPanel);
	m_viewPanel->AddWidget(HUDPanel);
	m_viewPanel->AddWidget(m_pausePanel);
	m_viewPanel->AddWidget(GUIPanel);

	// HUD panel
	{
		HUDPanel->SetLayoutMethod(LayoutMethod(LayoutMethod::Linear));

		auto GUISpacerPanel = std::make_shared<Panel>();
		auto HUDContentPanel = std::make_shared<Panel>();
		LayoutProperties HUDPanelLayoutProps;
		HUDPanelLayoutProps.m_sizeWeight = 4.0f;
		HUDContentPanel->SetLayoutProperties(HUDPanelLayoutProps);

		HUDPanel->AddWidget(HUDContentPanel);
		HUDPanel->AddWidget(GUISpacerPanel);

		HUDContentPanel->SetLayoutMethod(LayoutMethod(
			LayoutMethod::Linear, LayoutMethod::Horizontal));

		auto HUDLeftPanel = std::make_shared<Panel>();
		auto HUDMiddlePanel = std::make_shared<Panel>();
		auto HUDRightPanel = std::make_shared<Panel>();

		HUDContentPanel->AddWidget(HUDLeftPanel);
		HUDContentPanel->AddWidget(HUDMiddlePanel);
		HUDContentPanel->AddWidget(HUDRightPanel);

		// HUD Left
		{
			HUDLeftPanel->SetLayoutMethod(LayoutMethod(LayoutMethod::Linear));

			auto spacerPanel = std::make_shared<Panel>();
			auto HUDLeftContentPanel = std::make_shared<Panel>();
			LayoutProperties HUDLeftContentLayoutProps;
			HUDLeftContentLayoutProps.m_sizeWeight = 3.0f;
			HUDLeftContentLayoutProps.m_growVertically = true;
			HUDLeftContentPanel->SetLayoutProperties(HUDLeftContentLayoutProps);

			HUDLeftPanel->AddWidget(spacerPanel);
			HUDLeftPanel->AddWidget(HUDLeftContentPanel);

			HUDLeftContentPanel->SetLayoutMethod(LayoutMethod(
				LayoutMethod::Linear, LayoutMethod::Vertical,
				LayoutMethod::Left, LayoutMethod::Top));

			HUDLeftContentPanel->AddWidget(soarPanel);
			HUDLeftContentPanel->AddWidget(ascentPanel);
			HUDLeftContentPanel->AddWidget(FPSPanel);
		}

		// HUD Middle
		{
			HUDMiddlePanel->SetLayoutMethod(LayoutMethod(LayoutMethod::Linear));

			auto spacerPanel = std::make_shared<Panel>();
			auto HUDMiddleContentPanel = std::make_shared<Panel>();
			LayoutProperties HUDMiddleContentLayoutProps;
			HUDMiddleContentLayoutProps.m_sizeWeight = 3.0f;
			HUDMiddleContentLayoutProps.m_growVertically = true;
			HUDMiddleContentPanel->SetLayoutProperties(
				HUDMiddleContentLayoutProps);

			HUDMiddlePanel->AddWidget(spacerPanel);
			HUDMiddlePanel->AddWidget(HUDMiddleContentPanel);

			auto DPadPanel = std::make_shared<Panel>();
			LayoutProperties DPadPanelLayoutProps;
			DPadPanelLayoutProps.m_aspectRatio = 1.0f;
			DPadPanel->SetLayoutProperties(DPadPanelLayoutProps);

			m_DPad = std::make_shared<DPad>("GUI/HUD/DPad.png", Vec4f(1.0f));
			m_DPad->SetListener(this);
			LayoutProperties DPadLayoutProps;
			DPadLayoutProps.m_relPosition = Vec2f(0.1f);
			DPadLayoutProps.m_relSize = Vec2f(0.8f);
			m_DPad->SetLayoutProperties(DPadLayoutProps);
			DPadPanel->AddWidget(m_DPad);

			auto comboTimerPanel = std::make_shared<Panel>();
			comboTimerPanel->SetLayoutMethod(
				LayoutMethod(LayoutMethod::Linear));
			comboTimerPanel->AddWidget(comboTimerCounter);

			HUDMiddleContentPanel->AddWidget(DPadPanel);
			HUDMiddleContentPanel->AddWidget(heightPanel);
			HUDMiddleContentPanel->AddWidget(comboTimerPanel);
		}

		// HUD Right
		{
			HUDRightPanel->SetLayoutMethod(LayoutMethod(
				LayoutMethod::Linear, LayoutMethod::Vertical,
				LayoutMethod::Right, LayoutMethod::Top));

			HUDRightPanel->AddWidget(scorePanel);
			HUDRightPanel->AddWidget(speedPanel);
			HUDRightPanel->AddWidget(comboPanel);
			HUDRightPanel->AddWidget(triggerPanel);
		}
	}

	// GUI panel
	{
		GUIPanel->SetLayoutMethod(LayoutMethod(LayoutMethod::Linear));

		auto GUIContentPanel = std::make_shared<Panel>();
		auto HUDSpacerPanel = std::make_shared<Panel>();
		LayoutProperties HUDSpacerPanelLayoutProps;
		HUDSpacerPanelLayoutProps.m_sizeWeight = 4.0f;
		HUDSpacerPanel->SetLayoutProperties(HUDSpacerPanelLayoutProps);

		GUIPanel->AddWidget(HUDSpacerPanel);
		GUIPanel->AddWidget(GUIContentPanel);

		GUIContentPanel->SetLayoutMethod(LayoutMethod(
			LayoutMethod::Linear, LayoutMethod::Horizontal));

		auto GUILeftPanel = std::make_shared<Panel>();
		auto GUIRightPanel = std::make_shared<Panel>();

		GUIContentPanel->AddWidget(GUILeftPanel);
		GUIContentPanel->AddWidget(GUIRightPanel);

		// GUI Left
		{
			GUILeftPanel->SetLayoutMethod(LayoutMethod(
				LayoutMethod::Linear, LayoutMethod::Horizontal,
				LayoutMethod::Left, LayoutMethod::VCenter));

			auto backButtonPressedPanel = std::make_shared<Panel>();
			backButtonPressedPanel->SetBackground(
				"", Vec4f(0.0f, 1.0f, 0.0f, 0.5f));
			backButtonPressedPanel->SetBorder(
				"GUI/edgetest.png", "GUI/cornertest.png",
				Vec4f(0.0f, 1.0f, 0.0f, 0.5f), 16.0f, dipGUISizeUnit,
				Frame::TopEdge | Frame::RightEdge);
			backButtonPressedPanel->SetLayoutMethod(
				LayoutMethod(LayoutMethod::Linear));
			backButtonPressedPanel->AddWidget(std::make_shared<StaticText>(
				"BaCk", font, 32.0f, dipGUISizeUnit));

			auto backButtonReleasedPanel = std::make_shared<Panel>();
			backButtonReleasedPanel->SetBackground(
				"", Vec4f(0.0f, 1.0f, 0.0f, 1.0f));
			backButtonReleasedPanel->SetBorder(
				"GUI/edgetest.png", "GUI/cornertest.png",
				Vec4f(0.0f, 1.0f, 0.0f, 1.0f), 16.0f, dipGUISizeUnit,
				Frame::TopEdge | Frame::RightEdge);
			backButtonReleasedPanel->SetLayoutMethod(
				LayoutMethod(LayoutMethod::Linear));
			backButtonReleasedPanel->AddWidget(std::make_shared<StaticText>(
				"BaCk", font, 32.0f, dipGUISizeUnit));

			m_backButton = std::make_shared<Button>(
				backButtonPressedPanel, backButtonReleasedPanel);
			m_backButton->SetListener(this);

			LayoutProperties buttonLayoutProps;
			buttonLayoutProps.m_aspectRatio = 1.5f;
			buttonLayoutProps.m_growVertically = true;
			buttonLayoutProps.m_growHorizontally = true;
			m_backButton->SetLayoutProperties(buttonLayoutProps);

			GUILeftPanel->AddWidget(m_backButton);
		}

		// GUI Right
		{
			GUIRightPanel->SetLayoutMethod(LayoutMethod(
				LayoutMethod::Linear, LayoutMethod::Horizontal,
				LayoutMethod::Right, LayoutMethod::VCenter));

			auto restartButtonPressedPanel = std::make_shared<Panel>();
			restartButtonPressedPanel->SetBackground(
				"", Vec4f(0.0f, 1.0f, 0.0f, 0.5f));
			restartButtonPressedPanel->SetBorder(
				"GUI/edgetest.png", "GUI/cornertest.png",
				Vec4f(0.0f, 1.0f, 0.0f, 0.5f), 16.0f, dipGUISizeUnit,
				Frame::TopEdge | Frame::LeftEdge);
			restartButtonPressedPanel->SetLayoutMethod(
				LayoutMethod(LayoutMethod::Linear));
			restartButtonPressedPanel->AddWidget(std::make_shared<StaticText>(
				"ReStArT", font, 32.0f, dipGUISizeUnit));

			auto restartButtonReleasedPanel = std::make_shared<Panel>();
			restartButtonReleasedPanel->SetBackground(
				"", Vec4f(0.0f, 1.0f, 0.0f, 1.0f));
			restartButtonReleasedPanel->SetBorder(
				"GUI/edgetest.png", "GUI/cornertest.png",
				Vec4f(0.0f, 1.0f, 0.0f, 1.0f), 16.0f, dipGUISizeUnit,
				Frame::TopEdge | Frame::LeftEdge);
			restartButtonReleasedPanel->SetLayoutMethod(
				LayoutMethod(LayoutMethod::Linear));
			restartButtonReleasedPanel->AddWidget(std::make_shared<StaticText>(
				"ReStArT", font, 32.0f, dipGUISizeUnit));

			m_restartButton = std::make_shared<Button>(
				restartButtonPressedPanel, restartButtonReleasedPanel);
			m_restartButton->SetListener(this);

			LayoutProperties buttonLayoutProps;
			buttonLayoutProps.m_aspectRatio = 1.5f;
			buttonLayoutProps.m_growVertically = true;
			buttonLayoutProps.m_growHorizontally = true;
			m_restartButton->SetLayoutProperties(buttonLayoutProps);

			GUIRightPanel->AddWidget(m_restartButton);
		}
	}

	SetState(GameViewStateStart);
}

void GameView::PanelInput(UUID panelId, const InputEvent& event)
{
	if (panelId == m_renderPanel->GetId())
	{
		if (event.m_action == PointerDownInputEventAction)
		{
			m_client->ScreenTouched();
		}
	}
	else if (panelId == m_pausePanel->GetId())
	{
		if (event.m_action == PointerDownInputEventAction)
		{
			m_client->Unpause();
			StateTransition(GameViewStatePlaying);
		}
	}
}

void GameView::ButtonPushed(UUID buttonId)
{
	if (buttonId == m_restartButton->GetId())
	{
		m_client->Restart();
		StateTransition(GameViewStateStart);
	}
	else if (buttonId == m_backButton->GetId())
	{
		if (m_viewState == GameViewStatePlaying)
		{
			m_client->Pause();
		}
		StateTransition(GameViewStatePaused);
		m_backButtonCallback();
	}
}

void GameView::DPadInput(UUID DPadId, DPadAction action, Vec2f position)
{
	if (DPadId == m_DPad->GetId())
	{
		if (action == DPadActionRelease)
		{
			const float DPadCornerDist = sqrt(
				position.x * position.x + (1 - position.y) * (1 - position.y));
			const float strength = std::min(DPadCornerDist, 1.0f) * 30.0f;
			const float angle = acosf(position.x / DPadCornerDist);

			m_client->KickOff(angle, strength);
			StateTransition(GameViewStatePlaying);
		}
	}
}

bool GameView::StateTransition(GameViewState viewState)
{
	bool success = false;

	switch (m_viewState)
	{
	case GameViewStateStart:
		success = viewState == GameViewStatePlaying;
		break;

	case GameViewStatePlaying:
		success = viewState != GameViewStatePlaying;
		break;

	case GameViewStatePaused:
		success = viewState == GameViewStateStart
			|| viewState == GameViewStatePlaying;
		break;

	case GameViewStateStoped:
		success = viewState == GameViewStateStart;
		break;
	}

	if (success)
	{
		SetState(viewState);
	}

	return success;
}

void GameView::SetState(GameViewState viewState)
{
	switch (viewState)
	{
	case GameViewStateStart:
		m_pausePanel->SetVisible(false);
		m_restartButton->SetVisible(false);
		m_DPad->SetVisible(true);
		break;

	case GameViewStatePlaying:
		m_pausePanel->SetVisible(false);
		m_restartButton->SetVisible(false);
		m_DPad->SetVisible(false);
		break;

	case GameViewStatePaused:
		m_pausePanel->SetVisible(true);
		m_restartButton->SetVisible(false);
		m_DPad->SetVisible(false);
		break;

	case GameViewStateStoped:
		m_pausePanel->SetVisible(false);
		m_restartButton->SetVisible(true);
		m_DPad->SetVisible(false);
		break;
	}

	m_viewState = viewState;
}
