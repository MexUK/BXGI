#pragma once

#include "nsbxgi.h"
#include "Object/Manager.h"
#include "CRWVersion.h"
#include "Pool/VectorPool.h"
#include <string>
#include <vector>
#include <unordered_map>

class bxgi::CRWVersionManager : public bxcf::Manager, public bxcf::VectorPool<bxgi::CRWVersion*>
{
public:
	void											init(void);
	void											uninit(void);

	CRWVersion*										addVersion(uint32 uiRawVersion, ERWVersion uiVersionId, bxcf::fileType::EFileType uiFileType, std::vector<bxgi::EPlatformedGame> vecPlatformedGames);

	std::string										getVersionText(uint32 uiRawVersion);

	CRWVersion*										getEntryByVersionCC(uint32 uiVersionCC);
	CRWVersion*										getEntryByVersionId(bxgi::ERWVersion ERWVersion);
	CRWVersion*										getRWVersionFromGame(bxgi::EPlatformedGame EPlatformedGame);
	std::unordered_map<uint32, std::vector<std::string>>		getVersionNames(void);

private:
	void											initRWVersions(void);

private:
	std::unordered_map<uint32, std::string>			m_umapRawVersionTexts;
};