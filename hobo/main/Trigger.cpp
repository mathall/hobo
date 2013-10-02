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

#include "main/Trigger.h"

#include <cassert>

#include "nanaka/entity/Entity.h"
#include "nanaka/scene/SceneEM.h"

#include "main/ComboController.h"
#include "main/Player.h"
#include "main/TimeController.h"
#include "main/TriggerET.h"

const float Trigger::s_playerSlowdownVelScale = 0.55f;

Trigger::Trigger(
	TriggerType type,
	Player& player,
	ComboController& comboController,
	TimeController& timeController)
	: m_type(type)
	, m_timeController(timeController)
	, m_comboController(comboController)
	, m_player(player)
	, m_sound("Sounds/trigger.ogg")
{
}

void Trigger::Initialize()
{
	m_collisionRegistered = false;

	auto sceneEM = GetEntity()->GetModule<SceneEM>();
	sceneEM->SetVisibility(TriggerET::s_gfxIdActive, true);
	sceneEM->SetVisibility(TriggerET::s_gfxIdInactive, false);

	SetDefaultAction();
}

void Trigger::OnCollision(const Collision& collision)
{
	if (!m_collisionRegistered)
	{
		switch (m_player.GetPass())
		{
		case PlayerPassRegular:
			SetAction(TriggerActionUseBlock);
			break;

		case PlayerPassSuper:
			if (m_type == TriggerTypeSlow
				|| m_type == TriggerTypeAngle
				|| m_type == TriggerTypeStop)
			{
				SetAction(TriggerActionUseBlock);
			}
			break;

		case PlayerPassNone:
			break;
		}

		m_comboController.TryActivateCombo(this);

		if (!m_comboController.IsComboActivated())
		{
			PerformAction();
		}

		m_player.TriggerCollision();

		m_sound.PlayOnce();
	}
	m_collisionRegistered = true;
}

void Trigger::SetDefaultAction()
{
	TriggerAction action;

	switch(m_type)
	{
	case TriggerTypeBoost30:
		action = TriggerActionBoost30;
		break;

	case TriggerTypeBoost45:
		action = TriggerActionBoost45;
		break;

	case TriggerTypeBoost60:
		action = TriggerActionBoost60;
		break;

	case TriggerTypeSlow:
		action = TriggerActionSlow;
		break;

	case TriggerTypeAngle:
		action = TriggerActionAngle;
		break;

	case TriggerTypeStop:
		action = TriggerActionStop;
		break;

	case TriggerTypeBlock:
		action = TriggerActionBlock;
		break;

	default:
		action = TriggerActionStop;
		assert("Bad m_type.");
		break;
	}

	SetAction(action);
}

void Trigger::PerformAction()
{
	switch (m_action)
	{
	case TriggerActionBoost30:
		m_player.AddVelocity(30.0f * 3.14f / 180.0f, 11.0f);
		break;

	case TriggerActionBoost45:
		m_player.AddVelocity(45.0f * 3.14f / 180.0f, 11.0f);
		break;

	case TriggerActionBoost60:
		m_player.AddVelocity(60.0f * 3.14f / 180.0f, 11.0f);
		break;

	case TriggerActionSlow:
		m_player.SetBounce(false);
		m_player.ScaleVelocity(s_playerSlowdownVelScale);
		break;

	case TriggerActionAngle:
		m_player.SetBounce(false);
		m_player.SkewTrajectory();
		break;

	case TriggerActionStop:
		m_player.StopByTrigger();
		break;

	case TriggerActionBlock:
		m_player.SetPass(PlayerPassRegular);
		break;

	case TriggerActionUseBlock:
		m_player.SetPass(PlayerPassNone);
		break;

	case TriggerActionBoost30Combo:
		m_player.AddVelocity(30.0f * 3.14f / 180.0f, 16.0f);
		break;

	case TriggerActionStopCombo:
		m_player.AddVelocity(45.0f * 3.14f / 180.0f, 22.0f);
		break;

	case TriggerActionBoost45Combo:
		m_nextTrigger->SetAction(TriggerActionBoost45ComboFollowup);
		m_player.HeadStraight(0.0f, 15.0f);
		break;

	case TriggerActionBoost60Combo:
		m_nextTrigger->SetAction(TriggerActionBoost60ComboFollowup);
		m_player.HeadStraight(0.0f, 15.0f);
		break;

	case TriggerActionBlock45Combo:
		m_nextTrigger->SetAction(TriggerActionBlock45ComboFollowup);
		m_player.SetPass(PlayerPassNone);
		m_player.HeadStraight(0.0f, 15.0f);
		break;

	case TriggerActionBlock60Combo:
		m_nextTrigger->SetAction(TriggerActionBlock60ComboFollowup);
		m_player.SetPass(PlayerPassNone);
		m_player.HeadStraight(0.0f, 15.0f);
		break;

	case TriggerActionBlock45ComboFollowup:
		m_player.SetBounce(true);
	case TriggerActionBoost45ComboFollowup:
		m_player.AddVelocity(45.0f * 3.14f / 180.0f, 16.0f);
		m_player.ResumeGravity();
		break;

	case TriggerActionBlock60ComboFollowup:
		m_player.SetPass(PlayerPassSuper);
	case TriggerActionBoost60ComboFollowup:
		m_player.AddVelocity(60.0f * 3.14f / 180.0f, 16.0f);
		m_player.ResumeGravity();
		break;
	}

	if (m_action != TriggerActionUseBlock && m_action != TriggerActionStopCombo)
	{
		auto sceneEM = GetEntity()->GetModule<SceneEM>();
		sceneEM->SetVisibility(TriggerET::s_gfxIdActive, false);
		sceneEM->SetVisibility(TriggerET::s_gfxIdInactive, true);
	}

	if (m_action != TriggerActionUseBlock && m_action != TriggerActionStop)
	{
		m_comboController.TriggerHit(m_type);
	}

	m_timeController.EnterSlowmotion();
}
