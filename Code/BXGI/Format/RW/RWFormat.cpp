#include "RWFormat.h"
#include "Engine/RW/RWManager.h"
#include "Engine/RW/RWVersionManager.h"
#include "RWSection.h"
#include "Sections/RWSection_String.h"
#include "Sections/RWSection_TextureNative.h"
#include "TextureEntry.h"
#include "Static/String2.h"
#include "Exception/EExceptionCode.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

RWFormat::RWFormat(void) :
	Format(true, bxcf::LITTLE_ENDIAN),
	m_pRWVersion(nullptr)
{
}

void					RWFormat::unserialize(void)
{
	RWSectionContainer::unserialize();

	if (getEntryCount() > 0)
	{
		setRWVersion(RWManager::get()->getVersionManager()->getEntryByVersionCC(getEntryByIndex(0)->getSectionRWVersion()));
	}
	else
	{
		setRWVersion(nullptr);
	}

	if (getRWVersion() != nullptr)
	{
		setPlatformedGames(getRWVersion()->getPlatformedGames());
	}

	if (doesHaveTextureWithInvalidTXDRasterDataFormat())
	{
		throw EXCEPTION_UNKNOWN_FORMAT;
	}
}

void					RWFormat::serialize(void)
{
	uint32 uiRWVersionCC;
	if (getEntryCount() == 0)
	{
		uiRWVersionCC = RWManager::get()->getVersionManager()->getEntryByVersionId(RW_3_4_0_3)->getRawVersion();
	}
	else
	{
		uiRWVersionCC = getEntryByIndex(0)->getSectionRWVersion();
	}

	RWManager::get()->setSerializationRWVersion(uiRWVersionCC);

	RWSectionContainer::serialize();
}

void					RWFormat::loadTextureEntries(void)
{
	static TextureEntry *pTextureEntry = nullptr;
	static bool bIsDiffuseTexture = true;

	for (RWSection *pRWSection : getSectionsByType(RW_SECTION_STRING))
	{
		RWSection_String *pRWSection_String = (RWSection_String*)pRWSection;

		if (pRWSection_String->getData().length() != 0)
		{
			if (pTextureEntry == nullptr)
			{
				pTextureEntry = new TextureEntry;
				pTextureEntry->setRWTextureSection((RWSection_Texture*)getParentNode()->getParentNode());
			}

			if (bIsDiffuseTexture)
			{
				pTextureEntry->setHasDiffuse(true);
				pTextureEntry->setDiffuseName(pRWSection_String->getData());
			}
			else
			{
				pTextureEntry->setHasAlpha(true);
				pTextureEntry->setAlphaName(pRWSection_String->getData());
			}
		}

		if (!bIsDiffuseTexture)
		{
			m_vecTextureEntries.push_back(pTextureEntry);
			pTextureEntry = nullptr;
		}

		bIsDiffuseTexture = !bIsDiffuseTexture;
	}
}

void									RWFormat::setRWVersion(RWVersion *pRWVersion, ERWSection ERWSectionValue)
{
	if (ERWSectionValue == -1)
	{
		m_pRWVersion = pRWVersion;
	}
	else
	{
		// todo
	}
}

RWVersion*								RWFormat::getRWVersion(ERWSection ERWSectionValue)
{
	if (ERWSectionValue == -1)
	{
		return m_pRWVersion;
	}
	else
	{
		// todo
		return nullptr;
	}
}

void				RWFormat::fixAlphaTextureStates(void)
{
	vector<TextureEntry*> vecTexturesToRemove;
	for (auto pTextureEntry : m_vecTextureEntries)
	{
		if (pTextureEntry->doesHaveDiffuse() && String2::toUpperCase(pTextureEntry->getDiffuseName()).c_str()[pTextureEntry->getDiffuseName().length() - 1] == 'A')
		{
			TextureEntry *pTextureEntry2 = getTextureByDiffuseName(pTextureEntry->getDiffuseName().substr(0, pTextureEntry->getDiffuseName().length() - 1));
			if (pTextureEntry2 != nullptr)
			{
				vecTexturesToRemove.push_back(pTextureEntry);
				pTextureEntry2->setHasAlpha(true);
				pTextureEntry2->setAlphaName(pTextureEntry->getDiffuseName());
			}
		}
	}
	for (auto pTextureEntry : vecTexturesToRemove)
	{
		auto it = std::find(m_vecTextureEntries.begin(), m_vecTextureEntries.end(), pTextureEntry);
		m_vecTextureEntries.erase(it);
		delete pTextureEntry;
	}
}

void				RWFormat::removeTextureEntry(TextureEntry *pTextureEntry)
{
	string strDiffuseName = String2::toUpperCase(pTextureEntry->getDiffuseName());
	string strAlphaName = String2::toUpperCase(pTextureEntry->getAlphaName());
	auto it = std::find(m_vecTextureEntries.begin(), m_vecTextureEntries.end(), pTextureEntry);
	if (it != m_vecTextureEntries.end())
	{
		m_vecTextureEntries.erase(it);

		if (m_umapTexturesByNameUpper.count(strDiffuseName) == 1)
		{
			m_umapTexturesByNameUpper.erase(strDiffuseName);
		}
		if (m_umapTexturesByNameUpper.count(strAlphaName) == 1)
		{
			m_umapTexturesByNameUpper.erase(strAlphaName);
		}
	}
}

TextureEntry*		RWFormat::getTextureByDiffuseName(string strTextureDiffuseName)
{
	strTextureDiffuseName = String2::toUpperCase(strTextureDiffuseName);
	for (auto pTextureEntry : m_vecTextureEntries)
	{
		if (pTextureEntry->doesHaveDiffuse() && String2::toUpperCase(pTextureEntry->getDiffuseName()) == strTextureDiffuseName)
		{
			return pTextureEntry;
		}
	}
	return nullptr;
}

TextureEntry*		RWFormat::getTextureByDiffuseOrAlphaName(string strTextureName)
{
	strTextureName = String2::toUpperCase(strTextureName);
	if (m_umapTexturesByNameUpper.count(strTextureName) == 1)
	{
		return m_umapTexturesByNameUpper[strTextureName];
	}
	else
	{
		return nullptr;
	}
}

vector<string>		RWFormat::getTextureNames(void)
{
	vector<string> vecTextureNames;
	for (auto pTextureEntry : m_vecTextureEntries)
	{
		if (pTextureEntry->doesHaveDiffuse())
		{
			vecTextureNames.push_back(pTextureEntry->getDiffuseName());
		}
		if (pTextureEntry->doesHaveAlpha())
		{
			vecTextureNames.push_back(pTextureEntry->getAlphaName());
		}
	}
	return vecTextureNames;
}

bool				RWFormat::doesHaveTextureWithInvalidTXDRasterDataFormat(void)
{
	for (auto pTexture : getSectionsByType(RW_SECTION_TEXTURE_NATIVE))
	{
		if (!((RWSection_TextureNative*)pTexture)->doesHaveValidTXDRasterDataFormat())
		{
			return true;
		}
	}
	return false;
}