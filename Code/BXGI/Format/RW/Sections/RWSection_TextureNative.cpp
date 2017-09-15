#include "RWSection_TextureNative.h"
#include "Stream/DataReader.h"
#include "Stream/DataWriter.h"
#include "Format/TXD/ETXDRasterDataFormat.h"
#include "Format/TXD/TXDManager.h"
#include "Static/String.h"
#include "Image/ImageManager.h"
#include "Exception/EExceptionCode.h"
#include "Format/RW/RWFormat.h"

using namespace std;
using namespace bxcf;
using namespace bxgi;

RWSection_TextureNative::RWSection_TextureNative(void) :
	m_EPlatform(_PLATFORM_UNKNOWN),
	m_bHasDiffuse(false),
	m_bHasAlpha(false),
	m_bPaletteIsUsed(false),
	m_uiPlatformId(0),
	m_ucFilterFlags(0),
	m_ucTextureWrapUV(0),
	m_uiTXDRasterDataFormat(0),
	m_ERasterDataFormat(RASTERDATAFORMAT_UNKNOWN),
	m_uiAlpha(0),
	m_ucBPP(0),
	m_ucRasterType(0),
	m_EDXTCompressionType(DXT_NOT_COMPRESSED),
	m_bCubeTexture(false),
	m_bAutoMipMaps(false),
	m_bIsNotRWCompatible(false),
	m_ucOriginalBPP(0),
	m_uiMipMapCount(0)
{
	setSectionId(RW_SECTION_TEXTURE_DICTIONARY);
	m_vecImageSize.x = 0;
	m_vecImageSize.y = 0;
}

// serialization
void							RWSection_TextureNative::_unserialize(void)
{
	DataReader *pDataReader = &m_pRWFormat->m_reader; // todo - make better code for peek and seek-restore
	uint64 uiPreviousSeek = pDataReader->getSeek();
	pDataReader->setPeek(true);
	uint32 uiPlatformId = pDataReader->readUint32();
	pDataReader->setPeek(false);
	pDataReader->setSeek(uiPreviousSeek);

	m_EPlatform = TXDManager::getPlatformFromTXDPlatformId(uiPlatformId);

	unserializeHeader();
	unserializeBody();
}

void							RWSection_TextureNative::_serialize(void)
{
	serializeHeader();
	serializeBody();
}

void							RWSection_TextureNative::unserializeHeader(void)
{
	switch (m_EPlatform)
	{
	case PLATFORM_PC:
	case PLATFORM_XBOX:
		unserializeHeader_Direct3D();
		break;
	case PLATFORM_PS2:
		unserializeHeader_PS2();
		break;
	case PLATFORM_ANDROID:
		unserializeHeader_Android();
		break;
	default:
		throw EXCEPTION_UNKNOWN_FORMAT;
	}
	setRasterDataFormat(detectRasterDataFormat(), false);
}

void							RWSection_TextureNative::unserializeBody(void)
{
	switch (m_EPlatform)
	{
	case PLATFORM_PC:
	case PLATFORM_XBOX:
		return unserializeBody_Direct3D();
	case PLATFORM_PS2:
		return unserializeBody_PS2();
	case PLATFORM_ANDROID:
		return unserializeBody_Android();
	default:
		throw EXCEPTION_UNKNOWN_FORMAT;
	}
}

void							RWSection_TextureNative::serializeHeader(void)
{
	switch (m_EPlatform)
	{
	case PLATFORM_PC:
	case PLATFORM_XBOX:
		return serializeHeader_Direct3D();
	case PLATFORM_PS2:
		return serializeHeader_PS2();
	case PLATFORM_ANDROID:
		return serializeHeader_Android();
	default:
		throw EXCEPTION_UNKNOWN_FORMAT;
	}
}

void							RWSection_TextureNative::serializeBody(void)
{
	switch (m_EPlatform)
	{
	case PLATFORM_PC:
	case PLATFORM_XBOX:
		return serializeBody_Direct3D();
	case PLATFORM_PS2:
		return serializeBody_PS2();
	case PLATFORM_ANDROID:
		return serializeBody_Android();
	default:
		throw EXCEPTION_UNKNOWN_FORMAT;
	}
}

