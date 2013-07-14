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

#ifndef HOBO_GUI_PLAYERHUD_H
#define HOBO_GUI_PLAYERHUD_H

#include <memory>

class Panel;
class Gauge;
class Counter;
class Font;

class PlayerHUD final
{
public:

	PlayerHUD(
		std::shared_ptr<Panel> FPSPanel,
		std::shared_ptr<Panel> soarPanel,
		std::shared_ptr<Panel> ascentPanel,
		std::shared_ptr<Panel> speedPanel,
		std::shared_ptr<Panel> scorePanel,
		std::shared_ptr<Panel> heightPanel,
		const Font& font);

	void Update(
		float speed,
		float distance,
		float soarLoad,
		float ascentLoad,
		float height);

private:

	std::shared_ptr<Counter> m_FPSCounter;
	std::shared_ptr<Counter> m_speedCounter;
	std::shared_ptr<Counter> m_distanceCounter;
	std::shared_ptr<Counter> m_heightCounter;

	std::shared_ptr<Gauge> m_soarGauge;
	std::shared_ptr<Gauge> m_ascentGauge;

	std::shared_ptr<Panel> m_heightPanel;
	bool m_heightPanelVisible;
};

#endif // HOBO_GUI_PLAYERHUD_H
