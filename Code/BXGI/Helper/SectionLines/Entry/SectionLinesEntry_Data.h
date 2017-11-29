#pragma once

#include "nsbxgi.h"
#include "nsbxcf.h"
#include "Type/Types.h"
#include "Game/EGameFlag.h"
#include <string>

class bxgi::SectionLinesEntry_Data
{
public:
	void						setFormatGames(uint32 uiFormatGames) { m_uiFormatGames = uiFormatGames; }
	uint32						getFormatGames(void) { return m_uiFormatGames; }
	bool						doesSupportFormatGame(bxgi::EGameFlag EGameFlagValue) { return (m_uiFormatGames & EGameFlagValue) == EGameFlagValue; }
	void						addFormatGame(bxgi::EGameFlag EGameFlagValue) { m_uiFormatGames |= EGameFlagValue; }
	void						removeFormatGame(bxgi::EGameFlag EGameFlagValue) { m_uiFormatGames &= ~EGameFlagValue; }

	virtual void				setObjectId(void) {}
	virtual uint32				getObjectId(void) { return -1; }

	virtual void				setModelName(std::string& strModelName) {}
	virtual std::string&		getModelName(void) { return bxcf::g_strBlankString; }
	
	virtual void				setTXDName(void) {}
	virtual std::string&		getTXDName(void) { return bxcf::g_strBlankString; }

private:
	uint32						m_uiFormatGames;
};