void							RWSection_TextureNative::unserializeHeader_Direct3D(void)
{
	DataReader *pDataReader = &m_pRWFormat->m_reader;

	// struct TextureFormat
	m_uiPlatformId = pDataReader->readUint32();
	m_ucFilterFlags = pDataReader->readUint8();
	m_ucTextureWrapUV = pDataReader->readUint8();
	string strPadding = pDataReader->readString(2);
	setDiffuseName(String::rtrimFromLeft(pDataReader->readString(32)));
	if (getDiffuseName() != "")
	{
		setHasDiffuse(true);
	}
	setAlphaName(String::rtrimFromLeft(pDataReader->readString(32)));
	if (getAlphaName() != "")
	{
		setHasAlpha(true);
	}

	// struct RasterFormat
	setTXDRasterDataFormat(pDataReader->readUint32());
	if (m_uiPlatformId == 9)
	{
		// GTA SA
		setDXTCompressionType(ImageManager::getDXTCompressionTypeFromFourCC(pDataReader->readString(4)));
		setAlpha(0);
	}
	else
	{
		// GTA III & VC
		setAlpha(pDataReader->readUint32());
	}
	Vec2u vecImageSize;
	vecImageSize.x = pDataReader->readUint16();
	vecImageSize.y = pDataReader->readUint16();
	setImageSize(vecImageSize);
	setBPP(pDataReader->readUint8());
	setMipMapCount(pDataReader->readUint8());
	setRasterType(pDataReader->readUint8());
	if (getPlatformId() == 9)
	{
		// GTA SA
		uint8 ucSAData = pDataReader->readUint8();
		setAlpha((ucSAData & 1) == 1);
		setCubeTexture((ucSAData & 2) == 2);
		setAutoMipMaps((ucSAData & 4) == 4);
		setIsNotRWCompatible((ucSAData & 8) == 8);
	}
	else
	{
		// GTA III & VC
		setDXTCompressionType((EDXTCompressionType)pDataReader->readUint8());
		setCubeTexture(false);
		setAutoMipMaps(false);
		setIsNotRWCompatible(false);
	}
}

void							RWSection_TextureNative::unserializeHeader_PS2(void)
{
	DataReader *pDataReader = &m_pRWFormat->m_reader;

	// struct TextureFormat
	setPlatformId(pDataReader->readUint32());
	setFilterFlags(pDataReader->readUint8());
	setTextureWrapUV(pDataReader->readUint8());
	string strPadding = pDataReader->readString(2);

	setDiffuseName(unserializeString());
	if (getDiffuseName() != "")
	{
		setHasDiffuse(true);
	}
	setAlphaName(unserializeString());
	if (getAlphaName() != "")
	{
		setHasAlpha(true);
	}

	// struct RasterFormat
	pDataReader->readUint32(); // RW section header
	pDataReader->readUint32();
	pDataReader->readUint32();

	pDataReader->readUint32(); // RW section header
	pDataReader->readUint32();
	pDataReader->readUint32();

	Vec2u vecImageSize;
	m_vecImageSize.x = pDataReader->readUint32();
	m_vecImageSize.y = pDataReader->readUint32();
	setImageSize(vecImageSize);
	setBPP((uint8)pDataReader->readUint32());
	setTXDRasterDataFormat(pDataReader->readUint32());
	uint32 uiTex0_a = pDataReader->readUint32();
	uint32 uiTex0_b = pDataReader->readUint32();
	uint32 uiTex1_a = pDataReader->readUint32();
	uint32 uiTex1_b = pDataReader->readUint32();
	uint32 miptbp1_a = pDataReader->readUint32();
	uint32 miptbp1_b = pDataReader->readUint32();
	uint32 miptbp2_a = pDataReader->readUint32();
	uint32 miptbp2_b = pDataReader->readUint32();
	uint32 uiTexelDataSectionSize = pDataReader->readUint32();
	uint32 uiPaletteDataSectionSize = pDataReader->readUint32();
	uint32 uiGpuDataAlignedSize = pDataReader->readUint32();
	uint32 uiSkyMipmapVal = pDataReader->readUint32();
}

