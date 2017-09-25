#pragma once

#include "nsbxgi.h"
#include "Format/IMG/Regular/IMGFormat.h"

class bxgi::IMGFormatVersion2 : public IMGFormat
{
public:
	IMGFormatVersion2(void);
	IMGFormatVersion2(std::string strFilePathOrData, bool bParam1IsFilePath);

	void					_unserialize(void);
	void					_serialize(void);
};