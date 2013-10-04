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

#include "gui/PlayerHUD.h"

#include "nanaka/gui/Counter.h"
#include "nanaka/gui/Gauge.h"
#include "nanaka/gui/Image.h"
#include "nanaka/gui/Panel.h"
#include "nanaka/gui/StaticText.h"
#include "nanaka/utils/GlobalClock.h"

PlayerHUD::PlayerHUD(
	std::shared_ptr<Panel> FPSPanel,
	std::shared_ptr<Panel> soarPanel,
	std::shared_ptr<Panel> ascentPanel,
	std::shared_ptr<Panel> speedPanel,
	std::shared_ptr<Panel> scorePanel,
	std::shared_ptr<Panel> heightPanel,
	const Font& font)
{
	m_FPSCounter = std::make_shared<Counter>(
		font, 24.0f, dipGUISizeUnit, 5, 0, ' ');
	m_speedCounter = std::make_shared<Counter>(
		font, 24.0f, dipGUISizeUnit, 5, 2, ' ');
	m_distanceCounter = std::make_shared<Counter>(
		font, 24.0f, dipGUISizeUnit, 8, 2, ' ');

	m_soarGauge = std::make_shared<Gauge>(
		false, 16, 4, dipGUISizeUnit, "GUI/HUD/GaugeFullEnd.png",
		"GUI/HUD/GaugeEmptyEnd.png", "GUI/HUD/GaugeFullMiddle.png",
		"GUI/HUD/GaugeEmptyMiddle.png", Vec4f(1.0f));
	m_ascentGauge = std::make_shared<Gauge>(
		false, 16, 4, dipGUISizeUnit, "GUI/HUD/GaugeFullEnd.png",
		"GUI/HUD/GaugeEmptyEnd.png", "GUI/HUD/GaugeFullMiddle.png",
		"GUI/HUD/GaugeEmptyMiddle.png", Vec4f(1.0f));

	m_heightCounter = std::make_shared<Counter>(
		font, 24.0f, dipGUISizeUnit, 5, 2, ' ');
	m_heightPanelVisible = false;

	LayoutProperties growHorizontalPanelLayoutProps;
	growHorizontalPanelLayoutProps.m_growHorizontally = true;

	// Speed panel
	{
		speedPanel->SetLayoutMethod(LayoutMethod(
			LayoutMethod::Linear, LayoutMethod::Horizontal));
		speedPanel->SetLayoutProperties(growHorizontalPanelLayoutProps);

		auto speedLabel = std::make_shared<StaticText>(
			"Speed: ", font, 24.0f, dipGUISizeUnit);

		auto spacerPanel = std::make_shared<Panel>();
		spacerPanel->SetLayoutProperties(growHorizontalPanelLayoutProps);

		auto unitText = std::make_shared<StaticText>(
			"mps", font, 24.0f, dipGUISizeUnit);

		speedPanel->AddWidget(speedLabel);
		speedPanel->AddWidget(spacerPanel);
		speedPanel->AddWidget(m_speedCounter);
		speedPanel->AddWidget(unitText);
	}

	// Score panel
	{
		scorePanel->SetLayoutMethod(LayoutMethod(
			LayoutMethod::Linear, LayoutMethod::Horizontal));
		scorePanel->SetLayoutProperties(growHorizontalPanelLayoutProps);

		auto scoreLabel = std::make_shared<StaticText>(
			"Distance: ", font, 24.0f, dipGUISizeUnit);

		auto spacerPanel = std::make_shared<Panel>();
		spacerPanel->SetLayoutProperties(growHorizontalPanelLayoutProps);

		auto unitText = std::make_shared<StaticText>(
			"m  ", font, 24.0f, dipGUISizeUnit);

		scorePanel->AddWidget(scoreLabel);
		scorePanel->AddWidget(spacerPanel);
		scorePanel->AddWidget(m_distanceCounter);
		scorePanel->AddWidget(unitText);
	}

	// FPS panel
	{
		FPSPanel->SetLayoutMethod(LayoutMethod(
			LayoutMethod::Linear, LayoutMethod::Horizontal,
			LayoutMethod::Left));
		FPSPanel->SetLayoutProperties(growHorizontalPanelLayoutProps);

		auto FPSLabel = std::make_shared<StaticText>(
			"FpS: ", font, 24.0f, dipGUISizeUnit);

		FPSPanel->AddWidget(FPSLabel);
		FPSPanel->AddWidget(m_FPSCounter);
	}

	// Soar panel
	{
		soarPanel->SetLayoutMethod(LayoutMethod(
			LayoutMethod::Linear, LayoutMethod::Horizontal));
		soarPanel->SetLayoutProperties(growHorizontalPanelLayoutProps);

		auto soarLabel = std::make_shared<StaticText>(
			"SoAr:   ", font, 24.0f, dipGUISizeUnit);

		soarPanel->AddWidget(soarLabel);
		soarPanel->AddWidget(m_soarGauge);
	}

	// Ascent panel
	{
		ascentPanel->SetLayoutMethod(LayoutMethod(
			LayoutMethod::Linear, LayoutMethod::Horizontal));
		ascentPanel->SetLayoutProperties(growHorizontalPanelLayoutProps);

		auto ascentLabel = std::make_shared<StaticText>(
			"AsCeNt: ", font, 24.0f, dipGUISizeUnit);

		ascentPanel->AddWidget(ascentLabel);
		ascentPanel->AddWidget(m_ascentGauge);
	}

	// Height panel
	{
		m_heightPanel = heightPanel;
		m_heightPanel->SetLayoutMethod(LayoutMethod(LayoutMethod::Linear));

		auto upArrowImage = std::make_shared<Image>(
			"GUI/HUD/HeightArrow.png", Vec4f(1.0f), Vec2f(0.0f), pxGUISizeUnit);
		LayoutProperties layoutProps;
		layoutProps.m_aspectRatio = 0.5f;
		upArrowImage->SetLayoutProperties(layoutProps);

		auto heightTextPanel = std::make_shared<Panel>();

		// Height text panel
		{
			heightTextPanel->SetLayoutMethod(LayoutMethod(
				LayoutMethod::Linear, LayoutMethod::Horizontal));

			auto heightLabel = std::make_shared<StaticText>(
				"HeIgHt: ", font, 24.0f, dipGUISizeUnit);

			heightTextPanel->AddWidget(heightLabel);
			heightTextPanel->AddWidget(m_heightCounter);
		}

		m_heightPanel->AddWidget(upArrowImage);
		m_heightPanel->AddWidget(heightTextPanel);

		m_heightPanel->SetVisible(false);
	}
}

void PlayerHUD::Update(
	float speed,
	float distance,
	float soarLoad,
	float ascentLoad,
	float height)
{
	m_FPSCounter->SetNumber(g_clock->GetFPS());
	m_speedCounter->SetNumber(speed);
	m_distanceCounter->SetNumber(distance);

	m_soarGauge->SetLoad(soarLoad);
	m_ascentGauge->SetLoad(ascentLoad);

	bool heightPanelVisible = height > 15.f;
	if (heightPanelVisible != m_heightPanelVisible)
	{
		m_heightPanelVisible = heightPanelVisible;
		m_heightPanel->SetVisible(m_heightPanelVisible);
	}
	if (m_heightPanelVisible)
	{
		m_heightCounter->SetNumber(height);
	}
}
