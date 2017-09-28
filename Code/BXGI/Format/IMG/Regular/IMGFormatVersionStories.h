#pragma once

#include "nsbxgi.h"
#include "Format/IMG/Regular/IMGFormat.h"

class bxgi::IMGFormatVersionStories : public IMGFormat
{
public:
	IMGFormatVersionStories(void);
	IMGFormatVersionStories(std::string strFilePathOrData, bool bParam1IsFilePath);

	void					_unserialize(void);
	void					_serialize(void);

	void					readWRLDFormat(void);
	void					readGameDTZ(void);
};