void							RWSection_TextureNative::unserializeHeader_Android(void)
{
	DataReader *pDataReader = &m_pRWFormat->m_reader;

	// struct TextureFormat
	setPlatformId(pDataReader->readUint32());
	setFilterFlags(pDataReader->readUint8());
	setTextureWrapUV(pDataReader->readUint8());
	string strPadding = pDataReader->readString(2);
	pDataReader->readUint32();
	pDataReader->readUint32();
	pDataReader->readString(16);
	setDiffuseName(String::rtrimFromLeft(pDataReader->readString(32)));
	if (getDiffuseName() != "")
	{
		setHasDiffuse(true);
	}
	setAlphaName(String::rtrimFromLeft(pDataReader->readString(32)));
	if (getAlphaName() != "")
	{
		setHasAlpha(true);
	}

	// struct RasterFormat
	pDataReader->readUint32();
	m_vecImageSize.x = pDataReader->readUint16();
	m_vecImageSize.y = pDataReader->readUint16();
	pDataReader->readUint32();
	pDataReader->readUint32();

	setBPP(8);
	setTXDRasterDataFormat(TXDRASTERDATAFORMAT_EXT_PAL8 | TXDRASTERDATAFORMAT_8888);
}

void							RWSection_TextureNative::unserializeBody_Direct3D(void)
{
	DataReader *pDataReader = &m_pRWFormat->m_reader;

	// texture native struct body - palette
	bool bPaletteUsed = false;
	uint32 uiPaletteSize = 0;
	if ((getTXDRasterDataFormat() & TXDRASTERDATAFORMAT_EXT_PAL4) == TXDRASTERDATAFORMAT_EXT_PAL4)
	{
		bPaletteUsed = true;
		uiPaletteSize = 16;
	}
	else if ((getTXDRasterDataFormat() & TXDRASTERDATAFORMAT_EXT_PAL8) == TXDRASTERDATAFORMAT_EXT_PAL8)
	{
		bPaletteUsed = true;
		uiPaletteSize = 256;
	}

	if (bPaletteUsed)
	{
		uint32 uiPaletteDataSize = uiPaletteSize * 4;
		setPaletteUsed(true);
		setPaletteData(pDataReader->readString(uiPaletteDataSize));
	}

	// texture native struct body - mipmap image data
	uint32
		uiMipmapWidth = getImageSize().x,
		uiMipmapHeight = getImageSize().y;
	for (uint32 uiMipmapIndex = 0, j = getMipMapCount(); uiMipmapIndex < j; uiMipmapIndex++)
	{
		uint32 uiImageDataSize = pDataReader->readUint32();

		Vec2u vecImageSize;
		vecImageSize.x = uiMipmapWidth;
		vecImageSize.y = uiMipmapHeight;

		RWEntry_TextureNative_MipMap *pMipmap = new RWEntry_TextureNative_MipMap(this);
		pMipmap->setImageSize(vecImageSize);
		pMipmap->setRasterData(pDataReader->readString(uiImageDataSize));
		getMipMaps().addEntry(pMipmap);

		uiMipmapWidth /= 2;
		uiMipmapHeight /= 2;
	}
}

