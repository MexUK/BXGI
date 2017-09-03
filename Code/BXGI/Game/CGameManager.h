#pragma once

#include "nsbxgi.h"
#include "nsbxcf.h"
#include "Object/Manager.h"
#include "Object/Singleton.h"
#include "Pool/VectorPool.h"
#include "Game/CGame.h"
#include "Game/EGame.h"
#include "Game/PlatformedGame.h"
#include "Game/EPlatformedGame.h"
#include <string>

class bxgi::CGameManager : public bxcf::Manager, public bxcf::Singleton<bxgi::CGameManager>, public bxcf::VectorPool<bxgi::CGame*>
{
public:
	void											init(void);
	void											uninit(void);

	bxcf::VectorPool<bxgi::PlatformedGame*>&		getPlatformedGames(void) { return m_vecPlatformedGames; }

	bxgi::CGame*									addGame(bxgi::EGame uiGameId, std::string strGameName);
	bxgi::PlatformedGame*							addPlatformedGame(bxgi::EPlatformedGame uiPlatformedGameId, std::string strGameName);

	std::string										getGameName(bxgi::EGame uiGameId);
	std::string										getPlatformedGameText(bxgi::EPlatformedGame uiPlatformedGameId);

private:
	void											initGames(void);
	void											uninitGames(void);

	void											initPlatformedGames(void);
	void											uninitPlatformedGames(void);

private:
	bxcf::VectorPool<bxgi::PlatformedGame*>		m_vecPlatformedGames;
};