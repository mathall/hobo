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

#include "gui/ComboHUD.h"

#include "nanaka/gui/Counter.h"
#include "nanaka/gui/Panel.h"

const float ComboHUD::s_activeAlpha = 1.0f;
const float ComboHUD::s_inactiveAlpha = 0.2f;

ComboHUD::ComboHUD(
	std::shared_ptr<Panel> container,
	std::shared_ptr<Counter> comboTimer)
{
	LayoutProperties containerLayoutProps;
	containerLayoutProps.m_aspectRatio = 4.0f;
	container->SetLayoutProperties(containerLayoutProps);

	m_block45Panel = std::make_shared<Panel>();
	m_block45Panel->SetBackground(
		"GUI/HUD/Block45ComboLayover.png", Vec4f(1.0f));
	m_block60Panel = std::make_shared<Panel>();
	m_block60Panel->SetBackground(
		"GUI/HUD/Block60ComboLayover.png", Vec4f(1.0f));
	m_passPanel = std::make_shared<Panel>();
	m_passPanel->SetBackground("GUI/HUD/PassComboLayover.png", Vec4f(1.0f));

	LayoutProperties layoverPanelsLayoutProps;
	layoverPanelsLayoutProps.m_z = 1;

	m_block45Panel->SetLayoutProperties(layoverPanelsLayoutProps);
	m_block60Panel->SetLayoutProperties(layoverPanelsLayoutProps);
	m_passPanel->SetLayoutProperties(layoverPanelsLayoutProps);

	m_combos = std::make_shared<Panel>();
	m_combos->SetLayoutMethod(LayoutMethod(
		LayoutMethod::Linear, LayoutMethod::Horizontal,
		LayoutMethod::Left, LayoutMethod::Bottom));

	Vec4f panelColor(1.0f, 1.0f, 1.0f, s_inactiveAlpha);
	m_stopPanel = std::make_shared<Panel>();
	m_stopPanel->SetBackground("GUI/HUD/StopCombo.png", panelColor);
	m_boost30Panel = std::make_shared<Panel>();
	m_boost30Panel->SetBackground("GUI/HUD/Boost30Combo.png", panelColor);
	m_boost45Panel = std::make_shared<Panel>();
	m_boost45Panel->SetBackground("GUI/HUD/Boost45Combo.png", panelColor);
	m_boost60Panel = std::make_shared<Panel>();
	m_boost60Panel->SetBackground("GUI/HUD/Boost60Combo.png", panelColor);

	m_combos->AddWidget(m_stopPanel);
	m_combos->AddWidget(m_boost30Panel);
	m_combos->AddWidget(m_boost45Panel);
	m_combos->AddWidget(m_boost60Panel);

	container->AddWidget(m_combos);
	container->AddWidget(m_block45Panel);
	container->AddWidget(m_block60Panel);
	container->AddWidget(m_passPanel);

	m_comboTimer = comboTimer;
}

void ComboHUD::SetComboAvailability(
	bool stop,
	bool boost30,
	bool boost45,
	bool boost60,
	bool block45,
	bool block60,
	bool pass)
{
	m_stopPanel->SetAlpha(stop ? s_activeAlpha : s_inactiveAlpha);
	m_boost30Panel->SetAlpha(boost30 ? s_activeAlpha : s_inactiveAlpha);
	m_boost45Panel->SetAlpha(boost45 ? s_activeAlpha : s_inactiveAlpha);
	m_boost60Panel->SetAlpha(boost60 ? s_activeAlpha : s_inactiveAlpha);

	m_block45Panel->SetAlpha(block45 ? 1.0f : 0.0f);
	m_block60Panel->SetAlpha(block60 ? 1.0f : 0.0f);
	m_passPanel->SetAlpha(pass ? 1.0f : 0.0f);
}

void ComboHUD::SetComboActive(bool active, float timeLeft)
{
	m_comboTimer->SetVisible(active);
	m_comboTimer->SetNumber(static_cast<int>(100 * timeLeft));
}
