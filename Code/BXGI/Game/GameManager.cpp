#include "GameManager.h"
#include "Game.h"
#include "PlatformedGame.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

// initialization
void										GameManager::init(void)
{
	initGames();
	initPlatformedGames();
}

void										GameManager::uninit(void)
{
	uninitGames();
	uninitPlatformedGames();
}

// game initialization
void										GameManager::initGames(void)
{
	addGame(UNKNOWN_GAME, "Unknown Game");
	addGame(GTA_III, "GTA III");
	addGame(GTA_VC, "GTA VC");
	addGame(GTA_SA, "GTA SA");
	addGame(GTA_IV, "GTA IV");
	addGame(MANHUNT, "Manhunt");
	addGame(BULLY, "Bully");
	addGame(SOL, "SOL");
}

void										GameManager::uninitGames(void)
{
	removeAllEntries();
}

// platformed game initialization
void										GameManager::initPlatformedGames(void)
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

void										GameManager::uninitPlatformedGames(void)
{
	getPlatformedGames().removeAllEntries();
}

// add/remove game
Game*										GameManager::addGame(EGame uiGameId, string strGameName)
{
	Game *pGame = new Game;
	pGame->setGameId(uiGameId);
	pGame->setName(strGameName);
	addEntry(pGame);
	return pGame;
}

// add/remove platformed game
PlatformedGame*							GameManager::addPlatformedGame(EPlatformedGame uiPlatformedGameId, string strPlatformedGameText)
{
	PlatformedGame *pPlatformedGame = new PlatformedGame;
	pPlatformedGame->setPlatformedGameId(uiPlatformedGameId);
	pPlatformedGame->setText(strPlatformedGameText);
	getPlatformedGames().addEntry(pPlatformedGame);
	return pPlatformedGame;
}

// game name
string										GameManager::getGameName(EGame EGameId)
{
	for (Game *pGame : getEntries())
	{
		if (pGame->getGameId() == EGameId)
		{
			return pGame->getName();
		}
	}
	return "Unknown Game";
}

// platformed game text
string										GameManager::getPlatformedGameText(EPlatformedGame uiPlatformedGameId)
{
	for (PlatformedGame *pPlatformedGame : getPlatformedGames().getEntries())
	{
		if (pPlatformedGame->getPlatformedGameId() == uiPlatformedGameId)
		{
			return pPlatformedGame->getText();
		}
	}
	return "Unknown Platformed Game";
}