#pragma once

#include "nsbxgi.h"
#include "Format/FormatManager.h"
#include "Object/Singleton.h"
#include "ETXDRasterDataFormat.h"
#include "Image/ERasterDataFormat.h"
#include "Game/EPlatformedGame.h"
#include "Image/EDXTCompressionType.h"
#include "Platform/Hardware/EPlatform.h"
#include <string>

class bxgi::CTXDFormat;
class bxgi::CIntermediateTextureFormat;

class bxgi::CTXDManager : public bxcf::FormatManager<bxgi::CTXDFormat>, public bxcf::Singleton<CTXDManager>
{
public:
	CTXDManager(void);

	void						init(void);
	void						uninit(void);

	bxgi::CTXDFormat*			createFormat(void);

	bxgi::CTXDFormat*			convertIntermediateTextureFileToTXDFile(bxgi::CIntermediateTextureFormat *pGeneralTextureFile);

	static uint32				getTXDRasterDataFormatFromRasterDataFormat(bxcf::ERasterDataFormat ERasterDataFormatValue);
	static std::string			getTXDRasterFormatText(uint32 uiTXDRasterDataFormatValue, bxcf::EDXTCompressionType EDXTCompressionTypeValue);
	static bxcf::EPlatform		getPlatformFromTXDPlatformId(uint32 uiTXDPlatformId);
};