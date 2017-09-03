#pragma once

#include "nsbxgi.h"
#include "WTDMipmap.h"
#include "Pool/VectorPool.h"
#include "Image/ERasterDataFormat.h"
#include "d3d9.h"
#include <string>

class bxgi::WTDMipmap;

class bxgi::WTDEntry : public bxcf::VectorPool<bxgi::WTDMipmap*>
{
public:
	WTDEntry(void);

	void						unload(void) {}

	void						setEntryName(std::string strEntryName) { m_strEntryName = strEntryName; }
	std::string					getEntryName(void) { return m_strEntryName; }

	void						setD3DFormat(D3DFORMAT eD3DFormat) { m_uiD3DFormat = eD3DFormat; }
	D3DFORMAT					getD3DFormat(void) { return m_uiD3DFormat; }

	void						setRasterDataFormat(bxcf::ERasterDataFormat ERasterDataFormatValue, bool bUpdateD3DFormat = true);
	bxcf::ERasterDataFormat		getRasterDataFormat(void) { return m_ERasterDataFormat; }

	void						setImageSize(bool bIsWidth, uint16 usImageSize) { m_usImageSize[bIsWidth ? 0 : 1] = usImageSize; }
	uint16						getImageSize(bool bIsWidth) { return m_usImageSize[bIsWidth ? 0 : 1]; }

	void						setRawDataOffset(uint32 uiRawDataOffset) { m_uiRawDataOffset = uiRawDataOffset; }
	uint32						getRawDataOffset(void) { return m_uiRawDataOffset; }

	void						setTextureHash(uint32 uiTextureHash) { m_uiTextureHash = uiTextureHash; }
	uint32						getTextureHash(void) { return m_uiTextureHash; }

	void						setLevels(uint8 ucLevels) { m_ucLevels = ucLevels; }
	uint8						getLevels(void) { return m_ucLevels; }

	void						stripNameHeaderAndFooter(void);

private:
	std::string					m_strEntryName;
	D3DFORMAT					m_uiD3DFormat;
	bxcf::ERasterDataFormat		m_ERasterDataFormat;
	uint32						m_uiRawDataOffset;
	uint16						m_usImageSize[2];
	uint8						m_ucLevels;
	uint32						m_uiTextureHash;
};