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

#include "main/BillboardET.h"

#include "nanaka/entity/Entity.h"
#include "nanaka/graphics/Model.h"

BillboardEMParams::BillboardEMParams(
	float width,
	float height,
	std::string textureFilePath)
	: m_width(width)
	, m_height(height)
	, m_textureFilePath(textureFilePath)
{
}

BillboardET::BillboardET()
	: EntityType({SceneEntityModuleType})
{
}

void BillboardET::InitializeEntity(
	Entity& entity,
	const EntityModuleParameters* spawnParams) const
{
	auto sceneEM = entity.GetModule<SceneEM>();

	const auto params = static_cast<const BillboardEMParams*>(spawnParams);
	float width = params->m_width;
	float height = params->m_height;
	std::string textureFilePath = params->m_textureFilePath;

	auto model = Model("nanaka/models/billboard.nmdl");
	Material material(model.GetMaterial());
	material.SetTexture(Texture(textureFilePath));
	model.SetMaterial(material);

	model.SetPosition(Vec3f(0.0f, 0.0f, 0.0f));
	model.SetRotation(Quat::NoRot());
	model.SetScale(Vec3f(width, 1.0f, height));

	sceneEM->SetAttachment<Model>(s_gfxId, model);
}
