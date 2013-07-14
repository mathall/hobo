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

#include "main/TriggerET.h"

#include "nanaka/entity/Entity.h"
#include "nanaka/game/GameEM.h"
#include "nanaka/graphics/Model.h"
#include "nanaka/physics/CollisionBox.h"
#include "nanaka/physics/PhysicsEM.h"
#include "nanaka/scene/SceneEM.h"

TriggerEMParams::TriggerEMParams(
	TriggerType triggerType,
	Player& player,
	ComboController& comboController,
	TimeController& timeController)
	: m_triggerType(triggerType)
	, m_timeController(timeController)
	, m_comboController(comboController)
	, m_player(player)
{
}

TriggerET::TriggerET()
	: EntityType({
		GameEntityModuleType,
		PhysicsEntityModuleType,
		SceneEntityModuleType})
{
}

void TriggerET::InitializeEntity(
	Entity& entity,
	const EntityModuleParameters* spawnParams) const
{
	auto gameEM = entity.GetModule<GameEM>();
	auto physicsEM = entity.GetModule<PhysicsEM>();
	auto sceneEM = entity.GetModule<SceneEM>();

	const auto params = static_cast<const TriggerEMParams*>(spawnParams);
	auto& timeController = params->m_timeController;
	auto& comboController = params->m_comboController;
	auto& player = params->m_player;
	auto triggerType = params->m_triggerType;

	auto trigger = std::unique_ptr<Trigger>(
		new Trigger(triggerType, player, comboController, timeController));

	auto collisionBox = std::unique_ptr<CollisionBox>(new CollisionBox());
	switch (triggerType)
	{
	case TriggerTypeBoost30:
	case TriggerTypeBoost45:
	case TriggerTypeBoost60:
		collisionBox->SetPosition(Vec3f(0.0f, 100.0f, -1.175f));
		collisionBox->SetDimensions(Vec3f(0.5f, 100.0f, 0.625f));
		break;

	case TriggerTypeSlow:
	case TriggerTypeAngle:
	case TriggerTypeStop:
	case TriggerTypeBlock:
		collisionBox->SetPosition(Vec3f(0.0f, 100.0f, -0.55f));
		collisionBox->SetDimensions(Vec3f(0.5f, 100.0f, 1.25f));
		break;
	}
	physicsEM->AddCollisionShape(std::move(collisionBox));
	physicsEM->SetCollisionListener(trigger.get());
	physicsEM->SetImmovable(true);

	gameEM->SetGameObject(std::move(trigger));

	std::string activeTextureFilePath;
	std::string inactiveTextureFilePath;

	switch(triggerType)
	{
	case TriggerTypeBoost30:
		activeTextureFilePath = "Triggers/Boost30Active.png";
		inactiveTextureFilePath = "Triggers/Boost30Inactive.png";
		break;

	case TriggerTypeBoost45:
		activeTextureFilePath = "Triggers/Boost45Active.png";
		inactiveTextureFilePath = "Triggers/Boost45Inactive.png";
		break;

	case TriggerTypeBoost60:
		activeTextureFilePath = "Triggers/Boost60Active.png";
		inactiveTextureFilePath = "Triggers/Boost60Inactive.png";
		break;

	case TriggerTypeSlow:
		activeTextureFilePath = "Triggers/SlowActive.png";
		inactiveTextureFilePath = "Triggers/SlowInactive.png";
		break;

	case TriggerTypeAngle:
		activeTextureFilePath = "Triggers/Angle.png";
		inactiveTextureFilePath = activeTextureFilePath;
		break;

	case TriggerTypeStop:
		activeTextureFilePath = "Triggers/StopActive.png";
		inactiveTextureFilePath = "Triggers/StopInactive.png";
		break;

	case TriggerTypeBlock:
		activeTextureFilePath = "Triggers/BlockActive.png";
		inactiveTextureFilePath = "Triggers/BlockInactive.png";
		break;
	}

	auto activeModel = Model("nanaka/models/billboard.nmdl");
	Material activeMaterial(activeModel.GetMaterial());
	activeMaterial.SetTexture(Texture(activeTextureFilePath));
	activeModel.SetMaterial(activeMaterial);

	activeModel.SetPosition(Vec3f(0.0f, 0.0f, 0.0f));
	activeModel.SetRotation(Quat::NoRot());
	activeModel.SetScale(Vec3f(2.0f, 1.0f, 4.0f));

	sceneEM->SetAttachment<Model>(s_gfxIdActive, activeModel);

	auto inactiveModel = Model("nanaka/models/billboard.nmdl");
	Material inactiveMaterial(inactiveModel.GetMaterial());
	inactiveMaterial.SetTexture(Texture(inactiveTextureFilePath));
	inactiveModel.SetMaterial(inactiveMaterial);

	inactiveModel.SetPosition(Vec3f(0.0f, 0.0f, 0.0f));
	inactiveModel.SetRotation(Quat::NoRot());
	inactiveModel.SetScale(Vec3f(2.0f, 1.0f, 4.0f));

	sceneEM->SetAttachment<Model>(s_gfxIdInactive, inactiveModel);
}