void							RWSection_TextureNative::unserializeBody_PS2(void)
{
	DataReader *pDataReader = &m_pRWFormat->m_reader;

	pDataReader->readUint32(); // RW section header
	pDataReader->readUint32();
	pDataReader->readUint32();

	uint32 uiMipmapWidth = getImageSize().x;
	uint32 uiMipmapHeight = getImageSize().y;
	uint32 j = getMipMaps().getEntryCount();
	if (j == 0)
	{
		j = 1;
	}
	for (uint32 uiMipmapIndex = 0; uiMipmapIndex < j; uiMipmapIndex++)
	{
		uint32 uiImageDataSize;
		uint32 uiSwizzledWidth;
		uint32 uiSwizzledHeight;

		if (getTXDRasterDataFormat() & TXDRASTERDATAFORMAT_PS2_SWIZZLEDWITHHEADER)
		{
			pDataReader->readString(8 * 4);
			uiSwizzledWidth = pDataReader->readUint32();
			uiSwizzledHeight = pDataReader->readUint32();
			pDataReader->readString(6 * 4);
			uiImageDataSize = pDataReader->readUint32() * 0x10;
			pDataReader->readString(3 * 4);
		}
		else
		{
			uiSwizzledWidth = uiMipmapWidth;
			uiSwizzledHeight = uiMipmapHeight;
			if (getTXDRasterDataFormat() & TXDRASTERDATAFORMAT_PS2_SWIZZLEDNOHEADER)
			{
				uiSwizzledWidth /= 2;
				uiSwizzledHeight /= 2;
			}

			uiImageDataSize = uiMipmapWidth * uiMipmapHeight * (uint32)((float32)getBPP() / 8.0f);
		}

		RWEntry_TextureNative_MipMap *pMipmap = new RWEntry_TextureNative_MipMap(this);

		Vec2u vecImageSize, vecSwizzledImageSize;
		vecImageSize.x = uiMipmapWidth;
		vecImageSize.y = uiMipmapHeight;
		vecSwizzledImageSize.x = uiSwizzledWidth;
		vecSwizzledImageSize.y = uiSwizzledHeight;
		pMipmap->setImageSize(vecImageSize);
		pMipmap->setSwizzledImageSize(vecSwizzledImageSize);
		pMipmap->setRasterData(pDataReader->readString(uiImageDataSize));
		getMipMaps().addEntry(pMipmap);

		uiMipmapWidth /= 2;
		uiMipmapHeight /= 2;
	}

	if ((getTXDRasterDataFormat() & TXDRASTERDATAFORMAT_EXT_PAL4) || (getTXDRasterDataFormat() & TXDRASTERDATAFORMAT_EXT_PAL8))
	{
		uint32 uiUnknown1, uiUnknown2, uiUnknown3, uiUnknown4;
		uiUnknown1 = 0;
		uiUnknown2 = 0;
		uiUnknown3 = 0;
		uiUnknown4 = 0;
		if (getTXDRasterDataFormat() & TXDRASTERDATAFORMAT_PS2_SWIZZLEDWITHHEADER)
		{
			pDataReader->readString(5 * 4);
			uiUnknown1 = pDataReader->readUint32();
			pDataReader->readString(2 * 4);
			uiUnknown2 = pDataReader->readUint32();
			uiUnknown3 = pDataReader->readUint32();
			pDataReader->readString(6 * 4);
			uiUnknown4 = pDataReader->readUint32();
			pDataReader->readString(3 * 4);
		}

		uint32 uiPaletteSize;
		if (getTXDRasterDataFormat() & TXDRASTERDATAFORMAT_EXT_PAL8)
		{
			uiPaletteSize = 0x100;
		}
		else
		{
			uiPaletteSize = 0x10;
		}

		setPaletteUsed(true);
		setPaletteData(pDataReader->readString(uiPaletteSize * 4));

		// need to work on 4bit palettes in vc & sa
		if (uiUnknown2 == 8 && uiUnknown3 == 3 && uiUnknown4 == 6)
		{
			pDataReader->readString(32);
		}
	}

	unswizzlePS2Format();
}

void							RWSection_TextureNative::unserializeBody_Android(void)
{
	// same body data as texture native for PC
	return unserializeBody_Direct3D();
}

void							RWSection_TextureNative::serializeHeader_Direct3D(void)
{
	// texture native struct header - 86 bytes

	DataWriter *pDataWriter = &m_pRWFormat->m_writer;

	// struct TextureFormat
	pDataWriter->writeUint32(m_uiPlatformId);
	pDataWriter->writeUint8(m_ucFilterFlags);
	pDataWriter->writeUint8(m_ucTextureWrapUV);
	pDataWriter->writeString(2); // 2 bytes padding
	
	if (doesHaveDiffuse())
	{
		pDataWriter->writeStringRef(m_strDiffuseName, 32);
	}
	else
	{
		pDataWriter->writeString(32);
	}

	if (doesHaveAlpha())
	{
		pDataWriter->writeStringRef(m_strAlphaName, 32);
	}
	else
	{
		pDataWriter->writeString(32);
	}

	// struct RasterFormat
	pDataWriter->writeUint32(getTXDRasterDataFormat() | (getMipMaps().getEntryCount() > 1 ? TXDRASTERDATAFORMAT_EXT_MIPMAP : 0));
	
	if (m_uiPlatformId == 9)
	{
		// GTA SA
		pDataWriter->writeString(ImageManager::getD3DFormatToPack(ImageManager::getD3DFormatFromRasterDataFormat(m_ERasterDataFormat)));
	}
	else
	{
		// GTA III & VC
		pDataWriter->writeUint32((doesHaveAlpha() ? 1 : 0));
	}

	pDataWriter->writeUint16((uint16)(getImageSize().x));
	pDataWriter->writeUint16((uint16)(getImageSize().y));
	pDataWriter->writeUint8(getBPP());
	pDataWriter->writeUint8((uint8)getMipMaps().getEntryCount());
	pDataWriter->writeUint8(getRasterType());
	if (m_uiPlatformId == 9)
	{
		// GTA SA
		uint8 ucSAData = 0;
		if (doesHaveAlpha())
		{
			ucSAData |= 1;
		}
		if (getCubeTexture())
		{
			ucSAData |= 2;
		}
		if (getAutoMipMaps())
		{
			ucSAData |= 4;
		}
		if (getIsNotRWCompatible())
		{
			ucSAData |= 8;
		}
		pDataWriter->writeUint8(ucSAData);
	}
	else
	{
		// GTA III & VC
		pDataWriter->writeUint8(getDXTCompressionType());
	}
}

