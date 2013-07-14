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

#ifndef HOBO_MAIN_CAMERAOBJECT_H
#define HOBO_MAIN_CAMERAOBJECT_H

#include <memory>

#include "nanaka/game/GameObject.h"
#include "nanaka/gui/RenderTargetPanel.h"
#include "nanaka/scene/Camera.h"

class CameraObject final : public GameObject
{
public:

	CameraObject();

	const Camera* GetCamera() const;

	float GetMaxViewWidth() const;
	float GetMaxViewHeight() const;

	void SetRenderTarget(std::shared_ptr<RenderTargetPanel> renderTarget);

	/**
	 * GameObject implementation.
	 */
	void Update(Time& time) override;

private:

	static const int s_maxViewHeight = 15;
	static const int s_minViewHeight = 7;
	static const int s_startStretchHeight = 4;

	std::unique_ptr<Camera> m_camera;

	std::shared_ptr<RenderTargetPanel> m_renderTarget;
};

inline CameraObject::CameraObject()
	: m_camera(std::unique_ptr<Camera>(new Camera()))
{
}

inline const Camera* CameraObject::GetCamera() const
{
	return m_camera.get();
}

inline float CameraObject::GetMaxViewWidth() const
{
	return s_maxViewHeight * m_renderTarget->GetAspectRatio();
}

inline float CameraObject::GetMaxViewHeight() const
{
	return s_maxViewHeight;
}

#endif // HOBO_MAIN_CAMERAOBJECT_H
