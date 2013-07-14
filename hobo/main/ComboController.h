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

#ifndef HOBO_MAIN_COMBOCONTROLLER_H
#define HOBO_MAIN_COMBOCONTROLLER_H

#include <array>
#include <list>

#include "main/Trigger.h"

class ComboHUD;
class TimeController;

enum ComboType
{
	ComboTypeBoost30,
	ComboTypeBoost45,
	ComboTypeBoost60,
	ComboTypeStop,
	ComboTypeBlock45,
	ComboTypeBlock60,
	ComboTypeNum,
};

class ComboController final
{
public:

	ComboController(TimeController& timeController);

	void Reset();
	void Update(unsigned int realTickLength);
	void UpdateComboHUD(ComboHUD& comboHUD) const;

	void TriggerAppeared(TriggerType triggerType);
	void TriggerHit(TriggerType triggerType);
	void GroundHit();
	void SetPlayerHasPass(bool hasPass);

	void TryActivateCombo(Trigger* hitTrigger);
	bool IsComboActivated() const;
	float ExecuteCombo();

private:

	void UpdateAvailableCombos();
	void TryActivateCombo(ComboType comboType);
	void ActivateCombo(ComboType comboType);
	void PerformTriggerAction();
	float CalculateScore() const;

	static const float s_timeMaxScore;
	static const float s_timeToExecute;
	static const size_t s_maxTriggers = 3;

	std::array<bool, ComboTypeNum> m_availableCombos;

	std::list<TriggerType> m_visibleTriggers;
	bool m_lastHitGround;
	bool m_lastTriggerHitBoost30;
	bool m_lastTriggerHitBoost;
	bool m_playerHasPass;

	int m_executionTimer;

	bool m_comboActive;
	ComboType m_activeComboType;
	Trigger* m_activeTrigger;

	TimeController& m_timeController;
};

inline float ComboController::CalculateScore() const
{
	return std::min(1.0f, m_executionTimer / s_timeMaxScore);
}

inline bool ComboController::IsComboActivated() const
{
	return m_comboActive;
}

#endif // HOBO_MAIN_COMBOCONTROLLER_H