void							RWSection_TextureNative::serializeHeader_PS2(void)
{
	DataWriter *pDataWriter = &m_pRWFormat->m_writer;

	// todo
}

void							RWSection_TextureNative::serializeHeader_Android(void)
{
	DataWriter *pDataWriter = &m_pRWFormat->m_writer;

	// todo
}

void							RWSection_TextureNative::serializeBody_Direct3D(void)
{
	DataWriter *pDataWriter = &m_pRWFormat->m_writer;

	for (auto pMipmap : getMipMaps().getEntries())
	{
		pDataWriter->writeUint32(pMipmap->getRasterData().length());
		pDataWriter->writeStringRef(pMipmap->getRasterData());
	}
}

void							RWSection_TextureNative::serializeBody_PS2(void)
{
	DataWriter *pDataWriter = &m_pRWFormat->m_writer;

	// todo
}

void							RWSection_TextureNative::serializeBody_Android(void)
{
	DataWriter *pDataWriter = &m_pRWFormat->m_writer;

	// todo
}

// serialization helpers
string							RWSection_TextureNative::unserializeString(void)
{
	DataReader *pDataReader = &m_pRWFormat->m_reader;

	pDataReader->readUint32(); // RW section header
	uint32 uiSectionSize = pDataReader->readUint32();
	pDataReader->readUint32();

	pDataReader->setPeek(true);
	uint64 uiPreviousSeek = pDataReader->getSeek();
	string strData = pDataReader->readStringUntilZero();
	pDataReader->setPeek(false);
	pDataReader->setSeek(uiPreviousSeek);

	uint32 uiStringLength;
	if (uiSectionSize == 0)
	{
		uiStringLength = (uint32)(ceil(((float32)(strData.length() + 1)) / 4.0) * 4);
	}
	else
	{
		uiStringLength = uiSectionSize;
	}

	pDataReader->readString(uiStringLength);
	return strData;
}

