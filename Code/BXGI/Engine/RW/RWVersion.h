#pragma once

#include "nsbxgi.h"
#include "ERWVersion.h"
#include "Game/EPlatformedGame.h"
#include "Format/EFileType.h"
#include <string>
#include <vector>

class bxgi::RWVersion
{
public:
	RWVersion(void);

	void									unload(void) {}

	void									setRawVersion(uint32 uiRawVersion) { m_uiRawVersion = uiRawVersion; }
	uint32									getRawVersion(void) { return m_uiRawVersion; }

	void									setVersionId(bxgi::ERWVersion uiVersionId) { m_uiVersionId = uiVersionId; }
	bxgi::ERWVersion						getVersionId(void) { return m_uiVersionId; }

	void									setEntryType(bxcf::fileType::EFileType uiEntryType) { m_uiEntryType = uiEntryType; }
	bxcf::fileType::EFileType				getEntryType(void) { return m_uiEntryType; }

	void									setPlatformedGames(std::vector<bxgi::EPlatformedGame>& vecPlatformedGames) { m_vecPlatformedGames = vecPlatformedGames; }
	std::vector<bxgi::EPlatformedGame>&		getPlatformedGames(void) { return m_vecPlatformedGames; }

	bool									doesUsEPlatformedGame(bxgi::EPlatformedGame EPlatformedGameId);

	std::string								getVersionText(void);
	std::string								getVersionTextWithGames(void);

	std::string								getGamesAsString(void);

	static uint32							packVersionStamp(uint32 uiRWVersionNumber, uint32 uiRWBuildNumber);
	static uint32							unpackVersionStamp(uint32 uiRWVersionStamp);
	static std::string						unpackVersionStampAsString(uint32 uiRWVersionPackedInt);
	static std::string						unpackVersionStampAsStringWithBuild(uint32 uiRWVersionPackedInt);

private:
	uint32									m_uiRawVersion;				// e.g. 0x00000302
	bxgi::ERWVersion						m_uiVersionId;				// e.g. 0
	bxcf::fileType::EFileType				m_uiEntryType;				// e.g. MODEL
	std::vector<EPlatformedGame>			m_vecPlatformedGames;		// e.g. PC_GTA_III, PC_GTA_VC
};