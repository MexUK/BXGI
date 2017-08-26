#pragma once

#include "bxgi.h"
#include "Game/eGame.h"
#include <string>

class bxgi::CGame
{
public:
	CGame(void);

	void					unload(void) {}

	void					setGameId(eGame uiGameId) { m_uiGameId = uiGameId; }
	eGame					getGameId(void) { return m_uiGameId; }

	void					setName(std::string& strName) { m_strName = strName; }
	std::string&			getName(void) { return m_strName; }

private:
	eGame					m_uiGameId;
	std::string				m_strName;
};