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

#include "Application.h"

#include "nanaka/pi/glfw/NanakaNativeWindowImpl.h"
#include "nanaka/renderer/Renderer.h"

LinuxApplication::LinuxApplication()
	: m_nanaka(*this)
	, running(false)
{
}

int LinuxApplication::Run()
{
	running = true;

	m_nanaka.StartThread();
	m_nanaka.OnResume();

	const Vec2f screenSize(1280, 720);
	const std::string title = "Chuck a Chicken";

	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 0);
	glfwWindowHint(GLFW_DEPTH_BITS, 16);
	glfwWindowHint(GLFW_STENCIL_BITS, 0);
	NanakaNativeWindowImpl window(*glfwCreateWindow(
		screenSize.x, screenSize.y, title.c_str(), NULL, NULL));

	g_renderer->OnWindowCreated(&window);

	DisplayProperties displayProps;
	displayProps.m_realSize = screenSize;
	displayProps.m_size = screenSize;
	displayProps.m_dpi = 2.0f;
	m_nanaka.SetDisplayProperties(displayProps);

	bool mouse_down = false;
	Vec2f mousePosition(Vec2f::Zero());

	while (running)
	{
		glfwPollEvents();
		int mouse_state = glfwGetMouseButton(
			&window.m_window, GLFW_MOUSE_BUTTON_LEFT);
		double mouse_new_x = 0.0f, mouse_new_y = 0.0f;
		glfwGetCursorPos(&window.m_window, &mouse_new_x, &mouse_new_y);
		Vec2f newPosition(mouse_new_x, mouse_new_y);
		if (mouse_down)
		{
			if (mouse_state == GLFW_PRESS)
			{
				if (!mousePosition.Equals(newPosition))
				{
					AddInputEvent(newPosition, PointerMoveInputEventAction, 0);
				}
			}
			else
			{
				mouse_down = false;
				AddInputEvent(newPosition, PointerUpInputEventAction, 0);
			}
		}
		else
		{
			if (mouse_state == GLFW_PRESS)
			{
				mouse_down = true;
				AddInputEvent(newPosition, PointerDownInputEventAction, 0);
			}
		}
		mousePosition = newPosition;

#if defined(SINGLE_THREADED)
		m_nanaka.ThreadLoop();
#endif // defined(SINGLE_THREADED)
	}

	m_nanaka.OnPause();
	m_nanaka.KillThread();

	return 0;
}

void LinuxApplication::AddInputEvent(
	Vec2f position,
	InputEventAction action,
	int pointerId)
{
	InputEvent event;
	event.m_type = MotionInputEventType;
	event.m_action = action;
	event.m_position = position;
	event.m_pointerId = pointerId;

	m_nanaka.AddInputEvent(event);
}
