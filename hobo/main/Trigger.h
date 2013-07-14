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

#ifndef HOBO_MAIN_TRIGGER_H
#define HOBO_MAIN_TRIGGER_H

#include <memory>

#include "nanaka/game/GameObject.h"
#include "nanaka/physics/CollisionListener.h"

enum TriggerType
{
	TriggerTypeBoost30,
	TriggerTypeBoost45,
	TriggerTypeBoost60,
	TriggerTypeSlow,
	TriggerTypeAngle,
	TriggerTypeStop,
	TriggerTypeBlock,
};

enum TriggerAction
{
	TriggerActionBoost30,
	TriggerActionBoost30Combo,
	TriggerActionBoost45,
	TriggerActionBoost45Combo,
	TriggerActionBoost45ComboFollowup,
	TriggerActionBoost60,
	TriggerActionBoost60Combo,
	TriggerActionBoost60ComboFollowup,
	TriggerActionSlow,
	TriggerActionAngle,
	TriggerActionStop,
	TriggerActionStopCombo,
	TriggerActionBlock,
	TriggerActionUseBlock,
	TriggerActionBlock45Combo,
	TriggerActionBlock45ComboFollowup,
	TriggerActionBlock60Combo,
	TriggerActionBlock60ComboFollowup,
};

class TimeController;
class ComboController;
class Player;

class Trigger final : public GameObject, public CollisionListener
{
public:

	Trigger(
		TriggerType type,
		Player& player,
		ComboController& comboController,
		TimeController& timeController);

	void Initialize();

	void SetDefaultAction();
	void SetAction(TriggerAction action);
	void PerformAction();

	void SetNextTrigger(Trigger* nextTrigger);

	TriggerType GetType() const;

	/**
	 * Collision implementation.
	 */
	void OnCollision(const Collision& collision) override;

private:

	static const float s_playerSlowdownVelScale;

	TriggerType m_type;
	TriggerAction m_action;
	Trigger* m_nextTrigger;

	TimeController& m_timeController;
	ComboController& m_comboController;
	Player& m_player;

	bool m_collisionRegistered;
};

inline TriggerType Trigger::GetType() const
{
	return m_type;
}

inline void Trigger::SetAction(TriggerAction action)
{
	m_action = action;
}

inline void Trigger::SetNextTrigger(Trigger* nextTrigger)
{
	m_nextTrigger = nextTrigger;
}

#endif // HOBO_MAIN_TRIGGER_H