void							RWSection_TextureNative::unswizzlePS2Format(void)
{
	uint32 uiWidth = getImageSize().x;
	uint32 uiHeight = getImageSize().y;
	uint32 k = getMipMaps().getEntryCount() == 0 ? 1 : getMipMaps().getEntryCount();
	for (uint32 j = 0; j < k; j++)
	{
		// skip mipmaps - as undocumented
		if (j > 0)
		{
			break;
		}
		bool swizzled = (getMipMaps().getEntryByIndex(j)->getSwizzledImageSize().y != uiHeight);

		string strRasterData = getMipMaps().getEntryByIndex(0)->getRasterData();
		const char *pRasterData = strRasterData.c_str();
		uint32 uiImageDataSize = strRasterData.length();
		string strNewRasterData;
		uint8 *oldtexels;
		uint8 *newtexels;
		switch (getBPP())
		{
		case 4:
			// convert from 4 BPP to 8 BPP

			oldtexels = (uint8*)pRasterData;
			uiImageDataSize *= 2;
			newtexels = new uint8[uiImageDataSize];
			for (uint32 i = 0; i < (uiImageDataSize / 2); i++)
			{
				newtexels[i * 2 + 0] = oldtexels[i] & 0x0F;
				newtexels[i * 2 + 1] = oldtexels[i] >> 4;
			}
			strNewRasterData.append((char*)newtexels, uiImageDataSize);
			delete[] newtexels;
			getMipMaps().getEntryByIndex(0)->setRasterData(strNewRasterData);
			setBPP(8);
			//setTXDRasterDataFormat(getTXDRasterDataFormat() | ~TXDRASTERDATAFORMAT_EXT_PAL4);
			//setTXDRasterDataFormat(getTXDRasterDataFormat() | TXDRASTERDATAFORMAT_EXT_PAL8);
			setRasterDataFormat(RASTERDATAFORMAT_PAL8, true);
			setOriginalBPP(4);

			if (swizzled)
			{
				processPs2Swizzle();
			}
			break;
		case 8:
			if (swizzled)
			{
				processPs2Swizzle();
			}
			unclut(getImageSize().x, getImageSize().y);
			break;
		case 32:
			newtexels = new uint8[uiImageDataSize];
			for (uint32 i = 0; i < uiWidth * uiHeight; i++)
			{
				// swap R and B
				uint8 tmp = pRasterData[i * 4 + 0];
				newtexels[i * 4 + 0] = pRasterData[i * 4 + 2];
				newtexels[i * 4 + 1] = pRasterData[i * 4 + 1];
				newtexels[i * 4 + 2] = tmp;
				newtexels[i * 4 + 3] = pRasterData[i * 4 + 3];
				// fix alpha
				uint32 newval = pRasterData[i * 4 + 3] * 0xff;
				newval /= 0x80;
				newtexels[i * 4 + 3] = (uint8)newval;
				//				if (texels[j][i*4+3] != 0xFF)
				//					hasAlpha = true;
			}
			strNewRasterData.append((char*)newtexels, uiImageDataSize);
			delete[] newtexels;
			getMipMaps().getEntryByIndex(0)->setRasterData(strNewRasterData);
			break;
		}
	}

	// can't understand ps2 mipmaps
	if (getMipMaps().getEntryCount() > 1)
	{
		auto pMipMap = getMipMaps().getEntryByIndex(0);
		getMipMaps().removeAllEntries();
		getMipMaps().addEntry(pMipMap);

		setTXDRasterDataFormat(~(TXDRASTERDATAFORMAT_EXT_AUTO_MIPMAP | TXDRASTERDATAFORMAT_EXT_MIPMAP));
	}

	if ((getTXDRasterDataFormat() & TXDRASTERDATAFORMAT_EXT_PAL4) || (getTXDRasterDataFormat() & TXDRASTERDATAFORMAT_EXT_PAL8))
	{
		uint32 uiPaletteSize = m_strPaletteData.length() / 4;
		uint32 uiAlphaDistribution = 0;
		int32 aref = 0x40;
		for (uint32 i = 0; i < uiPaletteSize; i++)
		{
			if ((uiAlphaDistribution & 0x1) == 0 && (m_strPaletteData.c_str()[i * 4 + 3] & 0xFF) >= aref)
			{
				uiAlphaDistribution |= 0x1;
			}
			else if ((uiAlphaDistribution & 0x2) == 0 && (m_strPaletteData.c_str()[i * 4 + 3] & 0xFF) < aref)
			{
				uiAlphaDistribution |= 0x2;
			}
			uint32 uiNewAlpha = (m_strPaletteData.c_str()[i * 4 + 3] & 0xFF) * 0xff;
			uiNewAlpha /= 0x80;
			m_strPaletteData[i * 4 + 3] = (int8)uiNewAlpha;
		}
	}

	setDXTCompressionType(DXT_NOT_COMPRESSED);
}

void							RWSection_TextureNative::processPs2Swizzle(void)
{
	auto pMipmap = getMipMaps().getEntryByIndex(0);
	uint32 uiSwizzledWidth = pMipmap->getSwizzledImageSize().x;
	uint32 uiSwizzledHeight = pMipmap->getSwizzledImageSize().y;

	//dataSizes[i] = swizzleWidth[i] * swizzleHeight[i] * 4;
	uint32 uiDataSize = uiSwizzledWidth * uiSwizzledHeight * 4;
	uint8 *pNewtexels = new uint8[uiDataSize];
	unswizzle8(pNewtexels, (uint8*)pMipmap->getRasterData().c_str(), uiSwizzledWidth * 2, uiSwizzledHeight * 2);
	string strRasterData;
	strRasterData.append((char*)pNewtexels, uiDataSize);
	delete[] pNewtexels;
	pMipmap->setRasterData(strRasterData);

	const char *pRasterData = pMipmap->getRasterData().c_str();
	uint32 uiStride = uiSwizzledWidth * 2;
	if (uiStride != pMipmap->getImageSize().x)
	{
		//dataSizes[i] = width[i] * height[i];
		uint32 uiDataSize2 = pMipmap->getImageSize().x * pMipmap->getImageSize().y;
		uint8 *pNewtexels2 = new uint8[uiDataSize2];
		for (uint32 y = 0; y < pMipmap->getImageSize().y; y++)
		{
			for (uint32 x = 0; x < pMipmap->getImageSize().x; x++)
			{
				pNewtexels2[y*((uint32)pMipmap->getImageSize().x) + x] = pRasterData[y*uiStride + x];
			}
		}
		string strRasterData2;
		strRasterData2.append((char*)pNewtexels2, uiDataSize2);
		delete[] pNewtexels2;
		pMipmap->setRasterData(strRasterData2);
	}
}

