#pragma once

#include "bxgi.h"
#include "eCOLVersion.h"
#include <string>

class bxgi::CCOLVersion
{
public:
	void				unload(void) {}

	void				setVersionId(bxgi::eCOLVersion eVersionId) { m_eVersionId = eVersionId; }
	bxgi::eCOLVersion	getVersionId(void) { return m_eVersionId; }

	void				setText(std::string strText) { m_strText = strText; }
	std::string			getText(void) { return m_strText; }

	void					setGames(std::vector<bxgi::eGame>& vecGames) { m_vecGames = vecGames; }
	std::vector<eGame>&		getGames(void) { return m_vecGames; }

private:
	bxgi::eCOLVersion	m_eVersionId;
	std::string			m_strText;
	std::vector<eGame>	m_vecGames;
};