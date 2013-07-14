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

#include "main/Menu.h"

#include "nanaka/entity/EntitySystem.h"
#include "nanaka/gui/RenderTargetPanel.h"
#include "nanaka/main/INanaka.h"
#include "nanaka/utils/Time.h"

#include "main/BillboardET.h"
#include "main/EntityTypes.h"

Menu::Menu(INanaka* nanaka, std::unique_ptr<MenuView> menuView)
	: m_nanaka(nanaka)
	, m_menuView(std::move(menuView))
{
	m_menuView->SetClient(this);

	BillboardEMParams skyParams(20.0f, 10.0f, "Layers/Sky.png");
	auto sky = nanaka->GetEntitySystem().SpawnEntity(
		BillboardEntityType, &skyParams, &GetEntityTree());
	sky->SetPosition(Vec3f(0.0f, 3.0f, 0.0f));

	BillboardEMParams sunParams(6.0f, 6.0f, "GUI/Menu/Sun.png");
	auto sun = nanaka->GetEntitySystem().SpawnEntity(
		BillboardEntityType, &sunParams, &GetEntityTree());
	sun->SetPosition(Vec3f(-1.0f, 2.0f, -1.0f));

	BillboardEMParams farmParams(10.0f, 5.0f, "GUI/Menu/Farm.png");
	auto farm = nanaka->GetEntitySystem().SpawnEntity(
		BillboardEntityType, &farmParams, &GetEntityTree());
	farm->SetPosition(Vec3f(0.0f, 1.0f, -2.5f));

	BillboardEMParams rayParams(1.0f, 25.0f, "GUI/Menu/SunLight.png");
	m_ray = nanaka->GetEntitySystem().SpawnEntity(
		BillboardEntityType, &rayParams, &GetEntityTree());
	m_ray->SetPosition(Vec3f(-1.0f, 0.0f, -1.0f));

	m_renderTarget = m_menuView->GetRenderPanel();
	m_camera.SetRenderContextId(m_renderTarget->GetRenderContextId());

	m_rotationAnimation.AddKeyframe(
		1000, 0.0f, Interpolation<float>::Linear());
	m_rotationAnimation.AddKeyframe(
		1, 3.142f * 2, Interpolation<float>::Linear());
	m_rotationAnimation.ResetAnimation();
}

void Menu::Update()
{
	Tick();
	const float viewHeight = 10.0f;
	const float viewWidth = viewHeight * m_renderTarget->GetAspectRatio();
	m_camera.SetViewDimensions(Vec2f(viewWidth, viewHeight));
	m_camera.SetPosition(Vec3f(0.0f, -20.0f, 0.0f));

	m_rotationAnimation.Animate(GetTime().GetGameTick() * 0.2f);
	m_ray->SetRotation(Quat(Vec3f::UnitY(), m_rotationAnimation.GetValue()));
}

void Menu::Draw() const
{
	World::Draw(m_camera);
}

void Menu::Shutdown()
{
	m_nanaka->Shutdown();
}
