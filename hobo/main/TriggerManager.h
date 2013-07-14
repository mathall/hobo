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

#ifndef HOBO_MAIN_TRIGGERMANAGER_H
#define HOBO_MAIN_TRIGGERMANAGER_H

#include <memory>
#include <vector>

#include "nanaka/utils/Random.h"

#include "gui/TriggerHUD.h"

class ComboController;
class INanaka;
class Player;
class TimeController;
class Trigger;
class World;

class TriggerManager final
{
public:

	TriggerManager(
		INanaka* nanaka,
		Player& player,
		TimeController& timeController,
		ComboController& comboController,
		std::shared_ptr<World> world);

	void Update(float x, bool allowBlock);
	void UpdateTriggerHUD(TriggerHUD& triggerHUD) const;
	void Reset(float x);
	void KickOff();

private:

	static const float s_triggerDistance;

	void IntroduceTrigger(float x, bool allowBlock);
	TriggerHUDIndicator TriggerHUDIndicatorAt(size_t idx) const;

	float m_reloadX;
	float m_startX;

	std::vector<Trigger*> m_availableTriggers;
	std::vector<Trigger*> m_activeTriggers;

	ComboController& m_comboController;
	std::shared_ptr<World> m_world;

	Random m_random;
};

#endif // HOBO_MAIN_TRIGGERMANAGER_H
