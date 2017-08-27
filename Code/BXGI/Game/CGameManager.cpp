#include "CGameManager.h"
#include "CGame.h"
#include "CPlatformedGame.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

// initialization
void										CGameManager::init(void)
{
	initGames();
	initPlatformedGames();
}

void										CGameManager::uninit(void)
{
	uninitGames();
	uninitPlatformedGames();
}

// game initialization
void										CGameManager::initGames(void)
{
	addGame(UNKNOWN_GAME, "Unknown Game");
	addGame(GTA_III, "GTA III");
	addGame(GTA_VC, "GTA VC");
	addGame(GTA_SA, "GTA SA");
	addGame(MANHUNT, "Manhunt");
	addGame(BULLY, "Bully");
	addGame(SOL, "SOL");
}

void										CGameManager::uninitGames(void)
{
	removeAllEntries();
}

// platformed game initialization
void										CGameManager::initPlatformedGames(void)
{
	addPlatformedGame(UNKNOWN_PLATFORMED_GAME, "Unknown Platformed Game");
	addPlatformedGame(PC_GTA_III, "GTA III (PC)");
	addPlatformedGame(PC_GTA_VC, "GTA VC (PC)");
	addPlatformedGame(PC_GTA_SA, "GTA SA (PC)");
	addPlatformedGame(PC_GTA_IV, "GTA IV (PC)");
	addPlatformedGame(PC_MANHUNT, "Manhunt (PC)");
	addPlatformedGame(PS2_BULLY, "Bully (PS2)");
	addPlatformedGame(PC_SOL, "SOL (PC)");
}

void										CGameManager::uninitPlatformedGames(void)
{
	getPlatformedGames().removeAllEntries();
}

// add/remove game
CGame*										CGameManager::addGame(EGame uiGameId, string strGameName)
{
	CGame *pGame = new CGame;
	pGame->setGameId(uiGameId);
	pGame->setName(strGameName);
	addEntry(pGame);
	return pGame;
}

// add/remove platformed game
CPlatformedGame*							CGameManager::addPlatformedGame(EPlatformedGame uiPlatformedGameId, string strPlatformedGameText)
{
	CPlatformedGame *pPlatformedGame = new CPlatformedGame;
	pPlatformedGame->setPlatformedGameId(uiPlatformedGameId);
	pPlatformedGame->setText(strPlatformedGameText);
	getPlatformedGames().addEntry(pPlatformedGame);
	return pPlatformedGame;
}

// game name
string										CGameManager::getGameName(EGame EGameId)
{
	for (CGame *pGame : getEntries())
	{
		if (pGame->getGameId() == EGameId)
		{
			return pGame->getName();
		}
	}
	return "Unknown Game";
}

// platformed game text
string										CGameManager::getPlatformedGameText(EPlatformedGame uiPlatformedGameId)
{
	for (CPlatformedGame *pPlatformedGame : getPlatformedGames().getEntries())
	{
		if (pPlatformedGame->getPlatformedGameId() == uiPlatformedGameId)
		{
			return pPlatformedGame->getText();
		}
	}
	return "Unknown Platformed Game";
}