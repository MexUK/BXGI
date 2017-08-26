#pragma once

#include "bxgi.h"
#include "bxcf.h"
#include "Object/CManager.h"
#include "Object/CSingleton.h"
#include "Pool/CVectorPool.h"
#include "Game/CGame.h"
#include "Game/eGame.h"
#include "Game/CPlatformedGame.h"
#include "Game/ePlatformedGame.h"
#include <string>

class bxgi::CGameManager : public bxcf::CManager, public bxcf::CSingleton<bxgi::CGameManager>, public bxcf::CVectorPool<bxgi::CGame*>
{
public:
	void											init(void);
	void											uninit(void);

	bxcf::CVectorPool<bxgi::CPlatformedGame*>&		getPlatformedGames(void) { return m_vecPlatformedGames; }

	bxgi::CGame*									addGame(bxgi::eGame uiGameId, std::string strGameName);
	bxgi::CPlatformedGame*							addPlatformedGame(bxgi::ePlatformedGame uiPlatformedGameId, std::string strGameName);

	std::string										getGameName(bxgi::eGame uiGameId);
	std::string										getPlatformedGameText(bxgi::ePlatformedGame uiPlatformedGameId);

private:
	void											initGames(void);
	void											uninitGames(void);

	void											initPlatformedGames(void);
	void											uninitPlatformedGames(void);

private:
	bxcf::CVectorPool<bxgi::CPlatformedGame*>		m_vecPlatformedGames;
};