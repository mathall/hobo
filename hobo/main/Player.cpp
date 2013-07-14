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

#include "main/Player.h"

#include <algorithm>
#include <cmath>

#include "nanaka/game/GameEM.h"
#include "nanaka/math/Quat.h"
#include "nanaka/physics/PhysicsEM.h"
#include "nanaka/utils/Time.h"

#include "main/ComboController.h"
#include "main/PlayerET.h"
#include "main/TimeController.h"

const float Player::s_ascentScoreFull = 5.0f;
const float Player::s_groundOffset = 0.75f;
const float Player::s_soarDistanceFull = 500.0f;
const float Player::s_groundHitVelScale = 0.8f;
const float Player::s_bounceBoost = 2.0f;
const long unsigned int Player::s_hitAnimDuration = 500;

Player::Player(ComboController& comboController, TimeController& timeController)
	: m_comboController(comboController)
	, m_timeController(timeController)
	, m_vel(0.0f)
{
	m_rotationAnimation.AddKeyframe(
		1000, 0.0f, Interpolation<float>::Linear());
	m_rotationAnimation.AddKeyframe(
		1, 3.142f * 2, Interpolation<float>::Linear());

	ResetPlayer();
}

void Player::Reset(float x)
{
	GetEntity()->SetRotation(Quat::NoRot());
	GetEntity()->SetPosition(Vec3f(x, 0.0f, s_groundOffset));

	auto physicsEM = GetEntity()->GetModule<PhysicsEM>();
	physicsEM->SetVelocity(Vec3f::Zero());

	ShowGfxId(PlayerET::s_gfxIdIdle);

	ResetPlayer();
}

void Player::ResetPlayer()
{
	m_rotationAnimation.ResetAnimation();

	m_distance = 0.0f;

	m_vel *= 0.0f;

	m_stopped = false;
	m_soaring = false;
	m_gravity = true;
	m_bounce = false;
	m_pass = PlayerPassNone;

	m_soarGuageLoad = 0.0f;
	m_ascentGuageLoad = 0.0f;

	m_triggerCollision = false;
	m_hitAnim = false;
	m_timeHit = 0;
}

void Player::Update(Time& time)
{
	if (!m_stopped)
	{
		if (m_hitAnim && m_timeHit + s_hitAnimDuration < time.GetGameTime())
		{
			ShowGfxId(PlayerET::s_gfxIdFlying);
			m_hitAnim = false;
		}

		Vec3f position = GetEntity()->GetPosition();

		bool groundHit = position.z < s_groundOffset;
		if (groundHit)
		{
			position.z = -(position.z - s_groundOffset) * s_groundHitVelScale
				+ s_groundOffset;
			GetEntity()->SetPosition(position);

			if (!m_triggerCollision)
			{
				if (m_bounce)
				{
					AddVelocity(atanf(m_vel.y / m_vel.x), s_bounceBoost);
					if (!m_comboController.IsComboActivated())
					{
						m_timeController.EnterSlowmotion();
					}
				}
				else
				{
					ScaleVelocity(s_groundHitVelScale);
				}

				m_comboController.GroundHit();

				if (m_vel.Length() > 1.0f)
				{
					WasHit();
				}
			}
		}

		if (m_soaring && (m_triggerCollision || groundHit))
		{
			StopSoar();
		}

		m_triggerCollision = false;

		if (m_gravity)
		{
			const Vec3f gravity = Vec3f(0.0f, 0.0f, -9.82f);
			auto physicsEM = GetEntity()->GetModule<PhysicsEM>();
			auto vel = physicsEM->GetVelocity();
			vel += gravity * time.GetGameStep() * 0.001f;
			physicsEM->SetVelocity(vel);
		}

		if (!m_soaring)
		{
			m_rotationAnimation.Animate(
				time.GetGameStep() * m_vel.Length() / 15.0f);
			GetEntity()->SetRotation(
				Quat(Vec3f::UnitY(), m_rotationAnimation.GetValue()));
		}

		m_soarGuageLoad = std::min(1.0f,
			m_soarGuageLoad + (position.x - m_distance) / s_soarDistanceFull);

		m_distance = position.x;

		if (m_vel.Length() < 0.01f)
		{
			Stop();
		}
	}
}

