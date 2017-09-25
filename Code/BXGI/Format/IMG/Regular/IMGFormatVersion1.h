#pragma once

#include "nsbxgi.h"
#include "Format/IMG/Regular/IMGFormat.h"

class bxgi::IMGFormatVersion1 : public IMGFormat
{
public:
	IMGFormatVersion1(void);
	IMGFormatVersion1(std::string strFilePathOrData, bool bParam1IsFilePath);

	void					_unserialize(void);
	void					_serialize(void);
};