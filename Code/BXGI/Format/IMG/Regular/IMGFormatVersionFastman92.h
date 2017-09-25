#pragma once

#include "nsbxgi.h"
#include "Format/IMG/Regular/IMGFormat.h"

class bxgi::IMGFormatVersionFastman92 : public IMGFormat
{
public:
	IMGFormatVersionFastman92(void);
	IMGFormatVersionFastman92(std::string strFilePathOrData, bool bParam1IsFilePath);

	void					_unserialize(void);
	void					_serialize(void);
};