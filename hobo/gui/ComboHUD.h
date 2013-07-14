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

#ifndef HOBO_GUI_COMBOHUD_H
#define HOBO_GUI_COMBOHUD_H

#include <memory>

class Counter;
class Panel;

class ComboHUD final
{
public:

	ComboHUD(
		std::shared_ptr<Panel> container,
		std::shared_ptr<Counter> comboTimer);

	void SetComboAvailability(
		bool stop,
		bool boost30,
		bool boost45,
		bool boost60,
		bool block45,
		bool block60,
		bool pass);
	void SetComboActive(bool active, float timeLeft);

private:

	static const float s_activeAlpha;
	static const float s_inactiveAlpha;

	std::shared_ptr<Panel> m_combos;
	std::shared_ptr<Panel> m_stopPanel;
	std::shared_ptr<Panel> m_boost30Panel;
	std::shared_ptr<Panel> m_boost45Panel;
	std::shared_ptr<Panel> m_boost60Panel;

	std::shared_ptr<Panel> m_block45Panel;
	std::shared_ptr<Panel> m_block60Panel;
	std::shared_ptr<Panel> m_passPanel;

	std::shared_ptr<Counter> m_comboTimer;
};

#endif // HOBO_GUI_COMBOHUD_H
