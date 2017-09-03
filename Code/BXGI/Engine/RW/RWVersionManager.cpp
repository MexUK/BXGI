#include "RWVersionManager.h"

using namespace std;
using namespace bxcf;
using namespace bxcf::fileType;
using namespace bxgi;

// initialization
void											RWVersionManager::init(void)
{
	initRWVersions();
}
void											RWVersionManager::uninit(void)
{
	removeAllEntries();
}

// version initialization
void											RWVersionManager::initRWVersions(void)
{
	addVersion(0x00000302, RW_3_0_0_0_2, MODEL, { PC_GTA_III }); // todo - PC or PS2?
	addVersion(0x00000304, RW_3_0_0_0_4, MODEL, { PC_GTA_III });
	addVersion(0x00000310, RW_3_0_0_0_10, MODEL, { PC_GTA_III });
	addVersion(0x0400CFFF, RW_3_1_0_0, MODEL, { PC_GTA_III });
	addVersion(0x0401FFFF, RW_3_1_0_1, MODEL, { PC_GTA_III });
	addVersion(0x0800FFFF, RW_3_2_0_0, MODEL, { PC_GTA_III });
	addVersion(0x0C02FFFF, RW_3_3_0_2, MODEL, { PC_GTA_III, PC_GTA_VC }); // todo - xbox too?
	addVersion(0x1003FFFF, RW_3_4_0_3, MODEL, { PC_GTA_VC, PC_GTA_SA });
	addVersion(0x1005FFFF, RW_3_4_0_5, MODEL, { PC_GTA_VC }); // todo - android only instead?
	addVersion(0x1400FFFF, RW_3_5_0_0, MODEL, { PC_GTA_III, PC_GTA_SA }); // todo - android only instead?
	addVersion(0x1803FFFF, RW_3_6_0_3, MODEL, { PC_GTA_SA, PC_MANHUNT });
	addVersion(0x1C000000, RW_3_7_0_0, MODEL, { PC_GTA_SA });
	addVersion(0x1C020037, RW_3_7_0_2, MODEL, { PS2_BULLY });
	addVersion(0x4F464E49, RW_4_3_3_46, ANIMATION, { PC_GTA_III, PC_GTA_VC });
}

// add/remove version
RWVersion*										RWVersionManager::addVersion(uint32 uiRawVersion, ERWVersion uiVersionId, EFileType uiFileType, vector<EPlatformedGame> vecPlatformedGames)
{
	RWVersion *pRWVersion = new RWVersion;
	pRWVersion->setRawVersion(uiRawVersion);
	pRWVersion->setVersionId(uiVersionId);
	pRWVersion->setFileType(uiFileType);
	pRWVersion->setPlatformedGames(vecPlatformedGames);
	addEntry(pRWVersion);
	return pRWVersion;
}

// version text
string											RWVersionManager::getVersionText(uint32 uiRawVersion)
{
	if (m_umapRawVersionTexts.count(uiRawVersion))
	{
		return m_umapRawVersionTexts[uiRawVersion];
	}
	else
	{
		string strVersionText = RWVersion::unpackVersionStampAsStringWithBuild(uiRawVersion);
		m_umapRawVersionTexts[uiRawVersion] = strVersionText;
		return strVersionText;
	}
}

// version names
unordered_map<uint32, vector<string>>	RWVersionManager::getVersionNames(void)
{
	unordered_map<uint32, vector<string>> umapRWVersionNames;
	for (auto pRWVersion : getEntries())
	{
		umapRWVersionNames[pRWVersion->getRawVersion()] = { pRWVersion->getVersionText(), "LocalizationKey" };
	}
	return umapRWVersionNames;
}

// entry fetching
RWVersion*										RWVersionManager::getEntryByVersionCC(uint32 uiVersionCC)
{
	for (auto pRWVersion : getEntries())
	{
		if (pRWVersion->getRawVersion() == uiVersionCC)
		{
			return pRWVersion;
		}
	}
	return nullptr;
}

RWVersion*										RWVersionManager::getEntryByVersionId(ERWVersion ERWVersion)
{
	for (auto pRWVersion : getEntries())
	{
		if (pRWVersion->getVersionId() == ERWVersion)
		{
			return pRWVersion;
		}
	}
	return nullptr;
}

RWVersion*										RWVersionManager::getRWVersionFromGame(EPlatformedGame EPlatformedGame)
{
	switch (EPlatformedGame)
	{
	case PC_GTA_III:	return getEntryByVersionId(RW_3_2_0_0);
	case PC_GTA_VC:		return getEntryByVersionId(RW_3_4_0_3);
	case PC_GTA_SA:		return getEntryByVersionId(RW_3_6_0_3);
	case ANDROID_GTA:	return getEntryByVersionId(RW_3_5_0_0);
	case XBOX_GTA:		return getEntryByVersionId(RW_3_3_0_2);
	default:							return nullptr;
	}
}