#pragma once

#include "nsbxgi.h"
#include "Format/FormatManager.h"
#include "Object/Singleton.h"

class bxgi::CWDRFormat;
class bxgi::CIntermediateModelFormat;

class bxgi::CWDRManager : public bxcf::FormatManager<bxgi::CWDRFormat>, public bxcf::Singleton<bxgi::CWDRManager>
{
public:
	CWDRManager(void);

	void								init(void);
	void								uninit(void);

	static bxgi::CWDRFormat*			convertIntermediateModelFileToWDRFile(bxgi::CIntermediateModelFormat *pGeneralModelFile);

	static uint32						getPackedOffset(uint32 uiOffset);
	static uint32						getPackedDataOffset(uint32 uiOffset);
};