void Player::KickOff(float angle, float vel)
{
	m_stopped = false;
	AddVelocity(angle, vel);
	WasHit();
}

bool Player::AddComboScore(float score)
{
	m_ascentGuageLoad = std::min(1.0f,
		m_ascentGuageLoad + score / s_ascentScoreFull);
	return m_ascentGuageLoad == 1.0f;
}

void Player::Soar()
{
	Vec3f position = GetEntity()->GetPosition();

	auto physicsEM = GetEntity()->GetModule<PhysicsEM>();
	Vec3f vel = physicsEM->GetVelocity();

	// Soar if full gauge, not stopped, falling, and between 2-15m above ground.
	if (m_soarGuageLoad == 1.0f && !m_stopped && vel.z < 0.0f
			&& position.z > 2.0f && position.z < 15.0f)
	{
		const float soarAngle = (10.0f / 180.0f) * 3.142f;
		physicsEM->SetVelocity(
			Vec3f(cos(soarAngle), 0.0f, -sin(soarAngle)) * vel.Length());

		m_soarGuageLoad = 0.0f;
		m_soaring = true;

		GetEntity()->SetRotation(Quat::NoRot());
		m_rotationAnimation.ResetAnimation();
	}
}

void Player::SkewTrajectory()
{
	std::swap(m_vel.x, m_vel.y);
	auto physicsEM = GetEntity()->GetModule<PhysicsEM>();
	physicsEM->SetVelocity(Vec3f(m_vel.x, 0.0f, m_vel.y));
}

void Player::StopByTrigger()
{
	Stop();
	ShowGfxId(PlayerET::s_gfxIdNone);
}

void Player::HeadStraight(float angle, float vel)
{
	m_gravity = false;
	auto physicsEM = GetEntity()->GetModule<PhysicsEM>();
	physicsEM->SetVelocity(Vec3f(cos(angle), 0.0f, sin(angle)) * vel);
}

void Player::SetPass(PlayerPass pass)
{
	m_pass = pass;
	m_comboController.SetPlayerHasPass(HasPass());
}

void Player::Stop()
{
	m_stopped = true;
	ScaleVelocity(0.0f);
}

void Player::ScaleVelocity(float velScaleFactor)
{
	m_vel *= velScaleFactor;
	auto physicsEM = GetEntity()->GetModule<PhysicsEM>();
	physicsEM->SetVelocity(Vec3f(m_vel.x, 0.0f, m_vel.y));
}

void Player::AddVelocity(float angle, float vel)
{
	m_vel += Vec2f(cos(angle), sin(angle)) * vel;
	auto physicsEM = GetEntity()->GetModule<PhysicsEM>();
	physicsEM->SetVelocity(Vec3f(m_vel.x, 0.0f, m_vel.y));
}

void Player::TriggerCollision()
{
	m_triggerCollision = true;
	WasHit();
}

void Player::WasHit()
{
	if (!m_stopped)
	{
		ShowGfxId(PlayerET::s_gfxIdHit);
		m_hitAnim = true;
		m_timeHit = m_timeController.TimeNow();
	}
}

void Player::ShowGfxId(AttachmentId gfxId)
{
	auto sceneEM = GetEntity()->GetModule<SceneEM>();
	sceneEM->SetVisibility(
		PlayerET::s_gfxIdIdle, gfxId == PlayerET::s_gfxIdIdle);
	sceneEM->SetVisibility(
		PlayerET::s_gfxIdFlying, gfxId == PlayerET::s_gfxIdFlying);
	sceneEM->SetVisibility(
		PlayerET::s_gfxIdHit, gfxId == PlayerET::s_gfxIdHit);
	if (gfxId != PlayerET::s_gfxIdNone)
	{
		sceneEM->ResetAnimation(gfxId);
	}
}
