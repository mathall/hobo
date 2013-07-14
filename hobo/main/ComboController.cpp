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

#include "main/ComboController.h"

#include <cassert>

#include "nanaka/main/World.h"
#include "nanaka/utils/Time.h"

#include "gui/ComboHUD.h"
#include "main/TimeController.h"

const float ComboController::s_timeToExecute = 700.0f;
const float ComboController::s_timeMaxScore = 500.0f;

ComboController::ComboController(TimeController& timeController)
	: m_timeController(timeController)
{
	Reset();
}

void ComboController::Reset()
{
	m_visibleTriggers.clear();

	m_comboActive = false;
	m_executionTimer = 0;

	m_lastHitGround = false;
	m_lastTriggerHitBoost30 = false;
	m_lastTriggerHitBoost = false;
	m_playerHasPass = false;

	m_availableCombos[ComboTypeBoost30] = false;
	m_availableCombos[ComboTypeBoost45] = false;
	m_availableCombos[ComboTypeBoost60] = false;
	m_availableCombos[ComboTypeStop] = false;
	m_availableCombos[ComboTypeBlock45] = false;
	m_availableCombos[ComboTypeBlock60] = false;
}

void ComboController::TriggerAppeared(TriggerType triggerType)
{
	m_visibleTriggers.push_back(triggerType);
	if (m_visibleTriggers.size() > s_maxTriggers)
	{
		m_visibleTriggers.erase(m_visibleTriggers.begin());
	}
	UpdateAvailableCombos();
}

void ComboController::GroundHit()
{
	m_lastHitGround = true;
	UpdateAvailableCombos();
}

void ComboController::TryActivateCombo(Trigger* hitTrigger)
{
	switch (hitTrigger->GetType())
	{
	case TriggerTypeStop:
		TryActivateCombo(ComboTypeStop);
		break;

	case TriggerTypeBoost30:
		TryActivateCombo(ComboTypeBoost30);
		break;

	case TriggerTypeBoost45:
		TryActivateCombo(ComboTypeBoost45);
		TryActivateCombo(ComboTypeBlock45);
		break;

	case TriggerTypeBoost60:
		TryActivateCombo(ComboTypeBoost60);
		TryActivateCombo(ComboTypeBlock60);
		break;

	default:
		break;
	}

	if (IsComboActivated())
	{
		m_activeTrigger = hitTrigger;
	}
}

void ComboController::TriggerHit(TriggerType triggerType)
{
	m_lastHitGround = false;
	m_lastTriggerHitBoost30 = triggerType == TriggerTypeBoost30;
	m_lastTriggerHitBoost = m_lastTriggerHitBoost30
		|| triggerType == TriggerTypeBoost45
		|| triggerType == TriggerTypeBoost60
		|| triggerType == TriggerTypeStop;
	UpdateAvailableCombos();
}

void ComboController::SetPlayerHasPass(bool hasPass)
{
	m_playerHasPass = hasPass;
	UpdateAvailableCombos();
}

void ComboController::Update(unsigned int realTickLength)
{
	if (IsComboActivated())
	{
		m_executionTimer -= realTickLength;

		if (m_executionTimer <= 0)
		{
			PerformTriggerAction();
		}
	}
}

float ComboController::ExecuteCombo()
{
	TriggerAction action;
	switch (m_activeComboType)
	{
	case ComboTypeBoost30:
		action = TriggerActionBoost30Combo;
		break;

	case ComboTypeBoost45:
		action = TriggerActionBoost45Combo;
		break;

	case ComboTypeBoost60:
		action = TriggerActionBoost60Combo;
		break;

	case ComboTypeStop:
		action = TriggerActionStopCombo;
		break;

	case ComboTypeBlock45:
		action = TriggerActionBlock45Combo;
		break;

	case ComboTypeBlock60:
		action = TriggerActionBlock60Combo;
		break;

	default:
		action = TriggerActionStop;
		assert("Bad m_activeComboType.");
		break;
	}
	m_activeTrigger->SetAction(action);

	PerformTriggerAction();

	return CalculateScore();
}

void ComboController::PerformTriggerAction()
{
	m_activeTrigger->PerformAction();
	m_comboActive = false;
}

void ComboController::UpdateAvailableCombos()
{
	bool boost45 = false;
	bool boost60 = false;

	auto current = m_visibleTriggers.begin();
	auto next = std::next(current, 1);
	while (next != m_visibleTriggers.end())
	{
		if (*current == TriggerTypeBoost45 && *next == TriggerTypeBoost60)
		{
			boost45 = true;
		}
		else if (*current == TriggerTypeBoost60 && *next == TriggerTypeBoost45)
		{
			boost60 = true;
		}
		current = next;
		next = std::next(current, 1);
	}

	m_availableCombos[ComboTypeBoost30] = m_lastTriggerHitBoost30;
	m_availableCombos[ComboTypeBoost45] = boost45 && !m_playerHasPass;
	m_availableCombos[ComboTypeBoost60] = boost60 && !m_playerHasPass;
	m_availableCombos[ComboTypeBlock45] = boost45 && m_playerHasPass;
	m_availableCombos[ComboTypeBlock60] = boost60 && m_playerHasPass;
	m_availableCombos[ComboTypeStop] =
		m_lastTriggerHitBoost && !m_lastHitGround;
}

void ComboController::TryActivateCombo(ComboType comboType)
{
	if (m_availableCombos[comboType])
	{
		ActivateCombo(comboType);
	}
}

void ComboController::ActivateCombo(ComboType comboType)
{
	m_timeController.Freeze();
	m_executionTimer = s_timeToExecute;
	m_activeComboType = comboType;
	m_comboActive = true;
}

void ComboController::UpdateComboHUD(ComboHUD& comboHUD) const
{
	comboHUD.SetComboAvailability(
		m_availableCombos[ComboTypeStop],
		m_availableCombos[ComboTypeBoost30],
		m_availableCombos[ComboTypeBoost45],
		m_availableCombos[ComboTypeBoost60],
		m_availableCombos[ComboTypeBlock45],
		m_availableCombos[ComboTypeBlock60],
		m_playerHasPass);
	comboHUD.SetComboActive(
		IsComboActivated(), m_executionTimer / s_timeToExecute);
}
