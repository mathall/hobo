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

#ifndef HOBO_MAIN_PLAYER_H
#define HOBO_MAIN_PLAYER_H

#include "nanaka/entity/Entity.h"
#include "nanaka/game/GameObject.h"
#include "nanaka/graphics/KeyframedAnimation.h"
#include "nanaka/math/Vec2f.h"
#include "nanaka/scene/SceneEM.h"

#include "gui/PlayerHUD.h"

class ComboController;
class TimeController;

enum PlayerPass
{
	PlayerPassNone,
	PlayerPassRegular,
	PlayerPassSuper,
};

class Player final : public GameObject
{
public:

	Player(ComboController& comboController, TimeController& timeController);

	void Reset(float x);

	void KickOff(float angle, float vel);
	bool AddComboScore(float score);
	void Soar();

	void SkewTrajectory();
	void ScaleVelocity(float velScaleFactor);
	void AddVelocity(float angle, float vel);
	void StopByTrigger();
	void HeadStraight(float angle, float vel);
	void ResumeGravity();
	void SetBounce(bool bounce);
	void SetPass(PlayerPass pass);
	void TriggerCollision();

	PlayerPass GetPass() const;
	bool HasPass() const;
	bool IsStopped() const;

	void UpdatePlayerHUD(PlayerHUD& playerHUD) const;

	/**
	 * GameObject implementation.
	 */
	void Update(Time& time) override;

private:

	static const float s_groundOffset;
	static const float s_soarDistanceFull;
	static const float s_ascentScoreFull;
	static const float s_groundHitVelScale;
	static const float s_bounceBoost;
	static const long unsigned int s_hitAnimDuration;

	void ResetPlayer();
	void StopSoar();
	void Stop();
	void WasHit();
	void ShowGfxId(AttachmentId gfxId);

	ComboController& m_comboController;
	TimeController& m_timeController;

	KeyframedAnimation<float> m_rotationAnimation;

	float m_distance;

	Vec2f m_vel;

	bool m_stopped;
	bool m_soaring;
	bool m_gravity;
	bool m_bounce;
	PlayerPass m_pass;

	float m_soarGuageLoad;
	float m_ascentGuageLoad;

	bool m_triggerCollision;
	bool m_hitAnim;
	long unsigned int m_timeHit;
};

inline void Player::ResumeGravity()
{
	m_gravity = true;
}

inline void Player::SetBounce(bool bounce)
{
	m_bounce = bounce;
}

inline PlayerPass Player::GetPass() const
{
	return m_pass;
}

inline bool Player::HasPass() const
{
	return m_pass != PlayerPassNone;
}

inline bool Player::IsStopped() const
{
	return m_stopped;
}

inline void Player::StopSoar()
{
	m_soaring = false;
}

inline void Player::UpdatePlayerHUD(PlayerHUD& playerHUD) const
{
	playerHUD.Update(m_vel.x, m_distance, m_soarGuageLoad, m_ascentGuageLoad,
		GetEntity()->GetPosition().z);
}

#endif // HOBO_MAIN_PLAYER_H
