#pragma once

#include "nsbxgi.h"
#include "Type/Types.h"
#include "Format/ContainerFormat.h"
#include "RWSectionContainer.h"
#include "Engine/RW/RWVersion.h"
#include "Format/RW/ERWSection.h"
#include "Game/EPlatformedGame.h"
#include <vector>
#include <unordered_map>

class bxgi::RWSection;
class bxgi::_2dEffect;
class bxgi::TextureEntry;

class bxgi::RWFormat : public bxcf::ContainerFormat, public bxgi::RWSectionContainer
{
public:
	RWFormat(void);
	RWFormat(std::string& strFilePathOrData, bool bStringIsFilePath = true);

	void												_readMetaData(void) {} // todo

	uint32												getRawVersion(void);



	void												setRWVersion(RWVersion* pRWVersion, bxgi::ERWSection ERWSectionValue = (bxgi::ERWSection)-1);	// Sets the RW version for all sections. Use -1 for ERWSectionValue to apply to all sections.
	RWVersion*											getRWVersion(bxgi::ERWSection ERWSectionValue = (bxgi::ERWSection)-1);							// Sets the RW version for all sections. Use -1 for ERWSectionValue to apply to all sections.

	bxgi::RWSection*									getLatestClump(void) { return nullptr; } // todo

	std::vector<bxgi::TextureEntry*>&						getTextureEntries(void) { return m_vecTextureEntries; } // todo - inconsisteny in func name: Entry - remove word: Entry
	std::unordered_map<std::string, bxgi::TextureEntry*>&	getTextureEntryNamesUpper(void) { return m_umapTexturesByNameUpper; } // todo - inconsisteny in func name: Entry - remove word: Entry
	void													removeTextureEntry(bxgi::TextureEntry *pTextureEntry);

	bxgi::TextureEntry*									getTextureByDiffuseName(std::string strTextureDiffuseName);
	bxgi::TextureEntry*									getTextureByDiffuseOrAlphaName(std::string strTextureName);
	std::vector<std::string>							getTextureNames(void);

	void												fixAlphaTextureStates(void);
	bool												doesHaveTextureWithInvalidTXDRasterDataFormat(void);

	void												setPlatformedGames(std::vector<bxgi::EPlatformedGame> vecGames) { m_vecGames = vecGames; }
	std::vector<bxgi::EPlatformedGame>&					getPlatformedGames(void) { return m_vecGames; }

protected:
	virtual void										_unserialize(void);
	virtual void										_serialize(void);

private:
	void												loadTextureEntries(void);

private:
	RWVersion*												m_pRWVersion;
	std::vector<bxgi::TextureEntry*>						m_vecTextureEntries; // todo - inconsisteny in func name: Entry - remove word: Entry
	std::vector<bxgi::EPlatformedGame>						m_vecGames;
	std::unordered_map<std::string, bxgi::TextureEntry*>	m_umapTexturesByNameUpper;
};