void							RWSection_TextureNative::unswizzle8(uint8 *pTexels, uint8 *pRawIndices, uint32 uiWidth, uint32 uiHeight)
{
	for (uint32 y = 0; y < uiHeight; y++)
	{
		for (uint32 x = 0; x < uiWidth; x++)
		{
			int32 block_loc = (y&(~0x0F))*uiWidth + (x&(~0x0F)) * 2;
			uint32 swap_sel = (((y + 2) >> 2) & 0x01) * 4;
			int32 ypos = (((y&(~3)) >> 1) + (y & 1)) & 0x07;
			int32 column_loc = ypos*uiWidth * 2 + ((x + swap_sel) & 0x07) * 4;
			int32 byte_sum = ((y >> 1) & 1) + ((x >> 2) & 2);
			uint32 swizzled = block_loc + column_loc + byte_sum;
			//			cout << swizzled << endl;
			pTexels[y*uiWidth + x] = pRawIndices[swizzled];
		}
	}
}

void							RWSection_TextureNative::unclut(uint32 uiWidth, uint32 uiHeight)
{
	auto pMipmap = getMipMaps().getEntryByIndex(0);
	string strRasterData = pMipmap->getRasterData();
	uint8 map[4] = { 0, 16, 8, 24 };
	for (uint32 i = 0; i < (uiWidth*uiHeight); i++)
	{
		strRasterData[i] = (strRasterData.c_str()[i] & ~0x18) | map[(strRasterData.c_str()[i] & 0x18) >> 3];
	}
	pMipmap->setRasterData(strRasterData);
}

// raster data format
void							RWSection_TextureNative::setRasterDataFormat(ERasterDataFormat ERasterDataFormatValue, bool bUpdatETXDRasterDataFormat)
{
	m_ERasterDataFormat = ERasterDataFormatValue;
	if (bUpdatETXDRasterDataFormat)
	{
		m_uiTXDRasterDataFormat = TXDManager::getTXDRasterDataFormatFromRasterDataFormat(ERasterDataFormatValue);
		m_EDXTCompressionType = ImageManager::getDXTCompressionTypeFromRasterDataFormat(ERasterDataFormatValue);
	}
}

// raster data format validation
bool							RWSection_TextureNative::doesHaveValidTXDRasterDataFormat(void)
{
	return RWSection_TextureNative::doesTXDRasterDataFormatExist(getTXDRasterDataFormat());
}

bool							RWSection_TextureNative::doesTXDRasterDataFormatExist(uint32 uiTXDRasterDataFormat)
{
	switch (uiTXDRasterDataFormat & 0xFFB) // (0xFFF - 4) As TXD format versions for PS2 can have a value 4 OR'd into the TXD raster data format value.
	{
	case TXDRASTERDATAFORMAT_4444:
	case TXDRASTERDATAFORMAT_1555:
	case TXDRASTERDATAFORMAT_565:
	case TXDRASTERDATAFORMAT_8888:
	case TXDRASTERDATAFORMAT_888:
		return true;
	}
	return false;
}

// fetch raster data format
ERasterDataFormat				RWSection_TextureNative::detectRasterDataFormat(void)
{
	if (m_uiTXDRasterDataFormat & TXDRASTERDATAFORMAT_EXT_PAL4)
	{
		return RASTERDATAFORMAT_PAL4;
	}
	else if (m_uiTXDRasterDataFormat & TXDRASTERDATAFORMAT_EXT_PAL8)
	{
		return RASTERDATAFORMAT_PAL8;
	}

	switch (m_uiTXDRasterDataFormat & 0xFFB)
	{
	case TXDRASTERDATAFORMAT_8888:
		return RASTERDATAFORMAT_BGRA32;
	case TXDRASTERDATAFORMAT_888:
		switch (m_ucBPP)
		{
		case 32:		return RASTERDATAFORMAT_BGR32;
		case 24:		return RASTERDATAFORMAT_BGR24;
		}
		break;
	case TXDRASTERDATAFORMAT_565:
	case TXDRASTERDATAFORMAT_1555:
	case TXDRASTERDATAFORMAT_4444:
		switch (m_EDXTCompressionType)
		{
		case DXT_1:		return RASTERDATAFORMAT_DXT1;
		case DXT_2:		return RASTERDATAFORMAT_DXT2;
		case DXT_3:		return RASTERDATAFORMAT_DXT3;
		case DXT_4:		return RASTERDATAFORMAT_DXT4;
		case DXT_5:		return RASTERDATAFORMAT_DXT5;
		}
		break;
	}

	return RASTERDATAFORMAT_UNKNOWN;
}

