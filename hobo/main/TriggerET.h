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

#ifndef HOBO_MAIN_TRIGGERET_H
#define HOBO_MAIN_TRIGGERET_H

#include "nanaka/entity/EntityType.h"
#include "nanaka/scene/SceneEM.h"

#include "main/Trigger.h"

class TimeController;
class ComboController;
class Player;

class TriggerEMParams final : public EntityModuleParameters
{
public:

	TriggerEMParams(
		TriggerType triggerType,
		Player& player,
		ComboController& comboController,
		TimeController& timeController);

	TriggerType m_triggerType;
	TimeController& m_timeController;
	ComboController& m_comboController;
	Player& m_player;
};

class TriggerET final : public EntityType
{
public:

	static const AttachmentId s_gfxIdActive = 0;
	static const AttachmentId s_gfxIdInactive = 1;

	TriggerET();

	/**
	 * EntityType implementation.
	 */
	void InitializeEntity(
		Entity& entity,
		const EntityModuleParameters* spawnParams) const override;
};

#endif // HOBO_MAIN_TRIGGERET_H
