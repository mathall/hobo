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

#ifndef HOBO_GUI_MENU_H
#define HOBO_GUI_MENU_H

#include <memory>

#include "nanaka/main/World.h"
#include "nanaka/graphics/KeyframedAnimation.h"
#include "nanaka/scene/Camera.h"

#include "gui/MenuView.h"

class Entity;
class INanaka;
class RenderTargetPanel;

class Menu final : public World, public MenuViewClient
{
public:

	Menu(INanaka* nanaka, std::unique_ptr<MenuView> menuView);

	void Update();
	void Draw() const;

	/**
	 * MenuViewClient implementation.
	 */
	void Shutdown() override;

private:

	INanaka* m_nanaka;

	Camera m_camera;
	KeyframedAnimation<float> m_rotationAnimation;
	std::shared_ptr<Entity> m_ray;

	std::shared_ptr<RenderTargetPanel> m_renderTarget;
	std::unique_ptr<MenuView> m_menuView;
};

#endif // HOBO_GUI_MENU_H
