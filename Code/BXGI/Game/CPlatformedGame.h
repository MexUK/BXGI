#pragma once

#include "bxgi.h"
#include "Game/ePlatformedGame.h"
#include <string>

class bxgi::CPlatformedGame
{
public:
	CPlatformedGame(void);

	void					unload(void) {}

	void					setPlatformedGameId(ePlatformedGame uiPlatformedGameId) { m_uiPlatformedGameId = uiPlatformedGameId; }
	ePlatformedGame			getPlatformedGameId(void) { return m_uiPlatformedGameId; }

	void					setText(std::string& strText) { m_strText = strText; }
	std::string&			getText(void) { return m_strText; }

private:
	ePlatformedGame			m_uiPlatformedGameId;
	std::string				m_strText;
};