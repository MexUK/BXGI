#include "RWVersion.h"
#include "Static/String2.h"
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
	return String2::join(vecGames, ",");
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
	uint32 uiRWVersionUnpackedInt = unpackVersionStamp(uiRWVersionPackedInt);
	string strStamp = "";
	strStamp += String2::toString((uiRWVersionUnpackedInt & 3221225472) >> 30);
	strStamp += "." + String2::toString((uiRWVersionUnpackedInt & 1006632960) >> 26);
	strStamp += "." + String2::toString((uiRWVersionUnpackedInt & 62914560) >> 22);
	strStamp += "." + String2::toString((uiRWVersionUnpackedInt & 4128768) >> 16);
	return strStamp;
}

string						RWVersion::unpackVersionStampAsStringWithBuild(uint32 uiRWVersionPackedInt)
{
	uint32 uiRWVersionUnpackedInt = ((uiRWVersionPackedInt >> 16) & 0xFFFF);

	string strStamp = "RW ";
	if (uiRWVersionUnpackedInt == 0)
	{
		strStamp += String2::toString((uiRWVersionUnpackedInt & 0xF00) >> 8);
		strStamp += "." + String2::toString((uiRWVersionUnpackedInt & 0xF0) >> 4);
		strStamp += "." + String2::toString(uiRWVersionUnpackedInt & 0xF);
	}
	else
	{
		strStamp += String2::toString(((uiRWVersionUnpackedInt & 0xC000) >> 14) + 3);
		strStamp += "." + String2::toString((uiRWVersionUnpackedInt & 0x3C00) >> 10);
		strStamp += "." + String2::toString((uiRWVersionUnpackedInt & 0x3C0) >> 6);
		strStamp += "." + String2::toString(uiRWVersionUnpackedInt & 0x3F);
		strStamp += " (Build " + String2::toString(uiRWVersionPackedInt & 0xFFFF) + ")";
	}
	return strStamp;
}