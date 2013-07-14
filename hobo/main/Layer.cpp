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

#include "main/Layer.h"

#include <cmath>

#include "nanaka/entity/Entity.h"
#include "nanaka/entity/EntitySystem.h"
#include "nanaka/main/INanaka.h"
#include "nanaka/main/World.h"

#include "main/EntityTypes.h"
#include "main/BillboardET.h"
#include "main/CameraObject.h"

Layer::Layer(
	INanaka* nanaka,
	CameraObject& camera,
	std::shared_ptr<World> world,
	LayerType type)
	: m_nanaka(nanaka)
	, m_camera(camera)
	, m_world(world)
{
	m_layerHeight = 0.0f;

	switch (type)
	{
	case GrassLayerType:
		m_billboardWidth = 5.0f;
		m_billboardHeight = 1.25f;
		m_layerDepth = 2.0f;
		m_scrollMultiplier = 1.0f;
		m_billboardTex = "Layers/Grass.png";
		break;

	case Hills1LayerType:
		m_billboardWidth = 20.0f;
		m_billboardHeight = 10.0f;
		m_layerDepth = 3.0f;
		m_scrollMultiplier = 0.2f;
		m_billboardTex = "Layers/Hills1.png";
		break;

	case Hills2LayerType:
		m_billboardWidth = 20.0f;
		m_billboardHeight = 10.0f;
		m_layerDepth = 4.0f;
		m_scrollMultiplier = 0.1f;
		m_billboardTex = "Layers/Hills2.png";
		break;

	case SkyLayerType:
		m_billboardWidth = 20.0f;
		m_billboardHeight = 20.0f;
		m_layerDepth = 5.0f;
		m_scrollMultiplier = 0.0f;
		m_billboardTex = "Layers/Sky.png";
		break;

	case CloudsLayerType:
		m_billboardWidth = 2.0f;
		m_billboardHeight = 2.0f;
		m_layerDepth = -1.0f;
		m_scrollMultiplier = 1.0f;
		m_layerHeight = m_camera.GetMaxViewHeight() - m_billboardHeight;
		m_billboardTex = "Layers/Clouds.png";
		break;
	}

	m_layerHeight += m_billboardHeight * 0.5f;

	UpdateViewSize();
}

void Layer::UpdateViewSize()
{
	// Adjust number of billboards in this layer if necessary.
	const int numBillboards =
		static_cast<int>(m_camera.GetMaxViewWidth() / m_billboardWidth) + 2;
	for (int i = m_billboards.size(); i < numBillboards; ++i)
	{
		BillboardEMParams billboardParams(
			m_billboardWidth, m_billboardHeight, m_billboardTex);
		m_billboards.push_back(m_nanaka->GetEntitySystem().SpawnEntity(
			BillboardEntityType, &billboardParams, &m_world->GetEntityTree()));
	}
	for (int i = m_billboards.size() - numBillboards; i > 0; --i)
	{
		m_billboards[m_billboards.size() - i]->DetachFromParent();
	}
	m_billboards.resize(numBillboards);
}

void Layer::SetScroll(float scroll)
{
	// Compute position of first billboard in layer, with parallax scroll.
	const float scaledScroll = scroll * m_scrollMultiplier;
	const float scaledStartX =
		static_cast<int>(scaledScroll / m_billboardWidth) * m_billboardWidth
			- (scaledScroll < 0 ? m_billboardWidth : 0.0f);
	const float startX = scroll - (scaledScroll - scaledStartX)
		+ m_billboardWidth * 0.5f;

	m_billboards[0]->SetPosition(Vec3f(startX, m_layerDepth, m_layerHeight));

	float lastBillboardX = startX;
	for (size_t i = 1; i < m_billboards.size(); ++i)
	{
		lastBillboardX += m_billboardWidth;
		m_billboards[i]->SetPosition(
			Vec3f(lastBillboardX, m_layerDepth, m_layerHeight));
	}
}