// convert game
void							RWSection_TextureNative::convertToGame(EPlatformedGame EPlatformedGame, vector<string>& vecMipmapsRemoved)
{
	switch (EPlatformedGame)
	{
	case PC_GTA_III:
		setPlatformId(8);
		setFilterFlags(2);
		setTXDRasterDataFormat(TXDRASTERDATAFORMAT_565);
		setAlpha(doesHaveAlpha() ? 1 : 0);
		setBPP(16);
		setDXTCompressionType(DXT_1);

		convertToRasterDataFormat(RASTERDATAFORMAT_DXT1, vecMipmapsRemoved);
		break;
	case PC_GTA_VC:
		setPlatformId(8);
		setFilterFlags(2);
		setTXDRasterDataFormat(TXDRASTERDATAFORMAT_565);
		setAlpha(doesHaveAlpha() ? 1 : 0);
		setBPP(16);
		setDXTCompressionType(DXT_1);

		convertToRasterDataFormat(RASTERDATAFORMAT_DXT1, vecMipmapsRemoved);
		break;
	case PC_GTA_SA:
		setPlatformId(9);
		setFilterFlags(1);
		setTXDRasterDataFormat(TXDRASTERDATAFORMAT_565);
		setAlpha(doesHaveAlpha() ? 1 : 0);
		setBPP(16);
		setDXTCompressionType(DXT_1);

		convertToRasterDataFormat(RASTERDATAFORMAT_DXT1, vecMipmapsRemoved);
		break;
	case ANDROID_GTA:
		setPlatformId(8);
		setFilterFlags(2);
		setTXDRasterDataFormat(TXDRASTERDATAFORMAT_888);
		setAlpha(doesHaveAlpha() ? 1 : 0);
		setBPP(32);
		setDXTCompressionType(DXT_NOT_COMPRESSED);

		convertToRasterDataFormat(RASTERDATAFORMAT_BGR32, vecMipmapsRemoved);
		break;
	case XBOX_GTA:
		setPlatformId(5);
		setFilterFlags(2);
		setTXDRasterDataFormat(TXDRASTERDATAFORMAT_888);
		setAlpha(doesHaveAlpha() ? 1 : 0);
		setBPP(32);
		setDXTCompressionType(DXT_NOT_COMPRESSED);

		convertToRasterDataFormat(RASTERDATAFORMAT_BGR32, vecMipmapsRemoved);
		break;
	}
}

// convert raster data format
void							RWSection_TextureNative::convertToRasterDataFormat(ERasterDataFormat ERasterDataFormatValue, vector<string>& vecMipmapsRemoved)
{
	vector<RWEntry_TextureNative_MipMap*> vecMipmapsToRemove;
	uint32 uiMipmapIndex = 0;
	for (auto pMipmap : getMipMaps().getEntries())
	{
		if (!pMipmap->canRasterDataBeConverted())
		{
			vecMipmapsToRemove.push_back(pMipmap);
			vecMipmapsRemoved.push_back("Texture (" + getDiffuseName() + ") - Mipmap #" + String::toString(uiMipmapIndex + 1));
			continue;
		}

		pMipmap->convertToRasterDataFormat(ERasterDataFormatValue);
		uiMipmapIndex++;
	}
	for (auto pMipmap : vecMipmapsToRemove)
	{
		getMipMaps().removeEntry(pMipmap);
	}

	setRasterDataFormat(ERasterDataFormatValue, true);
	//setTXDRasterDataFormat(TXDManager::getTXDRasterDataFormatFromRasterDataFormat(ERasterDataFormatValue));
	setBPP(ImageManager::getBPPFromRasterDataFormat(ERasterDataFormatValue));
	setDXTCompressionType(ImageManager::getDXTCompressionTypeFromRasterDataFormat(ERasterDataFormatValue));

	if (ERasterDataFormatValue != RASTERDATAFORMAT_PAL4 && ERasterDataFormatValue != RASTERDATAFORMAT_PAL8)
	{
		string strPaletteData = "";

		setPaletteUsed(false);
		setPaletteData(strPaletteData);
	}
}