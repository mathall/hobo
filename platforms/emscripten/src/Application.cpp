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

#include "nanaka/renderer/Renderer.h"

#include <GL/glfw.h>

class GLContextManagerImpl final : public GLContextManager
{
public:

	/**
	 * GLContextManager implementation.
	 */
	void CreateContext(NanakaNativeWindow& nativeWindow) override {}
	void DestroyContext(NanakaNativeWindow& nativeWindow) override {}
	void CreateSurface(NanakaNativeWindow& nativeWindow) override {}
	void Swap(NanakaNativeWindow& nativeWindow) override;
};

GLContextManager* GLContextManager::Create()
{
	return new GLContextManagerImpl();
}

void GLContextManagerImpl::Swap(NanakaNativeWindow& nativeWindow)
{
	glfwSwapBuffers();
}

WebApplication::WebApplication()
	: m_nanaka(*this)
	, m_mouseDown(false)
	, m_mousePosition(Vec2f::Zero())
{
}

void WebApplication::Loop()
{
	int mouseState = glfwGetMouseButton(GLFW_MOUSE_BUTTON_LEFT);
	int mouseNewX, mouseNewY;
	glfwGetMousePos(&mouseNewX, &mouseNewY);
	Vec2f newPosition(mouseNewX, mouseNewY);
	if (m_mouseDown)
	{
		if (mouseState == GLFW_PRESS)
		{
			if (!m_mousePosition.Equals(newPosition))
			{
				AddInputEvent(newPosition, PointerMoveInputEventAction, 0);
			}
		}
		else
		{
			m_mouseDown = false;
			AddInputEvent(newPosition, PointerUpInputEventAction, 0);
		}
	}
	else
	{
		if (mouseState == GLFW_PRESS)
		{
			m_mouseDown = true;
			AddInputEvent(newPosition, PointerDownInputEventAction, 0);
		}
	}
	m_mousePosition = newPosition;

	m_nanaka.ThreadLoop();
}

int WebApplication::Initialize()
{
	m_nanaka.StartThread();
	m_nanaka.OnResume();

	const Vec2f screenSize(640, 320);

	glewInit();
	glfwInit();
	glfwOpenWindow(screenSize.x, screenSize.y, 8, 8, 8, 0, 16, 0, GLFW_WINDOW);

	NanakaNativeWindow window;
	g_renderer->OnWindowCreated(&window);

	DisplayProperties displayProps;
	displayProps.m_realSize = screenSize;
	displayProps.m_size = screenSize;
	displayProps.m_dpi = 1.0f;
	m_nanaka.SetDisplayProperties(displayProps);

	return 0;
}

int WebApplication::Finalize()
{
	m_nanaka.OnPause();
	m_nanaka.KillThread();

	return 0;
}

void WebApplication::AddInputEvent(
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
