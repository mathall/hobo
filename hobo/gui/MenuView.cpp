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

#include "gui/MenuView.h"

#include "nanaka/gui/Image.h"
#include "nanaka/gui/Panel.h"
#include "nanaka/gui/RenderTargetPanel.h"
#include "nanaka/gui/StaticText.h"

MenuView::MenuView(
	std::function<void(void)> playButtonCallback,
	const Font& font)
	: m_viewState(MenuViewStateNonstarted)
	, m_playButtonCallback(playButtonCallback)
{
	// Render panel
	m_renderPanel = std::make_shared<RenderTargetPanel>();

	// Button panel
	auto buttonPanel = std::make_shared<Panel>();

	LayoutProperties buttonPanelLayoutProps;
	buttonPanelLayoutProps.m_z = 1;
	buttonPanel->SetLayoutProperties(buttonPanelLayoutProps);

	// Exit dialogue panel
	m_exitDialogue = std::make_shared<Panel>();
	m_exitDialogue->SetSwallowsInput(true);

	LayoutProperties exitDialogueLayoutProps;
	exitDialogueLayoutProps.m_z = 2;
	m_exitDialogue->SetLayoutProperties(exitDialogueLayoutProps);

	// View
	m_viewPanel = std::make_shared<Panel>();
	m_viewPanel->AddWidget(m_renderPanel);
	m_viewPanel->AddWidget(buttonPanel);
	m_viewPanel->AddWidget(m_exitDialogue);

	// Button panel
	{
		buttonPanel->SetLayoutMethod(LayoutMethod(
			LayoutMethod::Linear, LayoutMethod::Horizontal));

		auto leftPanel = std::make_shared<Panel>();
		auto middlePanel = std::make_shared<Panel>();
		auto rightPanel = std::make_shared<Panel>();

		LayoutProperties middlePanelLayoutProps;
		middlePanelLayoutProps.m_sizeWeight = 2.0f;
		middlePanel->SetLayoutProperties(middlePanelLayoutProps);

		buttonPanel->AddWidget(leftPanel);
		buttonPanel->AddWidget(middlePanel);
		buttonPanel->AddWidget(rightPanel);

		// Left buttons
		{
			leftPanel->SetLayoutMethod(LayoutMethod(
				LayoutMethod::Linear, LayoutMethod::Vertical,
				LayoutMethod::HCenter, LayoutMethod::VFillout));

			m_playButton = CreateButton("PlAy", false, font);
			m_resumeButton = CreateButton("ReSuMe", false, font);
			m_howtoButton = CreateButton("HoW To", false, font);
			m_highscoreButton = CreateButton("ScOrE\nBoArD", false, font);

			leftPanel->AddWidget(m_resumeButton);
			leftPanel->AddWidget(m_playButton);
			leftPanel->AddWidget(m_howtoButton);
			leftPanel->AddWidget(m_highscoreButton);
		}

		// Right buttons
		{
			rightPanel->SetLayoutMethod(LayoutMethod(
				LayoutMethod::Linear, LayoutMethod::Vertical,
				LayoutMethod::HCenter, LayoutMethod::VFillout));

			m_optionsButton = CreateButton("OpTiOnS", true, font);
			m_aboutButton = CreateButton("AbOuT", true, font);
			m_exitButton = CreateButton("ExIt", true, font);

			rightPanel->AddWidget(m_optionsButton);
			rightPanel->AddWidget(m_aboutButton);
			rightPanel->AddWidget(m_exitButton);
		}

		// Title
		{
			middlePanel->SetLayoutMethod(LayoutMethod(
				LayoutMethod::Linear, LayoutMethod::Vertical,
				LayoutMethod::HCenter, LayoutMethod::Top));

			auto spacerPanel = std::make_shared<Panel>();
			LayoutProperties spacerProps;
			spacerProps.m_aspectRatio = 4.0f;
			spacerPanel->SetLayoutProperties(spacerProps);

			auto titleImage = std::make_shared<Image>(
				"GUI/Menu/Title.png", Vec4f(1.0f), Vec2f(0.0f), pxGUISizeUnit);
			LayoutProperties titleImageLayoutProps;
			titleImageLayoutProps.m_aspectRatio = 2.0f;
			titleImage->SetLayoutProperties(titleImageLayoutProps);

			middlePanel->AddWidget(spacerPanel);
			middlePanel->AddWidget(titleImage);
		}
	}

	// Exit dialogue
	{
		auto shadingPanel = std::make_shared<Panel>();
		shadingPanel->SetBackground("", Vec4f(0.0f, 0.0f, 0.0f, 0.5f));

		auto popupPanel = std::make_shared<Panel>();
		popupPanel->SetBackground("", Vec4f(0.0f, 1.0f, 0.0f, 1.0f));
		popupPanel->SetBorder(
			"GUI/edgetest.png", "GUI/cornertest.png",
			Vec4f(0.0f, 1.0f, 0.0f, 1.0f), 32.0f, dipGUISizeUnit,
			Frame::AllEdges);

		LayoutProperties popupPanelLayoutProps;
		popupPanelLayoutProps.m_relPosition = Vec2f(0.1f, 0.1f);
		popupPanelLayoutProps.m_relSize = Vec2f(0.8f, 0.8f);
		popupPanelLayoutProps.m_z = 1;
		popupPanel->SetLayoutProperties(popupPanelLayoutProps);

		m_exitDialogue->AddWidget(shadingPanel);
		m_exitDialogue->AddWidget(popupPanel);

		// Popup
		{
			auto exitQuestionPanel = std::make_shared<Panel>();
			exitQuestionPanel->SetLayoutMethod(LayoutMethod(
				LayoutMethod::Linear, LayoutMethod::Horizontal));
			exitQuestionPanel->AddWidget(std::make_shared<StaticText>(
				"Do YoU rEaLlY wAnT tO eXiT?", font, 48.0f, dipGUISizeUnit));

			LayoutProperties exitQuestionLayoutProps;
			exitQuestionLayoutProps.m_relPosition = Vec2f(0.0f, 0.0f);
			exitQuestionLayoutProps.m_relSize = Vec2f(1.0f, 0.2f);
			exitQuestionPanel->SetLayoutProperties(exitQuestionLayoutProps);

			m_exitYetButton = CreateButton("YeS", false, font);
			m_exitNoButton = CreateButton("No", true, font);

			LayoutProperties yesButtonLayoutProps;
			yesButtonLayoutProps.m_relPosition = Vec2f(0.0f, 0.3f);
			yesButtonLayoutProps.m_relSize = Vec2f(0.5f, 0.7f);
			m_exitYetButton->SetLayoutProperties(yesButtonLayoutProps);

			LayoutProperties noButtonLayoutProps;
			noButtonLayoutProps.m_relPosition = Vec2f(0.5f, 0.3f);
			noButtonLayoutProps.m_relSize = Vec2f(0.5f, 0.7f);
			m_exitNoButton->SetLayoutProperties(noButtonLayoutProps);

			popupPanel->AddWidget(exitQuestionPanel);
			popupPanel->AddWidget(m_exitYetButton);
			popupPanel->AddWidget(m_exitNoButton);
		}
	}

	SetState(m_viewState);
}

