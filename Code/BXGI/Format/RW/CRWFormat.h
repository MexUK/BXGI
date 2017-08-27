#pragma once

#include "nsbxgi.h"
#include "Type/Types.h"
#include "Format/CFormat.h"
#include "CRWSectionContainer.h"
#include "Engine/RW/CRWVersion.h"
#include "Format/RW/ERWSection.h"
#include "Game/EPlatformedGame.h"
#include <vector>
#include <unordered_map>

class bxgi::CRWSection;
class bxgi::C2dEffect;
class bxgi::CTextureEntry;

class bxgi::CRWFormat : public bxcf::CFormat, public bxgi::CRWSectionContainer
{
public:
	CRWFormat(void);

	void												setRWVersion(CRWVersion* pRWVersion, bxgi::ERWSection ERWSectionValue = (bxgi::ERWSection)-1);	// Sets the RW version for all sections. Use -1 for ERWSectionValue to apply to all sections.
	CRWVersion*											getRWVersion(bxgi::ERWSection ERWSectionValue = (bxgi::ERWSection)-1);							// Sets the RW version for all sections. Use -1 for ERWSectionValue to apply to all sections.

	bxgi::CRWSection*											getLatestClump(void) { return nullptr; } // todo

	std::vector<bxgi::CTextureEntry*>&						getTextureEntries(void) { return m_vecTextureEntries; } // todo - inconsisteny in func name: Entry - remove word: Entry
	std::unordered_map<std::string, bxgi::CTextureEntry*>&	getTextureEntryNamesUpper(void) { return m_umapTexturesByNameUpper; } // todo - inconsisteny in func name: Entry - remove word: Entry
	void												removeTextureEntry(bxgi::CTextureEntry *pTextureEntry);

	bxgi::CTextureEntry*										getTextureByDiffuseName(std::string strTextureDiffuseName);
	bxgi::CTextureEntry*										getTextureByDiffuseOrAlphaName(std::string strTextureName);
	std::vector<std::string>							getTextureNames(void);

	void												fixAlphaTextureStates(void);
	bool												doesHaveTextureWithInvalidTXDRasterDataFormat(void);

	void												setPlatformedGames(std::vector<bxgi::EPlatformedGame> vecGames) { m_vecGames = vecGames; }
	std::vector<bxgi::EPlatformedGame>&					getPlatformedGames(void) { return m_vecGames; }

private:
	void												unserialize(void);
	void												serialize(void);

	void												loadTextureEntries(void);

private:
	CRWVersion*											m_pRWVersion;
	std::vector<bxgi::CTextureEntry*>							m_vecTextureEntries; // todo - inconsisteny in func name: Entry - remove word: Entry
	std::vector<bxgi::EPlatformedGame>					m_vecGames;
	std::unordered_map<std::string, bxgi::CTextureEntry*>		m_umapTexturesByNameUpper;
};