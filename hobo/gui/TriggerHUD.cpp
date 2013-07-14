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

#include "gui/TriggerHUD.h"

#include "nanaka/gui/Panel.h"

TriggerHUD::TriggerHUD(std::shared_ptr<Panel> container)
	: m_first(NoneTriggerHUDIndicator)
	, m_second(NoneTriggerHUDIndicator)
	, m_third(NoneTriggerHUDIndicator)
{
	LayoutProperties containerLayoutProps;
	containerLayoutProps.m_aspectRatio = 3.0f;
	container->SetLayoutProperties(containerLayoutProps);

	container->SetLayoutMethod(LayoutMethod(
		LayoutMethod::Linear, LayoutMethod::Horizontal,
		LayoutMethod::Left, LayoutMethod::VCenter));

	auto leftSpacerPanel = std::make_shared<Panel>();
	m_indicators = std::make_shared<Panel>();
	auto rightSpacerPanel = std::make_shared<Panel>();

	container->AddWidget(leftSpacerPanel);
	container->AddWidget(m_indicators);
	container->AddWidget(rightSpacerPanel);

	LayoutProperties indicatorsLayoutProps;
	indicatorsLayoutProps.m_sizeWeight = 2.0f;
	indicatorsLayoutProps.m_aspectRatio = 3.0f;
	m_indicators->SetLayoutProperties(indicatorsLayoutProps);

	m_indicators->SetLayoutMethod(LayoutMethod(
		LayoutMethod::Linear, LayoutMethod::Horizontal,
		LayoutMethod::Left, LayoutMethod::Top));

	m_boost30Panel = std::make_shared<Panel>();
	m_boost30Panel->SetBackground("GUI/HUD/Boost30Indicator.png", Vec4f(1.0f));
	m_boost45Panel = std::make_shared<Panel>();
	m_boost45Panel->SetBackground("GUI/HUD/Boost45Indicator.png", Vec4f(1.0f));
	m_boost60Panel = std::make_shared<Panel>();
	m_boost60Panel->SetBackground("GUI/HUD/Boost60Indicator.png", Vec4f(1.0f));
	m_slowPanel = std::make_shared<Panel>();
	m_slowPanel->SetBackground("GUI/HUD/SlowIndicator.png", Vec4f(1.0f));
	m_anglePanel = std::make_shared<Panel>();
	m_anglePanel->SetBackground("GUI/HUD/AngleIndicator.png", Vec4f(1.0f));
	m_stopPanel = std::make_shared<Panel>();
	m_stopPanel->SetBackground("GUI/HUD/StopIndicator.png", Vec4f(1.0f));
	m_blockPanel = std::make_shared<Panel>();
	m_blockPanel->SetBackground("GUI/HUD/BlockIndicator.png", Vec4f(1.0f));
}

void TriggerHUD::SetIndicators(
	TriggerHUDIndicator first,
	TriggerHUDIndicator second,
	TriggerHUDIndicator third)
{
	if (m_first != first || m_second != second || m_third != third)
	{
		RemoveIndicator(m_first);
		RemoveIndicator(m_second);
		RemoveIndicator(m_third);
		AddIndicator(first);
		AddIndicator(second);
		AddIndicator(third);
	}
	m_first = first;
	m_second = second;
	m_third = third;
}

std::shared_ptr<Panel> TriggerHUD::GetIndicatorPanel(
	TriggerHUDIndicator indicator) const
{
	std::shared_ptr<Panel> ret;
	switch (indicator)
	{
	case Boost30TriggerHUDIndicator:
		ret = m_boost30Panel;
		break;

	case Boost45TriggerHUDIndicator:
		ret = m_boost45Panel;
		break;

	case Boost60TriggerHUDIndicator:
		ret = m_boost60Panel;
		break;

	case SlowTriggerHUDIndicator:
		ret = m_slowPanel;
		break;

	case AngleTriggerHUDIndicator:
		ret = m_anglePanel;
		break;

	case StopTriggerHUDIndicator:
		ret = m_stopPanel;
		break;

	case BlockTriggerHUDIndicator:
		ret = m_blockPanel;
		break;

	default:
		break;
	}
	return ret;
}

void TriggerHUD::RemoveIndicator(TriggerHUDIndicator indicator)
{
	auto indicatorPanel = GetIndicatorPanel(indicator);
	if (indicatorPanel)
	{
		indicatorPanel->RemoveFromParent();
	}
}

void TriggerHUD::AddIndicator(TriggerHUDIndicator indicator)
{
	auto indicatorPanel = GetIndicatorPanel(indicator);
	if (indicatorPanel)
	{
		m_indicators->AddWidget(indicatorPanel);
	}
}
