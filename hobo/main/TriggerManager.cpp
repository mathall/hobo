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

#include "main/TriggerManager.h"

#include "nanaka/entity/EntitySystem.h"
#include "nanaka/game/GameEM.h"
#include "nanaka/main/INanaka.h"
#include "nanaka/main/World.h"

#include "main/EntityTypes.h"
#include "main/Trigger.h"
#include "main/TriggerET.h"
#include "main/ComboController.h"

const float TriggerManager::s_triggerDistance = 10.0f;

TriggerManager::TriggerManager(
	INanaka* nanaka,
	Player& player,
	TimeController& timeController,
	ComboController& comboController,
	std::shared_ptr<World> world)
	: m_comboController(comboController)
	, m_world(world)
{
	std::vector<TriggerType> triggerTypes = {
		TriggerTypeBoost30,
		TriggerTypeBoost45,
		TriggerTypeBoost60,
		TriggerTypeSlow,
		TriggerTypeAngle,
		TriggerTypeStop,
		TriggerTypeBlock,
	};

	// Spawn one trigger of each type.
	for (auto triggerType : triggerTypes)
	{
		TriggerEMParams triggerParams(
			triggerType, player, comboController, timeController);
		auto trigger = nanaka->GetEntitySystem().SpawnEntity(
			TriggerEntityType, &triggerParams, &m_world->GetEntityTree());
		m_availableTriggers.push_back(
			&trigger->GetModule<GameEM>()->GetGameObject<Trigger>());
	}
}

void TriggerManager::Update(float x, bool allowBlock)
{
	while (m_reloadX < x)
	{
		// Return the left-most trigger.
		auto trigger = *m_activeTriggers.begin();
		m_activeTriggers.erase(m_activeTriggers.begin());

		// Introduce a new trigger.
		IntroduceTrigger(m_reloadX + s_triggerDistance * 2.8f, allowBlock);

		m_availableTriggers.push_back(trigger);

		// Move the reload position forward.
		m_reloadX += s_triggerDistance;
	}
}

void TriggerManager::Reset(float x)
{
	// Find good position for first trigger if the player starts at x.
	m_startX = s_triggerDistance + s_triggerDistance * static_cast<int>(
		(x + s_triggerDistance - 0.001f) / s_triggerDistance);

	// Return all active triggers to the available array.
	for (auto trigger : m_activeTriggers)
	{
		m_availableTriggers.push_back(trigger);
	}
	m_activeTriggers.clear();

	// Put all triggers in unreachable positions.
	for (auto trigger : m_availableTriggers)
	{
		trigger->GetEntity()->SetPosition(Vec3f(0.0f, 1.0f, -500.0f));
	}

	// Set reload position right after the first trigger.
	m_reloadX = m_startX + s_triggerDistance * 0.2f;
}

void TriggerManager::KickOff()
{
	// Introduce three new triggers.
	IntroduceTrigger(m_startX, true);
	IntroduceTrigger(m_startX + s_triggerDistance, true);
	IntroduceTrigger(m_startX + s_triggerDistance * 2, true);
}

void TriggerManager::IntroduceTrigger(float x, bool allowBlock)
{
	// Take a random available trigger.
	auto triggerIt = m_availableTriggers.begin()
		+ m_random.Int(m_availableTriggers.size());
	auto newTrigger = *triggerIt;
	m_availableTriggers.erase(triggerIt);

	// Make sure the block trigger does not appear if it's not allowed.
	if (!allowBlock && newTrigger->GetType() == TriggerTypeBlock)
	{
		triggerIt = m_availableTriggers.begin()
			+ m_random.Int(m_availableTriggers.size());
		auto newNewTrigger = *triggerIt;
		m_availableTriggers.erase(triggerIt);
		m_availableTriggers.push_back(newTrigger);
		newTrigger = newNewTrigger;
	}

	if (!m_activeTriggers.empty())
	{
		m_activeTriggers.back()->SetNextTrigger(newTrigger);
	}

	// Initialize the new trigger and place it in the world.
	newTrigger->Initialize();
	newTrigger->GetEntity()->SetPosition(Vec3f(x, 1.0f, 2.0f));
	m_activeTriggers.push_back(newTrigger);

	// Inform the combo system that a new trigger has appeared.
	m_comboController.TriggerAppeared(newTrigger->GetType());
}

void TriggerManager::UpdateTriggerHUD(TriggerHUD& triggerHUD) const
{
	triggerHUD.SetIndicators(
		TriggerHUDIndicatorAt(0),
		TriggerHUDIndicatorAt(1),
		TriggerHUDIndicatorAt(2));
}

TriggerHUDIndicator TriggerManager::TriggerHUDIndicatorAt(size_t idx) const
{
	if (m_activeTriggers.size() <= idx)
	{
		return NoneTriggerHUDIndicator;
	}

	TriggerHUDIndicator ret =  NoneTriggerHUDIndicator;

	switch (m_activeTriggers.at(idx)->GetType())
	{
		case TriggerTypeBoost30:
			ret = Boost30TriggerHUDIndicator;
			break;

		case TriggerTypeBoost45:
			ret = Boost45TriggerHUDIndicator;
			break;

		case TriggerTypeBoost60:
			ret = Boost60TriggerHUDIndicator;
			break;

		case TriggerTypeSlow:
			ret = SlowTriggerHUDIndicator;
			break;

		case TriggerTypeAngle:
			ret = AngleTriggerHUDIndicator;
			break;

		case TriggerTypeStop:
			ret = StopTriggerHUDIndicator;
			break;

		case TriggerTypeBlock:
			ret = BlockTriggerHUDIndicator;
			break;
	}

	return ret;
}