void MenuView::ButtonPushed(UUID buttonId)
{
	if (buttonId == m_playButton->GetId())
	{
		StateTransition(MenuViewStateStarted);
		m_playButtonCallback();
	}
	else if (buttonId == m_resumeButton->GetId())
	{
		m_playButtonCallback();
	}
	else if (buttonId == m_exitButton->GetId())
	{
		if (m_viewState == MenuViewStateNonstarted)
		{
			StateTransition(MenuViewStateExitNonstarted);
		}
		else if (m_viewState == MenuViewStateStarted)
		{
			StateTransition(MenuViewStateExitStarted);
		}
	}
	else if (buttonId == m_exitYetButton->GetId())
	{
		m_client->Shutdown();
	}
	else if (buttonId == m_exitNoButton->GetId())
	{
		if (m_viewState == MenuViewStateExitNonstarted)
		{
			StateTransition(MenuViewStateNonstarted);
		}
		else if (m_viewState == MenuViewStateExitStarted)
		{
			StateTransition(MenuViewStateStarted);
		}
	}
}

bool MenuView::StateTransition(MenuViewState viewState)
{
	bool success = false;

	switch (m_viewState)
	{
	case MenuViewStateNonstarted:
		success = viewState == MenuViewStateStarted
			|| viewState == MenuViewStateExitNonstarted;
		break;

	case MenuViewStateStarted:
		success = viewState == MenuViewStateExitStarted;
		break;

	case MenuViewStateExitNonstarted:
		success = viewState == MenuViewStateNonstarted;
		break;

	case MenuViewStateExitStarted:
		success = viewState == MenuViewStateStarted;
		break;
	}

	if (success)
	{
		SetState(viewState);
	}

	return success;
}

void MenuView::SetState(MenuViewState viewState)
{
	switch (viewState)
	{
	case MenuViewStateNonstarted:
		m_playButton->SetVisible(true);
		m_resumeButton->SetVisible(false);
		m_exitDialogue->SetVisible(false);
		break;

	case MenuViewStateStarted:
		m_playButton->SetVisible(false);
		m_resumeButton->SetVisible(true);
		m_exitDialogue->SetVisible(false);
		break;

	case MenuViewStateExitNonstarted:
		m_playButton->SetVisible(true);
		m_resumeButton->SetVisible(false);
		m_exitDialogue->SetVisible(true);
		break;

	case MenuViewStateExitStarted:
		m_playButton->SetVisible(false);
		m_resumeButton->SetVisible(true);
		m_exitDialogue->SetVisible(true);
		break;
	}

	m_viewState = viewState;
}

std::shared_ptr<Button> MenuView::CreateButton(
	std::string buttonText,
	bool flipH,
	const Font& font)
{
	std::shared_ptr<Button> button;

	auto buttonPressedState = std::make_shared<Panel>();
	buttonPressedState->SetLayoutMethod(LayoutMethod(
		LayoutMethod::Linear, LayoutMethod::Vertical,
		LayoutMethod::HCenter, LayoutMethod::VCenter));
	buttonPressedState->SetBackground(
		"GUI/Menu/Button.png", Vec4f(1.0f), ImageRotationNone, flipH);
	buttonPressedState->AddWidget(std::make_shared<StaticText>(
		buttonText, font, 32.0f, dipGUISizeUnit));

	auto buttonReleasedState = std::make_shared<Panel>();
	buttonReleasedState->SetLayoutMethod(LayoutMethod(
		LayoutMethod::Linear, LayoutMethod::Vertical,
		LayoutMethod::HCenter, LayoutMethod::VCenter));
	buttonReleasedState->SetBackground(
		"GUI/Menu/Button.png", Vec4f(1.0f), ImageRotationNone, flipH);
	buttonReleasedState->AddWidget(std::make_shared<StaticText>(
		buttonText, font, 32.0f, dipGUISizeUnit));

	button = std::make_shared<Button>(buttonPressedState, buttonReleasedState);
	button->SetListener(this);

	LayoutProperties buttonLayoutProps;
	buttonLayoutProps.m_aspectRatio = 2.0f;
	buttonLayoutProps.m_growHorizontally = true;
	buttonLayoutProps.m_growVertically = true;
	button->SetLayoutProperties(buttonLayoutProps);

	return button;
}
