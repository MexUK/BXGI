#include "CRWVersion.h"
#include "Static/CString2.h"
#include "Engine/RW/CRWManager.h"
#include "Engine/RW/CRWVersionManager.h"
#include "Game/CGameManager.h"

using namespace std;
using namespace bxcf;
using namespace bxcf::fileType;
using namespace bxgi;

CRWVersion::CRWVersion(void) :
	m_uiRawVersion(0),
	m_uiVersionId(UNKNOWN_RW_VERSION),
	m_uiFileType(fileType::UNKNOWN)
{
}

// platformed game
bool						CRWVersion::doesUsePlatformedGame(ePlatformedGame ePlatformedGameId)
{
	return std::find(m_vecPlatformedGames.begin(), m_vecPlatformedGames.end(), ePlatformedGameId) != m_vecPlatformedGames.end();
}

// version text
string						CRWVersion::getVersionText(void)
{
	return CRWManager::get()->getVersionManager()->getVersionText(m_uiRawVersion);
}

string						CRWVersion::getVersionTextWithGames(void)
{
	return getVersionText() + " (" + getGamesAsString() + ")";
}

// games for rw version
string						CRWVersion::getGamesAsString(void)
{
	CGameManager *pGameManager = CGameManager::get();
	vector<string> vecGames;
	for (ePlatformedGame uiPlatformedGame : m_vecPlatformedGames)
	{
		vecGames.push_back(pGameManager->getPlatformedGameText(uiPlatformedGame));
	}
	return CString2::join(vecGames, ",");
}

// pack/unpack version
uint32						CRWVersion::packVersionStamp(uint32 uiRWVersionNumber, uint32 uiRWBuildNumber)
{
	uint32 uiRWVersionPackedInt;
	if (uiRWVersionNumber <= 0x31000)
		uiRWVersionPackedInt = uiRWVersionNumber >> 8;
	uiRWVersionPackedInt = (uiRWVersionNumber - 0x30000 & 0x3FF00) << 14 | (uiRWVersionNumber & 0x3F) << 16 | (uiRWBuildNumber & 0xFFFF);
	return uiRWVersionPackedInt;
}

uint32						CRWVersion::unpackVersionStamp(uint32 uiRWVersionPackedInt)
{
	if (uiRWVersionPackedInt & 0xFFFF0000)
		return (uiRWVersionPackedInt >> 14 & 0x3FF00) + 0x30000 | (uiRWVersionPackedInt >> 16 & 0x3F);
	return uiRWVersionPackedInt << 8;
}

string						CRWVersion::unpackVersionStampAsString(uint32 uiRWVersionPackedInt)
{
	uint32 uiRWVersionUnpackedInt = unpackVersionStamp(uiRWVersionPackedInt);
	string strStamp = "";
	strStamp += CString2::toString((uiRWVersionUnpackedInt & 3221225472) >> 30);
	strStamp += "." + CString2::toString((uiRWVersionUnpackedInt & 1006632960) >> 26);
	strStamp += "." + CString2::toString((uiRWVersionUnpackedInt & 62914560) >> 22);
	strStamp += "." + CString2::toString((uiRWVersionUnpackedInt & 4128768) >> 16);
	return strStamp;
}

string						CRWVersion::unpackVersionStampAsStringWithBuild(uint32 uiRWVersionPackedInt)
{
	uint32 uiRWVersionUnpackedInt = ((uiRWVersionPackedInt >> 16) & 0xFFFF);

	string strStamp = "RW ";
	if (uiRWVersionUnpackedInt == 0)
	{
		strStamp += CString2::toString((uiRWVersionUnpackedInt & 0xF00) >> 8);
		strStamp += "." + CString2::toString((uiRWVersionUnpackedInt & 0xF0) >> 4);
		strStamp += "." + CString2::toString(uiRWVersionUnpackedInt & 0xF);
	}
	else
	{
		strStamp += CString2::toString(((uiRWVersionUnpackedInt & 0xC000) >> 14) + 3);
		strStamp += "." + CString2::toString((uiRWVersionUnpackedInt & 0x3C00) >> 10);
		strStamp += "." + CString2::toString((uiRWVersionUnpackedInt & 0x3C0) >> 6);
		strStamp += "." + CString2::toString(uiRWVersionUnpackedInt & 0x3F);
		strStamp += " (Build " + CString2::toString(uiRWVersionPackedInt & 0xFFFF) + ")";
	}
	return strStamp;
}