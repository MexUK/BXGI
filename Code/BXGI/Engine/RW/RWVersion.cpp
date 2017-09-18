#include "RWVersion.h"
#include "Static/String.h"
#include "Engine/RW/RWManager.h"
#include "Engine/RW/RWVersionManager.h"
#include "Game/GameManager.h"

using namespace std;
using namespace bxcf;
using namespace bxcf::fileType;
using namespace bxgi;

RWVersion::RWVersion(void) :
	m_uiRawVersion(0),
	m_uiVersionId(UNKNOWN_RW_VERSION),
	m_uiFileType(fileType::UNKNOWN)
{
}

// platformed game
bool						RWVersion::doesUsEPlatformedGame(EPlatformedGame EPlatformedGameId)
{
	return std::find(m_vecPlatformedGames.begin(), m_vecPlatformedGames.end(), EPlatformedGameId) != m_vecPlatformedGames.end();
}

// version text
string						RWVersion::getVersionText(void)
{
	return RWManager::get()->getVersionManager()->getVersionText(m_uiRawVersion);
}

string						RWVersion::getVersionTextWithGames(void)
{
	return getVersionText() + " (" + getGamesAsString() + ")";
}

// games for rw version
string						RWVersion::getGamesAsString(void)
{
	GameManager *pGameManager = GameManager::get();
	vector<string> vecGames;
	for (EPlatformedGame uiPlatformedGame : m_vecPlatformedGames)
	{
		vecGames.push_back(pGameManager->getPlatformedGameText(uiPlatformedGame));
	}
	return String::join(vecGames, ",");
}

// pack/unpack version
uint32						RWVersion::packVersionStamp(uint32 uiRWVersionNumber, uint32 uiRWBuildNumber)
{
	uint32 uiRWVersionPackedInt;
	if (uiRWVersionNumber <= 0x31000)
		uiRWVersionPackedInt = uiRWVersionNumber >> 8;
	uiRWVersionPackedInt = (uiRWVersionNumber - 0x30000 & 0x3FF00) << 14 | (uiRWVersionNumber & 0x3F) << 16 | (uiRWBuildNumber & 0xFFFF);
	return uiRWVersionPackedInt;
}

uint32						RWVersion::unpackVersionStamp(uint32 uiRWVersionPackedInt)
{
	if (uiRWVersionPackedInt & 0xFFFF0000)
		return (uiRWVersionPackedInt >> 14 & 0x3FF00) + 0x30000 | (uiRWVersionPackedInt >> 16 & 0x3F);
	return uiRWVersionPackedInt << 8;
}

string						RWVersion::unpackVersionStampAsString(uint32 uiRWVersionPackedInt)
{
	string strStamp = "RW ";
	if (uiRWVersionPackedInt & 0xFFFF0000)
	{
		uint32 uiRWVersionUnpackedInt = (uiRWVersionPackedInt >> 16) & 0xFFFF;
		strStamp += String::toString(((uiRWVersionUnpackedInt & 0xC000) >> 14) + 3);
		strStamp += "." + String::toString((uiRWVersionUnpackedInt & 0x3C00) >> 10);
		strStamp += "." + String::toString((uiRWVersionUnpackedInt & 0x3C0) >> 6);
		strStamp += "." + String::toString(uiRWVersionUnpackedInt & 0x3F);
	}
	else
	{
		uint32 uiRWVersionUnpackedInt = uiRWVersionPackedInt & 0xFFFF;
		strStamp += String::toString((uiRWVersionUnpackedInt & 0xF00) >> 8);
		strStamp += "." + String::toString((uiRWVersionUnpackedInt & 0xF0) >> 4);
		strStamp += "." + String::toString(uiRWVersionUnpackedInt & 0xF);
	}
	return strStamp;
}

string						RWVersion::unpackVersionStampAsStringWithBuild(uint32 uiRWVersionPackedInt)
{
	string strStamp = "RW ";
	if (uiRWVersionPackedInt & 0xFFFF0000)
	{
		uint32 uiRWVersionUnpackedInt = (uiRWVersionPackedInt >> 16) & 0xFFFF;
		strStamp += String::toString(((uiRWVersionUnpackedInt & 0xC000) >> 14) + 3);
		strStamp += "." + String::toString((uiRWVersionUnpackedInt & 0x3C00) >> 10);
		strStamp += "." + String::toString((uiRWVersionUnpackedInt & 0x3C0) >> 6);
		strStamp += "." + String::toString(uiRWVersionUnpackedInt & 0x3F);
		strStamp += " (Build " + String::toString(uiRWVersionPackedInt & 0xFFFF) + ")";
	}
	else
	{
		uint32 uiRWVersionUnpackedInt = uiRWVersionPackedInt & 0xFFFF;
		strStamp += String::toString((uiRWVersionUnpackedInt & 0xF00) >> 8);
		strStamp += "." + String::toString((uiRWVersionUnpackedInt & 0xF0) >> 4);
		strStamp += "." + String::toString(uiRWVersionUnpackedInt & 0xF);
	}
	return strStamp;
}