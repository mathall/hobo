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

#include "main/PlayerET.h"

#include "nanaka/entity/Entity.h"
#include "nanaka/game/GameEM.h"
#include "nanaka/physics/CollisionBox.h"
#include "nanaka/physics/PhysicsEM.h"

#include "main/Player.h"

PlayerEMParams::PlayerEMParams(
	ComboController& comboController,
	TimeController& timeController)
	: m_comboController(comboController)
	, m_timeController(timeController)
{
}

PlayerET::PlayerET()
	: EntityType({
		GameEntityModuleType,
		PhysicsEntityModuleType,
		SceneEntityModuleType})
{
}

void PlayerET::InitializeEntity(
	Entity& entity,
	const EntityModuleParameters* spawnParams) const
{
	auto gameEM = entity.GetModule<GameEM>();
	auto physicsEM = entity.GetModule<PhysicsEM>();
	auto sceneEM = entity.GetModule<SceneEM>();

	const auto params = static_cast<const PlayerEMParams*>(spawnParams);
	auto& comboController = params->m_comboController;
	auto& timeController = params->m_timeController;

	gameEM->SetGameObject(std::unique_ptr<GameObject>(
		new Player(comboController, timeController)));

	auto collisionBox = std::unique_ptr<CollisionBox>(new CollisionBox());
	collisionBox->SetPosition(Vec3f(0.0f, 100.0f, 0.0f));
	collisionBox->SetDimensions(Vec3f(0.375f, 100.0f, 0.375f));
	physicsEM->AddCollisionShape(std::move(collisionBox));

	auto idleAnimatedBillboard = AnimatedBillboard("Player/Idle.banim");
	idleAnimatedBillboard.SetScale(Vec3f(1.5f, 1.0f, 1.5f));
	sceneEM->SetAttachment<AnimatedBillboard>(
		s_gfxIdIdle, idleAnimatedBillboard);

	auto soaringAnimatedBillboard = AnimatedBillboard("Player/Flying.banim");
	soaringAnimatedBillboard.SetScale(Vec3f(1.5f, 1.0f ,1.5f));
	sceneEM->SetAttachment<AnimatedBillboard>(
		s_gfxIdFlying, soaringAnimatedBillboard);

	auto hitAnimatedBillboard = AnimatedBillboard("Player/Hit.banim");
	hitAnimatedBillboard.SetScale(Vec3f(1.5f, 1.0f, 1.5f));
	sceneEM->SetAttachment<AnimatedBillboard>(
		s_gfxIdHit, hitAnimatedBillboard);
}
