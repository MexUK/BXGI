#pragma once

#include "nsbxgi.h"
#include "eRWVersion.h"
#include "Game/ePlatformedGame.h"
#include "Format/EFileType.h"
#include <string>
#include <vector>

class bxgi::CRWVersion
{
public:
	CRWVersion(void);

	void									unload(void) {}

	void									setRawVersion(uint32 uiRawVersion) { m_uiRawVersion = uiRawVersion; }
	uint32									getRawVersion(void) { return m_uiRawVersion; }

	void									setVersionId(bxgi::eRWVersion uiVersionId) { m_uiVersionId = uiVersionId; }
	bxgi::eRWVersion						getVersionId(void) { return m_uiVersionId; }

	void									setFileType(bxcf::fileType::EFileType uiFileType) { m_uiFileType = uiFileType; }
	bxcf::fileType::EFileType				getFileType(void) { return m_uiFileType; }

	void									setPlatformedGames(std::vector<bxgi::ePlatformedGame>& vecPlatformedGames) { m_vecPlatformedGames = vecPlatformedGames; }
	std::vector<bxgi::ePlatformedGame>&		getPlatformedGames(void) { return m_vecPlatformedGames; }

	bool									doesUsePlatformedGame(bxgi::ePlatformedGame ePlatformedGameId);

	std::string								getVersionText(void);
	std::string								getVersionTextWithGames(void);

	std::string								getGamesAsString(void);

	static uint32							packVersionStamp(uint32 uiRWVersionNumber, uint32 uiRWBuildNumber);
	static uint32							unpackVersionStamp(uint32 uiRWVersionStamp);
	static std::string						unpackVersionStampAsString(uint32 uiRWVersionPackedInt);
	static std::string						unpackVersionStampAsStringWithBuild(uint32 uiRWVersionPackedInt);

private:
	uint32									m_uiRawVersion;				// e.g. 0x00000302
	bxgi::eRWVersion						m_uiVersionId;				// e.g. 0
	bxcf::fileType::EFileType				m_uiFileType;				// e.g. MODEL
	std::vector<ePlatformedGame>			m_vecPlatformedGames;		// e.g. PC_GTA_III, PC_GTA_VC
};